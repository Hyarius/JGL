#pragma once

#include "jgl_widget.h"
#include "jgl_basic_types.h"

namespace jgl
{
	class Text_entry : public jgl::Widget
	{
	protected:
		jgl::String _text;

		jgl::Bool _is_selected = false;

		jgl::Vector2Int _cursor_pos = 0;
		jgl::Size_t _cursor = 0;
		jgl::Size_t _high_cursor = 0;
		jgl::Size_t _low_cursor = 0;

		jgl::Widget_component::Box _box;
		jgl::Int _text_size = -1;
		jgl::Widget_component::Text_label _label;

		void _compute_text();

		void _compute_high_cursor_from_low();
		void _compute_high_cursor();
		void _compute_low_cursor();
		void _compute_cursor_pos();

		void _on_geometry_change();
		bool _fixed_update();
		bool _update();
		void _render();

	public:
		Text_entry(jgl::String p_text = "", jgl::Widget* p_parent = nullptr);

		void set_text(jgl::String p_text);

		void reset_text(jgl::String p_text = "");

		void set_text_size(jgl::Int p_size) { _text_size = p_size; _label.set_text_size(p_size); }

		jgl::Widget_component::Box& box() { return (_box); }
		jgl::Widget_component::Text_label& label() { return (_label); }
		jgl::String text() const { return (_text); }

		jgl::Bool is_selected() { return (_is_selected); }
		void select();
		void unselect();
	};
}