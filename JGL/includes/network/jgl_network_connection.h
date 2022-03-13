#ifndef JGL_NETWORK_CONNEXION_H
#define JGL_NETWORK_CONNEXION_H

#include "network/jgl_network_message.h"

#include "application/jgl_application.h"

namespace jgl
{
	template <typename T, typename std::enable_if<std::is_enum<T>::value == true>::type* = nullptr>
	class Connection
	{
	public:
		/*
			Define if the connection is owned by the server or a client
		*/
		enum class Owner
		{
			error = -1,
			server,
			client
		};

		/*
			Define the connection status :
			- Unknow if during validation,
			- Accepted if the connection has been authorized to communicate
			- Refused if not
		*/
		enum class State
		{
			Unknown = 0,
			Refused = -1,
			Accepted = 1
		};

	protected:
		Connection::State _state;
		asio::ip::tcp::socket _socket;
		asio::io_context& _context;
		jgl::Locked_queue<Message<T>> _output;
		jgl::Locked_queue<jgl::Input_message<T>>* _input;
		jgl::Message<T> _tmp_message;
		Connection::Owner _owner = Owner::error;
		jgl::Uint _id;

	public:

		/*
			Create a new connection, setting it internal value.

			Shouldn't be called by user manualy
		*/
		Connection(Connection::Owner parent, asio::io_context& p_context, asio::ip::tcp::socket p_socket, jgl::Locked_queue<jgl::Input_message<T>>* p_input)
			: _context(p_context), _socket(std::move(p_socket)), _input(p_input), _tmp_message({})
		{
			_state = Connection::State::Unknown;
			_id = 0;
			_owner = parent;
		}

		/*
			Return the connection status
		*/
		State state() {return (_state);}

		/*
			Set the connection status to accepted
		*/
		void accepted_by_server()
		{
			_state = Connection::State::Accepted;
		}

		/*
			Set the connection status to refused
		*/
		void refused_by_server()
		{
			_state = Connection::State::Refused;
		}

		/*
			Basic destructor
		*/
		virtual ~Connection()
		{}

		/*
			Return the message pool of this connection
		*/
		jgl::Locked_queue<jgl::Input_message<T>>* input()
		{
			return (_input);
		}

		/*
		*	Used by server only :
			Return the id of the connection
		*/
		jgl::Uint id() const
		{
			return (_id);
		}

		/*
		*	Used by server only :
			Connect to the desired client and set the connection id to [u_id]
		*/
		void connect_to_client(jgl::Uint u_id = 0)
		{
			if (_owner == Connection::Owner::server)
			{
				if (_socket.is_open())
				{
					_id = u_id;
					_read_header();
				}
			}
		}

		/*
		*	Used by clients only :
			Connect to the desired server
		*/
		void connect_to_server(const asio::ip::tcp::resolver::results_type& endpoints)
		{
			if (_owner == Connection::Owner::client)
			{
				asio::async_connect(_socket, endpoints,
					[this](std::error_code ec, asio::ip::tcp::endpoint endpoint)
					{
						if (!ec)
							_read_header();
					});
			}
		}

		/*
			Disconnect the connection from where it's linked
		*/
		void disconnect()
		{
			if (is_connected())
				asio::post(_context, [this]() { _socket.close(); });
		}

		/*
			Return if the connection is connected to a destination
		*/
		bool is_connected() const
		{
			return (_socket.is_open());
		}

		/*
			Send the desired message to where the connection is connected

			If the connection isn't active, do nothing
		*/
		void send(const jgl::Message<T>& msg)
		{
			if (is_connected() == false)
				return;

			asio::post(_context,
				[this, msg]()
				{
					bool state = _output.empty();
					_output.push_back(msg);
					if (state == true)
						_write_header();
				});
		}

	private:
		/*
		
		* -------- PART OF MESSAGE SENDING/READING
		
		*/
		void _write_header()
		{
			asio::async_write(_socket, asio::buffer(&_output.front().header, sizeof(Message_header<T>)),
				[this](std::error_code ec, size_t length)
				{
					if (!ec)
					{
						if (_output.front().content.size() > 0)
							_write_content();
						else
						{
							_output.pop_front();

							if (!_output.empty())
								_write_header();
						}
					}
					else
					{
						jgl::cout << "[" << _id << "] Write Header Fail.\n";
						_socket.close();
					}
				});
		}

		void _write_content()
		{
			asio::async_write(_socket, asio::buffer(_output.front().content.data(), _output.front().content.size()),
				[this](std::error_code ec, size_t length)
				{
					if (!ec)
					{
						_output.pop_front();

						if (!_output.empty())
							_write_header();
					}
					else
					{
						jgl::cout << "[" << _id << "] Write Body Fail.\n";
						_socket.close();
					}
				});
		}

		void _read_header()
		{
			asio::async_read(_socket, asio::buffer(&_tmp_message.header, sizeof(Message_header<T>)),
				[this](std::error_code ec, size_t length)
				{
					if (!ec)
					{
						if (_tmp_message.header.size > 0)
						{
							_tmp_message.content.resize(_tmp_message.header.size);
							_read_content();
						}
						else
						{
							_add_message_to_queue();
						}
					}
					else
					{
						jgl::cout << "[" << _id << "] Read Header Fail.\n";
						_socket.close();
					}
				});
		}

		void _read_content()
		{
			asio::async_read(_socket, asio::buffer(_tmp_message.content.data(), _tmp_message.content.size()),
				[this](std::error_code ec, size_t length)
				{
					if (!ec)
					{
						_add_message_to_queue();
					}
					else
					{
						jgl::cout << "[" << _id << "] Read Body Fail.\n";
						_socket.close();
					}
				});
		}

		void _add_message_to_queue()
		{
			if (_owner == Owner::server)
				_input->push_back({ this, _tmp_message, (jgl::Application::active_application() != nullptr ? jgl::Application::active_application()->time() : 0) });
			else
				_input->push_back({ nullptr, _tmp_message, (jgl::Application::active_application() != nullptr ? jgl::Application::active_application()->time() : 0) });
			_read_header();
		}
	};
}

#endif