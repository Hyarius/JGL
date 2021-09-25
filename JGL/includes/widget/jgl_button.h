#pragma once

#include "jgl_widget.h"
#include "jgl_basic_types.h"
#include "structure/jgl_data_contener.h"
#include "jgl_widget_components.h"

namespace jgl
{
	class Button : public jgl::Widget
	{
	private:
		std::function< void(jgl::Data_contener) > _funct = nullptr;
		jgl::Data_contener _param = jgl::Data_contener();

		jgl::Bool _selected;
		Widget_component::Box _box[2];
		Widget_component::Text_label _label;

		bool _update();
		void _on_geometry_change();
		void _render();

	public:
		Button(std::function< void(jgl::Data_contener) > p_funct = nullptr, jgl::Data_contener p_param = jgl::Data_contener(), jgl::Widget* p_parent = nullptr);

		void define_action(std::function< void(jgl::Data_contener) > p_funct, jgl::Data_contener p_param);

		Widget_component::Box& selected_box() { return (_box[1]); }
		Widget_component::Box& unselected_box() { return (_box[0]); }
		Widget_component::Box* boxes() { return (_box); }
		Widget_component::Text_label& label() { return (_label); }
	};
}