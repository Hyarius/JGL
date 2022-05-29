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
	};
}