#include "jgl.h"

namespace jgl
{
	void Frame::_on_geometry_change()
	{
		_box.set_geometry(_anchor, _area, _depth);
	}

	void Frame::_render()
	{
		_box.render();
	}

	Frame::Frame(jgl::Widget* p_parent) : jgl::Widget(p_parent)
	{
		_box = jgl::Widget_component::Box(this);
		_box.set_color(jgl::Color(150, 150, 150), jgl::Color(100, 100, 100));
	}
}