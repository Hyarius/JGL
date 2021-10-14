#pragma once

#include "jgl_widget.h"
#include "jgl_basic_types.h"

namespace jgl
{
	class Contener : public jgl::Widget
	{
		void _on_geometry_change()
		{

		}

		void _render()
		{

		}

	public:
		Contener(jgl::Widget* p_parent = nullptr) : jgl::Widget(p_parent)
		{

		}
	};
}