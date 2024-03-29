#include "jgl.h"

namespace jgl
{
	extern jgl::Vector2Int g_viewport_actual_size;


	Vector3 convert_screen_to_opengl(const Vector2Int source, jgl::Float level)
	{
		if (jgl::Application::active_application() == nullptr)
			THROW_EXCEPTION(jgl::Error_level::Critical, 0, "jgl::Application not created");
		GLint viewport_info[4];
		glGetIntegerv(GL_VIEWPORT, viewport_info);
		jgl::Float w = viewport_info[2] / 2;
		jgl::Float h = viewport_info[3] / 2;
		jgl::Float x = (source.x) / w - 1.0f;
		jgl::Float y = -((source.y) / h - 1.0f);
		return (Vector3(x, y, level / 10000.0f));
	}

	Vector2 convert_screen_to_opengl(const Vector2Int source)
	{
		if (jgl::Application::active_application() == nullptr)
			THROW_EXCEPTION(jgl::Error_level::Critical, 0, "jgl::Application not created");
		GLint viewport_info[4];
		glGetIntegerv(GL_VIEWPORT, viewport_info);
		jgl::Float w = viewport_info[2] / 2;
		jgl::Float h = viewport_info[3] / 2;
		jgl::Float x = (source.x) / w - 1.0f;
		jgl::Float y = -((source.y) / h - 1.0f);
		return (Vector2(x, y));
	}

	Vector2Int convert_opengl_to_screen(const Vector2 source)
	{
		if (jgl::Application::active_application() == nullptr)
			THROW_EXCEPTION(jgl::Error_level::Critical, 0, "jgl::Application not created");
		if (jgl::Application::active_application() == nullptr)
			THROW_EXCEPTION(jgl::Error_level::Critical, 0, "No active viewport");
		jgl::Float x = (source.x + 1.0f) * (jgl::Application::active_application()->active_viewport()->area().x / 2.0f);
		jgl::Float y = (source.y - 1.0f) * (jgl::Application::active_application()->active_viewport()->area().y / -2.0f);
		return (Vector2Int(x, y));
	}

	Vector2Int convert_opengl_to_screen(const Vector3 source)
	{
		return (convert_opengl_to_screen(jgl::Vector2(source.x, source.y)));
	}

	void draw_triangle_color(jgl::Color color, jgl::Vector2Int a, jgl::Vector2Int b, jgl::Vector2Int c, jgl::Float depth)
	{

	}

	static jgl::Uint element_index[6] = { 0, 3, 1, 2, 3, 0 };
	static jgl::Vector2Int delta_pos[4] = {
		jgl::Vector2Int(0, 0),
		jgl::Vector2Int(1, 0),
		jgl::Vector2Int(0, 1),
		jgl::Vector2Int(1, 1)
	};

	void prepare_rectangle_color(
		jgl::Array<jgl::Vector3>& vertex_content, jgl::Array<jgl::Color>& color_content, jgl::Array<jgl::Uint>& indexes_array,
		jgl::Color color, jgl::Vector2Int pos, jgl::Vector2Int size, jgl::Float depth)
	{
		prepare_rectangle_color(&vertex_content, &color_content, &indexes_array, color, pos, size, depth);
	}

	void prepare_rectangle_color(
		jgl::Array<jgl::Vector3>* vertex_content, jgl::Array<jgl::Color>* color_content, jgl::Array<jgl::Uint>* indexes_array,
		jgl::Color color, jgl::Vector2Int pos, jgl::Vector2Int size, jgl::Float depth)
	{
		jgl::Size_t base_index = 0;
		if (vertex_content != nullptr)
			base_index = vertex_content->size();

		for (size_t i = 0; i < 4; i++)
		{
			if (vertex_content != nullptr)
				vertex_content->push_back(convert_screen_to_opengl(pos + size * delta_pos[i], depth));
			if (color_content != nullptr)
				color_content->push_back(color);
		}
		for (jgl::Size_t i = 0; i < 6; i++)
		{
			if (indexes_array != nullptr)
				indexes_array->push_back(base_index + element_index[i]);
		}
	}

	void cast_draw_color(jgl::Array<jgl::Vector3>& p_vertex_array, jgl::Array<jgl::Color>& p_color_array, jgl::Array<jgl::Uint>& p_indexes_array)
	{
		const jgl::String shader_name = "Color_shader_2D";

		static jgl::Shader* tmp_shader = nullptr;
		static jgl::Buffer* model_buffer = nullptr;
		static jgl::Buffer* color_buffer = nullptr;

		if (tmp_shader == nullptr)
			tmp_shader = jgl::Application::active_application()->shader(shader_name);

		if (tmp_shader == nullptr)
			THROW_EXCEPTION(jgl::Error_level::Error, 0, "No shader named " + shader_name);

		if (model_buffer == nullptr)
			model_buffer = tmp_shader->buffer("model_space");
		if (color_buffer == nullptr)
			color_buffer = tmp_shader->buffer("color_space");
		tmp_shader->activate();
		model_buffer->send(p_vertex_array.all(), p_vertex_array.size());
		color_buffer->send(p_color_array.all(), p_color_array.size());
		tmp_shader->indexes_buffer()->send(p_indexes_array.all(), p_indexes_array.size());
		tmp_shader->launch(jgl::Shader::Mode::Triangle);
	}

	void draw_rectangle_color(jgl::Color color, jgl::Vector2Int pos, jgl::Vector2Int size, jgl::Float depth)
	{
		const jgl::String shader_name = "Color_shader_2D";

		Vector3 vertex_content[4];
		Color color_content[4];

		for (size_t i = 0; i < 4; i++)
		{
			vertex_content[i] = convert_screen_to_opengl(pos + size * delta_pos[i], depth);
			color_content[i] = color;
		}

		static jgl::Shader* tmp_shader = nullptr;
		
		if (tmp_shader == nullptr)
			tmp_shader = jgl::Application::active_application()->shader(shader_name);

		if (tmp_shader == nullptr)
			THROW_EXCEPTION(jgl::Error_level::Error, 0, "No shader named " + shader_name);

		static jgl::Buffer* model_buffer = nullptr;
		static jgl::Buffer* color_buffer = nullptr;

		if (model_buffer == nullptr)
			model_buffer = tmp_shader->buffer("model_space");
		if (color_buffer == nullptr)
			color_buffer = tmp_shader->buffer("color_space");

		model_buffer->send(vertex_content, 4);
		color_buffer->send(color_content, 4);
		tmp_shader->indexes_buffer()->send(element_index, 6);
		tmp_shader->launch(jgl::Shader::Mode::Triangle);
	}

	jgl::Vector2Int draw_text(jgl::String text, jgl::Vector2Int pos, jgl::Uint size, jgl::Float level, jgl::Float alpha, jgl::Color text_color, jgl::Color outline_color)
	{
		if (jgl::Application::active_application() == nullptr)
			THROW_EXCEPTION(jgl::Error_level::Critical, 1, "No application started while trying to render text");
		if (jgl::Application::active_application()->default_font() == nullptr)
			THROW_EXCEPTION(jgl::Error_level::Critical, 1, "No default font created");
		return (jgl::Application::active_application()->default_font()->draw_text(text, pos, size, level, alpha, text_color, outline_color));
	}

	jgl::Vector2Int prepare_render_text(jgl::Array<Vector3>& vertex_array, jgl::Array<jgl::Vector2>& uvs_array, jgl::Array<jgl::Color>& color_array, jgl::Array<jgl::Color>& color_outline_array, jgl::Array<jgl::Float>& alpha_array, jgl::Array<jgl::Uint>& index_array,
		jgl::String text, jgl::Vector2Int pos, jgl::Uint size, jgl::Float level, jgl::Float alpha, jgl::Color text_color, jgl::Color outline_color)
	{
		return (jgl::Application::active_application()->default_font()->prepare_render_text(&vertex_array, &uvs_array, &color_array, &color_outline_array, &alpha_array, &index_array,
			text, pos, size, level, alpha, text_color, outline_color));
	}

	jgl::Vector2Int prepare_render_text(jgl::Array<Vector3>* vertex_array, jgl::Array<jgl::Vector2>* uvs_array, jgl::Array<jgl::Color>* color_array, jgl::Array<jgl::Color>* color_outline_array, jgl::Array<jgl::Float>* alpha_array, jgl::Array<jgl::Uint>* index_array,
		jgl::String text, jgl::Vector2Int pos, jgl::Uint size, jgl::Float level, jgl::Float alpha, jgl::Color text_color, jgl::Color outline_color)
	{
		if (jgl::Application::active_application() == nullptr)
			THROW_EXCEPTION(jgl::Error_level::Critical, 1, "No application started while trying to render text");
		if (jgl::Application::active_application()->default_font() == nullptr)
			THROW_EXCEPTION(jgl::Error_level::Critical, 1, "No default font created");
		return (jgl::Application::active_application()->default_font()->prepare_render_text(vertex_array, uvs_array, color_array, color_outline_array, alpha_array, index_array,
			text, pos, size, level, alpha, text_color, outline_color));
	}
}