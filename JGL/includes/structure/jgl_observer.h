#pragma once

#include "jgl_basic_types.h"
#include "Structure/jgl_singleton.h"
#include "Structure/jgl_data_contener.h"

namespace jgl
{
	template<typename TEvent, typename TContext>
	class Publisher
	{
	public:
		typedef std::function<void(TContext&)> Activity;

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

			void update(TContext& p_param)
			{
				if (_funct != nullptr)
					_funct(p_param); 
			}
		};

	protected:
		TContext _context;
		jgl::Array<Subscriber> _event_subscribers;

	public:
		Publisher(TContext p_context = {}) :
			_context(p_context)
		{

		}

		TContext* context() { return (&_context); }
		const TContext* context() const { return (&_context); }

		void notify(TEvent p_event)
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
		void subscribe(TEvent p_event, Activity p_funct)
		{
			_event_subscribers.push_back(Subscriber(p_event, p_funct));
		}
	};

}