#include "jgl.h"

namespace jgl
{
	Timer::Timer(jgl::Ulong p_timer_period)
	{
		_state = State::Stopped;
		_timer_duration = p_timer_period;
	}

	void Timer::set_duration(jgl::Ulong p_timer_period)
	{
		_state = State::Stopped;
		_timer_duration = p_timer_period;
	}

	void Timer::start()
	{
		_starting_time = jgl::get_time();
		_state = State::Running;
	}

	void Timer::stop()
	{
		_state = State::Stopped;
	}

	Timer::State Timer::timeout()
	{
		if (_state == State::Running && _starting_time + _timer_duration <= jgl::Application::active_application()->time())
		{
			_state = State::Timeout;
		}

		return (_state);
	}
}