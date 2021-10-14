#include "jgl.h"

#define STB_TRUETYPE_IMPLEMENTATION
#include "external_lib/std_truetype.h"

extern jgl::Array<jgl::Font*>* g_application_font_array;

namespace jgl
{
	Font::Font(jgl::String path) : jgl::Image_handler()
	{
		jgl::Long size;
		jgl::Uchar* fontBuffer;

		if (jgl::check_file_exist(path) == false)
			THROW_EXCEPTION(jgl::Error_level::Error, 1, "Try to parse file " + path + " to load a font : file doesn't exist");
		FILE* fontFile = NULL;

		errno_t err = fopen_s(
			&fontFile,
			path.c_str(), "rb"
		);
		fseek(fontFile, 0, SEEK_END);
		size = ftell(fontFile); /* how long is the file ? */
		fseek(fontFile, 0, SEEK_SET); /* reset */

		fontBuffer = (jgl::Uchar *)malloc(size);

		fread(fontBuffer, size, 1, fontFile);
		fclose(fontFile);

		const jgl::Size_t nb_char = 256;
		const jgl::Int oversample_x = 3;
		const jgl::Int oversample_y = 3;

		const jgl::Size_t char_size = 90;

		jgl::Uchar* atlasData;

		jgl::Int width = 32;
		jgl::Int height = 32;

		stbtt_packedchar* char_info = new stbtt_packedchar[nb_char];

		while (1) {
			atlasData = new jgl::Uchar[width * height];

			stbtt_pack_context context;
			if (!stbtt_PackBegin(&context, atlasData, width, height, 0, 1, nullptr))
				THROW_EXCEPTION(jgl::Error_level::Error, 1, "Failed to initialize font");

			stbtt_PackSetOversampling(&context, 1, 1);
			if (!stbtt_PackFontRange(&context, fontBuffer, 0, char_size, 0, nb_char, char_info)) {
				// too small
				delete [] atlasData;
				stbtt_PackEnd(&context);
				width *= 2;
				height *= 2;
			}
			else {
				stbtt_PackEnd(&context);
				break;
			}
		}

		glGenTextures(1, &_id);
		glBindTexture(GL_TEXTURE_2D, _id);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, atlasData);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		jgl::Float width_delta = 0.5f / width;
		jgl::Float height_delta = 0.5f / height;
		for (jgl::Size_t i = 0; i < nb_char; i++)
		{
			jgl::Glyph c = i;
			Glyph_data data;

			stbtt_aligned_quad quad;

			stbtt_GetPackedQuad(char_info, width, height, c.value(), &data.offset.x, &data.offset.y, &quad, 1);
			const jgl::Float xmin = quad.x0;
			const jgl::Float xmax = quad.x1;
			const jgl::Float ymin = -quad.y1;
			const jgl::Float ymax = -quad.y0;

			data.height = char_size;
			data.positions[0] = jgl::Vector2(xmin / char_size, ymin / char_size);
			data.positions[1] = jgl::Vector2(xmax / char_size, ymin / char_size);
			data.positions[2] = jgl::Vector2(xmin / char_size, ymax / char_size);
			data.positions[3] = jgl::Vector2(xmax / char_size, ymax / char_size);
			data.offset /= char_size;

			data.uvs[0] = { quad.s0 - width_delta, quad.t0 - height_delta };
			data.uvs[1] = { quad.s1 + width_delta, quad.t0 - height_delta };
			data.uvs[2] = { quad.s0 - width_delta, quad.t1 + height_delta };
			data.uvs[3] = { quad.s1 + width_delta, quad.t1 + height_delta };

			_atlas[c] = data;
		}

		const jgl::String shader_name = "Texture_text_shader_2D";
		jgl::Shader* tmp_shader = jgl::Application::active_application()->shader(shader_name);

		if (tmp_shader == nullptr)
			THROW_EXCEPTION(jgl::Error_level::Error, 0, "No shader named " + shader_name);

		_vertex_buffer = tmp_shader->buffer("model_space");
		_uvs_buffer = tmp_shader->buffer("vertexUV");
		_color_buffer = tmp_shader->buffer("color_space");
		_color_outline_buffer = tmp_shader->buffer("color_outline_space");
		_alpha_buffer = tmp_shader->buffer("alpha_value");
		_indexes_buffer = tmp_shader->indexes_buffer();

		THROW_INFORMATION("Font created successfully with truetypefile [" + path + "] at id [" + jgl::itoa(_id) + "]");
	}

	Font::~Font()
	{

	}

	Font::Glyph_data& Font::_get_data(jgl::Glyph to_draw)
	{
		if (_atlas.count(to_draw) == 0)
			THROW_EXCEPTION(jgl::Error_level::Error, 1, "Trying to print unespected glyph");
		return (_atlas[to_draw]);
	}
	void Font::draw(jgl::Vector2Int pos, jgl::Vector2Int size, jgl::Float level, jgl::Float alpha)
	{

		jgl::Vector3 vertex_content[4] = {
			jgl::convert_screen_to_opengl(pos + size * jgl::Vector2Int(0, 0), level),
			jgl::convert_screen_to_opengl(pos + size * jgl::Vector2Int(1, 0), level),
			jgl::convert_screen_to_opengl(pos + size * jgl::Vector2Int(0, 1), level),
			jgl::convert_screen_to_opengl(pos + size * jgl::Vector2Int(1, 1), level),
		};

		jgl::Vector2 uv_content[4] = {
			jgl::Vector2Int(0, 0),
			jgl::Vector2Int(1, 0),
			jgl::Vector2Int(0, 1),
			jgl::Vector2Int(1, 1),
		};

		jgl::Color color_content[4] = {
			jgl::Color::white(),
			jgl::Color::white(),
			jgl::Color::white(),
			jgl::Color::white()
		};

		jgl::Color color_outline_content[4] = {
			jgl::Color::black(),
			jgl::Color::black(),
			jgl::Color::black(),
			jgl::Color::black()
		};

		jgl::Float alpha_content[4] = {
			alpha,
			alpha,
			alpha,
			alpha
		};

		static jgl::Uint element_index[6] = { 0, 3, 1, 2, 3, 0 };

		const jgl::String shader_name = "Texture_text_shader_2D";
		jgl::Shader* tmp_shader = jgl::Application::active_application()->shader(shader_name);

		if (tmp_shader == nullptr)
			THROW_EXCEPTION(jgl::Error_level::Error, 0, "No shader named " + shader_name);

		tmp_shader->activate();

		activate();

		_vertex_buffer->send(vertex_content, 4);
		_uvs_buffer->send(uv_content, 4);
		_color_buffer->send(color_content, 4);
		_color_outline_buffer->send(color_outline_content, 4);
		_alpha_buffer ->send(alpha_content, 4);
		_indexes_buffer->send(element_index, 6);
		tmp_shader->uniform("textureID")->send(0);

		tmp_shader->launch(jgl::Shader::Mode::Triangle);

		desactivate();

	}
	jgl::Vector2Int Font::draw_text(jgl::String text, jgl::Vector2Int pos, jgl::Uint size, jgl::Float level, jgl::Float alpha, jgl::Color text_color)
	{
		return (draw_text(text, pos, size, level, alpha, text_color, text_color));
	}
	jgl::Vector2Int Font::prepare_render_char(
		jgl::Array<Vector3>& vertex_array, jgl::Array<jgl::Vector2>& uvs_array, jgl::Array<jgl::Color>& color_array, jgl::Array<jgl::Color>& color_outline_array, jgl::Array<jgl::Float>& alpha_array, jgl::Array<jgl::Uint>& index_array,
		jgl::Glyph to_draw, jgl::Vector2Int pos, jgl::Uint size, jgl::Float level, jgl::Float alpha, jgl::Color text_color, jgl::Color outline_color)
	{
		static jgl::Vector2 tmp_delta_pos[4] = {
			Vector2(0, 0), Vector2(1, 0), Vector2(0, 1), Vector2(1, 1)
		};
		static jgl::Uint element_index[6] = { 0, 3, 1, 2, 3, 0 };

		jgl::Uint base_index = vertex_array.size();
		Font::Glyph_data& glyph_data = _get_data(to_draw);
		jgl::Vector2Int tmp_size = calc_char_size(to_draw, size);
		jgl::Vector2Int delta_pos = jgl::Vector2(0, size - tmp_size.y);

		for (jgl::Size_t i = 0; i < 4; i++)
		{
			vertex_array.push_back(jgl::convert_screen_to_opengl(pos + delta_pos + tmp_size * tmp_delta_pos[i], level));
			uvs_array.push_back(glyph_data.uvs[i]);
			color_array.push_back(text_color);
			color_outline_array.push_back(outline_color);
			alpha_array.push_back(alpha);
		}
		for (jgl::Size_t i = 0; i < 6; i++)
		{
			index_array.push_back(base_index + element_index[i]);
		}

		return (glyph_data.offset * size);
	}

	jgl::Vector2Int Font::prepare_render_text(
		jgl::Array<Vector3>& vertex_array, jgl::Array<jgl::Vector2>& uvs_array, jgl::Array<jgl::Color>& color_array, jgl::Array<jgl::Color>& color_outline_array, jgl::Array<jgl::Float>& alpha_array, jgl::Array<jgl::Uint>& index_array,
		jgl::String text, jgl::Vector2Int pos, jgl::Uint size, jgl::Float level, jgl::Float alpha, jgl::Color text_color, jgl::Color outline_color)
	{
		static jgl::Vector2 tmp_delta_pos[4] = {
			Vector2(0, 0), Vector2(1, 0), Vector2(0, 1), Vector2(1, 1)
		};
		static jgl::Uint element_index[6] = { 0, 3, 1, 2, 3, 0 };

		jgl::Vector2Int delta = 0;
		for (jgl::Size_t i = 0; i < text.size(); i++)
		{
			jgl::Uint base_index = vertex_array.size();
			Font::Glyph_data& glyph_data = _get_data(text[i]);
			jgl::Vector2Int tmp_size = calc_char_size(text[i], size);
			jgl::Vector2Int delta_pos = jgl::Vector2(0, size - tmp_size.y);

			for (jgl::Size_t i = 0; i < 4; i++)
			{
				vertex_array.push_back(jgl::convert_screen_to_opengl(pos + delta + delta_pos + tmp_size * tmp_delta_pos[i], level));
				uvs_array.push_back(glyph_data.uvs[i]);
				color_array.push_back(text_color);
				color_outline_array.push_back(outline_color);
				alpha_array.push_back(alpha);
			}
			for (jgl::Size_t i = 0; i < 6; i++)
			{
				index_array.push_back(base_index + element_index[i]);
			}

			delta.x += glyph_data.offset.x * size;
		}
		delta.y = size;
		return (delta);
	}

	jgl::Vector2Int Font::draw_text(jgl::String text, jgl::Vector2Int pos, jgl::Uint size, jgl::Float level, jgl::Float alpha, jgl::Color text_color, jgl::Color outline_color)
	{
		static jgl::Vector2 tmp_delta_pos[4] = {
			Vector2(0, 0), Vector2(1, 0), Vector2(0, 1), Vector2(1, 1)
		};
		static jgl::Uint element_index[6] = { 0, 3, 1, 2, 3, 0 };

		jgl::Array<jgl::Vector3> vertex_array;
		jgl::Array<jgl::Vector2> uvs;
		jgl::Array<jgl::Color> colors;
		jgl::Array<jgl::Color> colors_outline;
		jgl::Array<jgl::Float> alphas;
		jgl::Array<jgl::Uint> indexes;

		jgl::Vector2Int delta = 0;
		for (jgl::Size_t i = 0; i < text.size(); i++)
		{
			jgl::Uint base_index = vertex_array.size();
			Font::Glyph_data& glyph_data = _get_data(text[i]);
			jgl::Vector2Int tmp_size = calc_char_size(text[i], size);
			jgl::Vector2Int delta_pos = jgl::Vector2(0, size - tmp_size.y);

			for (jgl::Size_t i = 0; i < 4; i++)
			{
				vertex_array.push_back(jgl::convert_screen_to_opengl(pos + delta + delta_pos + tmp_size * tmp_delta_pos[i], level));
				uvs.push_back(glyph_data.uvs[i]);
				colors.push_back(text_color);
				colors_outline.push_back(outline_color);
				alphas.push_back(alpha);
			}
			for (jgl::Size_t i = 0; i < 6; i++)
			{
				indexes.push_back(base_index + element_index[i]);
			}


			delta.x += glyph_data.offset.x * size;
		}
		delta.y = size;

		const jgl::String shader_name = "Texture_text_shader_2D";
		jgl::Shader* tmp_shader = jgl::Application::active_application()->shader(shader_name);

		if (tmp_shader == nullptr)
			THROW_EXCEPTION(jgl::Error_level::Error, 0, "No shader named " + shader_name);

		tmp_shader->activate();

		activate(GL_TEXTURE0);

		tmp_shader->uniform("textureID")->send(0);

		_vertex_buffer->send(vertex_array.all(), vertex_array.size());
		_uvs_buffer->send(uvs.all(), uvs.size());
		_color_buffer->send(colors.all(), colors.size());
		_color_outline_buffer->send(colors_outline.all(), colors_outline.size());
		_alpha_buffer ->send(alphas.all(), alphas.size());
		_indexes_buffer->send(indexes.all(), indexes.size());

		tmp_shader->launch(jgl::Shader::Mode::Triangle);

		desactivate();

		return (delta);
	}

	jgl::Vector2Int Font::calc_text_size(jgl::String text, jgl::Uint size)
	{
		jgl::Vector2Int delta = 0;
		delta.y = size;
		for (jgl::Size_t i = 0; i < text.size(); i++)
		{
			Font::Glyph_data glyph_data = _get_data(text[i]);
			delta.x += (glyph_data.offset * size).x;
		}
		return (delta);
	}

	jgl::Vector2Int Font::calc_char_offset(jgl::Glyph to_draw, jgl::Uint size)
	{
		Glyph_data& data = _get_data(to_draw);
		jgl::Vector2 tmp = data.offset;
		tmp *= (size);
		return (tmp);
	}

	jgl::Vector2Int Font::calc_char_size(jgl::Glyph to_draw, jgl::Uint size)
	{
		Glyph_data& data = _get_data(to_draw);
		jgl::Vector2 tmp = (data.positions[3] - data.positions[0]);
		tmp *= (size);
		return (tmp);
	}

	jgl::Vector2Int Font::draw_char(jgl::Glyph to_draw, jgl::Vector2Int pos, jgl::Uint size, jgl::Float level, jgl::Float alpha, jgl::Color text_color)
	{
		return (draw_char(to_draw, pos, size, level, alpha, text_color, text_color));
	}

	jgl::Vector2Int Font::draw_char(jgl::Glyph to_draw, jgl::Vector2Int pos, jgl::Uint size, jgl::Float level, jgl::Float alpha, jgl::Color text_color, jgl::Color text_outline_color)
	{
		jgl::Vector2Int tmp_size = calc_char_size(to_draw, size);

		Font::Glyph_data& glyph_data = _get_data(to_draw);

		jgl::Vector2Int delta_pos = jgl::Vector2(0, size - tmp_size.y);
		if (to_draw == '\'')
			delta_pos = jgl::Vector2(0, tmp_size.y);

		jgl::Vector3 vertex_content[4] = {
			jgl::convert_screen_to_opengl(pos + delta_pos + tmp_size * Vector2(0, 0), level),
			jgl::convert_screen_to_opengl(pos + delta_pos + tmp_size * Vector2(1, 0), level),
			jgl::convert_screen_to_opengl(pos + delta_pos + tmp_size * Vector2(0, 1), level),
			jgl::convert_screen_to_opengl(pos + delta_pos + tmp_size * Vector2(1, 1), level)
		};

		jgl::Vector2 uv_content[4] = {
			glyph_data.uvs[0],
			glyph_data.uvs[1],
			glyph_data.uvs[2],
			glyph_data.uvs[3],
		};

		jgl::Color color_content[4] = {
			text_color,
			text_color,
			text_color,
			text_color
		};

		jgl::Color color_outline_content[4] = {
			text_outline_color,
			text_outline_color,
			text_outline_color,
			text_outline_color
		};

		jgl::Float alpha_content[4] = {
			alpha,
			alpha,
			alpha,
			alpha
		};

		static jgl::Uint element_index[6] = { 0, 3, 1, 2, 3, 0 };


		const jgl::String shader_name = "Texture_text_shader_2D";
		jgl::Shader* tmp_shader = jgl::Application::active_application()->shader(shader_name);

		if (tmp_shader == nullptr)
			THROW_EXCEPTION(jgl::Error_level::Error, 0, "No shader named " + shader_name);

		tmp_shader->activate();

		activate(GL_TEXTURE0);

		tmp_shader->uniform("textureID")->send(0);

		_vertex_buffer->send(vertex_content, 4);
		_uvs_buffer->send(uv_content, 4);
		_color_buffer->send(color_content, 4);
		_color_outline_buffer->send(color_outline_content, 4);
		_alpha_buffer ->send(alpha_content, 4);
		_indexes_buffer->send(element_index, 6);

		tmp_shader->launch(jgl::Shader::Mode::Triangle);

		desactivate();

		return (glyph_data.offset * size);
	}
}