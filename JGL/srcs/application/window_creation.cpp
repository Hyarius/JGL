#include "jgl.h"

namespace jgl
{
	void actualize_button(jgl::Mouse& mouse, jgl::Mouse_button p_button, jgl::Input_status p_status)
	{
		mouse._button[static_cast<jgl::Int>(p_button)] = p_status;
	}
	void actualize_wheel(jgl::Mouse& mouse, jgl::Mouse_wheel_axis p_axis, jgl::Float delta)
	{
		mouse._wheel_timer[static_cast<jgl::Int>(p_axis)] = jgl::Application::active_application()->time();
		mouse._wheel[static_cast<jgl::Int>(p_axis)] += delta;
	}
	void actualize_pos(jgl::Mouse& mouse, jgl::Vector2Int delta)
	{
		mouse._pos = delta;
	}

	void actualize_key(jgl::Keyboard& keyboard, jgl::Key p_key, jgl::Input_status p_status)
	{
		jgl::Input_status& tmp = keyboard._data[static_cast<jgl::Int>(p_key)];
		if ((p_status == jgl::Input_status::Pressed && tmp != jgl::Input_status::Down) ||
			(p_status == jgl::Input_status::Release && tmp != jgl::Input_status::Up))
			tmp = p_status;
	}
	void actualize_entry(jgl::Keyboard& keyboard, jgl::Glyph entry)
	{
		keyboard._actualize_entry(entry);
	}
}

std::mutex jgl::Application::_win_message_pool_mutex;
std::mutex jgl::Application::_win_message_deque_mutex;

namespace jgl
{
	void Application::push_win_message(Application::Win_message* p_msg)
	{
		_win_message_deque_mutex.lock();
		_win_message_deque.push_back(p_msg);
		_win_message_deque_mutex.unlock();
	}

	Application::Win_message* Application::take_win_message()
	{
		_win_message_pool_mutex.lock();
		Win_message* tmp_message = _win_message_pool.obtain();
		_win_message_pool_mutex.unlock();

		tmp_message->clear();

		return (tmp_message);
	}

	void Application::treat_win_message()
	{
		if (_win_message_deque.empty() == false)
		{
			_win_message_deque_mutex.lock();
			Win_message* tmp_message = _win_message_deque.front();
			_win_message_deque.pop_front();
			_win_message_deque_mutex.unlock();

			switch (tmp_message->id) {
					// ----- Application part
				case WM_SHOWWINDOW:
				{
					if (jgl::Application::active_application()->masked() == true)
					{
						jgl::Application::active_application()->set_masked(false);
						THROW_INFORMATION("Showing application " + jgl::Application::active_application()->title());
					}
					else
					{
						jgl::Application::active_application()->set_masked(false);
						THROW_INFORMATION("Masking application " + jgl::Application::active_application()->title());
					}
					break;
				}
				case WM_CLOSE:
				{
					jgl::Application::active_application()->quit();
					THROW_INFORMATION("Close application " + jgl::Application::active_application()->title());
					break;
				}
				case WM_MOVE:
				{
					break;
				}
				case WM_MDIRESTORE:
				{
					THROW_INFORMATION("Restoring the application " + jgl::Application::active_application()->title());
					break;
				}
				case WM_SETFOCUS:
				{
					THROW_INFORMATION("Set focus on application " + jgl::Application::active_application()->title());
					break;
				}
				case WM_KILLFOCUS:
				{
					THROW_INFORMATION("Lost focus on application " + jgl::Application::active_application()->title());
					break;
				}
				case WM_DESTROY: // En cas de fermeture
				{
					jgl::Application::active_application()->quit();
					THROW_INFORMATION("Destroy application " + jgl::Application::active_application()->title());
					break;
				}
				case WM_QUIT: // En cas de fermeture
				{
					jgl::Application::active_application()->quit();
					THROW_INFORMATION("Quit application " + jgl::Application::active_application()->title());
					break;
				}


				// ----- Mouse part
				case WM_LBUTTONDOWN:
				{
					actualize_button(jgl::Application::active_application()->mouse(), jgl::Mouse_button::Left, jgl::Input_status::Pressed);
					THROW_INFORMATION("Left button down operation");
					break;
				}
				case WM_MBUTTONDOWN:
				{
					actualize_button(jgl::Application::active_application()->mouse(), jgl::Mouse_button::Center, jgl::Input_status::Pressed);
					THROW_INFORMATION("Center button down operation");
					break;
				}
				case WM_RBUTTONDOWN:
				{
					actualize_button(jgl::Application::active_application()->mouse(), jgl::Mouse_button::Right, jgl::Input_status::Pressed);
					THROW_INFORMATION("right button down operation");
					break;
				}
				case WM_XBUTTONDOWN:
				{
					actualize_button(jgl::Application::active_application()->mouse(), jgl::Mouse_button::Xbutton, jgl::Input_status::Pressed);
					THROW_INFORMATION("X button down operation");
					break;
				}
				case WM_LBUTTONUP:
				{
					actualize_button(jgl::Application::active_application()->mouse(), jgl::Mouse_button::Left, jgl::Input_status::Release);
					THROW_INFORMATION("Left button up operation");
					break;
				}
				case WM_MBUTTONUP:
				{
					actualize_button(jgl::Application::active_application()->mouse(), jgl::Mouse_button::Center, jgl::Input_status::Release);
					THROW_INFORMATION("Center button up operation");
					break;
				}
				case WM_RBUTTONUP:
				{
					actualize_button(jgl::Application::active_application()->mouse(), jgl::Mouse_button::Right, jgl::Input_status::Release);
					THROW_INFORMATION("Right button up operation");
					break;
				}
				case WM_XBUTTONUP:
				{
					actualize_button(jgl::Application::active_application()->mouse(), jgl::Mouse_button::Xbutton, jgl::Input_status::Release);
					THROW_INFORMATION("X button up operation");
					break;
				}
				case WM_MOUSEHWHEEL:
				{
					jgl::Short value;

					(*tmp_message) >> value;
					jgl::Float delta = value / 100.0f;
					actualize_wheel(jgl::Application::active_application()->mouse(), jgl::Mouse_wheel_axis::Horizontal, delta);
					THROW_INFORMATION("Mouse wheel horizontaly scrolled " + jgl::ftoa(jgl::Application::active_application()->mouse().wheel(jgl::Mouse_wheel_axis::Horizontal)));
					break;
				}
				case WM_MOUSEWHEEL:
				{
					jgl::Short value;

					(*tmp_message) >> value;
					jgl::Float delta = value / 100.0f;
					actualize_wheel(jgl::Application::active_application()->mouse(), jgl::Mouse_wheel_axis::Vertical, delta);
					THROW_INFORMATION("Mouse wheel scrolled to " + jgl::ftoa(jgl::Application::active_application()->mouse().wheel(jgl::Mouse_wheel_axis::Vertical)));
					break;
				}
				case WM_MOUSEMOVE:
				{
					jgl::Int x;
					jgl::Int y;

					(*tmp_message) >> x;
					(*tmp_message) >> y;

					jgl::Application::active_application()->mouse().place(jgl::Vector2Int(x, y));
					break;
				}

				// ----- Keyboard part
				case WM_CHAR:
				{
					jgl::Size_t value;

					(*tmp_message) >> value;

					actualize_entry(jgl::Application::active_application()->keyboard(), static_cast<jgl::Glyph>(value));
					break;
				}
				case WM_KEYDOWN:
				{
					jgl::Key value;

					(*tmp_message) >> value;
					actualize_key(jgl::Application::active_application()->keyboard(), value, jgl::Input_status::Pressed);
					THROW_INFORMATION("Key pressed : " + jgl::Keyboard::key_name(value));
					break;
				}
				case WM_KEYUP:
				{
					jgl::Key value;

					(*tmp_message) >> value;
					actualize_key(jgl::Application::active_application()->keyboard(), value, jgl::Input_status::Release);
					THROW_INFORMATION("Key release : " + jgl::Keyboard::key_name(value));
					break;
				}
				case WM_HOTKEY:
				{
					jgl::Key value;

					(*tmp_message) >> value;
					actualize_key(jgl::Application::active_application()->keyboard(), value, jgl::Input_status::Pressed);
					THROW_INFORMATION("Hotkey pressed : " + jgl::Keyboard::key_name(value));
					break;
				}
				case WM_SYSKEYDOWN:
				{
					jgl::Key value;

					(*tmp_message) >> value;
					actualize_key(jgl::Application::active_application()->keyboard(), value, jgl::Input_status::Release);
					THROW_INFORMATION("Key system pressed : " + jgl::Keyboard::key_name(value));
					break;
				}
				case WM_SYSKEYUP:
				{
					jgl::Key value;

					(*tmp_message) >> value;
					THROW_INFORMATION("Key system release : " + jgl::Keyboard::key_name(value));
					break;
				}
			}

			_win_message_pool_mutex.lock();
			_win_message_pool.release(tmp_message);
			_win_message_pool_mutex.unlock();
		}
	}
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {

	jgl::Application::Win_message* tmp_message = jgl::Application::active_application()->take_win_message();
	tmp_message->id = message;

	switch (message) {
		// ----- Application part
		case WM_SIZE: // En cas de redimensionnement
		{
			if (wParam == SIZE_RESTORED)
				jgl::Application::active_application()->set_masked(false);

			jgl::Uint width = LOWORD(lParam);
			jgl::Uint height = HIWORD(lParam);

			if (width != 0 && height != 0)
				jgl::Application::active_application()->resize(width, height);

			break;
		}

		case WM_MOUSEHWHEEL:
		case WM_MOUSEWHEEL:
		{
			jgl::Short value = GET_WHEEL_DELTA_WPARAM(wParam);

			(*tmp_message) << value;

			break;
		}
		case WM_MOUSEMOVE:
		{
			jgl::Int x = LOWORD(lParam);
			jgl::Int y = HIWORD(lParam);

			(*tmp_message) << x;
			(*tmp_message) << y;

			break;
		}

		// ----- Keyboard part
		case WM_CHAR:
		case WM_KEYDOWN:
		case WM_KEYUP:
		case WM_HOTKEY:
		case WM_SYSKEYDOWN:
		case WM_SYSKEYUP:
		{
			jgl::Key value = static_cast<jgl::Key>(wParam);

			(*tmp_message) << value;
			break;
		}
	}
	jgl::Application::active_application()->push_win_message(tmp_message);
	return DefWindowProc(hWnd, message, wParam, lParam);
}


namespace jgl
{
	void Application::_create_window(jgl::String title, jgl::Int width, jgl::Int height, jgl::Uint major, jgl::Uint minor)
	{
		RECT WindowRect = RECT();
		DWORD dwStyle;
		WNDCLASS windowClass = WNDCLASS();
		DWORD dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;

		THROW_INFORMATION("Creating window [" + title + "]...");

		size_t origsize = strlen(title.c_str()) + 1;
		const size_t newsize = 100;
		size_t convertedChars = 0;
		wchar_t wcstring[newsize];
		mbstowcs_s(&convertedChars, wcstring, origsize, title.c_str(), _TRUNCATE);

		LPCTSTR ptr = wcstring;

		_hInstance = GetModuleHandle(NULL);
		THROW_INFORMATION("Instance created");

		windowClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
		windowClass.lpfnWndProc = (WNDPROC)WndProc;
		windowClass.cbClsExtra = 0;
		windowClass.cbWndExtra = 0;
		windowClass.hInstance = _hInstance;
		windowClass.hIcon = LoadIcon(NULL, IDI_WINLOGO);
		windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
		windowClass.hbrBackground = NULL;
		windowClass.lpszMenuName = NULL;
		windowClass.lpszClassName = ptr;
		THROW_INFORMATION("Window class created");

		if (RegisterClass(&windowClass) == false)
			THROW_EXCEPTION(jgl::Error_level::Critical, -7, "Error while registering a new windowClass");
		THROW_INFORMATION("RegisterClass completed");

		WindowRect.left = (long)0;
		WindowRect.right = (long)width;
		WindowRect.top = (long)0;
		WindowRect.bottom = (long)height;

		dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
		dwStyle = WS_OVERLAPPEDWINDOW;
		AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);

		_window_frame = CreateWindowEx(dwExStyle, ptr, ptr, WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, 0, WindowRect.right - WindowRect.left, WindowRect.bottom - WindowRect.top, NULL, NULL, _hInstance, NULL);
		THROW_INFORMATION("Window frame created");

		_opengl_context.create_opengl_context(_window_frame, major, minor);
		THROW_INFORMATION("Opengl context created");

		ShowWindow(_window_frame, SW_SHOW);
		UpdateWindow(_window_frame);
		wglSwapIntervalEXT(0);
	}
}