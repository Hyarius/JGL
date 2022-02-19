#include "jgl.h"

namespace jgl
{
	std::mutex Stream::_console_mutex;

	void Stream::flush()
	{
		std::lock_guard<std::mutex> lock(_console_mutex);
		if (jgl::Thread::nb_thread() >= 1)
			std::cout << "[" << _prefix << "] - ";
		std::cout << _content.str() << std::endl;
		_content.clear();
		_content.str("");
	}

	Stream::Stream(jgl::String p_prefix)
	{
		set_prefix(p_prefix);
	}

	void Stream::set_prefix(jgl::String p_prefix)
	{
		_prefix = p_prefix;
	}
}