#pragma once

#include "jgl.h"

namespace jgl
{
	class Debug_screen : public jgl::Widget
	{
	private:
		static const jgl::Size_t C_NB_LINES = 26;
		static const jgl::Size_t C_NB_COLLUMNS = 5;

		jgl::Text_label* _lines[C_NB_LINES][C_NB_COLLUMNS];

		jgl::Timer _update_timer;

		void _render();
		void _on_geometry_change();

		jgl::Bool _update();
		jgl::Bool _fixed_update();

	public:
		Debug_screen(jgl::Widget* p_parent);
		jgl::Timer& update_timer() { return (_update_timer); }

		void set_text(jgl::String p_text, jgl::Size_t p_collumns, jgl::Size_t p_line);
	};
}