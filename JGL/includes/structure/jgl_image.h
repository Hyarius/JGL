#pragma once

#include "jgl_define.h"
#include "jgl_drawing_function.h"
#include "application/jgl_shader.h"
#include "application/jgl_gl_object.h"

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

		static Shader* _shader;

		static Buffer* _model_space_buffer;
		static Buffer* _vertexUV_buffer;
		static Buffer* _alpha_value_buffer;
		static Buffer* _element_index_buffer;
		static Uniform* _texture_uniform;

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

		static void draw(GLuint p_id, jgl::Vector2Int pos, jgl::Vector2Uint size, jgl::Vector2 uv_pos, jgl::Vector2 uv_size,
			jgl::Float depth, jgl::Float alpha);



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
		void draw(jgl::Vector2Int pos, jgl::Vector2Uint size, jgl::Vector2 uv_a, jgl::Vector2 uv_b, jgl::Vector2 uv_c, jgl::Vector2 uv_d,
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
		void draw(jgl::Vector2Int pos, jgl::Vector2Uint size, jgl::Vector2 uv_pos, jgl::Vector2 uv_size,
			jgl::Float depth, jgl::Float alpha);

		void prepare_draw(jgl::Array<jgl::Vector3>& vertex_array, jgl::Array<jgl::Vector2>& uv_array, jgl::Array<jgl::Float>& alpha_array, jgl::Array<jgl::Uint>& element_array,
			jgl::Vector2Int pos, jgl::Vector2Uint size, jgl::Vector2 uv_pos, jgl::Vector2 uv_size, jgl::Float depth, jgl::Float alpha);
	};

	class Image_output 
	{
	private:
		jgl::Vector2Uint _size;
		GLuint _frameBuffer;
		GLuint _id;
		GLenum _drawBuffers[1];

	public:
		Image_output(jgl::Vector2Uint p_size);

		Image_output(jgl::Uint p_width, jgl::Uint p_heigth);

		GLuint id() const { return (_id); }

		void associate();

		void clear();

		void desassociate();

		jgl::Image* save();
	
		static void check_framebuffer_status(jgl::String msg);
	};
} 