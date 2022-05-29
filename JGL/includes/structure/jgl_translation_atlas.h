#pragma once

#include "jgl_basic_types.h"
#include "structure/jgl_string.h"

namespace jgl
{
	class Translation_atlas
	{
	private:
		jgl::Map<jgl::String, jgl::String> _strings;

	public:
		void load(jgl::String p_path);
		jgl::String string(jgl::String p_key);
	};
}