#include "jgl.h"

#define STB_IMAGE_IMPLEMENTATION
#include "external_lib/stb_image.h"

#include "jgl_drawing_function.h"

namespace jgl
{
	jgl::Shader* Image_handler::_shader = nullptr;

	jgl::Buffer* Image_handler::_model_space_buffer = nullptr;
	jgl::Buffer* Image_handler::_vertexUV_buffer = nullptr;
	jgl::Buffer* Image_handler::_alpha_value_buffer = nullptr;
	jgl::Buffer* Image_handler::_element_index_buffer = nullptr;
	jgl::Uniform* Image_handler::_texture_uniform = nullptr;

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

		if (_shader == nullptr)
			_shader = jgl::Application::active_application()->shader(shader_name);

		if (_shader == nullptr)
			THROW_EXCEPTION(jgl::Error_level::Error, 0, "No shader named " + shader_name);

		if (_model_space_buffer == nullptr)
			_model_space_buffer = _shader->buffer("model_space");

		if (_vertexUV_buffer == nullptr)
			_vertexUV_buffer = _shader->buffer("vertexUV");

		if (_alpha_value_buffer == nullptr)
			_alpha_value_buffer = _shader->buffer("alpha_value");

		if (_element_index_buffer == nullptr)
			_element_index_buffer = _shader->indexes_buffer();

		if (_texture_uniform == nullptr)
			_texture_uniform = _shader->uniform("textureID");

		_model_space_buffer->send(vertex_content, 3);
		_vertexUV_buffer->send(uv_content, 3);
		_alpha_value_buffer->send(alpha_content, 3);
		_element_index_buffer->send(element_index, 3);
		_texture_uniform->send(0);

		_shader->launch(jgl::Shader::Mode::Triangle);

		desactivate();
	}
	Image_handler::Image_handler()
	{
		_id = -1;
	}
	Image_handler::Image_handler(jgl::String path)
	{
		_data = stbi_load(path.c_str(), &_width, &_height, &_nbChannels, 0);

		if (_data == nullptr)
			THROW_EXCEPTION(jgl::Error_level::Warning, 1, "Can't load file " + path);

		glGenTextures(1, &_id);
		glBindTexture(GL_TEXTURE_2D, _id);
		if (_nbChannels == 3)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _width, _height, 0, GL_RGB, GL_UNSIGNED_BYTE, _data);
		else if (_nbChannels == 4)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, _data);
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
	jgl::Uint Image_handler::pixel(jgl::Uint p_x, jgl::Uint p_y)
	{
		if (p_x > static_cast<jgl::Uint>(_width) || p_y > static_cast<jgl::Uint>(_height))
		{
			jgl::cout << "Pixel out of range : [" << p_x << " vs " << _width << "] / [" << p_y << " vs " << _height << "]" << jgl::endl;
			return (0);
		}
		unsigned char datas[4] = {0, 0, 0, 0};

		jgl::Uint index = (p_x * _nbChannels) + (_width * _nbChannels) * p_y;
		for (jgl::Size_t comp = 0; comp < _nbChannels; comp++)
			datas[comp] = _data[index + (_nbChannels - 1 - comp)];

		if (_nbChannels == 3)
		{
			datas[3] = 0xFF;
		}

		return (*(reinterpret_cast<jgl::Uint*>(datas)));
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
	static jgl::Vector2Uint delta_pos[4] = {
		jgl::Vector2Uint(0, 0),
		jgl::Vector2Uint(1, 0),
		jgl::Vector2Uint(0, 1),
		jgl::Vector2Uint(1, 1)
	};

	void Image::draw(GLuint p_id, jgl::Vector2Int pos, jgl::Vector2Uint size, jgl::Vector2 uv_pos, jgl::Vector2 uv_size,
		jgl::Float depth, jgl::Float alpha)
	{
		const jgl::String shader_name = "Texture_shader_2D";

		//static jgl::Uint element_index[6] = { 0, 3, 1, 2, 3, 0 };
		static jgl::Vector2Uint delta_pos[4] = {
			jgl::Vector2Uint(0, 0),
			jgl::Vector2Uint(1, 0),
			jgl::Vector2Uint(0, 1),
			jgl::Vector2Uint(1, 1)
		};

		Vector3 vertex_content[4];
		Vector2 uv_content[4];
		Float alpha_content[4];

		for (size_t i = 0; i < 4; i++)
		{
			jgl::Vector2Uint tmp_delta = size * delta_pos[i];
			vertex_content[i] = convert_screen_to_opengl(pos + jgl::Vector2Int(tmp_delta.x, tmp_delta.y), depth);
			uv_content[i] = (uv_pos + uv_size * delta_pos[i]);
			alpha_content[i] = (alpha);
		}


		if (_shader == nullptr)
			_shader = jgl::Application::active_application()->shader(shader_name);

		if (_shader == nullptr)
			THROW_EXCEPTION(jgl::Error_level::Error, 0, "No shader named " + shader_name);

		static jgl::Uint element_index[6] = { 0, 3, 1, 2, 3, 0 };


		_shader->activate();

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, p_id);

		if (_model_space_buffer == nullptr)
			_model_space_buffer = _shader->buffer("model_space");

		if (_vertexUV_buffer == nullptr)
			_vertexUV_buffer = _shader->buffer("vertexUV");

		if (_alpha_value_buffer == nullptr)
			_alpha_value_buffer = _shader->buffer("alpha_value");

		if (_element_index_buffer == nullptr)
			_element_index_buffer = _shader->indexes_buffer();

		if (_texture_uniform == nullptr)
			_texture_uniform = _shader->uniform("textureID");

		_model_space_buffer->send(vertex_content, 4);
		_vertexUV_buffer->send(uv_content, 4);
		_alpha_value_buffer->send(alpha_content, 4);
		_element_index_buffer->send(element_index, 6);
		_texture_uniform->send(0);

		_shader->launch(jgl::Shader::Mode::Triangle);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void Image::draw(jgl::Vector2Int pos, jgl::Vector2Uint size, jgl::Vector2 uv_a, jgl::Vector2 uv_b, jgl::Vector2 uv_c, jgl::Vector2 uv_d, jgl::Float depth, jgl::Float alpha)
	{
		activate();

		const jgl::String shader_name = "Texture_shader_2D";

		Vector3 vertex_content[4];
		Vector2 uv_content[4];
		Float alpha_content[4];


		for (size_t i = 0; i < 4; i++)
		{
			jgl::Vector2Uint tmp_delta = size * delta_pos[i];
			vertex_content[i] = convert_screen_to_opengl(pos + jgl::Vector2Int(tmp_delta.x, tmp_delta.y), depth);
			alpha_content[i] = alpha;
		}
		uv_content[0] = uv_a;
		uv_content[1] = uv_b;
		uv_content[2] = uv_c;
		uv_content[3] = uv_d;

		if (_shader == nullptr)
			_shader = jgl::Application::active_application()->shader(shader_name);

		if (_shader == nullptr)
			THROW_EXCEPTION(jgl::Error_level::Error, 0, "No shader named " + shader_name);


		if (_model_space_buffer == nullptr)
			_model_space_buffer = _shader->buffer("model_space");

		if (_vertexUV_buffer == nullptr)
			_vertexUV_buffer = _shader->buffer("vertexUV");

		if (_alpha_value_buffer == nullptr)
			_alpha_value_buffer = _shader->buffer("alpha_value");

		if (_element_index_buffer == nullptr)
			_element_index_buffer = _shader->indexes_buffer();

		if (_texture_uniform == nullptr)
			_texture_uniform = _shader->uniform("textureID");

		_model_space_buffer->send(vertex_content, 4);
		_vertexUV_buffer->send(uv_content, 4);
		_alpha_value_buffer->send(alpha_content, 4);
		_element_index_buffer->send(element_index, 6);
		_texture_uniform->send(0);

		_shader->launch(jgl::Shader::Mode::Triangle);

		desactivate();
	}

	void Image::draw(jgl::Vector2Int pos, jgl::Vector2Uint size, jgl::Vector2 uv_pos, jgl::Vector2 uv_size, jgl::Float depth, jgl::Float alpha)
	{
		const jgl::String shader_name = "Texture_shader_2D";

		//static jgl::Uint element_index[6] = { 0, 3, 1, 2, 3, 0 };
		static jgl::Vector2Uint delta_pos[4] = {
			jgl::Vector2Uint(0, 0),
			jgl::Vector2Uint(1, 0),
			jgl::Vector2Uint(0, 1),
			jgl::Vector2Uint(1, 1)
		};

		Vector3 vertex_content[4];
		Vector2 uv_content[4];
		Float alpha_content[4];

		for (size_t i = 0; i < 4; i++)
		{
			jgl::Vector2Uint tmp_delta = size * delta_pos[i];
			vertex_content[i] = convert_screen_to_opengl(pos + jgl::Vector2Int(tmp_delta.x, tmp_delta.y), depth);
			uv_content[i] = (uv_pos + uv_size * delta_pos[i]);
			alpha_content[i] = (alpha);
		}


		if (_shader == nullptr)
			_shader = jgl::Application::active_application()->shader(shader_name);

		if (_shader == nullptr)
			THROW_EXCEPTION(jgl::Error_level::Error, 0, "No shader named " + shader_name);

		static jgl::Uint element_index[6] = { 0, 3, 1, 2, 3, 0 };

		_shader->activate();

		activate();

		if (_model_space_buffer == nullptr)
			_model_space_buffer = _shader->buffer("model_space");

		if (_vertexUV_buffer == nullptr)
			_vertexUV_buffer = _shader->buffer("vertexUV");

		if (_alpha_value_buffer == nullptr)
			_alpha_value_buffer = _shader->buffer("alpha_value");

		if (_element_index_buffer == nullptr)
			_element_index_buffer = _shader->indexes_buffer();

		if (_texture_uniform == nullptr)
			_texture_uniform = _shader->uniform("textureID");

		_model_space_buffer->send(vertex_content, 4);
		_vertexUV_buffer->send(uv_content, 4);
		_alpha_value_buffer->send(alpha_content, 4);
		_element_index_buffer->send(element_index, 6);
		_texture_uniform->send(0);

		_shader->launch(jgl::Shader::Mode::Triangle);

		desactivate();
	}

	void Image::prepare_draw(jgl::Array<jgl::Vector3>& vertex_array, jgl::Array<jgl::Vector2>& uv_array, jgl::Array<jgl::Float>& alpha_array, jgl::Array<jgl::Uint>& element_array,
		jgl::Vector2Int pos, jgl::Vector2Uint size, jgl::Vector2 uv_pos, jgl::Vector2 uv_size, jgl::Float depth, jgl::Float alpha)
	{
		prepare_draw(&vertex_array, &uv_array, &alpha_array, &element_array, pos, size, uv_pos, uv_size, depth, alpha);
	}

	void Image::prepare_draw(jgl::Array<jgl::Vector3>* vertex_array, jgl::Array<jgl::Vector2>* uv_array, jgl::Array<jgl::Float>* alpha_array, jgl::Array<jgl::Uint>* element_array,
		jgl::Vector2Int pos, jgl::Vector2Uint size, jgl::Vector2 uv_pos, jgl::Vector2 uv_size, jgl::Float depth, jgl::Float alpha)
	{
		static jgl::Uint element_index[6] = { 0, 3, 1, 2, 3, 0 };
		static jgl::Vector2Uint delta_pos[4] = {
			jgl::Vector2Uint(0, 0),
			jgl::Vector2Uint(1, 0),
			jgl::Vector2Uint(0, 1),
			jgl::Vector2Uint(1, 1)
		};
		jgl::Size_t vertex_array_entry_size = 0;
		if (vertex_array != nullptr)
			vertex_array_entry_size = vertex_array->size();

		for (size_t i = 0; i < 4; i++)
		{
			jgl::Vector2Uint tmp_delta = size * delta_pos[i];
			if (vertex_array != nullptr)
				vertex_array->push_back(convert_screen_to_opengl(pos + jgl::Vector2Int(tmp_delta.x, tmp_delta.y), depth));
			if (uv_array != nullptr)
				uv_array->push_back(uv_pos + uv_size * delta_pos[i]);
			if (alpha_array != nullptr)
				alpha_array->push_back(alpha);
		}
		for (jgl::Size_t i = 0; i < 6; i++)
		{
			if (element_array != nullptr)
				element_array->push_back(element_index[i] + vertex_array_entry_size);
		}
	}

	void Image::print()
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, _id);

		int w, h;
		int miplevel = 0;
		glGetTexLevelParameteriv(GL_TEXTURE_2D, miplevel, GL_TEXTURE_WIDTH, &w);
		glGetTexLevelParameteriv(GL_TEXTURE_2D, miplevel, GL_TEXTURE_HEIGHT, &h);

		GLsizei nrChannels = 4;
		GLsizei stride = nrChannels * w;
		GLsizei bufferSize = stride * h;
		std::vector<char> buffer(bufferSize);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, _id);

		glPixelStorei(GL_PACK_ALIGNMENT, 4);
		glGetTextureImage(_id, 0, GL_RGBA, GL_UNSIGNED_BYTE, bufferSize, buffer.data());

		jgl::cout << "Image output buffer : " << jgl::endl;
		for (jgl::Size_t y = 0; y < static_cast<jgl::Size_t>(h); y++)
		{
			for (jgl::Size_t x = 0; x < static_cast<jgl::Size_t>(w); x++)
			{
				if (x != 0)
					jgl::cout << " - ";

				for (jgl::Size_t i = 0; i < 4; i++)
				{
					jgl::cout << "[" << jgl::normalize_int(static_cast<unsigned char>(buffer[i + x * 4 + y * w * 4]), ' ', 3) << "]";
				}
			}
			jgl::cout << jgl::endl;
		}
		jgl::cout << jgl::endl;
	}

	void Image::save_to_png(jgl::String p_path)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, _id);

		int w, h;
		int miplevel = 0;
		glGetTexLevelParameteriv(GL_TEXTURE_2D, miplevel, GL_TEXTURE_WIDTH, &w);
		glGetTexLevelParameteriv(GL_TEXTURE_2D, miplevel, GL_TEXTURE_HEIGHT, &h);

		GLsizei nrChannels = 4;
		GLsizei stride = nrChannels * w;
		GLsizei bufferSize = stride * h;
		std::vector<char> buffer(bufferSize);

		glPixelStorei(GL_PACK_ALIGNMENT, 4);
		glGetTextureImage(_id, 0, GL_RGBA, GL_UNSIGNED_BYTE, bufferSize, buffer.data());

		if (p_path.size() > 4 && p_path.substr(p_path.size() - 4, 4) != ".png")
		{
			jgl::save_to_png(p_path, jgl::Vector2Int(w, h), buffer.data());
		}
		else if (p_path.size() > 4 && p_path.substr(p_path.size() - 4, 4) != ".jpg")
		{
			jgl::save_to_jpg(p_path, jgl::Vector2Int(w, h), buffer.data());
		}
		else if (p_path.size() > 4 && p_path.substr(p_path.size() - 4, 4) != ".bmp")
		{
			jgl::save_to_bmp(p_path, jgl::Vector2Int(w, h), buffer.data());
		}
		else if (p_path.size() > 4 && p_path.substr(p_path.size() - 4, 4) != ".tga")
		{
			jgl::save_to_tga(p_path, jgl::Vector2Int(w, h), buffer.data());
		}
		else
		{
			THROW_EXCEPTION(jgl::Error_level::Warning, 1, "Can't save such image");
		}
	}
}