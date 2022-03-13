#include "jgl.h"

namespace jgl
{

	void Text_entry::_on_geometry_change()
	{
		_box.set_geometry(_anchor, _area, _depth);
		_label.set_geometry(_anchor + _box.border_size() * 2, _area - _box.border_size() * 4, _depth + 1);
		if (_text_size != -1)
		{
			_label.set_text_size(_text_size);
		}
		_compute_high_cursor();
		_compute_low_cursor();
		_compute_text();
		_compute_cursor_pos();
	}

	bool Text_entry::_fixed_update()
	{
		if (jgl::Application::active_application()->mouse().get_button(jgl::Mouse_button::Left) == jgl::Input_status::Down)
		{
			if (_box.is_pointed() == true)
			{
				select();
			}
			else
			{
				unselect();
			}
		}
		if (_is_selected == true)
		{
			
		}
		return (false);
	}

	void Text_entry::_compute_high_cursor()
	{
		jgl::Vector2Int to_draw_size;

		for (_high_cursor = _cursor; _high_cursor < _text.size() && to_draw_size.x < _area.x - _box.border_size() * 4; _high_cursor++)
		{
			to_draw_size.x += _label.font()->calc_char_offset(_text[_high_cursor], _label.text_size()).x;
			if (to_draw_size.x >= _area.x - _box.border_size() * 4)
				_high_cursor--;
		}
		if (_high_cursor == _text.size())
			_high_cursor++;
	}

	void Text_entry::_compute_high_cursor_from_low()
	{
		jgl::Vector2Int to_draw_size;

		for (_high_cursor = _low_cursor; _high_cursor < _text.size() && to_draw_size.x < _area.x - _box.border_size() * 4; _high_cursor++)
		{
			to_draw_size.x += _label.font()->calc_char_offset(_text[_high_cursor], _label.text_size()).x;
			if (to_draw_size.x >= _area.x - _box.border_size() * 4)
				_high_cursor--;
		}
	}

	void Text_entry::_compute_low_cursor()
	{
		jgl::Vector2Int to_draw_size;

		for (_low_cursor = _high_cursor; _low_cursor > 0 && to_draw_size.x < _area.x - _box.border_size() * 4; _low_cursor--)
		{
			to_draw_size.x += _label.font()->calc_char_offset(_text[_low_cursor], _label.text_size()).x;
			if (to_draw_size.x >= _area.x - _box.border_size() * 4)
				_low_cursor++;
		}
	}

	void Text_entry::_compute_cursor_pos()
	{
		_cursor_pos.x = 0;
		for (jgl::Size_t i = _low_cursor; i < _text.size() && i < _cursor; i++)
		{
			if (_label.hidden() == true)
				_cursor_pos.x += _label.font()->calc_char_offset('*', _label.text_size()).x;
			else
				_cursor_pos.x += _label.font()->calc_char_offset(_text[i], _label.text_size()).x;
		}
	}

	void Text_entry::_compute_text()
	{
		jgl::String to_draw = "";

		for (jgl::Size_t i = _low_cursor; i < _high_cursor && i < _text.size(); i++)
			to_draw.push_back(_text[i]);

		_label.set_text_alone(to_draw);
	}

	bool Text_entry::_update()
	{
		if (_is_selected == true)
		{
			jgl::Glyph tmp_entry = jgl::Application::active_application()->keyboard().get_text_entry();
			jgl::Ulong actual_time = jgl::Application::active_application()->time();
			static jgl::Ulong next_input[4] = {
				actual_time,
				actual_time,
				actual_time,
				actual_time
			};
			static const jgl::Ulong delta_time = 100;

			if (jgl::Application::active_application()->keyboard().get_key(jgl::Key::Left_arrow) == jgl::Input_status::Down && next_input[0] <= actual_time && _cursor > 0)
			{
				_cursor--;
				if (_cursor < _low_cursor)
				{
					_low_cursor--;
					_compute_high_cursor_from_low();
				}
				_compute_cursor_pos();
				_compute_text();
				next_input[0] = actual_time + delta_time;
			}
			else if (jgl::Application::active_application()->keyboard().get_key(jgl::Key::Right_arrow) == jgl::Input_status::Down && next_input[1] <= actual_time && _cursor < _text.size())
			{
				_cursor++;
				if (_cursor > _high_cursor)
				{
					_low_cursor++;
					_compute_high_cursor_from_low();
				}
				_compute_cursor_pos();
				_compute_text();
				next_input[1] = actual_time + delta_time;
			}
			else if (jgl::Application::active_application()->keyboard().get_key(jgl::Key::Backspace) == jgl::Input_status::Down && _cursor > 0 && next_input[2] <= actual_time)
			{
				_text.erase(_cursor - 1);
				_cursor--;
				if (_cursor < _low_cursor)
				{
					_low_cursor--;
					_compute_high_cursor_from_low();
				}
				_compute_cursor_pos();
				_compute_text();
				next_input[2] = actual_time + delta_time;
			}
			else if (jgl::Application::active_application()->keyboard().get_key(jgl::Key::Delete) == jgl::Input_status::Down && _cursor < _text.size() && next_input[3] <= actual_time)
			{
				_text.erase(_cursor);
				_compute_high_cursor_from_low();
				_compute_text();
				next_input[3] = actual_time + delta_time;
			}
			else if (tmp_entry.is_printable() == true)
			{
				_text.insert(tmp_entry, _cursor);
				_cursor++;
				if (_cursor > _high_cursor || _label.text().size() == _high_cursor)
				{
					_high_cursor++;
					_compute_low_cursor();
				}
				_compute_cursor_pos();
				_compute_text();
			}
		}
		return (false);
	}


	void Text_entry::_render()
	{
		_box.render();
		_label.render();
		if (_is_selected == true)
		{
			if (((jgl::Application::active_application()->time() / 500) % 2) == 0)
				jgl::draw_rectangle_color(jgl::Color(0, 0, 0), _anchor + _cursor_pos + _label.text_pos() + _box.border_size() * 2, jgl::Vector2Int(3, _label.text_size()), depth() + 1);
		}
	}

	Text_entry::Text_entry(jgl::String p_text, jgl::Widget* p_parent) : jgl::Widget(p_parent)
	{
		_text = p_text;

		_cursor = 0;

		_box = jgl::Widget_component::Box(this);
		_box.set_color(jgl::Color(150, 150, 150), jgl::Color(100, 100, 100));
		_label = jgl::Widget_component::Text_label("", this);
		_label.set_vertical_align(jgl::Vertical_alignment::Down);
	}

	void Text_entry::set_text(jgl::String p_text)
	{
		_cursor = 0;
		_text = p_text;
		compute();
	}

	void Text_entry::reset_text(jgl::String p_text)
	{
		_cursor = p_text.size();
		_text = p_text;
		_label.set_text("");
		compute();
	}

	void Text_entry::select()
	{
		_is_selected = true;
	}

	void Text_entry::unselect()
	{
		_is_selected = false;
	}
}