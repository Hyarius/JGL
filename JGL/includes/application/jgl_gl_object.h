#pragma once

#include "jgl_includes.h"
#include "jgl_define.h"
#include "structure/jgl_string.h"
#include "structure/jgl_vector.h"
#include "structure/jgl_matrix.h"
#include "jgl_basic_functions.h"

namespace jgl
{
	class Buffer
	{
	public:
		enum class Mode
		{
			Error = -1,
			Indexes = GL_ELEMENT_ARRAY_BUFFER,
			Array = GL_ARRAY_BUFFER
		};
		enum class Type
		{
			Error = -1,
			Float = GL_FLOAT,
			Int = GL_INT,
			Uint = GL_UNSIGNED_INT,
			Bool = GL_BOOL,
			Double = GL_DOUBLE
		};
	private:
		jgl::String _name;
		jgl::Int _location;
		jgl::Uint _element_vertex;
		GLuint _buffer;
		Type _data_type;
		Mode _mode;
		jgl::Size_t _size;
		jgl::Size_t _max_size;

		~Buffer();
		static jgl::String str(Mode p_type);
		static jgl::String str(Type p_type);

	public:
		Buffer(jgl::String p_name = "undefined", jgl::Int p_location = -1, jgl::Uint p_element_vertex = 3, Type p_data_type = Type::Float, Mode p_mode = Mode::Array);
		Buffer* copy() const;

		const jgl::String name() const { return (_name); }
		const jgl::Int location() const { return (_location); }
		const jgl::Uint element_vertex() const { return (_element_vertex); }
		const Type data_type() const { return (_data_type); }
		const Mode mode() const { return (_mode); }

		const jgl::Size_t size() const { return (_size); }
		const GLint buffer() const { return (_buffer); }

		void activate();

		template<typename T>
		void send(T* data_array, jgl::Size_t nb)
		{
			if (_location == -1)
				return;

			_size = sizeof(T) * nb;
			glBindBuffer(static_cast<GLenum>(_mode), _buffer);

			if (_max_size < _size)
			{
				glBufferData(static_cast<GLenum>(_mode), _size, data_array, GL_DYNAMIC_DRAW);
				_max_size = _size;
			}
			else
			{
				glBufferSubData(static_cast<GLenum>(_mode), 0, _size, data_array);
			}
		}

		template<typename T>
		jgl::Array<T> get()
		{
			T* data_array = new T[_size / sizeof(T)];
			jgl::Array<T> result;

			glGetNamedBufferSubData(_buffer, 0, _size, data_array);

			for (size_t i = 0; i < _size / sizeof(T); i++)
				result.push_back(data_array[i]);

			return (result);
		}
	};

	class Uniform
	{
	public:
		enum class Mode
		{
			Error,
			Array,
			Element
		};
		enum class Type
		{
			Error,
			Float,
			Int,
			Uint,
			Matrix,
			Sampler
		};
		enum class Size
		{
			Error,
			One,
			Two,
			Three,
			Four
		};

	private:
		jgl::String _name;
		jgl::Int _location;
		Mode _mode;
		Type _type;
		Size _size;
		~Uniform();
		static jgl::String str(Mode value);

		static jgl::String str(Type p_type, Size p_size);
		bool _uniform_checker(Mode expected_mode, Type expected_type, Size expected_size);

	public:
		Uniform(jgl::String p_name = "undefined", jgl::Int p_location = -1, Mode p_mode = Mode::Element, Type p_type = Type::Float, Size p_size = Size::One);

		Uniform* copy() const;
		const String name() const { return (_name); }
		const Int location() const { return (_location); }
		const Mode mode() const { return (_mode); }
		const Type type() const { return (_type); }
		const Size size() const { return (_size); }

		// Element part
		template <typename T, typename std::enable_if < std::is_same <jgl::Float, T>::value == true > ::type* = nullptr >
		void send(T data)
		{
			if (_uniform_checker(Mode::Element, Type::Float, Size::One) == true)
				glUniform1f(_location, data);
		}
		template <typename T, typename std::enable_if < std::is_same <jgl::Int, T>::value == true > ::type* = nullptr >
		void send(T data)
		{
			if (_uniform_checker(Mode::Element, Type::Int, Size::One) == true)
				glUniform1i(_location, data);
		}
		template <typename T, typename std::enable_if < std::is_same <jgl::Uint, T>::value == true > ::type* = nullptr >
		void send(T data)
		{
			if (_uniform_checker(Mode::Element, Type::Uint, Size::One) == true)
				glUniform1ui(_location, data);
		}
		template <typename T, typename std::enable_if < std::is_same <jgl::Vector2, T>::value == true > ::type* = nullptr >
		void send(T data)
		{
			if (_uniform_checker(Mode::Element, Type::Float, Size::Two) == true)
				glUniform2f(_location, data.x, data.y);
		}
		template <typename T, typename std::enable_if < std::is_same <jgl::Vector2Int, T>::value == true > ::type* = nullptr >
		void send(T data)
		{
			if (_uniform_checker(Mode::Element, Type::Int, Size::Two) == true)
				glUniform2i(_location, data.x, data.y);
		}
		template <typename T, typename std::enable_if < std::is_same <jgl::Vector2Uint, T>::value == true > ::type* = nullptr >
		void send(T data)
		{
			if (_uniform_checker(Mode::Element, Type::Uint, Size::Two) == true)
				glUniform2ui(_location, data.x, data.y);
		}
		template <typename T, typename std::enable_if < std::is_same <jgl::Vector3, T>::value == true > ::type* = nullptr >
		void send(T data)
		{
			if (_uniform_checker(Mode::Element, Type::Float, Size::Three) == true)
				glUniform3f(_location, data.x, data.y, data.z);
		}
		template <typename T, typename std::enable_if < std::is_same <jgl::Vector3Int, T>::value == true > ::type* = nullptr >
		void send(T data)
		{
			if (_uniform_checker(Mode::Element, Type::Int, Size::Three) == true)
				glUniform3i(_location, data.x, data.y, data.z);
		}
		template <typename T, typename std::enable_if < std::is_same <jgl::Vector3Uint, T>::value == true > ::type* = nullptr >
		void send(T data)
		{
			if (_uniform_checker(Mode::Element, Type::Uint, Size::Three) == true)
				glUniform3ui(_location, data.x, data.y, data.z);
		}
		template <typename T, typename std::enable_if < std::is_same <jgl::Vector4, T>::value == true > ::type* = nullptr >
		void send(T data)
		{
			if (_uniform_checker(Mode::Element, Type::Float, Size::Four) == true)
				glUniform4f(_location, data.x, data.y, data.z, data.w);
		}
		template <typename T, typename std::enable_if < std::is_same <jgl::Vector4Int, T>::value == true > ::type* = nullptr >
		void send(T data)
		{
			if (_uniform_checker(Mode::Element, Type::Int, Size::Four) == true)
				glUniform4i(_location, data.x, data.y, data.z, data.w);
		}
		template <typename T, typename std::enable_if < std::is_same <jgl::Vector4Uint, T>::value == true > ::type* = nullptr >
		void send(T data)
		{
			if (_uniform_checker(Mode::Element, Type::Uint, Size::Four) == true)
				glUniform4ui(_location, data.x, data.y, data.z, data.w);
		}
		template <typename T, typename std::enable_if < std::is_same <jgl::Matrix2x2, T>::value == true > ::type* = nullptr >
		void send(T data)
		{
			if (_uniform_checker(Mode::Element, Type::Matrix, Size::Two) == true)
				glUniformMatrix2fv(_location, 1, GL_FALSE, &(data));
		}
		template <typename T, typename std::enable_if < std::is_same <jgl::Matrix3x3, T>::value == true > ::type* = nullptr >
		void send(T data)
		{
			if (_uniform_checker(Mode::Element, Type::Matrix, Size::Three) == true)
				glUniformMatrix3fv(_location, 1, GL_FALSE, &(data));
		}
		template <typename T, typename std::enable_if < std::is_same <jgl::Matrix4x4, T>::value == true > ::type* = nullptr >
		void send(T data)
		{
			if (_uniform_checker(Mode::Element, Type::Matrix, Size::Four) == true)
				glUniformMatrix4fv(_location, 1, GL_FALSE, &(data));
		}

		// Array part
		template <typename T, typename std::enable_if < std::is_same <jgl::Float, T>::value == true > ::type* = nullptr >
		void send(T* data, jgl::Size_t nb_element)
		{
			if (_uniform_checker(Mode::Array, Type::Float, Size::One) == true)
				glUniform1fv(_location, nb_element, data);
		}
		template <typename T, typename std::enable_if < std::is_same <jgl::Int, T>::value == true > ::type* = nullptr >
		void send(T* data, jgl::Size_t nb_element)
		{
			if (_uniform_checker(Mode::Array, Type::Int, Size::One) == true)
				glUniform1iv(_location, nb_element, data);
		}
		template <typename T, typename std::enable_if < std::is_same <jgl::Uint, T>::value == true > ::type* = nullptr >
		void send(T* data, jgl::Size_t nb_element)
		{
			if (_uniform_checker(Mode::Array, Type::Uint, Size::One) == true)
				glUniform1uiv(_location, nb_element, data);
		}
		template <typename T, typename std::enable_if < std::is_same <jgl::Vector2, T>::value == true > ::type* = nullptr >
		void send(T* data, jgl::Size_t nb_element)
		{
			if (_uniform_checker(Mode::Array, Type::Float, Size::Two) == true)
				glUniform2fv(_location, nb_element, data);
		}
		template <typename T, typename std::enable_if < std::is_same <jgl::Vector2Int, T>::value == true > ::type* = nullptr >
		void send(T* data, jgl::Size_t nb_element)
		{
			if (_uniform_checker(Mode::Array, Type::Int, Size::Two) == true)
				glUniform2iv(_location, nb_element, data);
		}
		template <typename T, typename std::enable_if < std::is_same <jgl::Vector2Uint, T>::value == true > ::type* = nullptr >
		void send(T* data, jgl::Size_t nb_element)
		{
			if (_uniform_checker(Mode::Array, Type::Uint, Size::Two) == true)
				glUniform2uiv(_location, nb_element, data);
		}
		template <typename T, typename std::enable_if < std::is_same <jgl::Vector3, T>::value == true > ::type* = nullptr >
		void send(T* data, jgl::Size_t nb_element)
		{
			if (_uniform_checker(Mode::Array, Type::Float, Size::Three) == true)
				glUniform3fv(_location, nb_element, data);
		}
		template <typename T, typename std::enable_if < std::is_same <jgl::Vector3Int, T>::value == true > ::type* = nullptr >
		void send(T* data, jgl::Size_t nb_element)
		{
			if (_uniform_checker(Mode::Array, Type::Int, Size::Three) == true)
				glUniform3iv(_location, nb_element, data);
		}
		template <typename T, typename std::enable_if < std::is_same <jgl::Vector3Uint, T>::value == true > ::type* = nullptr >
		void send(T* data, jgl::Size_t nb_element)
		{
			if (_uniform_checker(Mode::Array, Type::Uint, Size::Three) == true)
				glUniform3uiv(_location, nb_element, data);
		}
		template <typename T, typename std::enable_if < std::is_same <jgl::Vector4, T>::value == true > ::type* = nullptr >
		void send(T* data, jgl::Size_t nb_element)
		{
			if (_uniform_checker(Mode::Array, Type::Float, Size::Four) == true)
				glUniform4fv(_location, nb_element, data);
		}
		template <typename T, typename std::enable_if < std::is_same <jgl::Vector4Int, T>::value == true > ::type* = nullptr >
		void send(T* data, jgl::Size_t nb_element)
		{
			if (_uniform_checker(Mode::Array, Type::Int, Size::Four) == true)
				glUniform4iv(_location, nb_element, data);
		}
		template <typename T, typename std::enable_if < std::is_same <jgl::Vector4Uint, T>::value == true > ::type* = nullptr >
		void send(T* data, jgl::Size_t nb_element)
		{
			if (_uniform_checker(Mode::Array, Type::Uint, Size::Four) == true)
				glUniform4uiv(_location, nb_element, data);
		}
		template <typename T, typename std::enable_if < std::is_same <jgl::Matrix2x2, T>::value == true > ::type* = nullptr >
		void send(T* data, jgl::Size_t nb_element)
		{
			if (_uniform_checker(Mode::Array, Type::Matrix, Size::Two) == true)
				glUniformMatrix2fv(_location, nb_element, GL_FALSE, &(data));
		}
		template <typename T, typename std::enable_if < std::is_same <jgl::Matrix3x3, T>::value == true > ::type* = nullptr >
		void send(T* data, jgl::Size_t nb_element)	
		{
			if (_uniform_checker(Mode::Array, Type::Matrix, Size::Three) == true)
				glUniformMatrix3fv(_location, nb_element, GL_FALSE, &(data));
		}
		template <typename T, typename std::enable_if < std::is_same <jgl::Matrix4x4, T>::value == true > ::type* = nullptr >
		void send(T* data, jgl::Size_t nb_element)
		{
			if (_uniform_checker(Mode::Array, Type::Matrix, Size::Four) == true)
				glUniformMatrix4fv(_location, nb_element, GL_FALSE, &(data));
		}
	};
}