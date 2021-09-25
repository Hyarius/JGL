#include "jgl.h"

namespace jgl
{
	void _jgl_string_raise_exception(const char* msg)
	{
		THROW_EXCEPTION(jgl::Error_level::Error, 1, jgl::String(msg));
	}
}