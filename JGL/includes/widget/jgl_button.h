#pragma once

#include "jgl_widget.h"
#include "jgl_basic_types.h"
#include "structure/jgl_data_contener.h"
#include "jgl_widget_components.h"

namespace jgl
{
	class Button : public jgl::Widget
	{
	protected:
		std::function< void(jgl::Data_contener&) > _funct = nullptr;
		jgl::Data_contener _param = jgl::Data_contener();

		jgl::Bool _selected;
		Widget_component::Box _box[2];
		Widget_component::Text_label _label;

		bool _update();
		void _on_geometry_change();
		void _render();

	public:
		Button(std::function< void(jgl::Data_contener&) > p_funct = nullptr, jgl::Widget* p_parent = nullptr);
		Button(std::function< void(jgl::Data_contener&) > p_funct, jgl::Data_contener p_param, jgl::Widget* p_parent);

		void define_function(std::function< void(jgl::Data_contener&) > p_funct);
		void define_action(std::function< void(jgl::Data_contener&) > p_funct, jgl::Data_contener p_param);

		const std::function< void(jgl::Data_contener&) >& function() const { return (_funct); }
		const jgl::Data_contener& param() const { return (_param); }

		Widget_component::Box& selected_box() { return (_box[1]); }
		Widget_component::Box& unselected_box() { return (_box[0]); }
		Widget_component::Box* boxes() { return (_box); }
		Widget_component::Text_label& label() { return (_label); }
	};
}