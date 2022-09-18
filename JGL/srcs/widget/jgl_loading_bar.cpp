#include "widget/jgl_loading_bar.h"

namespace jgl
{
	void Loading_bar::_render()
	{
		_box.render();
			
		jgl::Vector2 size = (_area - _box.border_size() * 2);
		jgl::draw_rectangle_color(_bar_color, _anchor + _box.border_size(), size * jgl::Vector2(_timer.percent(), 1.0f), _depth + 3);

		_label.render();
	}

	void Loading_bar::_on_geometry_change()
	{
		_box.set_border_size(_area.y / 10);
		_box.set_geometry(_anchor, _area, _depth);
		_label.set_geometry(_anchor + _box.border_size(), _area - _box.border_size() * 2, _depth + 5);
	}

	jgl::Bool Loading_bar::_update()
	{
		return (false);
	}

	jgl::Bool Loading_bar::_fixed_update()
	{
		return (false);
	}

	Loading_bar::Loading_bar(jgl::Widget* p_parent) : jgl::Widget(p_parent)
	{
		_box = jgl::Widget_component::Box(this);
		_box.set_color(jgl::Color(150, 150, 150), jgl::Color(100, 100, 100));

		_label = jgl::Widget_component::Text_label("", this);
		_label.set_horizontal_align(jgl::Horizontal_alignment::Centred);
		_label.set_vertical_align(jgl::Vertical_alignment::Centred);
		_label.set_text_color(jgl::Color(150, 150, 150));
	}
}