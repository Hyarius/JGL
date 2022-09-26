#pragma once

#include "widget/jgl_singleton_widget.h"
#include "Network/jgl_network_server.h"

namespace jgl
{
	template <typename TMessage>
	class Server_manager : public jgl::Widget, public jgl::Singleton_widget< Server_manager<TMessage> >
	{
		friend class jgl::Singleton_widget< Server_manager<TMessage> >;
	private:
		void _render()
		{

		}
		void _on_geometry_change()
		{

		}

		jgl::Bool _update()
		{
			if (_server != nullptr)
				_server->update();
			return(false);
		}

		jgl::Server<TMessage> * _server;

		Server_manager(jgl::Widget* p_parent) : jgl::Widget(p_parent)
		{
			_server = nullptr;
		}
		~Server_manager()
		{
			if (_server != nullptr)
				delete _server;
		}

	public:
		void start(jgl::Size_t p_server_port)
		{
			_server = new jgl::Server<TMessage>(p_server_port);
			_server->start();
		}

		jgl::Server<TMessage>* get_server()
		{
			return (_server);
		}

		static jgl::Server<TMessage>* server()
		{
			if (jgl::Singleton_widget< Server_manager<TMessage> >::instance() == nullptr)
				return (nullptr);
			return (jgl::Singleton_widget< Server_manager<TMessage> >::instance()->get_server());
		}

		void send_to(jgl::Connection<TMessage>* p_client, const jgl::Message<TMessage> p_msg)
		{
			if (jgl::Singleton_widget< Server_manager<TMessage> >::instance() != nullptr &&
				jgl::Singleton_widget< Server_manager<TMessage> >::instance()->get_server() != nullptr)
			{
				jgl::Singleton_widget< Server_manager<TMessage> >::instance()->get_server()->send_to(p_client, p_msg);
			}
			else
			{
				THROW_EXCEPTION(jgl::Error_level::Error, 1, "No Server accesible");
			}
		}

		void send_to_all(const jgl::Message<TMessage> p_msg, jgl::Connection<TMessage>* p_ignore = nullptr)
		{
			if (jgl::Singleton_widget< Server_manager<TMessage> >::instance() != nullptr &&
				jgl::Singleton_widget< Server_manager<TMessage> >::instance()->get_server() != nullptr)
			{
				jgl::Singleton_widget< Server_manager<TMessage> >::instance()->get_server()->send_to_all(p_msg, p_ignore);
			}
			else
			{
				THROW_EXCEPTION(jgl::Error_level::Error, 1, "No Server accesible");
			}
		}

		void send_to_array(const jgl::Message<TMessage> p_msg, jgl::Array<jgl::Connection<TMessage>*>& p_list)
		{
			if (jgl::Singleton_widget< Server_manager<TMessage> >::instance() != nullptr &&
				jgl::Singleton_widget< Server_manager<TMessage> >::instance()->get_server() != nullptr)
			{
				jgl::Singleton_widget< Server_manager<TMessage> >::instance()->get_server()->send_to_array(p_msg, p_list);
			}
			else
			{
				THROW_EXCEPTION(jgl::Error_level::Error, 1, "No Server accesible");
			}
		}

		void add_activity(TMessage p_msg_type, std::function< void(jgl::Connection<TMessage>*, jgl::Message<TMessage>&, jgl::Data_contener&) > p_funct, jgl::Data_contener p_param = jgl::Data_contener())
		{
			if (jgl::Singleton_widget< Server_manager<TMessage> >::instance() != nullptr &&
				jgl::Singleton_widget< Server_manager<TMessage> >::instance()->get_server() != nullptr)
			{
				jgl::Singleton_widget< Server_manager<TMessage> >::instance()->get_server()->add_activity(p_msg_type, p_funct, p_param);
			}
			else
			{
				THROW_EXCEPTION(jgl::Error_level::Error, 1, "No Server accesible");
			}
		}
	};
}