#pragma once

#include "jgl_basic_types.h"
#include "Structure/jgl_singleton.h"
#include "Structure/jgl_data_contener.h"

namespace jgl
{
	template<typename TEvent>
	class Publisher
	{
	public:
		typedef std::function<void()> Activity;

		class Subscriber
		{
		private:
			TEvent _event;
			Activity _funct;

		public:
			Subscriber(TEvent p_event = {}, Activity p_funct = nullptr) :
				_event(p_event),
				_funct(p_funct)
			{

			}

			TEvent event() {
				return (_event);
			}

			void update()
			{
				if (_funct != nullptr)
					_funct(); 
			}
		};

	protected:
		jgl::Array<Subscriber> _event_subscribers;

	public:
		Publisher()
		{

		}

		void notify(TEvent p_event)
		{
			for (jgl::Size_t i = 0; i < _event_subscribers.size(); i++)
			{
				Subscriber& tmp_subscriber = _event_subscribers[i];
				if (tmp_subscriber.event() == p_event)
					tmp_subscriber.update();
			}
		}
		void subscribe(TEvent p_event, Activity p_funct)
		{
			_event_subscribers.push_back(Subscriber(p_event, p_funct));
		}
	};

}