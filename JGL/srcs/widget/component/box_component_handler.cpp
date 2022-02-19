#include "jgl.h"

namespace jgl::Widget_component
{
	Box::Box(jgl::Widget* p_owner) : Graphical_component(p_owner)
	{
		_border_size = 5;
		_composed_texture_data = Composed_texture_data();
		_image_data = Single_image_data();
		_color_data = Color_data();
	}

	void Box::set_geometry(jgl::Vector2Int p_anchor, jgl::Vector2Int p_area, jgl::Float p_depth)
	{
		Graphical_component::set_geometry(p_anchor, p_area, p_depth);
		_composed_texture_data.computed = false;
		_image_data.computed = false;
		_color_data.computed = false;
	}

	void Box::_compute_composed_texture_data()
	{
		jgl::Array<jgl::Vector3> vertex_content;
		jgl::Array<jgl::Vector2> uvs_content;
		jgl::Array<jgl::Float> alpha_content;
		jgl::Array<jgl::Uint> element_content;

		_composed_texture_data.sprite_sheet->prepare_draw(
			vertex_content, uvs_content, alpha_content, element_content,
			jgl::Vector2Int(0, 0), _anchor, _border_size, _depth, 1.0f);
		_composed_texture_data.sprite_sheet->prepare_draw(
			vertex_content, uvs_content, alpha_content, element_content,
			jgl::Vector2Int(1, 0), _anchor + jgl::Vector2Int(_border_size, 0), jgl::Vector2Int(_area.x - _border_size * 2, _border_size), _depth, 1.0f);
		_composed_texture_data.sprite_sheet->prepare_draw(
			vertex_content, uvs_content, alpha_content, element_content,
			jgl::Vector2Int(2, 0), _anchor + jgl::Vector2Int(_area.x - _border_size, 0), _border_size, _depth, 1.0f);

		_composed_texture_data.sprite_sheet->prepare_draw(
			vertex_content, uvs_content, alpha_content, element_content,
			jgl::Vector2Int(0, 1), _anchor + jgl::Vector2Int(0, _border_size), jgl::Vector2Int(_border_size, _area.y - _border_size * 2), _depth, 1.0f);
		_composed_texture_data.sprite_sheet->prepare_draw(
			vertex_content, uvs_content, alpha_content, element_content,
			jgl::Vector2Int(1, 1), _anchor + _border_size, jgl::Vector2Int(_area.x - _border_size * 2, _area.y - _border_size * 2), _depth, 1.0f);
		_composed_texture_data.sprite_sheet->prepare_draw(
			vertex_content, uvs_content, alpha_content, element_content,
			jgl::Vector2Int(2, 1), _anchor + jgl::Vector2Int(_area.x - _border_size, _border_size), jgl::Vector2Int(_border_size, _area.y - _border_size * 2), _depth, 1.0f);

		_composed_texture_data.sprite_sheet->prepare_draw(
			vertex_content, uvs_content, alpha_content, element_content,
			jgl::Vector2Int(0, 2), _anchor + jgl::Vector2Int(0, _area.y - _border_size), _border_size, _depth, 1.0f);
		_composed_texture_data.sprite_sheet->prepare_draw(
			vertex_content, uvs_content, alpha_content, element_content,
			jgl::Vector2Int(1, 2), _anchor + jgl::Vector2Int(_border_size, _area.y - _border_size), jgl::Vector2Int(_area.x - _border_size * 2, _border_size), _depth, 1.0f);
		_composed_texture_data.sprite_sheet->prepare_draw(
			vertex_content, uvs_content, alpha_content, element_content,
			jgl::Vector2Int(2, 2), _anchor + jgl::Vector2Int(_area.x - _border_size, _area.y - _border_size), _border_size, _depth, 1.0f);

		if (_composed_texture_data.shader == nullptr)
			_composed_texture_data.shader = jgl::Application::active_application()->shader("Texture_shader_2D");
		if (_composed_texture_data.model_buffer == nullptr)
			_composed_texture_data.model_buffer = _composed_texture_data.shader->buffer("model_space")->copy();
		if (_composed_texture_data.uvs_buffer == nullptr)
			_composed_texture_data.uvs_buffer = _composed_texture_data.shader->buffer("vertexUV")->copy();
		if (_composed_texture_data.alpha_buffer == nullptr)
			_composed_texture_data.alpha_buffer = _composed_texture_data.shader->buffer("alpha_value")->copy();
		if (_composed_texture_data.indexes_buffer == nullptr)
			_composed_texture_data.indexes_buffer = _composed_texture_data.shader->indexes_buffer()->copy();
		if (_composed_texture_data.texture_uniform == nullptr)
			_composed_texture_data.texture_uniform = _composed_texture_data.shader->uniform("textureID")->copy();

		_composed_texture_data.model_buffer->send(vertex_content.all(), vertex_content.size());
		_composed_texture_data.uvs_buffer->send(uvs_content.all(), uvs_content.size());
		_composed_texture_data.alpha_buffer->send(alpha_content.all(), alpha_content.size());
		_composed_texture_data.indexes_buffer->send(element_content.all(), element_content.size());

		_composed_texture_data.computed = true;
	}

	void Box::_render_composed_texture()
	{
		if (_composed_texture_data.computed == false)
		{
			_compute_composed_texture_data();
		}
		_composed_texture_data.shader->activate();

		_composed_texture_data.sprite_sheet->activate();
		_composed_texture_data.texture_uniform->send(0);

		_composed_texture_data.model_buffer->activate();
		_composed_texture_data.uvs_buffer->activate();
		_composed_texture_data.alpha_buffer->activate();
		_composed_texture_data.indexes_buffer->activate();

		_composed_texture_data.shader->cast(jgl::Shader::Mode::Triangle, _composed_texture_data.indexes_buffer->size() / sizeof(jgl::Uint));
	}

	void Box::_compute_image_data()
	{
		jgl::Array<jgl::Vector3> vertex_content;
		jgl::Array<jgl::Vector2> uvs_content;
		jgl::Array<jgl::Float> alpha_content;
		jgl::Array<jgl::Uint> element_content;

		_image_data.image->prepare_draw(
			vertex_content, uvs_content, alpha_content, element_content,
			_anchor, _area, 0, 1, _depth, 1.0f);

		if (_image_data.shader == nullptr)
			_image_data.shader = jgl::Application::active_application()->shader("Texture_shader_2D");
		if (_image_data.model_buffer == nullptr)
			_image_data.model_buffer = _image_data.shader->buffer("model_space")->copy();
		if (_image_data.uvs_buffer == nullptr)
			_image_data.uvs_buffer = _image_data.shader->buffer("vertexUV")->copy();
		if (_image_data.alpha_buffer == nullptr)
			_image_data.alpha_buffer = _image_data.shader->buffer("alpha_value")->copy();
		if (_image_data.indexes_buffer == nullptr)
			_image_data.indexes_buffer = _image_data.shader->indexes_buffer()->copy();
		if (_image_data.texture_uniform == nullptr)
			_image_data.texture_uniform = _image_data.shader->uniform("textureID")->copy();

		_image_data.model_buffer->send(vertex_content.all(), vertex_content.size());
		_image_data.uvs_buffer->send(uvs_content.all(), uvs_content.size());
		_image_data.alpha_buffer->send(alpha_content.all(), alpha_content.size());
		_image_data.indexes_buffer->send(element_content.all(), element_content.size());

		_image_data.computed = true;
	}

	void Box::_render_image()
	{
		if (_image_data.computed == false)
		{
			_compute_image_data();
		}
		_image_data.shader->activate();

		_image_data.image->activate();
		_image_data.texture_uniform->send(0);

		_image_data.model_buffer->activate();
		_image_data.uvs_buffer->activate();
		_image_data.alpha_buffer->activate();
		_image_data.indexes_buffer->activate();

		_image_data.shader->cast(jgl::Shader::Mode::Triangle, _image_data.indexes_buffer->size() / sizeof(jgl::Uint));
	}

	void Box::_compute_color_data()
	{
		jgl::Array<jgl::Vector3> vertex_content;
		jgl::Array<jgl::Color> color_content;
		jgl::Array<jgl::Uint> element_content;


		jgl::prepare_rectangle_color(vertex_content, color_content, element_content,
			_color_data.border_color, _anchor, _area, _depth);
		jgl::prepare_rectangle_color(vertex_content, color_content, element_content,
			_color_data.background_color, _anchor + _border_size, _area - _border_size * 2, _depth + 0.01f);

		if (_color_data.shader == nullptr)
			_color_data.shader = jgl::Application::active_application()->shader("Color_shader_2D");
		if (_color_data.model_buffer == nullptr)
			_color_data.model_buffer = _color_data.shader->buffer("model_space")->copy();
		if (_color_data.color_buffer == nullptr)
			_color_data.color_buffer = _color_data.shader->buffer("color_space")->copy();
		if (_color_data.indexes_buffer == nullptr)
			_color_data.indexes_buffer = _color_data.shader->indexes_buffer()->copy();

		_color_data.model_buffer->send(vertex_content.all(), vertex_content.size());
		_color_data.color_buffer->send(color_content.all(), color_content.size());
		_color_data.indexes_buffer->send(element_content.all(), element_content.size());

		_color_data.computed = true;
	}

	void Box::_render_color()
	{
		if (_color_data.computed == false)
		{
			_compute_color_data();
		}
		_color_data.shader->activate();

		_color_data.model_buffer->activate();
		_color_data.color_buffer->activate();
		_color_data.indexes_buffer->activate();

		_color_data.shader->cast(jgl::Shader::Mode::Triangle, _color_data.indexes_buffer->size() / sizeof(jgl::Uint));
	}

	void Box::render()
	{
		if (_image_data.image != nullptr)
		{
			_render_image();
		}
		else if (_composed_texture_data.sprite_sheet != nullptr)
		{
			_render_composed_texture();
		}
		else
		{
			_render_color();
		}
	}
}