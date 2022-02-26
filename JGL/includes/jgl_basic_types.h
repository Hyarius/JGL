#pragma once

#include "jgl_includes.h"

namespace jgl
{
	using Bool = bool;

	using Char = int8_t;
	using Uchar = uint8_t;

	using Short = int16_t;
	using Ushort = uint16_t;

	using Int = int32_t;
	using Uint = uint32_t;
	using Size_t = uint32_t;

	using Long = int64_t;
	using Ulong = uint64_t;

	using Float = float_t;
	using Double = double_t;

	using Sampler2D = uint32_t;
	using Isampler2D = uint32_t;
	using Uisampler2D = uint32_t;

	template<class K, class E>
	using Map = std::map<K, E>;

	using File = std::fstream;

	enum class File_mode
	{
		in = std::ios_base::in,
		out = std::ios_base::out,
		clear_out = std::ios_base::out | std::ios_base::trunc,
		out_binary = std::ios_base::out | std::ios_base::binary,
		clear_in = std::ios_base::in | std::ios_base::trunc,
		in_binary = std::ios_base::in | std::ios_base::binary,
		app = std::ios_base::app,
		binary = std::ios_base::binary,
		ate = std::ios_base::ate,
	};

	enum class File_pos
	{
		Begin = std::ios_base::beg,
		End = std::ios_base::end,
		Current = std::ios_base::cur 
	};

	enum class File_type {
		Error = DT_UNKNOWN,
		File = DT_REG,
		Directory = DT_DIR,
		Fifo = DT_FIFO,
		Socket = DT_SOCK,
		Char_device = DT_CHR,
		Block = DT_BLK,
		Link = DT_LNK
	};
}