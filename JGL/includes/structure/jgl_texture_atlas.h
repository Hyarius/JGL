#pragma once

#include "jgl_basic_types.h"
#include "structure/jgl_string.h"
#include "structure/jgl_image.h"

namespace jgl
{
	class Texture_atlas
	{
	private:
		jgl::Map<jgl::String, jgl::Image_handler*> _content;

	public:
		void add_texture(jgl::String p_name, jgl::Image_handler* p_image_handler);
		jgl::Image_handler* get_texture(jgl::String p_name);
	};
}