#pragma once

#include "jgl_widget.h"
#include "jgl_basic_types.h"

namespace jgl
{
	class Text_label : public jgl::Widget
	{
	protected:
		jgl::Widget_component::Text_label _label;
		jgl::Widget_component::Box _box;

		void _on_geometry_change();
		void _render();

	public:
		Text_label(jgl::String p_text, jgl::Widget* p_parent);

		jgl::Widget_component::Text_label& label() { return (_label); }
		jgl::Widget_component::Box& box() { return (_box); }
	};
}