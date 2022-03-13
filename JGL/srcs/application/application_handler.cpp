#include "jgl.h"

extern jgl::String color_shader_vert;
extern jgl::String color_shader_frag;
extern jgl::String texture_shader_vert;
extern jgl::String texture_shader_frag;
extern jgl::String texture_bitmap_shader_vert;
extern jgl::String texture_bitmap_shader_frag;
extern jgl::String color_model_shader_vert;
extern jgl::String color_model_shader_frag;
extern jgl::String texture_model_shader_vert;
extern jgl::String texture_model_shader_frag;

jgl::Application* jgl::Application::_active_application = nullptr;

void GLAPIENTRY
MessageCallback(GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar* message,
	const void* userParam)
{
	if (severity == GL_DEBUG_TYPE_ERROR || severity == GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR || severity == GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR ||
		severity == GL_DEBUG_TYPE_ERROR_ARB || severity == GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR_ARB || severity == GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR_ARB)
		fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s", (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""), type, severity, message);
}

namespace jgl
{
	Application::Application(const jgl::String& p_title, const jgl::Vector2Int& p_size, jgl::Color p_background)
	{
		jgl::start();

		_active_application = this;

		_title = p_title;
		_size = p_size;
		_background = p_background;
		_play = true;

		_create_window(p_title, _size.x, _size.y, 4, 2);
		THROW_INFORMATION("Window created");
		add_shader("Color_shader_2D", color_shader_vert, color_shader_frag);
		add_shader("Texture_shader_2D", texture_shader_vert, texture_shader_frag);
		add_shader("Texture_text_shader_2D", texture_bitmap_shader_vert, texture_bitmap_shader_frag);
		//add_shader("Color_shader_3D", color_model_shader_vert, color_model_shader_frag);
		//add_shader("Texture_shader_3D", texture_model_shader_vert, texture_model_shader_frag);

		// During init, enable debug output
		glEnable(GL_DEBUG_OUTPUT);
		glDebugMessageCallback(MessageCallback, 0);

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_GREATER);
		glClearDepth(-1.0f);
		glEnable(GL_BLEND);
		glCullFace(GL_BACK);
		glDisable(GL_CULL_FACE);
		glCullFace(GL_FRONT_AND_BACK);
		//glEnable(GL_CULL_FACE);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glFrontFace(GL_CCW);
		glDisable(GL_CULL_FACE);

		_central_widget = new Central_widget();
		_central_widget->set_geometry(0, _size);
		_central_widget->activate();

		resize(_size.x, _size.y);

		set_active_application(this);

		_active_viewport = _central_widget->viewport();
	}

	Application::~Application()
	{
		jgl::stop();
	}

	void Application::_handle_win_message()
	{
		MSG msg;
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) { // If we have a message to process, process it
			if (msg.message == WM_QUIT) {
				quit(); // Set running to false if we have a message to quit
			}
			else if (msg.message == WM_SYSKEYDOWN && msg.wParam == VK_MENU)
			{
				jgl::Key value = static_cast<jgl::Key>(msg.wParam);

				actualize_key(jgl::Application::active_application()->keyboard(), value, jgl::Input_status::Pressed);
				THROW_INFORMATION("Key system pressed : " + jgl::Keyboard::key_name(value));
			}
			else if (msg.message == WM_SYSKEYUP && msg.wParam == VK_MENU)
			{
				jgl::Key value = static_cast<jgl::Key>(msg.wParam);

				actualize_key(jgl::Application::active_application()->keyboard(), value, jgl::Input_status::Release);
				THROW_INFORMATION("Key system release : " + jgl::Keyboard::key_name(value));
			}
			else {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
	}

	void Application::resize(jgl::Int x, jgl::Int y)
	{
		_size = jgl::Vector2Int(x, y);

		_opengl_context.resize(x, y);
		if (_central_widget != nullptr)
		{
			_central_widget->resize(0, _size);
		}
		if (_central_widget != nullptr && _central_widget->viewport() != nullptr)
			_central_widget->viewport()->use();
	}

	void Application::_clear()
	{
		_opengl_context.clear();
	}

	void Application::_render()
	{
		_opengl_context.render();
	}

	void Application::quit()
	{
		_play = false;
	}

	jgl::Ulong Application::getTime() const
	{
		auto epoch = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now()).time_since_epoch();

		return (std::chrono::duration_cast<std::chrono::milliseconds>(epoch).count());
	}


	void Application::_renderer_run()
	{
		jgl::Ulong old_time = getTime();

		jgl::Ulong delta_time = 0;

		jgl::Ulong frame = 0;

		_opengl_context.setup(_background);

		while (_play == true)
		{
			if ((_time - old_time) > 1000.0f)
			{
				_fps_render = frame;
				frame = 0;
				old_time = _time;
			}
			else
				frame++;

			_handle_win_message();

			_clear();

			_central_widget->viewport()->use();

			_central_widget->_render_children();

			_render();
		}
	}

	void Application::_updater_run()
	{
		jgl::Ulong next_tick = getTime();

		jgl::Ulong old_time = getTime();

		jgl::Ulong delta_time = 0;

		jgl::Ulong frame = 0;

		while (_play == true)
		{
			_update_input();
			_time = getTime();

			treat_win_message();

			if ((_time - old_time) > 1000.0f)
			{
				_fps_update = frame;
				frame = 0;
				old_time = _time;
			}
			else
				frame++;

			if (next_tick <= _time)
			{
				next_tick = next_tick + _tick_delta;
				_central_widget->_fixed_update_children();
			}

			_central_widget->_update_children();
		}
	}

	jgl::Int Application::run()
	{
		if (_multithread == false)
		{
			jgl::Ulong next_tick = getTime();
			_opengl_context.setup(_background);

			jgl::Ulong old_time = getTime();

			jgl::Ulong delta_time = 0;

			jgl::Ulong frame = 0;

			while (_play == true)
			{
				_update_input();

				_time = getTime();

				if ((_time - old_time) > 1000.0f)
				{
					_fps_render = frame;
					_fps_update = frame;
					frame = 0;
					old_time = _time;
				}
				else
					frame++;

				_central_widget->viewport()->use();

				_clear();
				_handle_win_message();
				treat_win_message();

				if (next_tick <= _time)
				{
					next_tick = next_tick + _tick_delta;
					_central_widget->_fixed_update_children();
				}

				_central_widget->_update_children();

				_central_widget->_render_children();

				_render();

			}
		}
		else
		{
			_updater_thread = new jgl::Thread("Updater thread", [&]() {_updater_run(); });

			_renderer_run();
			_updater_thread->join();
		}

		return (1);
	}

	jgl::Shader* Application::add_shader_from_file(jgl::String name, jgl::String vertex_shader_path, jgl::String fragment_shader_path)
	{
		THROW_INFORMATION("Creating shader [" + name + "] from file");
		_shader_map[name] = jgl::Shader::compile_from_file(vertex_shader_path, fragment_shader_path);
		return (_shader_map[name]);
	}

	jgl::Shader* Application::add_shader(jgl::String name, jgl::String vertex_content, jgl::String fragment_content)
	{
		THROW_INFORMATION("Creating shader [" + name + "] from source");
		_shader_map[name] = jgl::Shader::compile(vertex_content, fragment_content);
		return (_shader_map[name]);
	}

	void jgl::Application::_update_input()
	{
		_keyboard._actualize_entry('\0');
		_mouse._update();
		_keyboard._update();
	}
}