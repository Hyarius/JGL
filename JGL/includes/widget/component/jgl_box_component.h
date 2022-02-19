#pragma once

#include "jgl_component.h"

#include "structure/jgl_sprite_sheet.h"
#include "structure/jgl_image.h"

namespace jgl::Widget_component
{
	class Box : public Graphical_component
	{
	private:
		struct Composed_texture_data
		{
			jgl::Bool computed;
			jgl::Sprite_sheet* sprite_sheet;

			jgl::Shader* shader;
			jgl::Buffer* model_buffer;
			jgl::Buffer* uvs_buffer;
			jgl::Buffer* alpha_buffer;
			jgl::Buffer* indexes_buffer;
			jgl::Uniform* texture_uniform;

			Composed_texture_data()
			{
				computed = false;
				sprite_sheet = nullptr;

				shader = nullptr;
				model_buffer = nullptr;
				uvs_buffer = nullptr;
				alpha_buffer = nullptr;
				indexes_buffer = nullptr;
				texture_uniform = nullptr;
			}
		};

		struct Single_image_data
		{
			jgl::Bool computed;
			jgl::Image* image;

			jgl::Shader* shader;
			jgl::Buffer* model_buffer;
			jgl::Buffer* uvs_buffer;
			jgl::Buffer* alpha_buffer;
			jgl::Buffer* indexes_buffer;
			jgl::Uniform* texture_uniform;

			Single_image_data()
			{
				computed = false;
				image = nullptr;

				shader = nullptr;
				model_buffer = nullptr;
				uvs_buffer = nullptr;
				alpha_buffer = nullptr;
				indexes_buffer = nullptr;
				texture_uniform = nullptr;
			}
		};
		
		struct Color_data
		{
			jgl::Bool computed;
			jgl::Color background_color;
			jgl::Color border_color;

			jgl::Shader* shader;
			jgl::Buffer* model_buffer;
			jgl::Buffer* color_buffer;
			jgl::Buffer* indexes_buffer;

			Color_data()
			{
				computed = false;
				background_color = jgl::Color(0, 0, 0, 0);
				border_color = jgl::Color(0, 0, 0, 0);

				shader = nullptr;
				model_buffer = nullptr;
				color_buffer = nullptr;
				indexes_buffer = nullptr;
			}
		};

		jgl::Size_t _border_size;

		Composed_texture_data _composed_texture_data;
		Single_image_data _image_data;
		Color_data _color_data;

		void _compute_composed_texture_data();
		void _compute_image_data();
		void _compute_color_data();

		void _render_composed_texture();
		void _render_image();
		void _render_color();

	public:
		Box(jgl::Widget* p_owner = nullptr);

		void set_border_size(jgl::Size_t p_border_size)
		{
			_border_size = p_border_size;
			_composed_texture_data.computed = false;
			_color_data.computed = false;
		}
		jgl::Size_t border_size() { return (_border_size); }

		void set_sprite_sheet(jgl::Sprite_sheet* p_sprite_sheet)
		{
			if (p_sprite_sheet->size() != 3)
				THROW_EXCEPTION(jgl::Error_level::Error, -1, "Wrong sprite sheet size given to a box component : need a 3x3 sprite sheet");
			_composed_texture_data.sprite_sheet = p_sprite_sheet;
		}
		jgl::Sprite_sheet* sprite_sheet() { return (_composed_texture_data.sprite_sheet); }

		void set_image(jgl::Image* p_image)
		{
			_image_data.image = p_image;
		}
		jgl::Image* image() { return (_image_data.image); }

		void set_color(jgl::Color p_background_color, jgl::Color p_border_color)
		{
			_color_data.background_color = p_background_color;
			_color_data.border_color = p_border_color;
			_color_data.computed = false;
		}
		void set_background_color(jgl::Color p_background_color)
		{
			_color_data.background_color = p_background_color;
			_color_data.computed = false;
		}
		void set_border_color(jgl::Color p_border_color)
		{
			_color_data.border_color = p_border_color;
			_color_data.computed = false;
		}

		jgl::Color background_color() { return (_color_data.background_color); }
		jgl::Color border_color() { return (_color_data.border_color); }

		void set_geometry(jgl::Vector2Int p_anchor, jgl::Vector2Int p_area, jgl::Float p_depth);
		void render();
	};
}