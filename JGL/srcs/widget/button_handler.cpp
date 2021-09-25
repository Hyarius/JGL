#include "jgl.h"

namespace jgl
{
	bool Button::_update()
	{
		if (_box[0].is_pointed() == true && jgl::Application::active_application()->mouse().get_button(jgl::Mouse_button::Left) == jgl::Input_status::Release)
		{
			if (_funct != nullptr)
			{
				_funct(_param);
			}
			return (true);
		}

		if (_selected == false && _box[0].is_pointed() == true && jgl::Application::active_application()->mouse().get_button(jgl::Mouse_button::Left) == jgl::Input_status::Down)
		{
			_selected = true;
		}
		else if (_selected == true && (jgl::Application::active_application()->mouse().get_button(jgl::Mouse_button::Left) != jgl::Input_status::Down || _box[0].is_pointed() == false))
		{
			_selected = false;
		}
		return (false);
	}

	void Button::_on_geometry_change()
	{
		_box[0].set_geometry(_anchor, _area, _depth);
		_box[1].set_geometry(_anchor, _area, _depth);
		_label.set_geometry(_anchor, _area, _depth);
	}

	void Button::_render()
	{
		if (_selected == true)
			_box[1].render();
		else
			_box[0].render();
		_label.render();
	}

	Button::Button(std::function< void(jgl::Data_contener) > p_funct, jgl::Data_contener p_param, jgl::Widget* p_parent) : jgl::Widget(p_parent)
	{
		_funct = p_funct;
		_param = p_param;
		_box[0] = jgl::Widget_component::Box(jgl::Color(100, 100, 100), jgl::Color(150, 150, 150), this);
		_box[1] = jgl::Widget_component::Box(jgl::Color(60, 60, 60), jgl::Color(100, 100, 100), this);
		_label = jgl::Widget_component::Text_label("Click me", this);
		_label.set_horizontal_align(jgl::Horizontal_alignment::Centred);
		_label.set_vertical_align(jgl::Vertical_alignment::Centred);
	}

	void Button::define_action(std::function< void(jgl::Data_contener) > p_funct, jgl::Data_contener p_param)
	{
		_funct = p_funct;
		_param = p_param;
	}
}