#pragma once

#include "jgl_includes.h"
#include "jgl_basic_types.h"
#include "application/jgl_gl_object.h"
#include "structure/jgl_exception.h"
#include "jgl_file_handler.h"

namespace jgl
{
	class Shader
	{
	private:
		jgl::Uint _program;
		jgl::Uint _buffer_array;
		jgl::Buffer* _element_buffer;
		jgl::Map<jgl::String, jgl::Uniform*> _uniforms;
		jgl::Map<jgl::String, jgl::Buffer*> _buffers;

	public:
		enum class Mode
		{
			Point = GL_POINTS,
			Line = GL_LINES,
			Triangle = GL_TRIANGLES,
		};
		static Shader* compile(jgl::String vertex_content, jgl::String fragment_content);
		static Shader* compile_from_file(jgl::String vertex_content_path, jgl::String fragment_content_path);

	private:
		Shader();
		void _initialize();
		void _parse_buffer(jgl::String base);
		void _parse_uniform(jgl::String base);
		void _parse_uniform_information(GLint _location, jgl::Array<jgl::String> tab);
		void _compile_shader(jgl::Uint p_id, jgl::String p_source);
		void _compute_program(jgl::Uint p_program_id, jgl::Uint p_vertex_shader_id, jgl::Uint p_fragment_shader_id);
		void _load_shaders(jgl::String vertex_content, jgl::String fragment_content);

	public:
		void extract(jgl::Map<jgl::String, jgl::Uniform*>& uniforms_dest, jgl::Map<jgl::String, jgl::Buffer*>& buffers_dest, jgl::Buffer*& element_buffer_dest)
		{
			for (auto tmp : _uniforms)
			{
				uniforms_dest[tmp.first] = tmp.second->copy();
			}
			for (auto tmp : _buffers)
			{
				buffers_dest[tmp.first] = tmp.second->copy();
			}
			element_buffer_dest = _element_buffer->copy();
		}

		void activate()
		{
			glUseProgram(_program);

			glBindVertexArray(_buffer_array);
		}

		void launch(jgl::Shader::Mode type);
		void launch_instancied(jgl::Shader::Mode type, jgl::Size_t nb_element);

		void cast(jgl::Shader::Mode type, jgl::Size_t nb_elem)
		{
			if (nb_elem != 0)
			{
				glDrawElements(static_cast<GLenum>(type), static_cast<GLsizei>(nb_elem), GL_UNSIGNED_INT, nullptr);
			}
		}

		void cast_instancied(jgl::Shader::Mode type, jgl::Size_t nb_vertex_model, jgl::Size_t nb_element)
		{
			if (nb_vertex_model != 0 && nb_element != 0)
			{
				glDrawElementsInstanced(static_cast<GLenum>(type), static_cast<GLsizei>(nb_vertex_model), GL_UNSIGNED_INT, nullptr, nb_element);
			}
		}

		const jgl::Uint program() const { return  (_program); }
		const jgl::Map<jgl::String, jgl::Uniform*>& uniforms() const { return (_uniforms); }
		const jgl::Map<jgl::String, jgl::Buffer*>& buffers() const { return (_buffers); }

		jgl::Uniform* uniform(jgl::String name)
		{
			if (_uniforms.count(name) == 0)
				THROW_EXCEPTION(jgl::Error_level::Error, 0, "No uniform named " + name + " in shader");
			return (_uniforms[name]);
		}

		jgl::Uniform* copy_uniform(jgl::String name)
		{
			if (_uniforms.count(name) == 0)
				THROW_EXCEPTION(jgl::Error_level::Error, 0, "No uniform named " + name + " in shader to copy");
			return (_uniforms[name]->copy());
		}

		jgl::Buffer* buffer(jgl::String name)
		{
			if (_buffers.count(name) == 0)
				THROW_EXCEPTION(jgl::Error_level::Error, 0, "No data buffer named " + name + " in shader");
			return (_buffers[name]);
		}

		jgl::Buffer* copy_buffer(jgl::String name)
		{
			if (_buffers.count(name) == 0)
				THROW_EXCEPTION(jgl::Error_level::Error, 0, "No buffer named " + name + " in shader to copy");
			return (_buffers[name]->copy());
		}

		jgl::Buffer* indexes_buffer() { return (_element_buffer); }

		void set_attrib_divider(jgl::Int id, jgl::Int divider)
		{
			activate();
			glVertexAttribDivisor(id, divider);
		}
	};
}