#pragma once


#include "jgl_includes.h"
#include "jgl_basic_types.h"
#include "jgl_vector.h"
#include "jgl_color.h"
#include "jgl_exception.h"

namespace jgl
{
	class Viewport
	{
	private:

		const class Widget* _owner;

		static jgl::Map< jgl::Uint, bool> _allocated_id;
		jgl::Uint _id;
		Vector2Int _anchor;
		Vector2Int _area;

		~Viewport()
		{
			_allocated_id[_id] = false;
		}

	public:
		const jgl::Vector2Int cumuled_anchor() const;
		const jgl::Vector2Int anchor() const { return (_anchor); }

		const jgl::Vector2Int area() const { return (_area); }

		Viewport(class Widget* p_owner = nullptr);

		void set_geometry(jgl::Vector2Int p_anchor, jgl::Vector2Int p_area);

		void reset();

		void use();
	};
}
