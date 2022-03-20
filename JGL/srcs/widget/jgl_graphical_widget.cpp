#include "jgl.h"

namespace jgl
{
	Graphical_widget::Graphical_widget(jgl::Widget* p_parent) : jgl::Widget(p_parent)
	{

	}

	jgl::Bool Graphical_widget::_update()
	{
		return (false);
	}

	jgl::Bool Graphical_widget::_fixed_update()
	{
		return (false);
	}
}