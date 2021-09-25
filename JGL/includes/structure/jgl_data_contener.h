#pragma once

#include "jgl_includes.h"
#include "jgl_basic_types.h"
#include "structure/jgl_exception.h"

namespace jgl
{
	class Data_contener
	{
	private:
		jgl::Uint _size = 0;
		jgl::Uint _readed = 0;
		std::vector<jgl::Uchar> _content = {};

	public:
		/*
			Create an empty contener
		*/
		Data_contener()
		{
			clear();
		}

		/*
			Return the number of bytes stored in the contener
		*/
		jgl::Uint size() { return (_size); }

		/*
			Return the number of bytes readed.
		*/
		jgl::Uint readed() { return (_readed); }

		/*
			Return of the totality of stored bytes has been read
		*/
		bool empty() { return (_size <= _readed); }

		/*
			Clear the content of the contener
		*/
		void clear()
		{
			_size = 0;
			_readed = 0;
			_content.clear();
		}

		/*
			Reset the number of bytes readded, to read it again
		*/
		void reset() { _readed = 0; }

		/*
			Add data to the contener
		*/
		template<typename DataType>
		friend Data_contener& operator << (Data_contener& contener, const DataType& data)
		{
			static_assert(std::is_standard_layout<DataType>::value, "Data is too complex to be pushed into vector");

			size_t i = contener._content.size();

			contener._content.resize(contener._content.size() + sizeof(DataType));

			std::memcpy(contener._content.data() + i, &data, sizeof(DataType));

			contener._size = contener._content.size();

			return contener;
		}

		/*
			Obtain data from the contener
		*/
		template<typename DataType>
		friend Data_contener& operator >> (Data_contener& contener, DataType& data)
		{
			static_assert(std::is_standard_layout<DataType>::value, "Data is too complex to be pulled from vector");

			jgl::Uint tmp = sizeof(DataType);

			if ((contener._size - contener.readed()) < tmp)
				THROW_EXCEPTION(jgl::Error_level::Error, -4, "Not enought content in a data operator");

			std::memcpy(&data, contener._content.data() + contener._readed, tmp);

			contener._readed += tmp;

			return contener;
		}
	};
}