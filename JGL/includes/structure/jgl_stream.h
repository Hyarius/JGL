#pragma once

#include "jgl_includes.h"
#include "jgl_basic_types.h"
#include "jgl_string.h"

namespace jgl
{
	class Stream
	{
	private:
		jgl::String _prefix;
		std::ostringstream  _content;

		static std::mutex _console_mutex;

		typedef Stream& (*Function_pointer)(Stream&);
	public:
		Stream(jgl::String p_prefix);

		void flush();

		template <typename T>
		Stream& operator << (T const & p_element)
		{
			_content << p_element;
			return (*this);
		}

		Stream& operator << (const Function_pointer& p_funct)
		{
			return (*p_funct)(*this);
		}

		Stream& operator <<(const String& value)
		{
			for (Size_t i = 0; i < value.size(); i++)
			{
				_content << value[i];
			}
			return *this;
		}

		void set_prefix(jgl::String p_prefix);
	};

#define DEBUG_LINE(){DEBUG_LINE_TEXT("");}
#define DEBUG_LINE_TEXT(text){jgl::cout << __FUNCTION__ << "::" << __LINE__;if(text != ""){jgl::cout << "::" << text;}jgl::cout << jgl::endl;}
}
