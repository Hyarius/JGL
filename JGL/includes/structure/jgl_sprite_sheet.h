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

		jgl::Vector2Uint size() const { return (_size); }

		void draw(jgl::Uint sprite_id, jgl::Vector2Int pos, jgl::Vector2Int size, jgl::Float depth, jgl::Float alpha);
		void draw(jgl::Vector2Int sprite, jgl::Vector2Int pos, jgl::Vector2Int size, jgl::Float depth, jgl::Float alpha);

		void prepare_draw(jgl::Array<jgl::Vector3>& vertex_array, jgl::Array<jgl::Vector2>& uv_array, jgl::Array<jgl::Float>& alpha_array, jgl::Array<jgl::Uint>& element_array,
			jgl::Uint sprite_id, jgl::Vector2Int pos, jgl::Vector2Int size, jgl::Float depth, jgl::Float alpha);
		void prepare_draw(jgl::Array<jgl::Vector3>& vertex_array, jgl::Array<jgl::Vector2>& uv_array, jgl::Array<jgl::Float>& alpha_array, jgl::Array<jgl::Uint>& element_array,
			jgl::Vector2Int sprite, jgl::Vector2Int pos, jgl::Vector2Int size, jgl::Float depth, jgl::Float alpha);
	};
}