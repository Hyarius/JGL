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

	Frame::Frame(jgl::Color p_back, jgl::Color p_front, jgl::Widget* p_parent) : jgl::Widget(p_parent)
	{
		_box = jgl::Widget_component::Box(p_back, p_front, this);
	}

	Frame::Frame(jgl::String p_path, jgl::Widget* p_parent) : jgl::Widget(p_parent)
	{
		_box = jgl::Widget_component::Box(p_path, this);
	}

	Frame::Frame(jgl::Sprite_sheet* p_background_tileset, jgl::Widget* p_parent) : jgl::Widget(p_parent)
	{
		_box = jgl::Widget_component::Box(p_background_tileset, this);
	}
}