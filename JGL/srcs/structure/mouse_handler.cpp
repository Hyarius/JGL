#include "jgl.h"

namespace jgl
{
	Mouse::Mouse()
	{
		_pos = 0;
		for (jgl::Size_t i = 0; i < MOUSE_WHEEL_AXIS_COUNT; i++)
			_wheel[i] = 0;
		for (jgl::Size_t i = 0; i < MOUSE_BUTTON_COUNT; i++)
			_button[i] = jgl::Input_status::Up;
	}
	Input_status Mouse::get_button(const Mouse_button button)
	{
		return (_button[static_cast<jgl::Int>(button)]);
	}
	void Mouse::place(jgl::Vector2Int p_pos)
	{
		_pos = p_pos;
	}
}