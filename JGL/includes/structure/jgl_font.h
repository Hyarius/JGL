#pragma once

#include "jgl_includes.h"
#include "jgl_basic_types.h"

#include "structure/jgl_vector.h"
#include "structure/jgl_color.h"
#include "structure/jgl_image.h"

#include "application/jgl_gl_object.h"

namespace jgl
{
	class Font : public jgl::Image_handler
	{
	private:
		struct Glyph_data
		{
			jgl::Size_t height;
			jgl::Vector2 positions[4];
			jgl::Vector2 uvs[4];
			jgl::Vector2 offset = 0;
		};

		jgl::Map<jgl::Glyph, Glyph_data> _atlas;

		jgl::Buffer* _vertex_buffer = nullptr;
		jgl::Buffer* _uvs_buffer = nullptr;
		jgl::Buffer* _color_buffer = nullptr;
		jgl::Buffer* _color_outline_buffer = nullptr;
		jgl::Buffer* _alpha_buffer = nullptr;
		jgl::Buffer* _indexes_buffer = nullptr;

		Glyph_data& _get_data(jgl::Glyph to_draw);
		
	public:
		Font(jgl::String path);
		~Font();

		void draw(jgl::Vector2Int pos, jgl::Vector2Int size, jgl::Float depth, jgl::Float alpha);

		jgl::Vector2Int prepare_render_char(
			jgl::Array<Vector3>& vertex_array, jgl::Array<jgl::Vector2>& uvs_array, jgl::Array<jgl::Color>& color_array, jgl::Array<jgl::Color>& color_outline_array, jgl::Array<jgl::Float>& alpha_array, jgl::Array<jgl::Uint>& index_array,
			jgl::Glyph to_draw, jgl::Vector2Int pos, jgl::Uint size, jgl::Float level, jgl::Float alpha, jgl::Color text_color, jgl::Color outline_color);

		jgl::Vector2Int prepare_render_text(
			jgl::Array<Vector3>& vertex_array, jgl::Array<jgl::Vector2>& uvs_array, jgl::Array<jgl::Color>& color_array, jgl::Array<jgl::Color>& color_outline_array, jgl::Array<jgl::Float>& alpha_array, jgl::Array<jgl::Uint>& index_array,
			jgl::String text, jgl::Vector2Int pos, jgl::Uint size, jgl::Float level, jgl::Float alpha, jgl::Color text_color, jgl::Color outline_color);

		jgl::Vector2Int draw_text(jgl::String text, jgl::Vector2Int pos, jgl::Uint size, jgl::Float depth, jgl::Float alpha, jgl::Color text_color);

		jgl::Vector2Int draw_char(jgl::Glyph to_draw, jgl::Vector2Int pos, jgl::Uint size, jgl::Float depth, jgl::Float alpha, jgl::Color text_color);

		jgl::Vector2Int draw_text(jgl::String text, jgl::Vector2Int pos, jgl::Uint size, jgl::Float depth, jgl::Float alpha, jgl::Color text_color, jgl::Color outline_color);

		jgl::Vector2Int draw_char(jgl::Glyph to_draw, jgl::Vector2Int pos, jgl::Uint size, jgl::Float depth, jgl::Float alpha, jgl::Color text_color, jgl::Color outline_color);

		jgl::Vector2Int calc_text_size(jgl::String text, jgl::Uint size);

		jgl::Vector2Int calc_char_size(jgl::Glyph to_draw, jgl::Uint size);

		jgl::Vector2Int calc_char_offset(jgl::Glyph text, jgl::Uint size);

	};
};