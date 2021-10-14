#pragma once

#include "jgl_define.h"
#include "jgl_drawing_function.h"

namespace jgl
{
	class Image_handler
	{
	protected:
		GLuint _id;

		void _draw(
			jgl::Vector2Int pos_a, jgl::Vector2Int pos_b, jgl::Vector2Int pos_c,
			jgl::Vector2 uv_a, jgl::Vector2 uv_b, jgl::Vector2 uv_c,
			jgl::Float depth, jgl::Float alpha);

	public:
		Image_handler();

		Image_handler(jgl::String path);

		Image_handler(GLuint p_id);

		~Image_handler();

		const jgl::Uint id() { return (_id); }

		void activate(GLenum texture = GL_TEXTURE0)
		{
			glActiveTexture(texture);
			glBindTexture(GL_TEXTURE_2D, _id);
		}

		void desactivate(GLenum texture = GL_TEXTURE0)
		{
			glActiveTexture(texture);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	};

	class Image : public Image_handler
	{
	protected:

	public:
		Image(GLuint p_id);

		Image(jgl::String path);

		~Image();

		void draw(jgl::Vector2Int pos_a, jgl::Vector2Int pos_b, jgl::Vector2Int pos_c,
			jgl::Vector2 uv_a, jgl::Vector2 uv_b, jgl::Vector2 uv_c,
			jgl::Float depth, jgl::Float alpha);

		/*
			Draw a rect as followed :
			Pos					Pos + size.x
					----------
					|        |
					|        |
					----------
			Pos + size.y		Pos + size

			with UVs as follow
				UV_a		  UV_b
					----------
					|        |
					|        |
					----------
				UV_c		  UV_d
		*/
		void draw(jgl::Vector2Int pos, jgl::Vector2Int size, jgl::Vector2 uv_a, jgl::Vector2 uv_b, jgl::Vector2 uv_c, jgl::Vector2 uv_d,
			jgl::Float depth, jgl::Float alpha);


		/*
			Draw a rect as followed :
			Pos					Pos + size.x
					----------
					|        |
					|        |
					----------
			Pos + size.y		Pos + size

			with UVs as follow
			uv_Pos					uv_Pos + uv_size.x
						----------
						|        |
						|        |
						----------
			uv_Pos + uv_size.y		uv_Pos + uv_size
		*/
		void draw(jgl::Vector2Int pos, jgl::Vector2Int size, jgl::Vector2 uv_pos, jgl::Vector2 uv_size,
			jgl::Float depth, jgl::Float alpha);

		void prepare_draw(jgl::Array<jgl::Vector3>& vertex_array, jgl::Array<jgl::Vector2>& uv_array, jgl::Array<jgl::Float>& alpha_array, jgl::Array<jgl::Uint>& element_array,
			jgl::Vector2Int pos, jgl::Vector2Int size, jgl::Vector2 uv_pos, jgl::Vector2 uv_size, jgl::Float depth, jgl::Float alpha);
	};
}