#include "jgl.h"

namespace jgl
{

		jgl::String Buffer::str(Buffer::Mode p_type)
		{
			switch (p_type)
			{
			case Mode::Error:
				return ("Error");
			case Mode::Indexes:
			{
				return ("Indexes");
			}
			case Mode::Array:
			{
				return ("Array");
			}
			default:
				return ("undefined");
			}
		}
		
		jgl::String Buffer::str(Buffer::Type p_type)
		{
			switch (p_type)
			{
			case Type::Error:
				return ("Error");
			case Type::Float:
			{
				return ("Float");
			}
			case Type::Int:
			{
				return ("Int");
			}
			case Type::Uint:
			{
				return ("Uint");
			}
			case Type::Bool:
			{
				return ("Bool");
			}
			case Type::Double:
			{
				return ("Double");
			}
			default:
				return ("undefined");
			}
		}

		Buffer::Buffer(jgl::String p_name, jgl::Int p_location, jgl::Uint p_element_vertex, Buffer::Type p_data_type, Buffer::Mode p_mode)
		{
			_element_vertex = p_element_vertex;
			_name = p_name;
			_location = p_location;
			_data_type = p_data_type;
			glGenBuffers(1, &_buffer);
			_size = 0;
			_max_size = 0;
			_mode = p_mode;
		}
		Buffer::~Buffer()
		{
			glDeleteBuffers(1, &_buffer);
		}

		Buffer* Buffer::copy() const
		{
			return (new Buffer(_name, _location, _element_vertex, _data_type, _mode));
		}

		void Buffer::activate()
		{
			if (_location == -1)
				return;

			glBindBuffer(static_cast<GLenum>(_mode), _buffer);
			if (_mode == Mode::Array)
			{
				glEnableVertexAttribArray(_location);
				glVertexAttribPointer(_location, _element_vertex, static_cast<GLenum>(_data_type), GL_FALSE, 0, NULL);
			}
		}

		jgl::String Uniform::str(Uniform::Mode value)
		{
			switch (value)
			{
			case Mode::Error:
				return ("Error");
			case Mode::Element:
				return ("Element");
			case Mode::Array:
				return ("Array");
			default:
				return ("undefined");
			}
		}

		jgl::String Uniform::str(Uniform::Type p_type, Uniform::Size p_size)
		{
			switch (p_type)
			{
			case Type::Error:
				return ("Error");
			case Type::Float:
			{
				switch (p_size)
				{
				case Size::Error:
					return ("Error");
				case Size::One:
					return ("Float");
				case Size::Two:
					return ("Vector2");
				case Size::Three:
					return ("Vector3");
				case Size::Four:
					return ("Vector4");
				default:
					return ("undefined");
				}
			}
			case Type::Int:
			{
				switch (p_size)
				{
				case Size::Error:
					return ("Error");
				case Size::One:
					return ("Int");
				case Size::Two:
					return ("Ivector2");
				case Size::Three:
					return ("Ivector3");
				case Size::Four:
					return ("Ivector4");
				default:
					return ("undefined");
				}
			}
			case Type::Uint:
			{
				switch (p_size)
				{
				case Size::Error:
					return ("Error");
				case Size::One:
					return ("Uint");
				case Size::Two:
					return ("Uivector2");
				case Size::Three:
					return ("Uivector3");
				case Size::Four:
					return ("Uivector4");
				default:
					return ("undefined");
				}
			}
			case Type::Matrix:
			{
				switch (p_size)
				{
				case Size::Error:
					return ("Error");
				case Size::Two:
					return ("Matrix2x2");
				case Size::Three:
					return ("Matrix3x3");
				case Size::Four:
					return ("Matrix4x4");
				default:
					return ("undefined");
				}
			}
			case Type::Sampler:
			{
				switch (p_size)
				{
				case Size::Error:
					return ("Error");
				case Size::One:
					return ("Sampler2D");
				default:
					return ("undefined");
				}
			}
			default:
				return ("undefined");
			}
		}

		bool Uniform::_uniform_checker(Uniform::Mode expected_mode, Uniform::Type expected_type, Uniform::Size expected_size)
		{
			if (_mode == Mode::Error || _type == Type::Error || _size == Size::Error)
				THROW_EXCEPTION(jgl::Error_level::Error, 0, "Trying to use uniform [" + _name + "] who isn't parsed correctly");
			if (_mode != expected_mode)
				THROW_EXCEPTION(jgl::Error_level::Error, 0, "Trying to send an " + str(expected_mode) + " into [" + _name + "] who is setup as an " + str(_mode) + " uniform");
			if (_type != expected_type || _size != expected_size)
				THROW_EXCEPTION(jgl::Error_level::Error, 0, "Trying to send a " + str(expected_type, expected_size) + " to uniform [" + _name + "] who is setup as " + str(_type, _size) + " uniform");
			if (_location == -1)
			{
				THROW_EXCEPTION(jgl::Error_level::Warning, 1, "Trying to use uniform [" + _name + "] who isn't parsed correctly (location = -1)");
				return (false);
			}
			return (true);
		}

	Uniform::Uniform(jgl::String p_name, jgl::Int p_location, Uniform::Mode p_mode, Uniform::Type p_type, Uniform::Size p_size) :
			_name(p_name),
			_location(p_location),
			_mode(p_mode),
			_type(p_type),
			_size(p_size)
	{

	}
	Uniform::~Uniform()
	{

	}

	Uniform* Uniform::copy() const
	{
		return (new Uniform(_name, _location, _mode, _type, _size));
	}
}
