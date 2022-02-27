#include "jgl.h"

namespace jgl::Widget_component
{
	void Text_label::_calc_text_size(jgl::Font* tmp_font)
	{
		int delta[5] = { 100, 50, 20, 10, 1 };
		_text_size = 2;

		if (_text == "")
		{
			_text_size = area().y;
			return;
		}

		jgl::String tmp_text;

		if (_hidden == true)
			tmp_text = jgl::String(jgl::Glyph('*'), _text.size());
		else
			tmp_text = _text;

		for (int i = 0; i < 5; i++)
		{
			bool enough = false;
			while (enough == false)
			{
				if (tmp_font == nullptr)
					THROW_EXCEPTION(jgl::Error_level::Error, 1, "No font give to label component");

				jgl::Vector2Int tmp_size = tmp_font->calc_text_size(tmp_text, _text_size + delta[i]);
				if (tmp_size.x >= _area.x || tmp_size.y >= _area.y)
					enough = true;
				else
					_text_size += delta[i];
			}
		}
	}
	void Text_label::_calc_text_pos_horizontal(jgl::Vector2Int& tmp_size)
	{
		if (_Halign == jgl::Horizontal_alignment::Left)
		{
			_text_pos.x = 0;
		}
		else if (_Halign == jgl::Horizontal_alignment::Centred)
		{
			_text_pos.x = (_area.x - tmp_size.x) / 2;
		}
		else
		{
			_text_pos.x = _area.x - tmp_size.x;
		}
	}
	void Text_label::_calc_text_pos_vertical(jgl::Vector2Int& tmp_size)
	{
		if (_Valign == jgl::Vertical_alignment::Top)
		{
			_text_pos.y = 0;
		}
		else if (_Valign == jgl::Vertical_alignment::Centred)
		{
			_text_pos.y = (_area.y - tmp_size.y) / 2 - tmp_size.y / 6;
		}
		else
		{
			_text_pos.y = _area.y - tmp_size.y;
		}
	}
	void Text_label::_calc_text_pos(jgl::Font* tmp_font)
	{
		_text_pos = 0;

		jgl::Vector2Int tmp_size = tmp_font->calc_text_size(_text, _text_size);
		_calc_text_pos_horizontal(tmp_size);
		_calc_text_pos_vertical(tmp_size);
	}
	void Text_label::set_text_size(jgl::Size_t p_text_size)
	{
		jgl::Font* tmp_font = _font;
		if (tmp_font == nullptr)
			tmp_font = jgl::Application::active_application()->default_font();

		_text_size = p_text_size;
		_calc_text_pos(tmp_font);

		_computed_font = tmp_font;
		_baked = false;
	}

	void Text_label::_recalc()
	{
		jgl::Font* tmp_font = _font;
		if (tmp_font == nullptr)
			tmp_font = jgl::Application::active_application()->default_font();

		_calc_text_size(tmp_font);
		_calc_text_pos(tmp_font);

		_computed_font = tmp_font;
	}

	Text_label::Text_label(jgl::String p_text, jgl::Widget* p_owner) : Graphical_component(p_owner)
	{
		_text = p_text;
		_Halign = jgl::Horizontal_alignment::Left;
		_Valign = jgl::Vertical_alignment::Top;

		_text_pos = 0;
		_text_size = 0;

		_text_color = jgl::Color(50, 50, 50);
		_text_outline_color = jgl::Color(0, 0, 0);

		_baked = false;

		_font = nullptr;
	}
	void Text_label::set_text_alone(jgl::String p_text)
	{
		_text = p_text;
		_baked = false;
	}
	void Text_label::set_text(jgl::String p_text)
	{
		_text = p_text;
		_recalc();
		_baked = false;
	}

	void Text_label::set_geometry(jgl::Vector2Int p_anchor, jgl::Vector2Int p_area, jgl::Float p_depth)
	{
		Graphical_component::set_geometry(p_anchor, p_area, p_depth);
		_recalc();
		_baked = false;
	}

	void Text_label::_unbake()
	{
		_baked = false;
	}

	void Text_label::_initialize_opengl()
	{
		const jgl::String shader_name = "Texture_text_shader_2D";
		
		if (_shader == nullptr)
			_shader = jgl::Application::active_application()->shader(shader_name);

		if (_shader == nullptr)
			THROW_EXCEPTION(jgl::Error_level::Error, 0, "No shader named " + shader_name);

		if (_texture_uniform == nullptr)
			_texture_uniform = _shader->uniform("textureID")->copy();

		if (_vertex_buffer == nullptr)
			_vertex_buffer = _shader->buffer("model_space")->copy();
		if (_uvs_buffer == nullptr)
			_uvs_buffer = _shader->buffer("vertexUV")->copy();
		if (_color_buffer == nullptr)
			_color_buffer = _shader->buffer("color_space")->copy();
		if (_color_outline_buffer == nullptr)
			_color_outline_buffer = _shader->buffer("color_outline_space")->copy();
		if (_alpha_buffer == nullptr)
			_alpha_buffer = _shader->buffer("alpha_value")->copy();
		if (_indexes_buffer == nullptr)
			_indexes_buffer = _shader->indexes_buffer()->copy();
	}

	void Text_label::_bake(jgl::Font* p_font)
	{
		_initialize_opengl();

		jgl::String tmp_text;

		if (_hidden == true)
			tmp_text = jgl::String(jgl::Glyph('*'), _text.size());
		else
			tmp_text = _text;

		jgl::Array<jgl::Vector3> vertex_array;
		jgl::Array<jgl::Vector2> uvs_array;
		jgl::Array<jgl::Color> color_array;
		jgl::Array<jgl::Color> color_outline_array;
		jgl::Array<jgl::Float> alpha_array;
		jgl::Array<jgl::Uint> index_array;

		p_font->prepare_render_text(
			vertex_array, uvs_array, color_array, color_outline_array, alpha_array, index_array,
			tmp_text, _anchor + _text_pos, _text_size, _depth + 0.05f, 1.0f, _text_color, _text_outline_color);

		_vertex_buffer->send(vertex_array.all(), vertex_array.size());
		_uvs_buffer->send(uvs_array.all(), uvs_array.size());
		_color_buffer->send(color_array.all(), color_array.size());
		_color_outline_buffer->send(color_outline_array.all(), color_outline_array.size());
		_alpha_buffer->send(alpha_array.all(), alpha_array.size());
		_indexes_buffer->send(index_array.all(), index_array.size());

		_baked = true;
	}

	void Text_label::_render(jgl::Font* p_font)
	{
		_shader->activate();

		p_font->activate();

		_texture_uniform->send(0);

		_vertex_buffer->activate();
		_uvs_buffer->activate();
		_color_buffer->activate();
		_color_outline_buffer->activate();
		_alpha_buffer->activate();
		_indexes_buffer->activate();

		_shader->cast(jgl::Shader::Mode::Triangle, _indexes_buffer->size() / sizeof(jgl::Uint));
	}

	void Text_label::render()
	{
		jgl::Font* tmp_font = _font;

		if (tmp_font == nullptr)
			tmp_font = jgl::Application::active_application()->default_font();

		if (_baked == false)
			_bake(tmp_font);

		_render(tmp_font);
	}
}