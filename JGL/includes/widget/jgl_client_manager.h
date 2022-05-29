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
	};

}