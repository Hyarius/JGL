#pragma once

#include "structure/jgl_array.h"
#include "structure/jgl_image.h"
#include "jgl_drawing_function.h"

namespace jgl
{
	class Sprite_sheet : public jgl::Image
	{
	private:
		jgl::Vector2Uint _size;
		jgl::Vector2 _unit;
		jgl::Array<jgl::Vector2> _sprites;

	public:
		Sprite_sheet(jgl::String path, jgl::Vector2Uint p_size) : jgl::Image(path)
		{
			_size = p_size;
			_unit = jgl::Vector2(1.0f, 1.0f) / _size;

			jgl::Vector2	tmp = 0;
			if (p_size == 1)
				_sprites.push_back(0);
			else
			{
				while (tmp.y != _size.y)
				{
					jgl::Vector2 result = jgl::Vector2(tmp.x, tmp.y);
					_sprites.push_back(result / _unit);
					tmp.x++;
					if (tmp.x >= _size.x)
					{
						tmp.x = 0;
						tmp.y++;
					}
				}
			}
		}

		void draw(jgl::Uint sprite_id, jgl::Vector2Int pos, jgl::Vector2Int size,
			jgl::Float depth, jgl::Float alpha)
		{
			jgl::Image::draw(pos, size, _sprites[sprite_id], _unit, depth, alpha);
		}

		void draw(jgl::Vector2Int sprite, jgl::Vector2Int pos, jgl::Vector2Int size,
			jgl::Float depth, jgl::Float alpha)
		{
			jgl::Uint sprite_id = (_size.x * sprite.y) + sprite.x;
			jgl::Image::draw(pos, size, _sprites[sprite_id], _unit, depth, alpha);
		}
	};
}