#pragma once

#include "jgl_basic_types.h"
#include "jgl_enum.h"
#include "structure/jgl_vector.h"

namespace jgl
{
	class Mouse
	{
	private:
		friend class Application;
		jgl::Vector2Int _pos;
		jgl::Float _wheel[MOUSE_WHEEL_AXIS_COUNT];
		jgl::Ulong _wheel_timer[MOUSE_WHEEL_AXIS_COUNT];
		jgl::Input_status _button[MOUSE_BUTTON_COUNT];

		void _update();

	public:
		/*
			Default constructor
		*/
		Mouse();
		
		/*
			Return the actual position
		*/
		jgl::Vector2Int pos() { return (_pos); }

		/*
			Return the wheel value 
		*/
		jgl::Float wheel(jgl::Mouse_wheel_axis axis = jgl::Mouse_wheel_axis::Vertical) { return (_wheel[static_cast<jgl::Int>(axis)]); }

		/*
			Return the status of a desired mouse button
		*/
		jgl::Input_status get_button(const jgl::Mouse_button button);

		/*
			Place the mouse at the desired position
		*/
		void place(jgl::Vector2Int p_pos);

		/*
			Method used by the WinProc function
			/!\ DO NOT OVERLOAD :!\
		*/
		friend void actualize_button(jgl::Mouse& mouse, jgl::Mouse_button p_button, jgl::Input_status p_status);
		/*
			Method used by the WinProc function
			/!\ DO NOT OVERLOAD :!\
		*/
		friend void actualize_wheel(jgl::Mouse& mouse, jgl::Mouse_wheel_axis p_axis, jgl::Float delta);
		/*
			Method used by the WinProc function
			/!\ DO NOT OVERLOAD :!\
		*/
		friend void actualize_pos(jgl::Mouse& mouse, jgl::Vector2Int delta);
	};
}