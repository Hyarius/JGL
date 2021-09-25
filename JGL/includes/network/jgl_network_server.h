#ifndef JGL_NETWORK_SERVER_H
#define JGL_NETWORK_SERVER_H

#include "structure/jgl_locked_queue.h"
#include "network/jgl_network_message.h"
#include "network/jgl_network_connection.h"

namespace jgl
{
	template <typename T, typename std::enable_if<std::is_enum<T>::value == true>::type* = nullptr >
	class Server
	{
	public :
		
	protected:

		/*
			Tuple Functer-Data to use when validating a new client
		*/
		std::function< void(jgl::Connection<T>*, jgl::Data_contener) > _login_funct = nullptr;
		jgl::Data_contener _login_param;

		/*
			Tuple Functer-Data to use when disconnecting a client
		*/
		std::function< void(jgl::Connection<T>*, jgl::Data_contener) > _logout_funct = nullptr;
		jgl::Data_contener _logout_param;

		jgl::Int _key = 0;
		jgl::Long _major_version_key_number = 0;
		jgl::Long _medium_version_key_number = 0;
		jgl::Long _minor_version_key_number = 0;
		jgl::Long _abstract_version_key_number = 0;

		jgl::Bool _is_active;

		jgl::Locked_queue<jgl::Input_message<T>> _input;

		std::deque<jgl::Connection<T>*> _active_connection;
		std::deque<jgl::Connection<T>*> _accepted_connection;

		asio::io_context _asio_context;
		std::thread _thread_context;

		asio::ip::tcp::acceptor _acceptor;

		std::map<T, std::function< void(jgl::Connection<T>*, jgl::Message<T>&, jgl::Data_contener) >> _activity_map;
		std::map<T, jgl::Data_contener> _activity_param_map;

		jgl::Uint _id_count = 10000;

		jgl::Long _compute_magic_number(jgl::Int value)
		{
			return (((_major_version_key_number << 48) ^ value) + ((_medium_version_key_number << 32) & value) + ((_minor_version_key_number << 16) | value) + (_abstract_version_key_number));
		}

		void _wait_for_connection()
		{
			_acceptor.async_accept(
				[this](std::error_code ec, asio::ip::tcp::socket socket)
				{
					if (!ec)
					{
						THROW_INFORMATION("[SERVER] New Connection: " + socket.remote_endpoint().address().to_string());

						jgl::Connection<T>* newconn = new jgl::Connection<T>(jgl::Connection<T>::Owner::server, _asio_context, std::move(socket), &_input);

						_client_connect(newconn);
						_active_connection.push_back(std::move(newconn));

						_active_connection.back()->connect_to_client(_id_count++);

						THROW_INFORMATION("[" + jgl::itoa(_active_connection.back()->id()) + "] Connection Approved");

					}
					else
					{
						THROW_INFORMATION("[SERVER] New Connection Error: " + ec.message());
					}

					_wait_for_connection();
				});
		}

		void _on_message_reception(jgl::Connection<T>* client, jgl::Message<T>& msg)
		{
			if (client->state() == jgl::Connection<T>::State::Accepted)
			{
				if (_activity_map.count(msg.type()) != 0)
				{
					_activity_map[msg.type()](client, msg, _activity_param_map[msg.type()]);
				}
				else
				{
					THROW_EXCEPTION(jgl::Error_level::Warning, 1, "Message_received of unknow id (" + jgl::itoa(static_cast<jgl::Int>(msg.type())) + ")");
				}
			}
			else if (client->state() == jgl::Connection<T>::State::Unknown)
			{
				_valid_client_connect(client, msg);
			}
		}

		void _client_connect(jgl::Connection<T>* client)
		{
			THROW_INFORMATION("Received connection from a new client [" + jgl::itoa(client->id()) + "]");
			jgl::Message<T> msg;

			msg << _key;
			client->send(msg);
		};

		jgl::Bool _valid_client_connect(jgl::Connection<T>* client, jgl::Message<T>& msg)
		{
			THROW_INFORMATION("Validation connection from a client [" + jgl::itoa(client->id()) + "]");
			jgl::Int key;
			jgl::Long presumed_result;
			jgl::Long real_result;

			msg >> key;
			msg >> presumed_result;


			THROW_INFORMATION("Key received : " + jgl::itoa(key) + " of size " + jgl::itoa(sizeof(key)));
			real_result = _compute_magic_number(key);
			THROW_INFORMATION("Awsner : " + jgl::itoa(real_result) + " of size " + jgl::itoa(sizeof(real_result)));
			THROW_INFORMATION("Magic number send : " + jgl::itoa(key) + " -> result proposed : " + jgl::itoa(presumed_result) + " vs espected " + jgl::itoa(real_result));
			if (real_result == presumed_result)
			{
				client->accepted_by_server();
				if (_login_funct != nullptr)
					_login_funct(client, _login_param);
				msg.clear();
				msg << true;
				client->send(msg);
				return (true);
			}
			else
			{
				client->refused_by_server();
				msg.clear();
				msg << false;
				client->send(msg);
				return (false);
			}
		}

		void _client_disconnect(jgl::Connection<T>* client)
		{
			if (_logout_funct != nullptr)
				_logout_funct(client, _logout_param);
		};

	public:
		/*
			Create a new server and set his port
		*/
		Server(uint16_t port)
			: _acceptor(_asio_context, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port))
		{

		}

		/*
			Server destructor
		*/
		virtual ~Server()
		{
			stop();
		}

		/*
			Define the functer [funct] with [param] parameter to use when receiving a message of id [msg_type]
		*/
		void add_activity(T msg_type, std::function< void(jgl::Connection<T>*, jgl::Message<T>&, jgl::Data_contener) > funct, jgl::Data_contener param)
		{
			_activity_map[msg_type] = funct;
			_activity_param_map[msg_type] = param;
		}

		/*
			Define the functer [funct] with [param] parameter to use when validating a new connection
		*/
		void set_login_function(std::function< void(jgl::Connection<T>*, jgl::Data_contener) > p_login_funct, jgl::Data_contener param)
		{
			_login_funct = p_login_funct;
			_login_param = param;
		}

		/*
			Define the functer [funct] with [param] parameter to use when disconnecting a client
		*/
		void set_logout_function(std::function< void(jgl::Connection<T>*, jgl::Data_contener) > p_logout_funct, jgl::Data_contener param)
		{
			_logout_funct = p_logout_funct;
			_logout_param = param;
		}

		/*
			Define the key used to authenticate new clients
		*/
		void set_key(jgl::Int p_key)
		{
			_key = p_key;
		}

		/*
			Define the version values used to authenticate new clients
		*/
		void set_version(jgl::Int major_version, jgl::Int medium_version, jgl::Int minor_version, jgl::Int abstract_version)
		{
			_major_version_key_number = major_version;
			_medium_version_key_number = medium_version;
			_minor_version_key_number = minor_version;
			_abstract_version_key_number = abstract_version;
		}

		/*
			Initialize the server
		*/
		void start()
		{
			try
			{
				_wait_for_connection();

				_thread_context = std::thread([this]() { _asio_context.run(); });
			}
			catch (std::exception& e)
			{
				THROW_EXCEPTION(jgl::Error_level::Error, 1, "[SERVER] Exception: " + jgl::String(e.what()));
				_is_active = false;
			}

			THROW_INFORMATION("[SERVER] Started");
			_is_active = true;
		}

		/*
			Stop the server

			Close every active connection to the server
		*/
		void stop()
		{
			_asio_context.stop();

			if (_thread_context.joinable())
				_thread_context.join();

			THROW_INFORMATION("[SERVER] Stopped");
		}

		/*
			Return if the server is started
		*/
		jgl::Bool is_active()
		{
			return (_is_active);
		}

		/*
			Send a message to desired client
		*/
		void send_to(jgl::Connection<T>* client, const jgl::Message<T>& msg)
		{
			if (client != nullptr && client->is_connected())
			{
				client->send(msg);
			}
			else
			{
				_client_disconnect(client);
				_active_connection.erase(
					std::remove(_active_connection.begin(), _active_connection.end(), client), _active_connection.end());
				_accepted_connection.erase(
					std::remove(_accepted_connection.begin(), _accepted_connection.end(), client), _accepted_connection.end());
			}
		}

		/*
			Send a message to every active connection.

			Optional :
			jgl::Connection<T>* p_ignore - A client to ignore. Set to nullptr by default
		*/
		void send_to_all(const jgl::Message<T>& msg, jgl::Connection<T>* p_ignore = nullptr)
		{
			jgl::Bool error = false;

			for (auto& client : _active_connection)
			{
				if (client != nullptr && client->is_connected())
				{
					if (client != p_ignore && client->state() == Connection<T>::State::Accepted)
						client->send(msg);
				}
				else if (client != nullptr)
				{
					_client_disconnect(client);
					delete client;
					client = nullptr;
					error = true;
				}
			}

			if (error == true)
			{
				_active_connection.erase(std::remove(_active_connection.begin(), _active_connection.end(), nullptr), _active_connection.end());
				_accepted_connection.erase(std::remove(_accepted_connection.begin(), _accepted_connection.end(), nullptr), _accepted_connection.end());
			}
		}

		/*
			Method to use to parse every incoming message
		*/
		void update()
		{
			while (_input.empty() == false)
			{
				auto msg = _input.pop_front();

				_on_message_reception(msg.remote, msg.msg);
			}
			for (auto& client : _active_connection)
			{
				if (client != nullptr && client->is_connected() == false)
				{
					_client_disconnect(client);
					delete client;
					client = nullptr;
				}
			}
		}

	};
}

#endif
