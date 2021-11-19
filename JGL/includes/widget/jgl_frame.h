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
	private:
		Widget_component::Box _box;

		void _on_geometry_change();
		void _render();

	public:
		Frame(jgl::Color p_back, jgl::Color p_front, jgl::Widget* p_parent = nullptr);
		Frame(jgl::String p_path, jgl::Widget* p_parent = nullptr);
		Frame(jgl::Sprite_sheet* p_background_tileset, jgl::Widget* p_parent = nullptr);

		Widget_component::Box& box() { return (_box); }
	};
}
