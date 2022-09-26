#pragma once

#include "widget/jgl_singleton_widget.h"
#include "network/jgl_network_client.h"

namespace jgl
{
	template <typename TMessage>
	class Client_manager : public jgl::Widget, public  jgl::Singleton_widget< Client_manager<TMessage> >
	{
		friend class jgl::Singleton_widget< Client_manager<TMessage> >;
	private:
		void _render()
		{

		}

		void _on_geometry_change()
		{

		}

		jgl::Bool _update()
		{
			_client->update();
			return(false);
		}

		Client<TMessage>* _client;

		Client_manager(jgl::Widget* p_parent) : jgl::Widget(p_parent)
		{
			_client = new Client<TMessage>();
		}
		~Client_manager()
		{
			if (_client != nullptr)
				delete _client;
			_client = nullptr;
		}

	public:
		jgl::Client<TMessage>* get_client()
		{
			return (_client);
		}

		static jgl::Client<TMessage>* client()
		{
			if (jgl::Singleton_widget< Client_manager<TMessage> >::instance() == nullptr)
				return (nullptr);
			return (jgl::Singleton_widget< Client_manager<TMessage> >::instance()->get_client());
		}

		void start(jgl::String p_address, jgl::Ushort p_server_port)
		{
			Client_manager::instance()->client()->connect(p_address, p_server_port);
		}

		static void send(jgl::Message<TMessage> p_msg)
		{
			if (Client_manager<TMessage>::client() != nullptr)
			{
				Client_manager<TMessage>::client()->send(p_msg);
			}
			else
			{
				THROW_EXCEPTION(jgl::Error_level::Error, 1, "No client accesible");
			}
		}

		void add_activity(TMessage p_msg_type, std::function< void(jgl::Message<TMessage>&, jgl::Data_contener&) > p_funct, jgl::Data_contener p_param = jgl::Data_contener())
		{
			if (Client_manager<TMessage>::client() != nullptr)
			{
				Client_manager<TMessage>::client()->add_activity(p_msg_type, p_funct, p_param);
			}
			else
			{
				THROW_EXCEPTION(jgl::Error_level::Error, 1, "No Client accesible");
			}
		}
	};

}