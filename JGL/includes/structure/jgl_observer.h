#pragma once

#include "jgl_basic_types.h"
#include "Structure/jgl_singleton.h"
#include "Structure/jgl_data_contener.h"

namespace jgl
{

	template<typename T, typename std::enable_if<std::is_enum<T>::value == true>::type* = nullptr >
	class Publisher
	{
	public:
		typedef T Event;

		class Subscriber
		{

		private:

		public:
			virtual void update(Event p_event, jgl::Data_contener& p_param) = 0;
		};

	protected:
		jgl::Map<Event, jgl::Array<Subscriber*>> _event_subscribers;

	public:
		Publisher()
		{

		}

		void notify(Event p_event, jgl::Data_contener p_param = jgl::Data_contener())
		{
			if (_event_subscribers.count(p_event) != 0)
			{
				jgl::Array<Subscriber*>& tmp_array = _event_subscribers[p_event];

				for (jgl::Size_t i = 0; i < tmp_array.size(); i++)
				{
					p_param.reset();
					tmp_array[i]->update(p_event, p_param);
				}
			}
		}
		void subscribe(Event p_event, Subscriber* p_subscriber)
		{
			_event_subscribers[p_event].push_back(p_subscriber);
		}
	};

}