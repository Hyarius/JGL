#pragma once

#include "widget/jgl_widget.h"
#include "widget/jgl_button.h"
#include "widget/jgl_text_label.h"

namespace jgl
{
	template<typename TElement>
	class Selecter : public jgl::Widget
	{
	private:
		void _on_geometry_change()
		{
			jgl::Vector2Int button_size = _area.y;
			jgl::Vector2Int label_size = jgl::Vector2Int(_area.x - button_size.x * 2);

			_previous_button->set_geometry(0, button_size);
			_label->set_geometry(jgl::Vector2Int(button_size.x, 0), label_size);
			_next_button->set_geometry(jgl::Vector2Int(button_size.x + label_size.x, 0), button_size);
		}

		void _render()
		{

		}

		jgl::Button* _next_button;
		jgl::Text_label* _label;
		jgl::Button* _previous_button;

		jgl::Array<TElement> _array;
		jgl::Size_t _array_index;

		std::function< jgl::String(TElement&) > _funct;

		void _set_label_text()
		{
			_label->label().set_text(_funct(_array[_array_index]));
		}

	public:
		Selecter(std::function< jgl::String(TElement&) > p_funct, jgl::Widget* p_parent) : jgl::Widget(p_parent)
		{
			_funct = p_funct;

			_next_button = new jgl::Button([&](jgl::Data_contener& p_param) {
				if (_array_index == _array.size() - 1)
					_array_index = 0;
				else
					_array_index += 1;
				_set_label_text();
			}, this);
			_next_button->activate();

			_label = new jgl::Text_label("", this);
			_label->activate();

			_previous_button = new jgl::Button([&](jgl::Data_contener& p_param) {
				if (_array_index == 0)
					_array_index = _array.size() - 1;
				else
					_array_index -= 1;
				_set_label_text();
			}, this);
			_previous_button->activate();
		}

		void add_value(TElement new_value)
		{
			_array.push_back(new_value);
			if (_array.size() == 1)
				_set_label_text();
		}

		void set_index(jgl::Size_t p_index)
		{
			_array_index = p_index;
		}
	};
}