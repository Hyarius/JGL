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
		Sprite_sheet(jgl::String path, jgl::Vector2Uint p_size);

		const jgl::Vector2Uint& size() const { return (_size); }
		const jgl::Vector2& unit() const { return (_unit); }
		const jgl::Array<jgl::Vector2>& sprites() const { return (_sprites);}
		const jgl::Vector2& sprite(jgl::Size_t index) const { if (_sprites.size() <= index) THROW_EXCEPTION(jgl::Error_level::Error, 1, "Segfault"); return (_sprites[index]); }
		const jgl::Vector2& sprite(jgl::Vector2Uint sprite_value) const { jgl::Uint sprite_id = (_size.x * sprite_value.y) + sprite_value.x; return (sprite(sprite_id)); }
		const jgl::Vector2& sprite(jgl::Vector2Int sprite_value) const { jgl::Uint sprite_id = (_size.x * sprite_value.y) + sprite_value.x; return (sprite(sprite_id)); }

		void draw(jgl::Uint sprite_id, jgl::Vector2Int pos, jgl::Vector2Uint size, jgl::Float depth, jgl::Float alpha);
		void draw(jgl::Vector2Int sprite, jgl::Vector2Int pos, jgl::Vector2Uint size, jgl::Float depth, jgl::Float alpha);

		void prepare_draw(jgl::Array<jgl::Vector3>& vertex_array, jgl::Array<jgl::Vector2>& uv_array, jgl::Array<jgl::Float>& alpha_array, jgl::Array<jgl::Uint>& element_array,
			jgl::Uint sprite_id, jgl::Vector2Int pos, jgl::Vector2Uint size, jgl::Float depth, jgl::Float alpha);
		void prepare_draw(jgl::Array<jgl::Vector3>& vertex_array, jgl::Array<jgl::Vector2>& uv_array, jgl::Array<jgl::Float>& alpha_array, jgl::Array<jgl::Uint>& element_array,
			jgl::Vector2Int sprite, jgl::Vector2Int pos, jgl::Vector2Uint size, jgl::Float depth, jgl::Float alpha);
	};
}