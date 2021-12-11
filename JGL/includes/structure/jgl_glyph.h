#pragma once

#include "jgl_basic_types.h"

namespace jgl
{
	class Glyph
	{
	private:
		char _content[5] = { 0, 0, 0, 0, 0 };

		static jgl::Int _count_char_len(const char to_parse)
		{
			jgl::Int limit = 0;

			if (	 (to_parse & 0b10000000) == 0)
				return (1);
			else if ((to_parse & 0b11100000) == 0b11000000)
				return (2);
			else if ((to_parse & 0b11110000) == 0b11100000)
				return (3);
			else if ((to_parse & 0b11111000) == 0b11110000)
				return (4);
			else
				return (-1);
		}

	public:
		/*
			Parse an array of char [base], starting at [index], for unicode glyph parsing, and place it in [result], than return the number of char used
			[result] need to be a char[5] or risk of segfault. This method will place in result 
		*/
		static jgl::Int create_char_unicode(char* result, const char* base, const uint32_t index)
		{
			unsigned char to_parse = base[index];
			jgl::Int limit = _count_char_len(to_parse);

			if (limit == -1)
				return (-1);

			if (result == nullptr)
				return (limit);

			for (jgl::Int j = 0; j < limit; j++)
				result[j] = base[index + j];
			result[limit] = 0;
			return (limit);
		}

		/*
			Glyph constructor base of a jgl::Int
			Glyph expressed as UTF-8 value
		*/
		Glyph(jgl::Int p_value) {
			char* str = (char*)(&p_value);
			jgl::Int len = create_char_unicode(nullptr, str, 0);
			for (jgl::Int i = 0; i < len; i++)
				_content[i] = str[i];
		}

		/*
			Glyph constructor base of a jgl::Size_t
			Glyph expressed as UTF-8 value
		*/
		Glyph(jgl::Size_t p_value) {
			char* str = (char*)(&p_value);
			jgl::Int len = create_char_unicode(nullptr, str, 0);
			for (jgl::Int i = 0; i < len; i++)
				_content[i] = str[i];
		}

		/*
			Glyph constructor base of a char
			Glyph expressed as UTF-8 value
		*/
		Glyph(char c)
		{
			_content[0] = c;
		}

		/*
			Glyph constructor base of a wchar_t
			Glyph expressed as UTF-8 value
		*/
		Glyph(wchar_t p_value)
		{
			char* str = (char*)(&p_value);
			jgl::Int len = create_char_unicode(nullptr, str, 0);
			for (jgl::Int i = 0; i < len; i++)
				_content[i] = str[i];
		}

		/*
			Glyph constructor base of a char array
			Glyph expressed as UTF-8 value
		*/
		Glyph(const char* str)
		{
			jgl::Int len = create_char_unicode(nullptr, str, 0);
			for (jgl::Int i = 0; i < len; i++)
				_content[i] = str[i];
		}

		/*
			Copy constructor
		*/
		Glyph(const Glyph& p_value)
		{
			for (jgl::Size_t i = 0; i < 4; i++)
				_content[i] = p_value._content[i];
		}

		/*
			Default constructor
			Glyph expressed as UTF-8 value
		*/
		Glyph()
		{
			for (jgl::Size_t i = 0; i < 4; i++)
				_content[i] = '\0';
		}

		/*
			Return the content of the glyph
			The content is a const char [5]
			/!\ DO NOT DELETE THIS POINTER /!\
		*/
		const char* content() const {
			return (_content);
		}

		/*
			Return the number of char used to describ this glyph
		*/
		jgl::Size_t size() const
		{
			jgl::Size_t result = 0;

			for (jgl::Size_t i = 0; _content[i] != 0; i++)
				result++;
			return (result);
		}

		/*
			Return the glyph value
			Glyph expressed as UTF-8 value
		*/
		jgl::Size_t value() const
		{
			jgl::Size_t self_value = static_cast<jgl::Uchar>(_content[0]) + 255 * static_cast<jgl::Uchar>(_content[1]) + 255 * 255 * static_cast<jgl::Uchar>(_content[2]) + 255 * 255 * 255 * static_cast<jgl::Uchar>(_content[3]);

			return (self_value);
		}

		/*
			Check if two glyph are equal
			Return true if equal, false if not
		*/
		bool operator == (const Glyph& other) const 
		{
			if (value() == other.value())
				return (true);
			return (false);
		}

		/*
			Check if two glyph are different
			Return true if different, false if not
		*/
		bool operator != (const Glyph& other) const
		{
			if (value() != other.value())
				return (true);
			return (false);
		}

		/*
			Check if one glyph is greater than [other]
			Return true if greater, false if not
		*/
		bool operator > (const Glyph& other) const
		{
			if (value() > other.value())
				return (true);
			return (false);
		}

		/*
			Check if one glyph is lower than [other]
			Return true if lower, false if not
		*/
		bool operator < (const Glyph& other) const
		{
			if (value() < other.value())
				return (true);
			return (false);
		}

		/*
			Check if one glyph is greater or equal than [other]
			Return true if greater or equal, false if not
		*/
		bool operator >= (const Glyph& other) const
		{
			if (value() >= other.value())
				return (true);
			return (false);
		}

		/*
			Check if one glyph is lower or equal than [other]
			Return true if lower or equal, false if not
		*/
		bool operator <= (const Glyph& other) const
		{
			if (value() <= other.value())
				return (true);
			return (false);
		}

		/*
			Print the glyph
		*/
		friend std::ostream& operator<<(std::ostream& os, const jgl::Glyph& value)
		{
			os << value.content();
			return os;
		}

		/*
			Return true if the glyph is printable
			See ASCII/UTF-8 chart for more information
		*/
		jgl::Bool is_printable() const
		{
			if (value() >= ' ')
				return (true);
			return (false);
		}
	};
}