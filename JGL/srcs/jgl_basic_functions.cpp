#include "jgl.h"

#include <time.h>
#include <stdlib.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "external_lib/stb_image_write.h"

#include "jgl_define.h"

jgl::Exception e_exception;

namespace jgl
{
	jgl::Int jgl::String::npos = -1;
	Glyph jgl::String::nullchar = '\0';
	jgl::Error_level Exception::_minimal_error_level = jgl::Error_level::Error;

	std::string methodName(const std::string& prettyFunction)
	{
		size_t begin = prettyFunction.rfind("::") + 2;
		size_t end = prettyFunction.rfind("(") - begin;

		return prettyFunction.substr(begin, end) + "()";
	}

	std::string className(const std::string& prettyFunction)
	{
		size_t colons = prettyFunction.rfind("::");
		if (colons == jgl::String::npos)
			return "::";
		size_t begin = prettyFunction.substr(0, colons).rfind(" ") + 1;
		size_t end = colons - begin;

		return prettyFunction.substr(begin, end);
	}

	jgl::String glGetTypeString(GLenum type)
	{
		switch (type)
		{
		case GL_NO_ERROR: return ("No error");
		case GL_INVALID_ENUM: return ("Invalid enum");
		case GL_INVALID_VALUE: return ("Invalid value");
		case GL_INVALID_OPERATION: return ("Invalid operation");
		case GL_INVALID_FRAMEBUFFER_OPERATION: return ("Invalid framebuffer operation");
		case GL_OUT_OF_MEMORY: return ("Out of memory");
		case GL_STACK_UNDERFLOW: return ("Stack underflow");
		case GL_STACK_OVERFLOW: return ("Stack overflow");
			
		case GL_ELEMENT_ARRAY_BUFFER: return ("GL_ELEMENT_ARRAY_BUFFER");
		case GL_ARRAY_BUFFER: return ("GL_ARRAY_BUFFER");
		case GL_BYTE: return "GLbyte";
		case GL_UNSIGNED_BYTE: return "GLubyte";
		case GL_UNSIGNED_SHORT: return "GLushort";
		case GL_2_BYTES: return "GL_2_BYTES";
		case GL_3_BYTES: return "GL_3_BYTES";
		case GL_4_BYTES: return "GL_4_BYTES";

		case 	GL_FLOAT: return "float";
		case 	GL_FLOAT_VEC2: return "vec2";
		case 	GL_FLOAT_VEC3: return "vec3";
		case 	GL_FLOAT_VEC4: return "vec4";
		case 	GL_DOUBLE: return "double";
		case 	GL_DOUBLE_VEC2: return "dvec2";
		case 	GL_DOUBLE_VEC3: return "dvec3";
		case 	GL_DOUBLE_VEC4: return "dvec4";
		case 	GL_INT: return "int";
		case 	GL_INT_VEC2: return "ivec2";
		case 	GL_INT_VEC3: return "ivec3";
		case 	GL_INT_VEC4: return "ivec4";
		case 	GL_UNSIGNED_INT: return "unsigned int";
		case 	GL_UNSIGNED_INT_VEC2: return "uvec2";
		case 	GL_UNSIGNED_INT_VEC3: return "uvec3";
		case 	GL_UNSIGNED_INT_VEC4: return "uvec4";
		case 	GL_BOOL: return "bool";
		case 	GL_BOOL_VEC2: return "bvec2";
		case 	GL_BOOL_VEC3: return "bvec3";
		case 	GL_BOOL_VEC4: return "bvec4";
		case 	GL_FLOAT_MAT2: return "mat2";
		case 	GL_FLOAT_MAT3: return "mat3";
		case 	GL_FLOAT_MAT4: return "mat4";
		case 	GL_FLOAT_MAT2x3: return "mat2x3";
		case 	GL_FLOAT_MAT2x4: return "mat2x4";
		case 	GL_FLOAT_MAT3x2: return "mat3x2";
		case 	GL_FLOAT_MAT3x4: return "mat3x4";
		case 	GL_FLOAT_MAT4x2: return "mat4x2";
		case 	GL_FLOAT_MAT4x3: return "mat4x3";
		case 	GL_DOUBLE_MAT2: return "dmat2";
		case 	GL_DOUBLE_MAT3: return "dmat3";
		case 	GL_DOUBLE_MAT4: return "dmat4";
		case 	GL_DOUBLE_MAT2x3: return "dmat2x3";
		case 	GL_DOUBLE_MAT2x4: return "dmat2x4";
		case 	GL_DOUBLE_MAT3x2: return "dmat3x2";
		case 	GL_DOUBLE_MAT3x4: return "dmat3x4";
		case 	GL_DOUBLE_MAT4x2: return "dmat4x2";
		case 	GL_DOUBLE_MAT4x3: return "dmat4x3";
		case 	GL_SAMPLER_1D: return "sampler1D";
		case 	GL_SAMPLER_2D: return "sampler2D";
		case 	GL_SAMPLER_3D: return "sampler3D";
		case 	GL_SAMPLER_CUBE: return "samplerCube";
		case 	GL_SAMPLER_1D_SHADOW: return "sampler1DShadow";
		case 	GL_SAMPLER_2D_SHADOW: return "sampler2DShadow";
		case 	GL_SAMPLER_1D_ARRAY: return "sampler1DArray";
		case 	GL_SAMPLER_2D_ARRAY: return "sampler2DArray";
		case 	GL_SAMPLER_1D_ARRAY_SHADOW: return "sampler1DArrayShadow";
		case 	GL_SAMPLER_2D_ARRAY_SHADOW: return "sampler2DArrayShadow";
		case 	GL_SAMPLER_2D_MULTISAMPLE: return "sampler2DMS";
		case 	GL_SAMPLER_2D_MULTISAMPLE_ARRAY: return "sampler2DMSArray";
		case 	GL_SAMPLER_CUBE_SHADOW: return "samplerCubeShadow";
		case 	GL_SAMPLER_BUFFER: return "samplerBuffer";
		case 	GL_SAMPLER_2D_RECT: return "sampler2DRect";
		case 	GL_SAMPLER_2D_RECT_SHADOW: return "sampler2DRectShadow";
		case 	GL_INT_SAMPLER_1D: return "isampler1D";
		case 	GL_INT_SAMPLER_2D: return "isampler2D";
		case 	GL_INT_SAMPLER_3D: return "isampler3D";
		case 	GL_INT_SAMPLER_CUBE: return "isamplerCube";
		case 	GL_INT_SAMPLER_1D_ARRAY: return "isampler1DArray";
		case 	GL_INT_SAMPLER_2D_ARRAY: return "isampler2DArray";
		case 	GL_INT_SAMPLER_2D_MULTISAMPLE: return "isampler2DMS";
		case 	GL_INT_SAMPLER_2D_MULTISAMPLE_ARRAY: return "isampler2DMSArray";
		case 	GL_INT_SAMPLER_BUFFER: return "isamplerBuffer";
		case 	GL_INT_SAMPLER_2D_RECT: return "isampler2DRect";
		case 	GL_UNSIGNED_INT_SAMPLER_1D: return "usampler1D";
		case 	GL_UNSIGNED_INT_SAMPLER_2D: return "usampler2D";
		case 	GL_UNSIGNED_INT_SAMPLER_3D: return "usampler3D";
		case 	GL_UNSIGNED_INT_SAMPLER_CUBE: return "usamplerCube";
		case 	GL_UNSIGNED_INT_SAMPLER_1D_ARRAY: return "usampler2DArray";
		case 	GL_UNSIGNED_INT_SAMPLER_2D_ARRAY: return "usampler2DArray";
		case 	GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE: return "usampler2DMS";
		case 	GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY: return "usampler2DMSArray";
		case 	GL_UNSIGNED_INT_SAMPLER_BUFFER: return "usamplerBuffer";
		case 	GL_UNSIGNED_INT_SAMPLER_2D_RECT: return "usampler2DRect";
		case 	GL_IMAGE_1D: return "image1D";
		case 	GL_IMAGE_2D: return "image2D";
		case 	GL_IMAGE_3D: return "image3D";
		case 	GL_IMAGE_2D_RECT: return "image2DRect";
		case 	GL_IMAGE_CUBE: return "imageCube";
		case 	GL_IMAGE_BUFFER: return "imageBuffer";
		case 	GL_IMAGE_1D_ARRAY: return "image1DArray";
		case 	GL_IMAGE_2D_ARRAY: return "image2DArray";
		case 	GL_IMAGE_2D_MULTISAMPLE: return "image2DMS";
		case 	GL_IMAGE_2D_MULTISAMPLE_ARRAY: return "image2DMSArray";
		case 	GL_INT_IMAGE_1D: return "iimage1D";
		case 	GL_INT_IMAGE_2D: return "iimage2D";
		case 	GL_INT_IMAGE_3D: return "iimage3D";
		case 	GL_INT_IMAGE_2D_RECT: return "iimage2DRect";
		case 	GL_INT_IMAGE_CUBE: return "iimageCube";
		case 	GL_INT_IMAGE_BUFFER: return "iimageBuffer";
		case 	GL_INT_IMAGE_1D_ARRAY: return "iimage1DArray";
		case 	GL_INT_IMAGE_2D_ARRAY: return "iimage2DArray";
		case 	GL_INT_IMAGE_2D_MULTISAMPLE: return "iimage2DMS";
		case 	GL_INT_IMAGE_2D_MULTISAMPLE_ARRAY: return "iimage2DMSArray";
		case 	GL_UNSIGNED_INT_IMAGE_1D: return "uimage1D";
		case 	GL_UNSIGNED_INT_IMAGE_2D: return "uimage2D";
		case 	GL_UNSIGNED_INT_IMAGE_3D: return "uimage3D";
		case 	GL_UNSIGNED_INT_IMAGE_2D_RECT: return "uimage2DRect";
		case 	GL_UNSIGNED_INT_IMAGE_CUBE: return "uimageCube";
		case 	GL_UNSIGNED_INT_IMAGE_BUFFER: return "uimageBuffer";
		case 	GL_UNSIGNED_INT_IMAGE_1D_ARRAY: return "uimage1DArray";
		case 	GL_UNSIGNED_INT_IMAGE_2D_ARRAY: return "uimage2DArray";
		case 	GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE: return "uimage2DMS";
		case 	GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE_ARRAY: return "uimage2DMSArray";
		case 	GL_UNSIGNED_INT_ATOMIC_COUNTER: return "atomic_uint";


		default: return "not a GLenum type";
		}
	}

	void check_opengl(jgl::String msg)
	{
		GLenum err = glGetError();
		if (err != GL_NO_ERROR)
		{
			THROW_EXCEPTION(jgl::Error_level::Error, 1, msg + " : type [" + glGetTypeString(err) + "]");
		}
	}

	jgl::Int start()
	{
		bool is_started = true;

		if (is_started == false)
			THROW_EXCEPTION(jgl::Error_level::Critical, 0, "Error while starting JGL data");

		srand(time(NULL));

		SetConsoleOutputCP(1252);
		SetConsoleCP(1252);

		THROW_INFORMATION("Encoding set to allow UTF16");

		return (0);
	}

	jgl::Int stop()
	{
		bool quit_completed = true;

		if (quit_completed == false)
			THROW_EXCEPTION(jgl::Error_level::Critical, -2, "Error while quiting JGL data");

		THROW_INFORMATION("Closing of jgl complete");
		return (0);
	}

	void throw_exception(jgl::Error_level level, jgl::Int id, jgl::String msg, const char* file, const char* function, jgl::Int line)
	{
		e_exception.set_level(level);
		e_exception.set_file(jgl::String(__FILE__));
		e_exception.set_function(jgl::String(__FUNCTION__));
		e_exception.set_line(__LINE__);
		e_exception.define(id, msg);
		jgl::cout << e_exception.partial() << jgl::endl;
		if (static_cast<jgl::Int>(jgl::Exception::minimal_error_level()) <= static_cast<jgl::Int>(level))
		{
			throw (e_exception);
		}
	}

	const Exception& get_error()
	{
		return (e_exception);
	}

	jgl::Float				degree_to_radian(const jgl::Float angle)
	{
		return ((angle * static_cast<jgl::Float>(M_PI)) / 180.0f);
	}

	jgl::Float				radian_to_degree(const jgl::Float radian)
	{
		return ((radian * 180.0f) / static_cast<jgl::Float>(M_PI));
	}

	jgl::Int floor(jgl::Float value)
	{
		return (static_cast<jgl::Int>(std::floor(value)));
	}

	jgl::Int round(jgl::Float value)
	{
		return (static_cast<jgl::Int>(std::round(value)));
	}

	jgl::Int ceil(jgl::Float value)
	{
		return (static_cast<jgl::Int>(std::ceil(value)));
	}

	jgl::String normalize_string(const jgl::String& str, const jgl::Glyph c, const jgl::Size_t size)
	{
		jgl::String result;

		result = str;

		while (result.size() < size)
			result.insert(c, 0);

		return (result);
	}

	jgl::String normalize_int(const jgl::Int num, const jgl::Glyph c, const jgl::Size_t size)
	{
		jgl::String result;

		result = itoa(num);
		while (result.size() < size)
			result.insert(c, 0);

		return (result);
	}

	jgl::String normalize_uint(const jgl::Uint num, const jgl::Glyph c, const jgl::Size_t size)
	{
		jgl::String result;

		result = itoa(num);
		while (result.size() < size)
			result.insert(c, 0);

		return (result);
	}

	jgl::String normalize_float(const jgl::Float num, const jgl::Int after_point, const jgl::Glyph c, const jgl::Size_t size)
	{
		jgl::String result;

		result = ftoa(num, after_point);
		while (result.size() < size)
			result.insert(c, 0);

		return (result);
	}

	void reverse(jgl::String& base)
	{
		jgl::Size_t i = 0, j = base.size() - 1;
		jgl::Glyph temp;
		while (i < j)
		{
			temp = base[i];
			base[i] = base[j];
			base[j] = temp;
			i++; j--;
		}
	}

	jgl::String itoa(const jgl::Int x, const jgl::Int d)
	{
		std::string result = std::to_string(x);
		size_t i = result.size();

		if (d > 0)
		{
			while (i < (uint32_t)d)
			{
				result.insert(0, " ");
				i++;
			}
		}

		return (jgl::String(result));
	}

	// Converts a jgl::Floating pojgl::Int number tostring.
	jgl::String ftoa(const jgl::Float n, const jgl::Int afterpoint, const jgl::Int length)
	{
		std::string text;
		std::stringstream out;
		jgl::Int tmp = afterpoint;

		if (n > 0 || n < -1)
			tmp += 2;
		out << std::setprecision(tmp) << n;
		text = out.str();
		while (length != -1 && text.length() < static_cast<jgl::Size_t>(length))
			text.insert(text.begin(), ' ');
		out.seekp(0);
		out << text;
		return (jgl::String(out.str()));
	}
	// Converts a floating pojgl::Int number tostring.
	/*jgl::String ftoa(const jgl::Double n, const jgl::Int afterpoint, const jgl::Int length)
	{
		return (ftoa((jgl::Float)n, afterpoint, length));
	}*/


	jgl::Float stof(jgl::String text)
	{
		const char* tmp = text.c_str();
		return (static_cast<jgl::Float>(std::atof(tmp)));
	}
	jgl::Int stoi(jgl::String text)
	{
		const char* tmp = text.c_str();
		return (std::atoi(tmp));
	}

	bool is_middle(const jgl::Int min, const jgl::Int value, const jgl::Int max)
	{
		if (value < min)
			return (false);
		if (value >= max)
			return (false);
		return (true);
	}

	bool is_middle(const jgl::Float min, const jgl::Float value, const jgl::Float max)
	{
		if (value < min)
			return (false);
		if (value >= max)
			return (false);
		return (true);
	}

	bool is_middle(const jgl::Double min, const jgl::Double value, const jgl::Double max)
	{
		if (value < min)
			return (false);
		if (value >= max)
			return (false);
		return (true);
	}

	bool is_middle(const jgl::Vector2 min, const jgl::Vector2 value, const jgl::Vector2 max)
	{
		if (is_middle(min.x, value.x, max.x) == true && is_middle(min.y, value.y, max.y) == true)
			return (true);
		return (false);
	}

	bool is_middle(const jgl::Vector3 min, const jgl::Vector3 value, const jgl::Vector3 max)
	{
		if (is_middle(min.x, value.x, max.x) == true && is_middle(min.y, value.y, max.y) == true && is_middle(min.z, value.z, max.z) == true)
			return (true);
		return (false);
	}

	void remove_char(jgl::String& src, jgl::String& to_remove)
	{
		jgl::String result;
		for (jgl::Size_t i = 0; i < src.size(); i++)
		{
			jgl::Glyph target = '\0';
			for (jgl::Size_t j = 0; j < to_remove.size(); j++)
				if (to_remove[j] == src[i])
					target = to_remove[j];

			if (target == '\0')
				result.push_back(src[i]);
		}
		src = result;
	}

	jgl::Long generate_nbr_from_2D(const jgl::Long seed, const jgl::Long x, const jgl::Long y)
	{
		jgl::Size_t h = static_cast<jgl::Uint>(seed + static_cast<jgl::Ulong>(x) * 374761393 + static_cast<jgl::Ulong>(y) * 668265263); //all constants are prime
		h = (h ^ (h >> 13)) * 1274126177;
		return h ^ (h >> 16);
	}


	jgl::Long generate_nbr(const jgl::Long min, const jgl::Long max)
	{
		if (min == max)
			return (min);

		jgl::Long value = 0;

		for (jgl::Size_t i = 0; i < 4; i++)
		{
			value = value << 8;
			jgl::Uint value_tmp;
			rand_s(&value_tmp);
			value += value_tmp;
		}
		return((value % (max - min)) + min);
	}

	jgl::String _compose_file_path(jgl::String p_path, jgl::String p_extension)
	{
		jgl::String result = p_path;

		if (p_path.size() < 5 || p_path.substr(p_path.size() - 4, 4) != p_extension)
			result.append(p_extension);

		return (result);
	}


	void save_to_tga(jgl::String p_path, jgl::Vector2Int p_image_size, char* p_buffer_data)
	{
		jgl::String full_path = _compose_file_path(p_path, ".tga");

		stbi_write_tga(full_path.c_str(), p_image_size.x, p_image_size.y, 4, p_buffer_data);
	}

	void save_to_png(jgl::String p_path, jgl::Vector2Int p_image_size, char* p_buffer_data)
	{
		jgl::String full_path = _compose_file_path(p_path, ".png");

		stbi_write_png(full_path.c_str(), p_image_size.x, p_image_size.y, 4, p_buffer_data, p_image_size.x * 4);
	}

	void save_to_jpg(jgl::String p_path, jgl::Vector2Int p_image_size, char* p_buffer_data)
	{
		jgl::String full_path = _compose_file_path(p_path, ".jpg");

		stbi_write_jpg(full_path.c_str(), p_image_size.x, p_image_size.y, 4, p_buffer_data, 100);
	}

	void save_to_bmp(jgl::String p_path, jgl::Vector2Int p_image_size, char* p_buffer_data)
	{
		jgl::String full_path = _compose_file_path(p_path, ".bmp");

		stbi_write_bmp(full_path.c_str(), p_image_size.x, p_image_size.y, 4, p_buffer_data);
	}

	Stream& endl(jgl::Stream& p_stream)
	{
		p_stream.flush();
		return (p_stream);
	}

	jgl::Ulong get_time()
	{
		auto epoch = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now()).time_since_epoch();

		return (std::chrono::duration_cast<std::chrono::milliseconds>(epoch).count());
	}

	jgl::Array<Vector2Int> 		calc_line_2d(Vector2Int p_start, Vector2Int p_end)
	{
		jgl::Array<Vector2Int> result;
		const bool steep = (fabs(p_end.y - p_start.y) > fabs(p_end.x - p_start.x));
		if (steep)
		{
			std::swap(p_start.x, p_start.y);
			std::swap(p_end.x, p_end.y);
		}
		if (p_start.x > p_end.x)
		{
			std::swap(p_start.x, p_end.x);
			std::swap(p_start.y, p_end.y);
		}

		float dx = p_end.x - p_start.x;
		float dy = fabs(p_end.y - p_start.y);

		float error = dx / 2.0f;
		int ystep = (p_start.y < p_end.y) ? 1 : -1;
		int y = (int)p_start.y;

		int maxX = (int)p_end.x;

		for (int x = (int)p_start.x; x < maxX; x++)
		{
			if (steep)
				result.push_back(Vector2Int(y, x));
			else
				result.push_back(Vector2Int(x, y));

			error -= dy;
			if (error < 0)
			{
				y += ystep;
				error += dx;
			}
		}
		return (result);
	}
}