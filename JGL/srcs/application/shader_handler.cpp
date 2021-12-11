#include "jgl.h"

extern bool g_simple_mode;

namespace jgl
{
	Shader::Shader()
	{
		_initialize();
	}

	jgl::Array<jgl::Array<jgl::String>> element_type = {
		{"float", "int", "uint", "bool", "double"},
		{"vec2", "ivec2", "uivec2", "bvec2", "dvec2"},
		{"vec3", "ivec3", "uivec3", "bvec3", "dvec3"},
		{"vec4", "ivec4", "uivec4", "bvec4", "dvec4"},
	};
	jgl::Array<jgl::Buffer::Type> element_type_result = {
		jgl::Buffer::Type::Float, jgl::Buffer::Type::Int, jgl::Buffer::Type::Uint, jgl::Buffer::Type::Bool, jgl::Buffer::Type::Double
	};

	void Shader::_parse_buffer(jgl::String base)
	{
		jgl::Array<jgl::String> tab = base.split("\n", false);

		glBindVertexArray(_buffer_array); 
		if (tab.size() >= 2)
		{
			for (jgl::Size_t j = 1; j < tab.size(); j++)
			{
				jgl::Array<jgl::String> tab2 = tab[j].split(";", false);
				for (jgl::Size_t i = 0; i < tab2.size(); i++)
				{
					if (tab2[i].size() >= 6u && (tab2[i].substr(0, 3) == "in " || tab2[i].substr(0, 6) == "layout"))
					{
						jgl::Array<jgl::String> tab = tab2[i].split(" ", false);
						jgl::String name = tab.last();

						GLint location_id = glGetAttribLocation(_program, name.c_str());
						if (location_id == GL_INVALID_OPERATION)
							THROW_EXCEPTION(jgl::Error_level::Critical, location_id, "Error while getting a new attrib location");

						jgl::Size_t nb_component = 0;
						jgl::Size_t tmp_element_type = 0;
						for (jgl::Size_t i = 0; i < element_type.size() && nb_component == 0; i++)
						{
							for (jgl::Size_t j = 0; j < element_type[i].size() && nb_component == 0; j++)
							{
								if (tab[tab.size() - 2] == element_type[i][j])
								{
									nb_component = j;
									tmp_element_type = i + 1;
								}
							}
						}
						THROW_INFORMATION("Creating a new location[" + name + "] at location[" + jgl::itoa(location_id) + "] with nb_component[" + jgl::itoa(tmp_element_type) + " x " + jgl::glGetTypeString(static_cast<GLenum>(element_type_result[nb_component])) + "]");
						_buffers[name] = new Buffer(name, location_id, tmp_element_type, element_type_result[nb_component], jgl::Buffer::Mode::Array);
					}
				}
			}
		}
		_element_buffer = new Buffer("element_buffer", 0, 1, jgl::Buffer::Type::Uint, jgl::Buffer::Mode::Indexes);
	}

	void Shader::_parse_uniform_information(GLint location, jgl::Array<jgl::String> tab)
	{
		jgl::String name = tab[2];
		jgl::Uniform::Mode mode = jgl::Uniform::Mode::Error;
		jgl::Uniform::Type type = jgl::Uniform::Type::Error;
		jgl::Uniform::Size size = jgl::Uniform::Size::Error;

		if (tab.last().back() == ']')
			mode = jgl::Uniform::Mode::Array;
		else
			mode = jgl::Uniform::Mode::Element;

		if (tab[1] == "int")
		{
			type = jgl::Uniform::Type::Int;
			size = jgl::Uniform::Size::One;
		}
		else if (tab[1] == "float")
		{
			type = jgl::Uniform::Type::Float;
			size = jgl::Uniform::Size::One;
		}
		else if (tab[1] == "uint")
		{
			type = jgl::Uniform::Type::Uint;
			size = jgl::Uniform::Size::One;
		}
		else if (tab[1] == "vec2")
		{
			type = jgl::Uniform::Type::Float;
			size = jgl::Uniform::Size::Two;
		}
		else if (tab[1] == "vec3")
		{
			type = jgl::Uniform::Type::Float;
			size = jgl::Uniform::Size::Three;
		}
		else if (tab[1] == "vec4")
		{
			type = jgl::Uniform::Type::Float;
			size = jgl::Uniform::Size::Four;
		}
		else if (tab[1] == "ivec2")
		{
			type = jgl::Uniform::Type::Int;
			size = jgl::Uniform::Size::Two;
		}
		else if (tab[1] == "ivec3")
		{
			type = jgl::Uniform::Type::Int;
			size = jgl::Uniform::Size::Three;
		}
		else if (tab[1] == "ivec4")
		{
			type = jgl::Uniform::Type::Int;
			size = jgl::Uniform::Size::Four;
		}
		else if (tab[1] == "uvec2")
		{
			type = jgl::Uniform::Type::Uint;
			size = jgl::Uniform::Size::Two;
		}
		else if (tab[1] == "uvec3")
		{
			type = jgl::Uniform::Type::Uint;
			size = jgl::Uniform::Size::Three;
		}
		else if (tab[1] == "uvec4")
		{
			type = jgl::Uniform::Type::Uint;
			size = jgl::Uniform::Size::Four;
		}
		else if (tab[1] == "mat2")
		{
			type = jgl::Uniform::Type::Matrix;
			size = jgl::Uniform::Size::Two;
		}
		else if (tab[1] == "mat3")
		{
			type = jgl::Uniform::Type::Matrix;
			size = jgl::Uniform::Size::Three;
		}
		else if (tab[1] == "mat4")
		{
			type = jgl::Uniform::Type::Matrix;
			size = jgl::Uniform::Size::Four;
		}
		else if (tab[1].find("sampler") != jgl::String::npos)
		{
			type = jgl::Uniform::Type::Int;
			size = jgl::Uniform::Size::One;
		}

		_uniforms[name] = new jgl::Uniform(name, location, mode, type, size);
	}

	void Shader::_parse_uniform(jgl::String base)
	{
		jgl::Array<jgl::String> tab = base.split("\n", false);

		glBindVertexArray(_buffer_array);
		if (tab.size() >= 2)
		{
			for (jgl::Size_t j = 1; j < tab.size(); j++)
			{
				jgl::Array<jgl::String> tab2 = tab[j].split(";", false);
				for (jgl::Size_t i = 0; i < tab2.size(); i++)
				{
					if (tab2[i].size() >= 8)
					{
						if (tab2[i].substr(0, 7) == "uniform")
						{
							jgl::Array<jgl::String> tab = tab2[i].split(" ", false);
							GLint id = glGetUniformLocation(_program, tab[2].c_str());
							if (id == GL_INVALID_VALUE || id == GL_INVALID_OPERATION)
								THROW_EXCEPTION(jgl::Error_level::Critical, 0, "Error while getting a new attrib location");

							_parse_uniform_information(id, tab);
						}
					}
				}
			}
		}
	}

	Shader* Shader::compile(jgl::String vertex_content, jgl::String fragment_content)
	{
		Shader* result = new jgl::Shader();

		result->_load_shaders(vertex_content, fragment_content);

		result->_parse_uniform(vertex_content);
		result->_parse_uniform(fragment_content);

		result->_parse_buffer(vertex_content);

		GLint isLinked = 0;
		glGetProgramiv(result->_program, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(result->_program, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(result->_program, maxLength, &maxLength, &infoLog[0]);
			return nullptr;
		}
		return (result);
	}

	Shader* Shader::compile_from_file(jgl::String vertex_content_path, jgl::String fragment_content_path)
	{
		jgl::String vertex_content;
		jgl::String frag_content;

		std::ifstream vShaderFile;
		std::ifstream fShaderFile;
		try
		{
			// open files
			vShaderFile.open(vertex_content_path.c_str());
			fShaderFile.open(fragment_content_path.c_str());
			std::stringstream vShaderStream, fShaderStream;
			// read file's buffer contents into streams
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();
			// close file handlers
			vShaderFile.close();
			fShaderFile.close();
			// convert stream into string
			vertex_content = vShaderStream.str();
			frag_content = fShaderStream.str();
		}
		catch (...)
		{
			THROW_EXCEPTION(jgl::Error_level::Error, 1, "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ");
		}
		return (compile(vertex_content, frag_content));
	}

	void Shader::_initialize()
	{
		_program = glCreateProgram();
		glGenVertexArrays(1, &_buffer_array);
		glBindVertexArray(_buffer_array);
	}

	void Shader::_compile_shader(GLuint p_id, jgl::String p_source)
	{
		GLint	result;
		jgl::Int len;
		const char* content  = p_source.c_str();
		
		result = GL_FALSE;
		glShaderSource(p_id, 1, &content, NULL);
		glCompileShader(p_id);
		glGetShaderiv(p_id, GL_COMPILE_STATUS, &result);
		if (result != GL_TRUE)
		{
			glGetShaderiv(p_id, GL_INFO_LOG_LENGTH, &len);
			std::vector<char> msg(len + 1);
			glGetShaderInfoLog(p_id, len, NULL, &msg[0]);
			THROW_EXCEPTION(jgl::Error_level::Critical, -2, &msg[0]);
		}
	}

	void Shader::_compute_program(GLuint p_program_id, GLuint p_vertex_shader_id, GLuint p_fragment_shader_id)
	{
		GLint	result;

		result = GL_FALSE;
		glAttachShader(p_program_id, p_vertex_shader_id);
		glAttachShader(p_program_id, p_fragment_shader_id);
		glLinkProgram(p_program_id);
		glGetProgramiv(p_program_id, GL_LINK_STATUS, &result);
		if (result != GL_TRUE)
			THROW_EXCEPTION(jgl::Error_level::Critical, -3, "Error while computing shader program");
		glDetachShader(p_program_id, p_vertex_shader_id);
		glDetachShader(p_program_id, p_fragment_shader_id);
		glDeleteShader(p_vertex_shader_id);
		glDeleteShader(p_fragment_shader_id);
	}

	void Shader::_load_shaders(jgl::String vertex_content, jgl::String fragment_content)
	{
		GLuint _vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
		GLuint _fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);

		_compile_shader(_vertex_shader_id, vertex_content);
		THROW_INFORMATION("Vertex shader parsed");
		_compile_shader(_fragment_shader_id, fragment_content);
		THROW_INFORMATION("Frag shader parsed");
		_compute_program(_program, _vertex_shader_id, _fragment_shader_id);
		THROW_INFORMATION("Program compiled");
	}
	void Shader::launch(jgl::Shader::Mode type)
	{
		activate();

		for (auto it = _buffers.begin(); it != _buffers.end(); it++)
		{
			it->second->activate();
		}

		_element_buffer->activate();

		cast(type, _element_buffer->size() / sizeof(jgl::Uint));
	}
}