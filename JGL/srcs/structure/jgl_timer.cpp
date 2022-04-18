#include "jgl.h"

namespace jgl
{
	Timer::Timer(jgl::Ulong p_timer_period)
	{
		_started = false;
		_timer_duration = p_timer_period;
	}

	void Timer::start()
	{
		_starting_time = jgl::get_time();
		_started = true;
	}

	void Timer::stop()
	{
		_started = false;
	}

	jgl::Bool Timer::timeout()
	{
		if (_started == false)
			return (true);
		return (_starting_time + _timer_duration <= jgl::Application::active_application()->time());
	}
}