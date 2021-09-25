#include "jgl.h"

namespace jgl
{
	void Text_label::_on_geometry_change()
	{
		_box.set_geometry(_anchor, _area, _depth);
		_label.set_geometry(_anchor + _box.border_size(), _area - _box.border_size() * 2, _depth);
	}
	void Text_label::_render()
	{
		_box.render();
		_label.render();
	}

	Text_label::Text_label(jgl::String p_text, jgl::Widget* p_parent) : jgl::Widget(p_parent)
	{
		_box = jgl::Widget_component::Box(jgl::Color(60, 60, 60), jgl::Color(100, 100, 100), this);
		_label = jgl::Widget_component::Text_label(p_text, this);
	}
}