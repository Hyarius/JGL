#pragma once

#include "jgl_includes.h"
#include "jgl_basic_types.h"

namespace jgl
{
	class Abstract_activity
	{
	protected:

	private:

	public:
		virtual ~Abstract_activity()
		{

		}
		virtual void execute() = 0;
		virtual void on_transition() = 0;
	};

	template <typename State>
	class State_machine
	{
	public:

	protected:

	private:
		jgl::Map<State, Abstract_activity*> _activities;

		Abstract_activity* _active_activity = nullptr;
		std::recursive_mutex _mutex;
		State _last_state;
		State _state;

	public:
		State_machine(State p_starting_state = {})
		{
			_activities.clear();
			_state = p_starting_state;
			_last_state = p_starting_state;
		}
		void set_state(State p_state)
		{
			if (_activities.count(_state) == 0)
			{
				THROW_EXCEPTION(jgl::Error_level::Error, 1, "No activity set for state [" + jgl::itoa(static_cast<jgl::Int>(_state)) + "]");
			}
			_mutex.lock();
			_last_state = _state;
			_state = p_state;
			_mutex.unlock();
		}
		void add_activity(State p_state, Abstract_activity* p_activity)
		{
			_activities[p_state] = p_activity;
		}

		void update()
		{
			if (_activities.count(_state) == 0)
				return;

			_mutex.lock();
			if (_state == _last_state && _active_activity != nullptr)
			{
				_active_activity->execute();
			}
			else
			{
				_last_state = _state;
				_active_activity = _activities[_state];
				if (_active_activity != nullptr)
					_active_activity->on_transition();
			}
			_mutex.unlock();
		}
	};
}