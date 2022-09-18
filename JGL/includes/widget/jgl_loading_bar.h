#pragma once

#include "jgl.h"

namespace jgl
{
	class Loading_bar : public jgl::Widget
	{
	private:
		jgl::Widget_component::Box _box;
		jgl::Widget_component::Text_label _label;
		jgl::Color _bar_color;

		jgl::Timer _timer;

		void _render();
		void _on_geometry_change();

		jgl::Bool _update();
		jgl::Bool _fixed_update();

	public:
		Loading_bar(jgl::Widget* p_parent);

		void set_text(jgl::String p_text)
		{
			_label.set_text(p_text);
		}

		jgl::Widget_component::Box& box() { return (_box); }
		jgl::Widget_component::Text_label& label() { return (_label); }

		jgl::Timer& timer() { return (_timer); }

		void set_bar_color(jgl::Color p_color) { _bar_color = p_color; }
	};
}