#pragma once

#include "jgl_includes.h"
#include "jgl_basic_types.h"

namespace jgl
{
	class Timer
	{
	private:
		jgl::Bool _started;
		jgl::Ulong _starting_time;
		jgl::Ulong _timer_duration;

	public:
		Timer(jgl::Ulong p_timer_period = 1000);

		jgl::Bool started() { return (_started);}

		void start();
		void stop();
		jgl::Bool timeout();
	};
}