#pragma once

#include "structure/jgl_color.h"
#include "structure/jgl_vector.h"

namespace jgl
{
	jgl::Vector3 convert_screen_to_opengl(const jgl::Vector2Int source, jgl::Float level);
	jgl::Vector2Int convert_opengl_to_screen(const jgl::Vector2 source);
	jgl::Vector2Int convert_opengl_to_screen(const jgl::Vector3 source);

	void draw_triangle_color(jgl::Color color, jgl::Vector2Int a, jgl::Vector2Int b, jgl::Vector2Int c, jgl::Float depth = 1);
	void draw_rectangle_color(jgl::Color color, jgl::Vector2Int pos, jgl::Vector2Int size, jgl::Float depth = 1);
	void prepare_rectangle_color(
		jgl::Array<jgl::Vector3>& vertex_content, jgl::Array<jgl::Color>& color_content, jgl::Array<jgl::Uint>& indexes_array,
		jgl::Color color, jgl::Vector2Int pos, jgl::Vector2Int size, jgl::Float depth = 1);
	void prepare_rectangle_color(
		jgl::Array<jgl::Vector3>* vertex_content, jgl::Array<jgl::Color>* color_content, jgl::Array<jgl::Uint>* indexes_array,
		jgl::Color color, jgl::Vector2Int pos, jgl::Vector2Int size, jgl::Float depth = 1);
	jgl::Vector2Int draw_text(jgl::String text, jgl::Vector2Int pos, jgl::Uint size, jgl::Float level, jgl::Float alpha, jgl::Color text_color, jgl::Color outline_color);
	jgl::Vector2Int prepare_render_text(jgl::Array<Vector3>& vertex_array, jgl::Array<jgl::Vector2>& uvs_array, jgl::Array<jgl::Color>& color_array, jgl::Array<jgl::Color>& color_outline_array, jgl::Array<jgl::Float>& alpha_array, jgl::Array<jgl::Uint>& index_array,
		jgl::String text, jgl::Vector2Int pos, jgl::Uint size, jgl::Float level, jgl::Float alpha, jgl::Color text_color, jgl::Color outline_color);
	jgl::Vector2Int prepare_render_text(jgl::Array<Vector3>* vertex_array, jgl::Array<jgl::Vector2>* uvs_array, jgl::Array<jgl::Color>* color_array, jgl::Array<jgl::Color>* color_outline_array, jgl::Array<jgl::Float>* alpha_array, jgl::Array<jgl::Uint>* index_array,
		jgl::String text, jgl::Vector2Int pos, jgl::Uint size, jgl::Float level, jgl::Float alpha, jgl::Color text_color, jgl::Color outline_color);

	void cast_draw_color(jgl::Array<jgl::Vector3>& vertex_content, jgl::Array<jgl::Color>& color_content, jgl::Array<jgl::Uint>& indexes_array);
}