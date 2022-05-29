#include "Structure/jgl_translation_atlas.h"
#include "structure/jgl_exception.h"
#include "jgl_file_handler.h"

namespace jgl
{
	void Translation_atlas::load(jgl::String p_path)
	{
		if (jgl::check_file_exist(p_path) == false)
		{
			THROW_EXCEPTION(jgl::Error_level::Error, 1, "Can't load file [" + p_path + "] : file does not exist");
		}
		else
		{
			_strings.clear();
			jgl::File file = jgl::open_file(p_path, jgl::File_mode::in);

			while (file.peek() != EOF)
			{
				jgl::String line = jgl::get_str(file);
				jgl::Array<jgl::String> tab = line.split(";");

				if (tab.size() != 2)
					THROW_EXCEPTION(jgl::Error_level::Error, 1, "Error while parsing langage file :: " + line + " is not a valid line");
				_strings[tab[0]] = tab[1];
			}
		}
	}

	jgl::String Translation_atlas::string(jgl::String p_key)
	{
		if (_strings.count(p_key) == 0)
			return (p_key);
		return (_strings[p_key]);
	}
}