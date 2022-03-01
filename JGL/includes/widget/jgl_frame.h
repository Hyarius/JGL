#pragma once

#include "jgl_widget_components.h"
#include "jgl_widget.h"
#include "jgl_basic_types.h"
#include "application/jgl_shader.h"
#include "structure/jgl_sprite_sheet.h"
#include "application/jgl_application.h"

namespace jgl
{
	class Frame : public jgl::Widget
	{
	protected:
		Widget_component::Box _box;

		void _on_geometry_change();
		void _render();

	public:
		Frame(jgl::Widget* p_parent = nullptr);

		Widget_component::Box& box() { return (_box); }
	};
}
