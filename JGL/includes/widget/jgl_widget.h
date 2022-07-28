#pragma once

#include "structure/jgl_vector.h"

#include "structure/jgl_viewport.h"

namespace jgl
{
	class Application;

	class Widget
	{
	protected:
		friend class Application;

		jgl::Viewport* _viewport = nullptr;

		jgl::Widget* _parent = nullptr;
		jgl::Array<jgl::Widget*> _childrens;

		jgl::Bool _active;
		jgl::Float _depth;
		jgl::Vector2Int _area;
		jgl::Vector2Int _anchor;

		bool _update_children();

		bool _fixed_update_children();

		void _render_children();

		virtual void _on_geometry_change() = 0;
		virtual void _render() = 0;

		virtual bool _update();

		virtual bool _fixed_update();

		void _add_children(jgl::Widget* p_children);

		void _remove_children(jgl::Widget* p_children);

	public:
		Widget(jgl::Widget* p_parent);
		~Widget();

		const jgl::Viewport* viewport() const { return (_viewport); }

		const jgl::Widget* parent() const { return (_parent); }
		jgl::Float depth() { return (_depth); }
		jgl::Bool active() { return _active; }
		jgl::Vector2Int area() const { return (_area); }
		jgl::Vector2Int anchor() const { return (_anchor); }
		jgl::Vector2Int cumuled_anchor() const { return ((_parent == nullptr ? 0 : _anchor + _parent->cumuled_anchor())); }

		void set_active(jgl::Bool p_state) { _active = p_state; }
		void activate() { _active = true; }
		void desactivate() { _active = false; }

		virtual jgl::Bool is_pointed();

		void resize(jgl::Vector2Int p_anchor, jgl::Vector2Int p_area);

		void set_parent(jgl::Widget* p_parent);

		void set_depth(jgl::Float p_depth) {
			jgl::Float delta = p_depth - _depth; _depth = p_depth;

			for (jgl::Size_t i = 0; i < _childrens.size(); i++)
				_childrens[i]->set_depth(p_depth + 1.0f + delta);
		}
		void set_geometry(jgl::Vector2Int p_anchor, jgl::Vector2Int p_area);
	};
}