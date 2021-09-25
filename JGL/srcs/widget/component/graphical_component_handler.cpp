#include "jgl.h"

namespace jgl::Widget_component
{
	Graphical_component::Graphical_component(jgl::Widget* p_owner) : Component(p_owner)
	{
		_anchor = 0;
		_area = 0;
		_depth = 0;
	}

	bool Graphical_component::is_pointed()
	{
		if (_owner == nullptr)
			return (false);

		Vector2Int pos1, pos2, mouse;

		pos1 = _owner->cumuled_anchor();

		pos2 = pos1 + _area;

		mouse = jgl::Application::active_application()->mouse().pos();

		if (mouse.x < pos1.x || mouse.x > pos2.x || mouse.y < pos1.y || mouse.y > pos2.y)
			return (false);
		return (true);
	}

	void Graphical_component::set_geometry(jgl::Vector2Int p_anchor, jgl::Vector2Int p_area, jgl::Float p_depth)
	{
		_anchor = p_anchor;
		_area = p_area;
		_depth = p_depth;
	}

	void Graphical_component::set_depth(jgl::Float p_depth)
	{
		_depth = p_depth;
	}
}