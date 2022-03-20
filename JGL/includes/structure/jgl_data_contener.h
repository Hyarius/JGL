#pragma once

#include "jgl_includes.h"
#include "jgl_basic_types.h"
#include "structure/jgl_exception.h"
#include "jgl_basic_functions.h"

namespace jgl
{
	class Data_contener
	{
	public:
		struct Header
		{
			jgl::Size_t size = 0;
			mutable jgl::Size_t readed = 0;
		};
	protected:
		Header _header;
		std::vector<jgl::Uchar> _content = {};

	public:
		Header& header()
		{
			return (_header);
		}

		const Header& header() const
		{
			return (_header);
		}

		std::vector<jgl::Uchar>& content()
		{
			return (_content);
		}

		const std::vector<jgl::Uchar>& content() const
		{
			return (_content);
		}

		jgl::Uint size() const
		{
			return (static_cast<jgl::Uint>(_header.size - _header.readed));
		}

		jgl::Bool empty() const
		{
			return (_header.size == _header.readed);
		}

		/*
			Create a new empty data_contener with undefined ID
		*/
		Data_contener()
		{
			_content.clear();
		}

		void clear()
		{
			_header.size = 0;
			_header.readed = 0;
			_content.clear();
		}

		/*
			Reset the number of bytes readed. Do not release any memory allocated
		*/
		void reset()
		{
			_header.readed = 0;
		}

		void skip(jgl::Size_t p_nb_to_skip)
		{
			_header.readed += p_nb_to_skip;
		}

		void add_in_array(jgl::Uchar* p_array, jgl::Size_t p_length)
		{
			jgl::Size_t old_size = _content.size();

			_content.resize(_content.size() + p_length);

			std::memcpy(_content.data() + old_size, p_array, p_length);

			_header.size = _content.size();
		}

		void load_from_array(void* p_address, jgl::Size_t p_length) const
		{
			jgl::Size_t next_size = _header.readed;// content.size() - sizeof(DataType);

			std::memcpy(p_address, _content.data() + next_size, p_length);

			_header.readed += p_length;
		}

		void print(jgl::String p_text) const 
		{
			jgl::cout << p_text << " : ";
			for (jgl::Size_t i = 0; i < _content.size(); i++)
			{
				if (i != 0)
					jgl::cout << "-";
				std::bitset<8> y(_content[i]);
				jgl::cout << "[" << y << "]";
			}
			jgl::cout << jgl::endl;
		}

		template<typename DataType>
		Data_contener& operator << (const DataType& data)
		{
			static_assert(std::is_standard_layout<DataType>::value, "Data is too complex to be pushed into vector");

			jgl::Size_t old_size = (*this)._content.size();

			(*this)._content.resize((*this)._content.size() + sizeof(DataType));

			std::memcpy((*this)._content.data() + old_size, &data, sizeof(DataType));

			(*this)._header.size = (*this)._content.size();

			return (*this);
		}

		template<typename DataType>
		const Data_contener& operator >> (DataType& data) const 
		{
			static_assert(std::is_standard_layout<DataType>::value, "Data is too complex to be pulled from vector");

			jgl::Size_t next_size = (*this)._header.readed;// (*this).content.size() - sizeof(DataType);

			std::memcpy(&data, (*this)._content.data() + next_size, sizeof(DataType));

			(*this)._header.readed += sizeof(DataType);

			return (*this);
		}

		template<>
		Data_contener& operator << <jgl::String>	(const jgl::String& text)
		{
			*this << text.size();
			for (jgl::Uint i = 0; i < text.size(); i++)
				*this << text[i];
			return *this;
		}

		template<>
		const Data_contener& operator >> <jgl::String>	(jgl::String& text) const
		{
			jgl::Size_t size;
			jgl::Size_t i = 0;

			text.clear();
			*this >> size;
			while (empty() == false && i < size)
			{
				jgl::Glyph c;
				*this >> c;
				text.push_back(c);
				i++;
			}

			return *this;
		}
	};
}