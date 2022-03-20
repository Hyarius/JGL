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
		Abstract_activity()
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

		State _last_state;
		State _state;

	public:
		State_machine(State p_starting_state = {})
		{
			_activities.clear();
			_state = p_starting_state;
			_last_state = {};
		}
		void set_state(State p_state)
		{
			if (_activities.count(_state) == 0)
				THROW_EXCEPTION(jgl::Error_level::Warning, 1, "No activity set for state [" + jgl::itoa(static_cast<jgl::Int>(_state)) + "]");
			_state = p_state;
		}
		void add_activity(State p_state, Abstract_activity* p_activity)
		{
			_activities[p_state] = p_activity;
		}

		void update()
		{
			if (_activities.count(_state) == 0)
				return;

			if (_state == _last_state)
				_activities[_state]->execute();
			else
				_activities[_state]->on_transition();
			_last_state = _state;
		}
	};
}