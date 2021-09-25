#pragma once

#include "jgl_component.h"

namespace jgl::Widget_component
{
	class Box : public Graphical_component
	{
	private:

		static const jgl::Uint C_ELEMENT_BUFFER_CONTENT[54];
		static const jgl::Vector2 C_UV_UNIT;
		static const jgl::Vector2 C_UV_BUFFER_CONTENT[16];
		static const jgl::Float C_ALPHA_BUFFER_CONTENT[16];

		jgl::Bool _computed = false;
		jgl::Bool _initialize = false;
		jgl::Shader* _shader = nullptr;
		jgl::Buffer* _element_buffer = nullptr;
		jgl::Map<jgl::String, jgl::Uniform*> _uniforms;
		jgl::Map<jgl::String, jgl::Buffer*> _buffers;
		jgl::Sprite_sheet* _background_tileset = nullptr;

		jgl::Color _background = jgl::Color(100, 100, 100);
		jgl::Color _frontground = jgl::Color(150, 150, 150);
		jgl::Int _border_size = 5;

		void _init();
		void _compute_texture();
		void _render_color();
		void _render_texture();

	public:
		Box(jgl::Widget* p_owner = nullptr);
		Box(jgl::Color p_background, jgl::Color p_frontground, jgl::Widget* p_owner);
		Box(jgl::String p_path, jgl::Widget* p_owner);
		Box(jgl::Sprite_sheet* p_background_tileset, jgl::Widget* p_owner);

		void set_front_color(jgl::Color p_frontground) { _frontground = p_frontground; }
		void set_back_color(jgl::Color p_background) { _background = p_background; }

		void set_border_size(jgl::Int p_border_size) { _border_size = p_border_size; }
		jgl::Int border_size() { return (_border_size); }
		void set_geometry(jgl::Vector2Int p_anchor, jgl::Vector2Int p_area, jgl::Float p_depth);
		void render();
	};
}