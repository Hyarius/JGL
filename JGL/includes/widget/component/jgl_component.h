#pragma once

#include "../jgl_widget.h"

namespace jgl::Widget_component
{
	class Component
	{
	protected:
		jgl::Widget* _owner;
	
	public:
		Component(jgl::Widget *p_owner)
		{
			_owner = p_owner;
		}
	};

	class Graphical_component : public Component
	{
	protected:
		jgl::Vector2Int _anchor = 0;
		jgl::Vector2Int _area = 0;
		jgl::Float _depth = 1;

	public:
		Graphical_component(jgl::Widget* p_owner);

		jgl::Vector2Int anchor() { return (_anchor); }
		jgl::Vector2Int area() { return (_area); }

		bool is_pointed();

		void set_geometry(jgl::Vector2Int p_anchor, jgl::Vector2Int p_area, jgl::Float p_depth);

		void set_depth(jgl::Float p_depth);
	};
}