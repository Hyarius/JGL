#pragma once

#include "jgl_component.h"

namespace jgl::Widget_component
{
	class Text_label : public Graphical_component
	{
	protected:
		jgl::Font* _font = nullptr;
		jgl::Font* _computed_font = nullptr;

		jgl::String _text = "Default text";

		jgl::Vector2Int _text_pos;
		jgl::Size_t _text_size;
		jgl::Horizontal_alignment _Halign;
		jgl::Vertical_alignment _Valign;

		jgl::Color _text_color;
		jgl::Color _text_outline_color;

		jgl::Bool _hidden = false;

		void _calc_text_size(jgl::Font* tmp_font);
		void _calc_text_pos_horizontal(jgl::Vector2Int& tmp_size);
		void _calc_text_pos_vertical(jgl::Vector2Int& tmp_size);
		void _calc_text_pos(jgl::Font* tmp_font);
		void _recalc();

		jgl::Bool _baked;

		Shader* _shader = nullptr;
		jgl::Uniform* _texture_uniform = nullptr;
		jgl::Buffer* _vertex_buffer = nullptr;
		jgl::Buffer* _uvs_buffer = nullptr;
		jgl::Buffer* _color_buffer = nullptr;
		jgl::Buffer* _color_outline_buffer = nullptr;
		jgl::Buffer* _alpha_buffer = nullptr;
		jgl::Buffer* _indexes_buffer = nullptr;

		void _initialize_opengl();
		void _unbake();
		void _bake(jgl::Font* p_font);
		void _render(jgl::Font* p_font);

	public:
		Text_label(jgl::String p_text = "default text", jgl::Widget* p_owner = nullptr);

		const jgl::String& text() { return (_text); }
		const jgl::Size_t& text_size() { return (_text_size); }
		const jgl::Vector2Int& text_pos() { return (_text_pos); }
		const jgl::Bool& hidden() { return (_hidden); }
		jgl::Font* font()
		{
			jgl::Font* tmp_font = _font;
			if (tmp_font == nullptr)
				tmp_font = jgl::Application::active_application()->default_font();
			return (tmp_font);
		}

		void set_hidden(jgl::Bool p_hidden) { _hidden = p_hidden; }
		void set_text_size(jgl::Size_t p_text_size);
		void set_font(jgl::Font* p_font) { _font = p_font; _recalc(); _baked = false; }
		void set_text(jgl::String p_text);
		void set_text_alone(jgl::String p_text);
		void set_horizontal_align(jgl::Horizontal_alignment p_align) { _Halign = p_align; _recalc(); _baked = false; }
		void set_vertical_align(jgl::Vertical_alignment p_align) { _Valign = p_align; _recalc(); _baked = false; }
		void set_text_color(jgl::Color p_color) { _text_color = p_color; }
		void set_text_outline_color(jgl::Color p_outline_color) { _text_outline_color = p_outline_color; }

		void set_geometry(jgl::Vector2Int p_anchor, jgl::Vector2Int p_area, jgl::Float p_depth);

		void render();
	};
}