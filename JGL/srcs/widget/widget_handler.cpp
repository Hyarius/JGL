#include "jgl.h"

namespace jgl
{
	Widget::Widget(jgl::Widget* p_parent)
	{
		_parent = nullptr;
		_active = false;
		_area = 0;
		_anchor = 0;
		_depth = 0;
		_viewport = new jgl::Viewport(this);
		set_parent(p_parent);
	}

	Widget::~Widget()
	{

	}

	jgl::Bool Widget::is_pointed()
	{
		if (jgl::Application::active_application() == nullptr)
			THROW_EXCEPTION(jgl::Error_level::Error, 1, "No application created");

		Vector2Int pos1, pos2, mouse;

		pos1 = cumuled_anchor();

		pos2 = pos1 + _area;

		mouse = jgl::Application::active_application()->mouse().pos();

		if (mouse.x < pos1.x || mouse.x > pos2.x || mouse.y < pos1.y || mouse.y > pos2.y)
			return (false);
		return (true);
	}

	void Widget::resize(jgl::Vector2Int p_anchor, jgl::Vector2Int p_area)
	{
		jgl::Vector2 ratio = jgl::Vector2(static_cast<float>(p_area.x) / static_cast<float>(_area.x), static_cast<float>(p_area.y) / static_cast<float>(_area.y));
		if (ratio == 1)
			return;
		
		_viewport->set_geometry(_anchor, _area);

		for (jgl::Size_t i = 0; i < _childrens.size(); i++)
		{
			jgl::Vector2 tmp_anchor = jgl::Vector2(_childrens[i]->anchor().x, _childrens[i]->anchor().y);
			jgl::Vector2 tmp_area = jgl::Vector2(_childrens[i]->area().x, _childrens[i]->area().y);
			_childrens[i]->resize((tmp_anchor * ratio).round(), (tmp_area * ratio).round());
		}

		set_geometry(p_anchor, p_area);
	}

	void Widget::set_parent(jgl::Widget* p_parent)
	{
		if (_parent != nullptr)
		{
			_parent->_remove_children(this);
		}
		_parent = p_parent;
		if (_parent == nullptr)
			_parent = jgl::Application::active_application()->central_widget();

		if (_parent == nullptr)
			_depth = 0;
		else
			_depth = _parent->depth() + 1;
		if (_parent != nullptr)
		{
			_parent->_add_children(this);
		}
	}

	void Widget::set_geometry(jgl::Vector2Int p_anchor, jgl::Vector2Int p_area)
	{
		_anchor = p_anchor;
		_area = p_area;
		_viewport->set_geometry(_anchor, _area);
		if (_area.x > 0 && _area.y > 0)
			_on_geometry_change();
	}

	bool Widget::_update_children()
	{
		for (jgl::Size_t i = 0; i < _childrens.size(); i++)
		{
			if (_childrens[i]->active() == true)
				if (_childrens[i]->_update_children() == true)
					return (true);
		}
		return (_update());
	}

	bool Widget::_fixed_update_children()
	{
		for (jgl::Size_t i = 0; i < _childrens.size(); i++)
		{
			if (_childrens[i]->active() == true)
				if (_childrens[i]->_fixed_update_children() == true)
					return (true);
		}
		return (_fixed_update());
	}

	void Widget::_render_children()
	{
		_render();
		for (jgl::Size_t i = 0; i < _childrens.size(); i++)
		{
			_viewport->use();
			if (_childrens[i]->active() == true)
				_childrens[i]->_render_children();
		}
	}

	bool Widget::_update()
	{
		return (false);
	}

	bool Widget::_fixed_update()
	{
		return (false);
	}

	void Widget::_add_children(jgl::Widget* p_children)
	{
		_childrens.push_back(p_children);
	}

	void Widget::_remove_children(jgl::Widget* p_children)
	{
		auto tmp = _childrens.find(p_children);
		if (tmp != _childrens.end())
			_childrens.erase(tmp);
	}

}