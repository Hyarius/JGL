#pragma once

#include "jgl_includes.h"
#include "jgl_basic_types.h"

namespace jgl
{
	class Timer
	{
	public:
		enum class State
		{
			Stopped,
			Timeout,
			Running
		};
	private:
		State _state;
		jgl::Ulong _starting_time;
		jgl::Ulong _timer_duration;

	public:
		Timer(jgl::Ulong p_timer_period = 1000);

		void set_duration(jgl::Ulong p_timer_period);
		jgl::Ulong duration() { return (_timer_duration); }
		void start();
		void stop();
		State timeout();
	};
}