#include "jgl.h"

#define STB_IMAGE_IMPLEMENTATION
#include "external_lib/stb_image.h"

#include "jgl_drawing_function.h"

namespace jgl
{
	void Image_handler::_draw(
		jgl::Vector2Int pos_a, jgl::Vector2Int pos_b, jgl::Vector2Int pos_c,
		jgl::Vector2 uv_a, jgl::Vector2 uv_b, jgl::Vector2 uv_c,
		jgl::Float depth, jgl::Float alpha)
	{
		if (_id == -1 || jgl::Application::active_application() == nullptr)
			THROW_EXCEPTION(jgl::Error_level::Error, 1, "Trying to print an image non-initialized or with no application created");

		activate();

		const jgl::String shader_name = "Texture_shader_2D";

		static jgl::Uint element_index[3] = { 0, 3, 1 };

		Vector3 vertex_content[3] = {
			convert_screen_to_opengl(pos_a, depth),
			convert_screen_to_opengl(pos_b, depth),
			convert_screen_to_opengl(pos_c, depth)
		};
		Vector2 uv_content[3] = {
			uv_a, uv_b, uv_c
		};
		Float alpha_content[3] = {
			alpha, alpha, alpha
		};

		jgl::Shader* tmp_shader = jgl::Application::active_application()->shader(shader_name);

		if (tmp_shader == nullptr)
			THROW_EXCEPTION(jgl::Error_level::Error, 0, "No shader named " + shader_name);

		tmp_shader->buffer("model_space")->send(vertex_content, 3);
		tmp_shader->buffer("vertexUV")->send(uv_content, 3);
		tmp_shader->buffer("alpha_value")->send(alpha_content, 3);
		tmp_shader->indexes_buffer()->send(element_index, 3);
		tmp_shader->uniform("textureID")->send(0);
		tmp_shader->launch(jgl::Shader::Mode::Triangle);

		desactivate();
	}
	Image_handler::Image_handler()
	{
		_id = -1;
	}
	Image_handler::Image_handler(jgl::String path)
	{
		jgl::Int width = 0, height = 0, nrChannels = 0;

		unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);

		if (data == nullptr)
			THROW_EXCEPTION(jgl::Error_level::Warning, 1, "Can't load file " + path);

		glGenTextures(1, &_id);
		glBindTexture(GL_TEXTURE_2D, _id);
		if (nrChannels == 3)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		else if (nrChannels == 4)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		if (jgl::Application::active_application() == nullptr)
			THROW_EXCEPTION(jgl::Error_level::Critical, 0, "No application started while loading an image");
		THROW_INFORMATION("Creating a new texture with id [" + jgl::itoa(_id) + "]");
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	}
	Image_handler::Image_handler(GLuint p_id)
	{
		_id = p_id;
	}

	Image_handler::~Image_handler()
	{
		if (_id != -1)
			glDeleteTextures(1, &_id);
	}

	Image::Image(GLuint p_id) : jgl::Image_handler(p_id)
	{

	}
	Image::Image(jgl::String path) : jgl::Image_handler(path)
	{

	}

	Image::~Image()
	{

	}

	void Image::draw(jgl::Vector2Int pos_a, jgl::Vector2Int pos_b, jgl::Vector2Int pos_c,
		jgl::Vector2 uv_a, jgl::Vector2 uv_b, jgl::Vector2 uv_c,
		jgl::Float depth, jgl::Float alpha)
	{
		_draw(pos_a, pos_b, pos_c,
			uv_a, uv_b, uv_c,
			depth, alpha);
	}

	static jgl::Uint element_index[6] = { 0, 3, 1, 2, 3, 0 };
	static jgl::Vector2Int delta_pos[4] = {
		jgl::Vector2Int(0, 0),
		jgl::Vector2Int(1, 0),
		jgl::Vector2Int(0, 1),
		jgl::Vector2Int(1, 1)
	};

	void Image::draw(jgl::Vector2Int pos, jgl::Vector2Int size, jgl::Vector2 uv_a, jgl::Vector2 uv_b, jgl::Vector2 uv_c, jgl::Vector2 uv_d, jgl::Float depth, jgl::Float alpha)
	{
		activate();

		const jgl::String shader_name = "Texture_shader_2D";

		Vector3 vertex_content[4];
		Vector2 uv_content[4];
		Float alpha_content[4];


		for (size_t i = 0; i < 4; i++)
		{
			vertex_content[i] = convert_screen_to_opengl(pos + size * delta_pos[i], depth);
			alpha_content[i] = alpha;
		}
		uv_content[0] = uv_a;
		uv_content[1] = uv_b;
		uv_content[2] = uv_c;
		uv_content[3] = uv_d;

		jgl::Shader* tmp_shader = jgl::Application::active_application()->shader(shader_name);

		if (tmp_shader == nullptr)
			THROW_EXCEPTION(jgl::Error_level::Error, 0, "No shader named " + shader_name);

		tmp_shader->buffer("model_space")->send(vertex_content, 4);
		tmp_shader->buffer("vertexUV")->send(uv_content, 4);
		tmp_shader->buffer("alpha_value")->send(alpha_content, 4);
		tmp_shader->indexes_buffer()->send(element_index, 6);
		tmp_shader->uniform("textureID")->send(static_cast<GLint>(0));
		tmp_shader->launch(jgl::Shader::Mode::Triangle);

		desactivate();
	}

	void Image::draw(jgl::Vector2Int pos, jgl::Vector2Int size, jgl::Vector2 uv_pos, jgl::Vector2 uv_size, jgl::Float depth, jgl::Float alpha)
	{

		const jgl::String shader_name = "Texture_shader_2D";

		//static jgl::Uint element_index[6] = { 0, 3, 1, 2, 3, 0 };
		static jgl::Vector2Int delta_pos[4] = {
			jgl::Vector2Int(0, 0),
			jgl::Vector2Int(1, 0),
			jgl::Vector2Int(0, 1),
			jgl::Vector2Int(1, 1)
		};

		Vector3 vertex_content[4];
		Vector2 uv_content[4];
		Float alpha_content[4];

		for (size_t i = 0; i < 4; i++)
		{
			vertex_content[i] = (convert_screen_to_opengl(pos + size * delta_pos[i], depth));
			uv_content[i] = (uv_pos + uv_size * delta_pos[i]);
			alpha_content[i] = (alpha);
		}



		jgl::Shader* tmp_shader = jgl::Application::active_application()->shader(shader_name);

		if (tmp_shader == nullptr)
			THROW_EXCEPTION(jgl::Error_level::Error, 0, "No shader named " + shader_name);

		static jgl::Uint element_index[6] = { 0, 3, 1, 2, 3, 0 };


		tmp_shader->activate();

		activate();

		tmp_shader->buffer("model_space")->send(vertex_content, 4);
		tmp_shader->buffer("vertexUV")->send(uv_content, 4);
		tmp_shader->buffer("alpha_value")->send(alpha_content, 4);

		tmp_shader->uniform("textureID")->send(0);

		tmp_shader->indexes_buffer()->send(element_index, 6);

		tmp_shader->launch(jgl::Shader::Mode::Triangle);

		desactivate();
	}

	void Image::prepare_draw(jgl::Array<jgl::Vector3>& vertex_array, jgl::Array<jgl::Vector2>& uv_array, jgl::Array<jgl::Float>& alpha_array, jgl::Array<jgl::Uint>& element_array,
		jgl::Vector2Int pos, jgl::Vector2Int size, jgl::Vector2 uv_pos, jgl::Vector2 uv_size, jgl::Float depth, jgl::Float alpha)
	{
		static jgl::Uint element_index[6] = { 0, 3, 1, 2, 3, 0 };
		static jgl::Vector2Int delta_pos[4] = {
			jgl::Vector2Int(0, 0),
			jgl::Vector2Int(1, 0),
			jgl::Vector2Int(0, 1),
			jgl::Vector2Int(1, 1)
		};
		jgl::Size_t vertex_array_entry_size = vertex_array.size();

		for (size_t i = 0; i < 4; i++)
		{
			vertex_array.push_back(convert_screen_to_opengl(pos + size * delta_pos[i], depth));
			uv_array.push_back(uv_pos + uv_size * delta_pos[i]);
			alpha_array.push_back(alpha);
		}
		for (jgl::Size_t i = 0; i < 6; i++)
		{
			element_array.push_back(element_index[i] + vertex_array_entry_size);
		}
	}
}