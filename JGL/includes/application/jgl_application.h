#pragma once

#include "jgl_includes.h"
#include "jgl_basic_types.h"

#include "structure/jgl_string.h"
#include "structure/jgl_vector.h"
#include "structure/jgl_font.h"

#include "structure/jgl_pool.h"

#include "application/jgl_shader.h"

#include "widget/jgl_widget.h"

#include "structure/jgl_mouse.h"
#include "structure/jgl_keyboard.h"

#include "structure/jgl_thread.h"

#include "structure/jgl_viewport.h"

namespace jgl
{
	class Application
	{
	public :
		static void set_active_application(jgl::Application* p_app)
		{
			_active_application = p_app;
			if (p_app != nullptr)
			{
				THROW_INFORMATION("Main application swap to " + _active_application->title());
			}
			else
				THROW_INFORMATION("Main application set to NULL");
		}

		static Application* active_application() { if (_active_application == nullptr)THROW_EXCEPTION(jgl::Error_level::Critical, 1, "No application started"); return (_active_application); }

		jgl::Widget* central_widget() { return (_central_widget); }

	private:

		class OpenGL_context
		{
		private:
			jgl::Vector2Int _size;

		protected:
			HGLRC _hrc;
			HDC _hdc;
			HWND _hwnd;
			PIXELFORMATDESCRIPTOR _pfd;

		public:
			OpenGL_context()
			{
				_hrc = nullptr;
				_hdc = nullptr;
				_hwnd = nullptr;
				memset(&_pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
			}
			~OpenGL_context(void)
			{
				wglMakeCurrent(_hdc, NULL);
				wglDeleteContext(_hrc);
				ReleaseDC(_hwnd, _hdc);
			}
			jgl::Vector2Int size() { return (_size); }
			void create_opengl_context(HWND hwnd, jgl::Int major_version = 3, jgl::Int minor_version = 2)
			{
				_hwnd = hwnd;
				_hdc = GetDC(_hwnd);
				THROW_INFORMATION("Getting DC completed");

				// Pixelformat descriptor cleaning
				memset(&_pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
				THROW_INFORMATION("Clearing pixelformatdescriptor");

				// Pixelformat descriptor definition
				_pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
				_pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
				_pfd.iPixelType = PFD_TYPE_RGBA;
				_pfd.cColorBits = 24;
				_pfd.cDepthBits = 32;
				_pfd.iLayerType = PFD_MAIN_PLANE;
				THROW_INFORMATION("Pixelformatdescriptor defined");

				// Pixelformat selection
				jgl::Int nPixelFormat = ChoosePixelFormat(_hdc, &_pfd);
				if (nPixelFormat == 0)
					THROW_EXCEPTION(jgl::Error_level::Critical, -7, "Can't choose a pixel format while creating a new opengl context");
				THROW_INFORMATION("ChoosePixelFormat defined");

				// Pixelformat definition
				bool bResult = SetPixelFormat(_hdc, nPixelFormat, &_pfd);
				if (!bResult)
					THROW_EXCEPTION(jgl::Error_level::Critical, -7, "Can't set a pixel format while creating a new opengl context");
				THROW_INFORMATION("SetPixelFormat defined");

				// Creating and using a context OpenGL2.1
				HGLRC tempOpenGLContext = wglCreateContext(_hdc);
				wglMakeCurrent(_hdc, tempOpenGLContext);
				THROW_INFORMATION("wglCreateContext completed");

				//Glew initiialization to start update context to opengl3+
				GLenum error = glewInit();
				if (error != GLEW_OK)
					THROW_EXCEPTION(jgl::Error_level::Critical, -7, "Can't initialize glew while creating a new opengl context");
				THROW_INFORMATION("glewInit completed");

				// OpenGL3+ attribut definition
				jgl::Int attributes[] = {
				WGL_CONTEXT_MAJOR_VERSION_ARB, major_version,
				WGL_CONTEXT_MINOR_VERSION_ARB, minor_version,
				WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB, // Configuration for opengl3+
				0
				};

				// If we can create such opengl context
				if (wglewIsSupported("WGL_ARB_create_context") == 1) {
					// Creating the new context
					_hrc = wglCreateContextAttribsARB(_hdc, NULL, attributes);

					// Unconnecting temporaty context created line 59
					wglMakeCurrent(NULL, NULL);

					// Deleting temporaty context created line 58
					wglDeleteContext(tempOpenGLContext);

					// Activating new context
					wglMakeCurrent(_hdc, _hrc); // On passe au contexte OpenGL 3.0
				}
				else {
					THROW_EXCEPTION(jgl::Error_level::Critical, -7, "OpenGL asked version not supported");
				}

				jgl::Int glVersion[2] = { -1, -1 }; // Des valeurs par défaut pour la version
				glGetIntegerv(GL_MAJOR_VERSION, &glVersion[0]); // On récupère la version majeure d'OpenGL utilisée
				glGetIntegerv(GL_MINOR_VERSION, &glVersion[1]); // On récupère la version mineure d'OpenGL utilisée

				if (glVersion[0] != major_version || glVersion[1] != minor_version)
				{
					THROW_EXCEPTION(jgl::Error_level::Critical, -7, "OpenGL asked version not supported");
				}
				else
				{
					THROW_INFORMATION("Using OpenGL: " + jgl::itoa(glVersion[0], 0) + "." + jgl::itoa(glVersion[1], 0));
				}
			}
			void setup(jgl::Color background)
			{
				glClearColor(background.r, background.g, background.b, background.a);
			}
			void resize(jgl::Int w, jgl::Int h)
			{
				_size.x = w;
				_size.y = h;
				//THROW_INFORMATION("New resolution : " + _size.text());
			}
			void clear()
			{
				glViewport(0, 0, _size.x, _size.y);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
			}
			void render(void)
			{
				SwapBuffers(_hdc);
			}
		};

		class Central_widget : public jgl::Widget
		{
		private:
			void _render()
			{

			}

			void _on_geometry_change()
			{

			}

		public:
			Central_widget() : jgl::Widget(nullptr)
			{

			}
		};

		HWND _window_frame;
		HINSTANCE _hInstance;
		OpenGL_context _opengl_context;

		static Application* _active_application;

		jgl::Font* _default_font = nullptr;
		jgl::Map<jgl::String, jgl::Shader*> _shader_map;

		jgl::String _title = "Title";
		jgl::Bool _multithread = false;
		jgl::Vector2Int _size = jgl::Vector2Int(640, 480);
		jgl::Color _background = jgl::Color(50, 50, 50);
		jgl::Bool _play = true;
		jgl::Bool _masked = false;

		jgl::Ulong _time = 0u;
		jgl::Ulong _fps_render = 0u;
		jgl::Ulong _fps_update = 0u;
		jgl::Ulong _tick_delta = 1000 / 60;

		jgl::Mouse _mouse;
		jgl::Keyboard _keyboard;

		Central_widget* _central_widget;

		const jgl::Viewport* _active_viewport;

		public:
		struct Win_message
		{
			UINT id;
			jgl::Size_t size;
			jgl::Size_t readed;
			std::vector<jgl::Uchar> content;

			Win_message()
			{
				clear();
			}

			void clear()
			{
				id = 0;
				size = 0;
				readed = 0;
				content.clear();
			}

			template<typename DataType>
			Win_message& operator << (const DataType& data)
			{
				static_assert(std::is_standard_layout<DataType>::value, "Data is too complex to be pushed into vector");

				jgl::Size_t old_size = (*this).content.size();

				(*this).content.resize((*this).content.size() + sizeof(DataType));

				std::memcpy((*this).content.data() + old_size, &data, sizeof(DataType));

				(*this).size = (*this).content.size();

				return (*this);
			}

			/*
				Take out a set of bytes, and store it in the data variable
			*/
			template<typename DataType>
			Win_message& operator >> (DataType& data)
			{
				static_assert(std::is_standard_layout<DataType>::value, "Data is too complex to be pulled from vector");

				jgl::Size_t next_size = (*this).readed;// (*this).content.size() - sizeof(DataType);

				std::memcpy(&data, (*this).content.data() + next_size, sizeof(DataType));

				(*this).readed += sizeof(DataType);

				return (*this);
			}
		};
	private:
		static std::mutex _win_message_pool_mutex;
		jgl::Pool<Win_message> _win_message_pool;
		static std::mutex _win_message_deque_mutex;
		std::deque<Win_message *> _win_message_deque;
		jgl::Thread* _updater_thread;

	private:
		void _create_window(jgl::String title, jgl::Int width, jgl::Int height, jgl::Uint major, jgl::Uint minor);

		void _clear();
		void _render();

		void _update_input();
		void _handle_win_message();

		void _renderer_run();
		void _updater_run();

	public:
		Application(const jgl::String& p_title, const jgl::Vector2Int& p_size, jgl::Color p_background);
		~Application();

		void push_win_message(Win_message* p_msg);
		Win_message* take_win_message();
		void treat_win_message();

		const jgl::Viewport* viewport() const { return (_central_widget->viewport()); }
		const jgl::Viewport* active_viewport() const { return (_active_viewport); }
		void set_active_viewport(const jgl::Viewport* p_viewport) { _active_viewport = p_viewport; }

		jgl::Font * default_font() { return (_default_font); }
		void set_default_font(jgl::String path) { _default_font = new jgl::Font(path); }

		jgl::Bool multithread() { return (_multithread); }
		void active_multithread() { _multithread = true; }

		void resize(jgl::Int x, jgl::Int y);
		void set_masked(jgl::Bool p_masked) { _masked = p_masked; }
		void set_max_fps(jgl::Ulong nb_fps) { _tick_delta = 1000 / nb_fps; }

		const jgl::Ulong& fps_render() const { return (_fps_render); }
		const jgl::Ulong& fps_update() const { return (_fps_update); }
		const jgl::Ulong& max_fps() const { return (1000 * _tick_delta); }
		const jgl::Vector2Int size() const { return (_size); }
		const jgl::String& title() const { return (_title); }
		const jgl::Ulong& time() const { return (_time); }
		const jgl::Bool masked() const { return (_masked); }

		jgl::Shader* shader(jgl::String p_name) const { if (_shader_map.count(p_name) == 0)return (nullptr); return (_shader_map.at(p_name)); }
		jgl::Shader* add_shader_from_file(jgl::String name, jgl::String vertex_shader_path, jgl::String fragment_shader_path);
		jgl::Shader* add_shader(jgl::String name, jgl::String vertex_content, jgl::String fragment_content);

		Mouse& mouse() { return (_mouse); }
		Keyboard& keyboard() { return (_keyboard); }

		jgl::Int run();
		void quit();
	};
}