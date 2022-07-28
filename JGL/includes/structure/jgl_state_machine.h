#pragma once

#include "jgl_includes.h"
#include "jgl_basic_types.h"

namespace jgl
{
	class Abstract_activity
	{
	protected:

	public:
		Abstract_activity()
		{

		}
		virtual ~Abstract_activity()
		{

		}
		virtual void execute() = 0;
		virtual void on_transition() = 0;
	};

	class Activity : public Abstract_activity
	{
	private:
		std::function<void()> _execute_funct;
		std::function<void()> _transition_funct;

	public:
		Activity(std::function<void()> p_execute_funct)
		{
			_execute_funct = p_execute_funct;
			_transition_funct = nullptr;
		}
		Activity(std::function<void()> p_execute_funct, std::function<void()> p_transition_funct)
		{
			_execute_funct = p_execute_funct;
			_transition_funct = p_transition_funct;
		}
		void execute()
		{
			if (_execute_funct != nullptr)
				_execute_funct();
		}
		void on_transition()
		{
			if (_transition_funct != nullptr)
				_transition_funct();
		}
	};

	template <typename TState>
	class State_machine
	{
	private:
		jgl::Map<TState, Abstract_activity*> _activities;
		jgl::Array<Abstract_activity*> _allocated_activity;

		Abstract_activity* _active_activity = nullptr;
		std::recursive_mutex _mutex;
		TState _last_state;
		TState _state;

	public:
		State_machine(TState p_starting_state = {})
		{
			_activities.clear();
			_state = p_starting_state;
			_last_state = p_starting_state;
		}
		~State_machine()
		{
			for (jgl::Size_t i = 0; i < _allocated_activity.size(); i++)
			{
				delete _allocated_activity[i];
			}
		}
		TState state() const
		{
			return (_state);
		}
		void set_state(TState p_state)
		{
			if (_activities.count(_state) == 0)
			{
				THROW_EXCEPTION(jgl::Error_level::Warning, 1, "No activity set for state [" + jgl::itoa(static_cast<jgl::Int>(_state)) + "]");
			}
			_mutex.lock();
			_last_state = _state;
			_state = p_state;
			_mutex.unlock();
		}
		void add_activity(TState p_state, Abstract_activity* p_activity)
		{
			_activities[p_state] = p_activity;
		}
		void add_activity(TState p_state, std::function<void()> p_execute_funct)
		{
			Activity* new_activity = new Activity(p_execute_funct);

			_allocated_activity.push_back(new_activity);
			_activities[p_state] = new_activity;
		}
		void add_activity(TState p_state, std::function<void()> p_execute_funct, std::function<void()> p_on_transition_funct)
		{
			Activity* new_activity = new Activity(p_execute_funct, p_on_transition_funct);

			_allocated_activity.push_back(new_activity);
			_activities[p_state] = new_activity;
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