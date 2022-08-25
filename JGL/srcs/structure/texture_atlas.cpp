#include "Structure/jgl_texture_atlas.h"

namespace jgl
{
	jgl::Image_handler* Texture_atlas::add_texture(jgl::String p_name, jgl::Image_handler* p_image_handler)
	{
		if (_content[p_name] != nullptr)
			delete _content[p_name];
		_content[p_name] = p_image_handler;
		return (p_image_handler);
	}

	jgl::Image_handler* Texture_atlas::get_texture(jgl::String p_name)
	{
		if (_content.count(p_name) == 0)
			return (nullptr);
		return (_content[p_name]);
	}
}