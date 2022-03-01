#pragma once

#include "jgl_widget.h"
#include "jgl_basic_types.h"
#include "component/jgl_box_component.h"

namespace jgl
{
	class Sprite_label : public jgl::Widget
	{
	protected:
		jgl::Widget_component::Box _box;
		jgl::Sprite_sheet* _sprite_sheet;
		jgl::Vector2Int _sprite;

		void _on_geometry_change()
		{
			_box.set_geometry(_anchor, _area, _depth);
		}

		void _render()
		{
			_box.render();
			if (_sprite_sheet != nullptr)
				_sprite_sheet->draw(_sprite, _anchor, _area, _depth + 1, 1.0f);
		}

	public:
		Sprite_label(jgl::Sprite_sheet* p_sprite_sheet, jgl::Widget *p_parent = nullptr) : jgl::Widget(p_parent)
		{
			_sprite_sheet = p_sprite_sheet;
			_sprite = 0;
		}

		void set_sprite_sheet(jgl::Sprite_sheet* p_sprite_sheet)
		{
			_sprite_sheet = p_sprite_sheet;
		}

		void set_sprite(jgl::Vector2Int p_sprite)
		{
			_sprite = p_sprite;
		}
	};
}