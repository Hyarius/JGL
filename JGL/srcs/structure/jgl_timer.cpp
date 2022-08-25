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

	jgl::Ulong Timer::elapsed_time()
	{
		return (jgl::Application::active_application()->time() - _starting_time);
	}

	jgl::Float Timer::percent()
	{
		return jgl::clamp(0.0f, static_cast<jgl::Float>(elapsed_time()) / _timer_duration, 100.0f);
	}

	jgl::String to_string(Timer::State p_state)
	{
		switch (p_state)
		{
		case Timer::State::Stopped:
			return ("Stopped");
		case Timer::State::Running:
			return ("Running");
		case Timer::State::Timeout:
			return ("Timeout");
		}
		return ("Unknow");
	}
}

std::ostream& operator << (std::ostream& p_os, const jgl::Timer::State& p_state)
{
	switch (p_state)
	{
	case jgl::Timer::State::Stopped:
		p_os << "Stopped"; break;
	case jgl::Timer::State::Running:
		p_os << "Running"; break;
	case jgl::Timer::State::Timeout:
		p_os << "Timeout"; break;
	default :
		p_os << "Unknow"; break;
	}
	return (p_os);
}