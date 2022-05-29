#pragma once

#include "jgl_widget.h"
#include "jgl_button.h"
#include "jgl_text_label.h"

namespace jgl
{
	template<typename T>
	class Selecter : public jgl::Widget
	{
	private:
		jgl::Array<T> _values;
		jgl::Size_t _value_index;

		jgl::Int _space;

		std::function< jgl::String(T&) > _convert_value_to_string;

		jgl::Button _previous_button;
		jgl::Text_label _text_label;
		jgl::Button _next_button;

		void _on_geometry_change()
		{
			if (area() != 0)
			{
				jgl::Vector2Int button_size = jgl::Vector2Int(_area.y < _area.x / 3 ? _area.y : _area.x / 3, _area.y);
				jgl::Float space = (_space == -1 ? button_size.x / 10 : _space);
				jgl::Vector2Int label_size = jgl::Vector2Int(_area.x - button_size.x * 2 - space * 2, _area.y);

				_previous_button.set_geometry(0, button_size);
				_text_label.set_geometry(jgl::Vector2Int(button_size.x + space, 0), label_size);
				_next_button.set_geometry(jgl::Vector2Int(button_size.x + label_size.x + space * 2, 0), button_size);
			}
		}

		void _render()
		{

		}

		void _calc_label_text()
		{
			if (_values.size() <= _value_index)
				return;
			_text_label.label().set_text(_convert_value_to_string(_values[_value_index]));
		}

	public:
		Selecter(std::function< jgl::String(T&) > p_convert_value_to_string, jgl::Widget* p_parent) :
			jgl::Widget(p_parent),
			_convert_value_to_string(p_convert_value_to_string),
			_previous_button([&](jgl::Data_contener& p_param) {
				if (_values.size() != 0)
				{
					if (_value_index == 0)
						_value_index = _values.size() - 1;
					else
						_value_index--;
					_calc_label_text();
				}
			}, this),
			_text_label("", this),
			_next_button([&](jgl::Data_contener& p_param) {
				if (_values.size() != 0)
				{
					_value_index++;
					if (_value_index == _values.size())
						_value_index = 0;
					_calc_label_text();
				}
			}, this),
			_space(-1)
		{
			_previous_button.label().set_text("<");
			_previous_button.activate();
			_text_label.label().set_horizontal_align(jgl::Horizontal_alignment::Centred);
			_text_label.label().set_vertical_align(jgl::Vertical_alignment::Centred);
			_text_label.activate();
			_calc_label_text();
			_next_button.label().set_text(">");
			_next_button.activate();
		}

		void add_value(T p_value)
		{
			_values.push_back(p_value);
			if (_values.size() == 1)
				_calc_label_text();
		}

		void set_index(jgl::Size_t p_index)
		{
			_value_index = p_index;
			_calc_label_text();
		}

		void set_text(jgl::String p_previous_text, jgl::Vector2Int p_next_text)
		{
			_previous_button.label().set_text(p_previous_text);
			_next_button.label().set_text(p_next_text);
		}

		void set_space(jgl::Int p_space)
		{
			_space = p_space;
		}
	};
}