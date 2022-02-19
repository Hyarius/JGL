#include "jgl_tester.h"

class Main_application : public jgl::Widget
{
private:
	jgl::Ulong _render_fps;
	jgl::Text_label* _render_fps_label;

	jgl::Ulong _update_fps;
	jgl::Text_label* _update_fps_label;

	void _on_geometry_change()
	{
		_render_fps_label->set_geometry(jgl::Vector2Int(50, 50), jgl::Vector2Int(250, 50));
		_update_fps_label->set_geometry(jgl::Vector2Int(50, 120), jgl::Vector2Int(255, 50));
	}

	void _render()
	{

	}

	jgl::Bool _update()
	{
		if (_render_fps != jgl::Application::active_application()->fps_render())
		{
			_render_fps = jgl::Application::active_application()->fps_render();
			_render_fps_label->label().set_text("Render FPS : " + jgl::itoa(_render_fps));
		}

		if (_update_fps != jgl::Application::active_application()->fps_update())
		{
			_update_fps = jgl::Application::active_application()->fps_update();
			_update_fps_label->label().set_text("Update FPS : " + jgl::itoa(_update_fps));
		}

		return (false);
	}

public:
	Main_application(jgl::Widget* p_parent) : jgl::Widget(p_parent)
	{
		_render_fps = 0;
		_render_fps_label = new jgl::Text_label("", this);
		_render_fps_label->activate();

		_update_fps = 0;
		_update_fps_label = new jgl::Text_label("", this);
		_update_fps_label->activate();
	}
};

int main(int argc, char **argv)
{
	jgl::Application app("JGL_tester", jgl::Vector2Int(600, 600), jgl::Color(80, 80, 80));
	app.set_default_font("ressource/font/karma suture.ttf");
	app.active_multithread();

	Main_application app_main(nullptr);
	app_main.set_geometry(0, app.size());
	app_main.activate();

	return (app.run());
}