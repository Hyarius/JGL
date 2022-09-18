#include "Widget/jgl_debug_screen.h"

namespace jgl
{
	void Debug_screen::_render()
	{

	}

	void Debug_screen::_on_geometry_change()
	{
		jgl::Vector2 unit = _area.y / static_cast<jgl::Float>(C_NB_LINES + 2);

		jgl::Vector2 label_size = jgl::Vector2((_area.x - unit.x * 2) / static_cast<jgl::Float>(C_NB_COLLUMNS), unit.y);

		for (jgl::Size_t i = 0; i < C_NB_LINES; i++)
		{
			for (jgl::Size_t j = 0; j < C_NB_COLLUMNS; j++)
			{
				_lines[i][j]->set_geometry(unit + label_size * jgl::Vector2(j, i), label_size);
			}
		}
	}

	jgl::Bool Debug_screen::_update()
	{
		if (_update_timer.timeout() == jgl::Timer::State::Timeout)
		{
			_update_timer.start();
		}
		return (false);
	}

	jgl::Bool Debug_screen::_fixed_update()
	{
		return (false);
	}

	Debug_screen::Debug_screen(jgl::Widget* p_parent) : jgl::Widget(p_parent),
		_update_timer(40)
	{
		for (jgl::Size_t i = 0; i < C_NB_LINES; i++)
		{
			for (jgl::Size_t j = 0; j < C_NB_COLLUMNS; j++)
			{
				_lines[i][j] = new jgl::Text_label("", this);
				_lines[i][j]->box().set_color(jgl::Color::transparent(), jgl::Color::transparent());
				_lines[i][j]->label().set_text_color(jgl::Color::white());
				if (j == C_NB_COLLUMNS - 1)
					_lines[i][j]->label().set_horizontal_align(jgl::Horizontal_alignment::Right);
				else if (j == 0)
					_lines[i][j]->label().set_horizontal_align(jgl::Horizontal_alignment::Left);
				else
					_lines[i][j]->label().set_horizontal_align(jgl::Horizontal_alignment::Centred);

				_lines[i][j]->label().set_vertical_align(jgl::Vertical_alignment::Centred);

				_lines[i][j]->activate();
			}
		}
	}

	void Debug_screen::set_text(jgl::String p_text, jgl::Size_t p_collumns, jgl::Size_t p_line)
	{
		if (p_line >= C_NB_LINES || p_collumns >= C_NB_COLLUMNS)
			THROW_EXCEPTION(jgl::Error_level::Error, 1, "Debug_line out of range");
		_lines[p_line][p_collumns]->label().set_text(p_text);
	}
}