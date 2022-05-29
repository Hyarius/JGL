#pragma once

#include "jgl_basic_types.h"
#include "structure/jgl_string.h"
#include "structure/jgl_stream.h"

namespace jgl
{
	jgl::String itoa(const jgl::Int x, const jgl::Int d);

	enum class Error_level
	{
		Information = 0,
		Warning = 1,
		Error = 2,
		Critical = 3
	};

	class Exception
	{
	private:
		static Error_level _minimal_error_level;
		Error_level _level;
		jgl::Int _id;
		jgl::String _what;
		jgl::String _file;
		jgl::String _function;
		jgl::Int _line;

	public:
		Exception();
		static jgl::String error_level_text(Error_level level) {
			if (level == Error_level::Information)
				return ("Information");
			else if (level == Error_level::Warning)
				return ("Warning");
			else if (level == Error_level::Error)
				return ("Error");
			else if (level == Error_level::Critical)
				return ("Critical");
			else
				return ("Unknow");
		}
		static void set_minimal_error_level(Error_level p_level) { _minimal_error_level = p_level; }
		static Error_level minimal_error_level() { return (_minimal_error_level); }
		void set_level(Error_level p_level);
		void set_file(jgl::String p_file);
		void set_function(jgl::String p_function);
		void set_line(jgl::Int p_line);
		void define(jgl::Int p_id, jgl::String p_what);
		jgl::Int& id();
		const jgl::Int id() const;
		jgl::String& what();
		const jgl::String& what() const;

		friend jgl::Stream& operator<<(jgl::Stream& os, const Exception error)
		{
			os << "Error [" << error.id() << "] : " << error.what();
			return os;
		}

		jgl::String full() const;
		jgl::String partial() const;
	};
}

extern jgl::Exception e_exception;

namespace jgl
{
	void throw_exception(Error_level level, jgl::Int id, jgl::String msg, const char* file, const char* function, jgl::Int line);
	const Exception& get_error();
}
#ifndef _DEBUG // works in VS
#define THROW_INFORMATION(text) {}
#else
#define THROW_INFORMATION(text) {if (_DEBUG){jgl::throw_exception(jgl::Error_level::Information, 0, text, __FILE__, __FUNCTION__, __LINE__);}}
#endif

#define THROW_EXCEPTION(level, value, text) {jgl::throw_exception(level, value, text, __FILE__, __FUNCTION__, __LINE__);}