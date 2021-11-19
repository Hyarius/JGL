#include "jgl.h"

namespace jgl::Widget_component
{
	Box::Box(jgl::Widget* p_owner) : Graphical_component(p_owner)
	{
		_background = jgl::Color(100, 100, 100);
		_frontground = jgl::Color(150, 150, 150);
		_depth = 0;
		_background_tileset = nullptr;
		_init_texture();
	}

	Box::Box(jgl::Color p_background, jgl::Color p_frontground, jgl::Widget* p_owner) : Box(p_owner)
	{
		_background = p_background;
		_frontground = p_frontground;
	}

	void Box::_init_texture()
	{
		_shader = jgl::Application::active_application()->shader("Texture_shader_2D");

		_element_buffer = _shader->indexes_buffer()->copy();
		const jgl::Map<jgl::String, jgl::Uniform*> uniforms = _shader->uniforms();
		for (auto it = uniforms.begin(); it != uniforms.end(); it++)
		{
			THROW_INFORMATION("Copying uniform : " + it->first);
			_uniforms[it->first] = it->second->copy();
		}

		const jgl::Map<jgl::String, jgl::Buffer*>& buffers = _shader->buffers();
		for (auto it = buffers.begin(); it != buffers.end(); it++)
		{
			THROW_INFORMATION("Copying buffer : " + it->first);
			_buffers[it->first] = it->second->copy();
		}

		_compute_texture();
	}

	Box::Box(jgl::String p_path, jgl::Widget* p_owner) : Box(p_owner)
	{
		_background_tileset = new jgl::Sprite_sheet(p_path, 3);
	}
	Box::Box(jgl::Sprite_sheet* p_background_tileset, jgl::Widget* p_owner) : Box(p_owner)
	{
		_background_tileset = p_background_tileset;
	}

	void Box::set_geometry(jgl::Vector2Int p_anchor, jgl::Vector2Int p_area, jgl::Float p_depth)
	{
		Graphical_component::set_geometry(p_anchor, p_area, p_depth);

		if (_background_tileset != nullptr)
		{
			_computed = false;
			_compute_texture();
		}
	}

	void Box::render()
	{
		if (_background_tileset == nullptr)
			_render_color();
		else
			_render_texture();
	}

	void Box::_init()
	{
		if (_initialize == true)
			return;

		if (_shader == nullptr || _element_buffer == nullptr || _uniforms.size() == 0 || _buffers.size() == 0)
		{
			THROW_EXCEPTION(jgl::Error_level::Error, 1, "Trying to compute a widget texture without the shader needed");
		}

		_buffers["vertexUV"]->send(C_UV_BUFFER_CONTENT, 16);
		_buffers["alpha_value"]->send(C_ALPHA_BUFFER_CONTENT, 16);
		_element_buffer->send(C_ELEMENT_BUFFER_CONTENT, 54);

		_initialize = true;
	}

	void Box::_compute_texture()
	{
		if (_shader == nullptr || _element_buffer == nullptr || _uniforms.size() == 0 || _buffers.size() == 0)
		{
			THROW_EXCEPTION(jgl::Error_level::Error, 1, "Trying to compute a widget texture without the shader needed");
		}

		_init();

		if (_area == 0)
			return;

		if (_computed == true)
			return;
		
		if (_owner->parent() != nullptr && _owner->parent()->viewport() != nullptr)
		{
			_owner->parent()->viewport()->use();
		}

		jgl::Vector3 tmp_vertex[16] = {
			jgl::convert_screen_to_opengl(_anchor + jgl::Vector2Int(0, 0), _depth),
			jgl::convert_screen_to_opengl(_anchor + jgl::Vector2Int(_angle_size, 0), _depth),
			jgl::convert_screen_to_opengl(_anchor + jgl::Vector2Int(_area.x - _angle_size, 0), _depth),
			jgl::convert_screen_to_opengl(_anchor + jgl::Vector2Int(_area.x, 0), _depth),

			jgl::convert_screen_to_opengl(_anchor + jgl::Vector2Int(0, _angle_size), _depth),
			jgl::convert_screen_to_opengl(_anchor + jgl::Vector2Int(_angle_size, _angle_size), _depth),
			jgl::convert_screen_to_opengl(_anchor + jgl::Vector2Int(_area.x - _angle_size, _angle_size), _depth),
			jgl::convert_screen_to_opengl(_anchor + jgl::Vector2Int(_area.x, _angle_size), _depth),

			jgl::convert_screen_to_opengl(_anchor + jgl::Vector2Int(0, _area.y - _angle_size), _depth),
			jgl::convert_screen_to_opengl(_anchor + jgl::Vector2Int(_angle_size, _area.y - _angle_size), _depth),
			jgl::convert_screen_to_opengl(_anchor + jgl::Vector2Int(_area.x - _angle_size, _area.y - _angle_size), _depth),
			jgl::convert_screen_to_opengl(_anchor + jgl::Vector2Int(_area.x, _area.y - _angle_size), _depth),

			jgl::convert_screen_to_opengl(_anchor + jgl::Vector2Int(0, _area.y), _depth),
			jgl::convert_screen_to_opengl(_anchor + jgl::Vector2Int(_angle_size, _area.y), _depth),
			jgl::convert_screen_to_opengl(_anchor + jgl::Vector2Int(_area.x - _angle_size, _area.y), _depth),
			jgl::convert_screen_to_opengl(_anchor + jgl::Vector2Int(_area.x, _area.y), _depth),
		};

		_buffers["model_space"]->send(tmp_vertex, 16);

		_computed = true;
	}

	void Box::_render_color()
	{
		jgl::draw_rectangle_color(_frontground, _anchor + _border_size, _area - _border_size * 2, _depth);
		jgl::draw_rectangle_color(_background, _anchor, _area, _depth);
	}
	void Box::_render_texture()
	{
		_shader->activate();

		_background_tileset->activate();

		_shader->uniform("textureID")->send(0);

		for (auto it = _buffers.begin(); it != _buffers.end(); it++)
		{
			it->second->activate();
		}
		_element_buffer->activate();
		_shader->cast(jgl::Shader::Mode::Triangle, _element_buffer->size() / sizeof(jgl::Uint));

		_background_tileset->desactivate();
	}

	const jgl::Vector2 jgl::Widget_component::Box::C_UV_UNIT = jgl::Vector2(1.0f / 3.0f, 1.0f / 3.0f);
	const jgl::Vector2 jgl::Widget_component::Box::C_UV_BUFFER_CONTENT[16] = {
		jgl::Vector2(0, 0) * C_UV_UNIT, jgl::Vector2(1, 0) * C_UV_UNIT, jgl::Vector2(2, 0) * C_UV_UNIT, jgl::Vector2(3, 0) * C_UV_UNIT,
		jgl::Vector2(0, 1) * C_UV_UNIT, jgl::Vector2(1, 1) * C_UV_UNIT, jgl::Vector2(2, 1) * C_UV_UNIT, jgl::Vector2(3, 1) * C_UV_UNIT,
		jgl::Vector2(0, 2) * C_UV_UNIT, jgl::Vector2(1, 2) * C_UV_UNIT, jgl::Vector2(2, 2) * C_UV_UNIT, jgl::Vector2(3, 2) * C_UV_UNIT,
		jgl::Vector2(0, 3) * C_UV_UNIT, jgl::Vector2(1, 3) * C_UV_UNIT, jgl::Vector2(2, 3) * C_UV_UNIT, jgl::Vector2(3, 3) * C_UV_UNIT,
	};

	const jgl::Float jgl::Widget_component::Box::C_ALPHA_BUFFER_CONTENT[16] = {
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
	};

	const jgl::Uint jgl::Widget_component::Box::C_ELEMENT_BUFFER_CONTENT[54] = {
		0, 4, 1, 1, 4, 5,
		1, 5, 2, 2, 5, 6,
		2, 6, 3, 3, 6, 7,
		4, 8, 5, 5, 8, 9,
		5, 9, 6, 6, 9, 10,
		6, 10, 7, 7, 10, 11,
		8, 12, 9, 9, 12, 13,
		9, 13, 10, 10, 13, 14,
		10, 14, 11, 11, 14, 15
	};
}