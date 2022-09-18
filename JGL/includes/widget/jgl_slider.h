#pragma once

#include "widget/jgl_widget_components.h"
#include "widget/jgl_button.h"
#include "structure/jgl_timer.h"

namespace jgl
{
	class Slider : public jgl::Widget
	{
	private:
		class Slidder_bar : public jgl::Widget
		{
		private:
			jgl::Widget_component::Box _box;

			void _render();
			void _on_geometry_change();

			jgl::Bool _update();
			jgl::Bool _fixed_update();

		public:
			Slidder_bar(jgl::Widget* p_parent);
		};

		jgl::Button* _up_button;
		Slidder_bar* _slidder_bar;
		jgl::Button* _down_button;

		jgl::Array<jgl::Widget*> _subscribed_widget;
		jgl::Array<jgl::Vector2Int> _subscribed_widget_base_anchor;

		jgl::Timer _motion_timer;

		jgl::Vector2Int _window_size;
		jgl::Vector2Int _min;
		jgl::Vector2Int _max;
		jgl::Vector2Int _delta;

		void _render();
		void _on_geometry_change();

		jgl::Bool _update();
		jgl::Bool _fixed_update();

		void _move_widget(jgl::Vector2Int p_delta);

	public:
		Slider(jgl::Widget* p_parent);
		void configure_window_size(jgl::Vector2Int p_window_size)
		{
			_window_size = p_window_size;
		}
		void subscribe(jgl::Widget* p_widget);
		void update_anchor();
	};
}