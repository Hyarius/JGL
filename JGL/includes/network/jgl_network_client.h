#ifndef JGL_NETWORK_CLIENT_H
#define JGL_NETWORK_CLIENT_H

#include "jgl_basic_types.h"
#include "structure/jgl_exception.h"
#include "structure/jgl_locked_queue.h"
#include "network/jgl_network_message.h"
#include "network/jgl_network_connection.h"

namespace jgl
{
	template <typename T, typename std::enable_if<std::is_enum<T>::value == true>::type* = nullptr >
	class Client
	{
	public :
		/*
			Function used as activity
		*/
		typedef void (*client_activity_funct_ptr)(jgl::Message<T>&, jgl::Data_contener);
	protected:
		asio::io_context _asio_context;
		std::thread _thread_context;
		jgl::Connection<T>* _connection;

		jgl::Ulong _message_timeout_delay = 0;

		/*
			data used to authenticate
		*/
		jgl::Long _major_version_key_number = 0;
		jgl::Long _medium_version_key_number = 0;
		jgl::Long _minor_version_key_number = 0;
		jgl::Long _abstract_version_key_number = 0;
		// -----------------------------

		/*
			Maps ranged activity ID, define by template T, and activity/data to used with it
		*/
		std::map<T, std::function< void(jgl::Message<T>&, jgl::Data_contener&) >> _activity_map;
		std::map<T, jgl::Data_contener> _activity_param_map;
		// -----------------------------

		/*
			Array storing all the message to read
		*/
		jgl::Locked_queue<jgl::Input_message<T>> _input;

		/*
			Method used to compute the magic number needed to authentificate
		*/
		jgl::Long _compute_magic_number(jgl::Int value)
		{
			return (((_major_version_key_number << 48) ^ value) + ((_medium_version_key_number << 32) & value) + ((_minor_version_key_number << 16) | value) + (_abstract_version_key_number));
		}

	public:
		/*
			Create a new client, you still need to connect it
		*/
		Client() : _connection()
		{

		}

		/*
			Destroy your client and try to disconnect it
		*/
		virtual ~Client()
		{
			disconnect();
		}

		/*
			Define the function [funct] to use with [param], when receiving message type [msg_type]
		*/
		void add_activity(T msg_type, std::function< void(jgl::Message<T>&, jgl::Data_contener&) > funct, jgl::Data_contener param = jgl::Data_contener())
		{
			_activity_map[msg_type] = funct;
			_activity_param_map[msg_type] = param;
		}

		/*
			Set the data to use to authentificate yourself
		*/
		void set_version(jgl::Int major_version, jgl::Int medium_version, jgl::Int minor_version, jgl::Int abstract_version)
		{
			_major_version_key_number = major_version;
			_medium_version_key_number = medium_version;
			_minor_version_key_number = minor_version;
			_abstract_version_key_number = abstract_version;
		}

		/*
			Connect the client to the desired address, represented by the string [host], at the connection port [port]
		*/
		bool connect(jgl::String host, const jgl::Ushort port)
		{
			THROW_INFORMATION("Client connection started");
			try
			{
				std::string host_converted = std::string(host.c_str());
				asio::ip::tcp::resolver resolver(_asio_context);
				asio::ip::tcp::resolver::results_type endpoints = resolver.resolve(host_converted, std::to_string(port));

				asio::ip::tcp::socket socket(_asio_context);

				socket.open(asio::ip::tcp::v4());

				asio::ip::tcp::no_delay no_delay(true);
				socket.set_option(no_delay);

				_connection = new Connection<T>(jgl::Connection<T>::Owner::client, _asio_context, std::move(socket), &_input);

				_connection->connect_to_server(endpoints);

				_thread_context = std::thread([this]() { _asio_context.run(); });
			}
			catch (const std::exception& e)
			{
				THROW_EXCEPTION(jgl::Error_level::Error, 1, "Client Exception: " + jgl::String(e.what()));
				return false;
			}
			catch (const std::string& ex)
			{
				THROW_EXCEPTION(jgl::Error_level::Error, 1, "Client Exception: " + ex);
				return false;
			}
			catch (...)
			{
				THROW_EXCEPTION(jgl::Error_level::Error, 1, "Client Exception unknow");
				return false;
			}
			return true;
		}

		/*
			Reconnect the client to the desired address, represented by the string [host], at the connection port [port]
		*/
		bool reconnect(jgl::String host, const jgl::Ushort port)
		{
			try
			{
				std::string host_converted = std::string(host.c_str());
				asio::ip::tcp::resolver resolver(_asio_context);
				asio::ip::tcp::resolver::results_type endpoints = resolver.resolve(host_converted, std::to_string(port));

				_connection->connect_to_server(endpoints);
				_connection->input()->clear();
			}
			catch (const std::exception& e)
			{
				THROW_EXCEPTION(jgl::Error_level::Warning, 1, "Client Exception: " + jgl::String(e.what()));
				return false;
			}
			catch (const std::string& ex)
			{
				THROW_EXCEPTION(jgl::Error_level::Warning, 1, "Client Exception: " + ex);
				return false;
			}
			catch (...)
			{
				THROW_EXCEPTION(jgl::Error_level::Warning, 1, "Client Exception unknow");
				return false;
			}
			return true;
		}

		/*
			Disconnect the client from where it is connected previously
		*/
		void disconnect()
		{
			if (is_connected())
			{
				_connection->disconnect();
			}
		
			_asio_context.stop();
			if (_thread_context.joinable())
			{
				_thread_context.join();
			}
			_input.clear();
		}

		/*
			Check if the connection exist and if she's active
		*/
		bool is_connected()
		{
			if (_connection)
				return _connection->is_connected();
			else
				return false;
		}

		/*
			If the message queue is not empty, pull one out and analyze/react accordingly

			If the transmitter isn't validated yet, try to validate him or ignore him
			If the transmitter is validated, try to use the user defined functer to the message ID, or ignore the message if no activity set for this message type
		*/
		void update()
		{
			if (_connection == nullptr)
				return;
			if (_connection->state() == jgl::Connection<T>::State::Unknown)
			{
				if (_input.empty() == false)
				{
					auto msg = _input.pop_front().msg;

					THROW_INFORMATION("Validation process begin : message received of size (" + jgl::itoa(msg.size()) + ")");
					if (msg.size() == sizeof(jgl::Long))
					{
						THROW_INFORMATION("Validation process begin : magic number message");
						jgl::Long key;
						jgl::Long result;

						msg >> key;

						msg.clear();
						THROW_INFORMATION("Key received : " + jgl::itoa(key) + " of size " + jgl::itoa(sizeof(key)));
						result = _compute_magic_number(key);
						THROW_INFORMATION("Awsner : " + jgl::itoa(result) + " of size " + jgl::itoa(sizeof(result)));
						msg << key;
						msg << result;
						send(msg);
					}
					else if (msg.size() == sizeof(bool))
					{
						bool Accepted;

						msg >> Accepted;
						if (Accepted == true)
						{
							THROW_INFORMATION("Validation process begin : confirmation type [Accepted]");
							_connection->accepted_by_server();
						}
						else
						{
							THROW_INFORMATION("Validation process begin : confirmation type [Rejected]");
							_connection->refused_by_server();
						}
					}
					else
					{
						THROW_INFORMATION("Validation process begin : unknow type");
					}
				}
			}
			else if (_connection->state() == jgl::Connection<T>::State::Accepted)
			{
				while (_input.empty() == false)
				{
					auto input = _input.pop_front();
					auto msg = input.msg;

					if (_activity_map.count(msg.type()) != 0)
					{
						if (jgl::Application::active_application() == nullptr || _message_timeout_delay == 0 || 
							input.time + _message_timeout_delay < jgl::Application::active_application()->time())
						{
							_activity_map[msg.type()](msg, _activity_param_map[msg.type()]);
						}
					}
					else
					{
						THROW_EXCEPTION(jgl::Error_level::Warning, 1, "Message_received of unknow id (" + jgl::itoa(static_cast<jgl::Int>(msg.type())) + ")");
					}
				}
			}
		}

		/*
			Send one message [msg] to the server
		*/
		void send(const jgl::Message<T>& msg)
		{
			if (is_connected() && _connection->state() != jgl::Connection<T>::State::Refused)
				_connection->send(msg);
		}

		/*
			Return the message pool
		*/
		jgl::Locked_queue<jgl::Input_message<T>>& input()
		{
			return (_input);
		}

		/*
			Return the const pointer of the client's connection
		*/
		jgl::Connection<T>* connection() const
		{
			return (_connection);
		}
	};
}

#endif