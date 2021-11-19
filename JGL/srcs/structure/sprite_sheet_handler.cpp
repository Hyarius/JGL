#include "jgl.h"

namespace jgl
{
	Sprite_sheet::Sprite_sheet(jgl::String path, jgl::Vector2Uint p_size) : jgl::Image(path)
	{
		_size = p_size;
		_unit = jgl::Vector2(1.0f, 1.0f) / _size;

		jgl::Vector2	tmp = 0;
		if (p_size == 1)
			_sprites.push_back(0);
		else
		{
			for (jgl::Size_t j = 0; j < _size.y; j++)
				for (jgl::Size_t i = 0; i < _size.x; i++)
				{
					_sprites.push_back(jgl::Vector2(i, j) * _unit);
				}
		}
	}

	void Sprite_sheet::draw(jgl::Uint sprite_id, jgl::Vector2Int pos, jgl::Vector2Uint size,
		jgl::Float depth, jgl::Float alpha)
	{
		jgl::Image::draw(pos, size, _sprites[sprite_id], _unit, depth, alpha);
	}

	void Sprite_sheet::draw(jgl::Vector2Int sprite, jgl::Vector2Int pos, jgl::Vector2Uint size,
		jgl::Float depth, jgl::Float alpha)
	{
		jgl::Uint sprite_id = (_size.x * sprite.y) + sprite.x;
		jgl::Image::draw(pos, size, _sprites[sprite_id], _unit, depth, alpha);
	}


	void Sprite_sheet::prepare_draw(jgl::Array<jgl::Vector3>& vertex_array, jgl::Array<jgl::Vector2>& uv_array, jgl::Array<jgl::Float>& alpha_array, jgl::Array<jgl::Uint>& element_array,
		jgl::Uint sprite_id, jgl::Vector2Int pos, jgl::Vector2Uint size, jgl::Float depth, jgl::Float alpha)
	{
		jgl::Image::prepare_draw(vertex_array, uv_array, alpha_array, element_array, pos, size, _sprites[sprite_id], _unit, depth, alpha);
	}

	void Sprite_sheet::prepare_draw(jgl::Array<jgl::Vector3>& vertex_array, jgl::Array<jgl::Vector2>& uv_array, jgl::Array<jgl::Float>& alpha_array, jgl::Array<jgl::Uint>& element_array,
		jgl::Vector2Int sprite, jgl::Vector2Int pos, jgl::Vector2Uint size, jgl::Float depth, jgl::Float alpha)
	{
		jgl::Uint sprite_id = (_size.x * sprite.y) + sprite.x;
		jgl::Image::prepare_draw(vertex_array, uv_array, alpha_array, element_array, pos, size, _sprites[sprite_id], _unit, depth, alpha);
	}

}