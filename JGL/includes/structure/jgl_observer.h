#pragma once

#include "jgl_basic_types.h"
#include "Structure/jgl_singleton.h"
#include "Structure/jgl_data_contener.h"

namespace jgl
{
	template<typename T, typename U>
	class Publisher
	{
	public:
		typedef T Event;
		typedef U Context;
		typedef std::function<void(Context&)> Activity;

		class Subscriber
		{
		private:
			Event _event;
			Activity _funct;

		public:
			Subscriber(Event p_event = {}, Activity p_funct = nullptr) :
				_event(p_event),
				_funct(p_funct)
			{

			}

			Event event() {
				return (_event);
			}

			void update(Context& p_param)
			{
				if (_funct != nullptr)
					_funct(p_param); 
			}
		};

	protected:
		Context _context;
		jgl::Array<Subscriber> _event_subscribers;

	public:
		Publisher(Context p_context = {}) :
			_context(p_context)
		{

		}

		Context* context() { return (&_context); }
		const Context* context() const { return (&_context); }

		void notify(Event p_event)
		{
			for (jgl::Size_t i = 0; i < _event_subscribers.size(); i++)
			{
				Subscriber& tmp_subscriber = _event_subscribers[i];
				if (tmp_subscriber.event() == p_event)
					tmp_subscriber.update(_context);
			}
		}
		void subscribe(Subscriber&& p_subscriber)
		{
			_event_subscribers.push_back(p_subscriber);
		}
		void subscribe(Event p_event, Activity p_funct)
		{
			_event_subscribers.push_back(Subscriber(p_event, p_funct));
		}
	};

}