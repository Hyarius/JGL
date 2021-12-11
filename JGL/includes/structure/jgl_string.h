#pragma once

#include "jgl_basic_types.h"
#include "jgl_glyph.h"
#include "jgl_array.h"

#define JGL_STRING_BUFFER_SIZE 100

namespace jgl
{
	class String
	{
	private:
		char* _str;

		Glyph _buffer[JGL_STRING_BUFFER_SIZE];
		Glyph** _content;
		Size_t _size;
		Size_t _max_size;

		void _clear_str()
		{
			if (_str != nullptr)
			{
				delete _str;
				_str = nullptr;
			}
		}

		void _resize(Size_t new_desired_size);

		void _add_new_line();

		void _delete_array_content();

		jgl::Size_t _count_word(const String& delim);

		jgl::Size_t _count_word(const jgl::Glyph& delim);

		jgl::Size_t _count_word_len(const jgl::Glyph& delim, const jgl::Size_t start);

		jgl::Size_t _count_word_len(const String& delim, const jgl::Size_t start);

	public:
		static jgl::Int npos;
		static Glyph nullchar;

		String();

		String(const char* str);

		String(Glyph* str);

		String(Glyph elem, Size_t nb = 1);

		String(const std::string& other);

		String(const std::wstring& other);

		String(const String& other);

		~String();

		const jgl::Size_t total_size() const
		{
			jgl::Size_t result = 0;

			for (jgl::Size_t i = 0; i < _size; i++)
			{
				result += this->operator[](i).size();
			}
			return (result);
		}

		const Size_t size() const;

		const Glyph& back() const;

		void push_back(const Glyph& elem);

		void push_front(const Glyph& elem);

		void append(const String& other);

		void clear();

		void resize(const Size_t& new_size);

		const jgl::Bool empty() const;

		const Int find(const String& to_find) const;

		const jgl::Bool compose_only(const String& to_find) const;

		String substr(const Size_t& start, const Size_t& size) const;

		void substr(String& result, const Size_t& start, const Size_t& size) const;

		void pop_back();

		void insert(const Glyph& to_add, const Size_t& index);

		void erase(Size_t index);

		const char* c_str();

		const unsigned char* uc_str();

		void split(jgl::Array<String>& tab, const String& delim, const jgl::Bool& regroup = true);

		jgl::Array<String> split(const String& delim, const jgl::Bool& regroup = true);

		Glyph& operator [](const Size_t index);

		const Glyph& operator [](const Size_t index) const;

		String& operator = (const String& p_value);

		String& operator = (const char* p_value);

		String operator + (const String& other) const;

		friend String operator + (const char* str, const String& delta);

		void operator += (const String& other);

		jgl::Bool operator == (const String& other) const;

		jgl::Bool operator != (const String& other) const;

		jgl::Bool operator < (const jgl::String& other) const;

		jgl::Bool operator > (const jgl::String& other) const;

		friend std::ostream& operator<<(std::ostream& os, const String& value)
		{
			for (Size_t i = 0; i < value.size(); i++)
			{
				os << value[i];
			}
			return os;
		}
	};
}