#pragma once

#include "jgl_widget.h"

namespace jgl
{
	class Graphical_widget : public jgl::Widget
	{
	protected:
		void _on_geometry_change() = 0;
		void _render() = 0;

		jgl::Bool _update() final;
		jgl::Bool _fixed_update() final;

	public:
		Graphical_widget(jgl::Widget* p_parent);
	};
}