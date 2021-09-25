#include "jgl.h"

namespace jgl
{
	Exception::Exception()
	{
		_level = Error_level::Error;
		_id = 0;
		_what = "";
		_file = "";
		_function = "";
		_line = 0;
	}
	void Exception::set_level(Error_level p_level)
	{
		_level = p_level;
	}
	void Exception::set_file(jgl::String p_file)
	{
		_file = p_file;
	}
	void Exception::set_function(jgl::String p_function)
	{
		_function = p_function;
	}
	void Exception::set_line(jgl::Int p_line)
	{
		_line = p_line;
	}
	void Exception::define(jgl::Int p_id, jgl::String p_what)
	{
		_id = p_id;
		_what = p_what;
	}
	jgl::Int& Exception::id()
	{
		return (_id);
	}
	const jgl::Int Exception::id() const
	{
		return (_id);
	}
	jgl::String& Exception::what()
	{
		return (_what);
	}
	const jgl::String& Exception::what() const
	{
		return (_what);
	}
	jgl::String Exception::full() const
	{
		jgl::String result;

		result = "[" + error_level_text(_level) + "] of id [" + jgl::itoa(_id, 0) + "] -> [" + _file + "][" + _function + "][" + jgl::itoa(_line, 0) + "]";
		result += what();

		return (result);
	}
	jgl::String Exception::partial() const
	{
		jgl::String result;
		if (_level == Error_level::Information)
		{
			result = "[" + error_level_text(_level) + "] : " + what();
		}
		else
			result = "[" + error_level_text(_level) + "] of id [" + jgl::itoa(_id, 0) + "] : " + what();
		return (result);
	}
}