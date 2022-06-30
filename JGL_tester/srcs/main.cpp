#include "jgl_tester.h"

using Node = jgl::INode;
using Chunk = jgl::IBakableChunk<Node, 16, 16>;
using Tiledmap = jgl::Tilemap<Chunk>;

class Main_application : public jgl::Widget
{
private:
	jgl::Ulong _render_fps;
	jgl::Text_label* _render_fps_label;

	jgl::Ulong _update_fps;
	jgl::Text_label* _update_fps_label;

	jgl::Button* _create_button;

	jgl::Text_entry* _text_entry;

	jgl::Selecter<jgl::Int>* _selecter;

	void _on_geometry_change()
	{
		_render_fps_label->set_geometry(jgl::Vector2Int(50, 50), jgl::Vector2Int(250, 50));
		_update_fps_label->set_geometry(jgl::Vector2Int(50, 120), jgl::Vector2Int(255, 50));
		_create_button->set_geometry(jgl::Vector2Int(50, 190), jgl::Vector2Int(255, 50));
		_text_entry->set_geometry(jgl::Vector2Int(50, 260), jgl::Vector2Int(455, 50));
		//_selecter->set_geometry(jgl::Vector2Int(50, 320), jgl::Vector2Int(455, 50));
	}

	void _render()
	{
		if (jgl::Application::active_application()->keyboard().get_key(jgl::Key::Alt) == jgl::Input_status::Down)
			jgl::draw_rectangle_color(jgl::Color::red(), 10, 300);
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
	enum class Test
	{
		A, B, C, D
	};
	Main_application(jgl::Widget* p_parent) : jgl::Widget(p_parent)
	{
		_render_fps = 0;
		_render_fps_label = new jgl::Text_label("", this);
		_render_fps_label->activate();

		_update_fps = 0;
		_update_fps_label = new jgl::Text_label("", this);
		_update_fps_label->activate();

		_text_entry = new jgl::Text_entry("", this);

		_create_button = new jgl::Button([&](jgl::Data_contener& p_param) {
			_text_entry->activate();
			}, this);
		_create_button->activate();

		_selecter = new jgl::Selecter<jgl::Int>([&](jgl::Int& p_value) {
				return (jgl::itoa(p_value));
			}, this);
		_selecter->add_value(0);
		_selecter->add_value(10);
		_selecter->add_value(20);
		_selecter->add_value(15);
		_selecter->add_value(5);
		_selecter->set_index(2);
		_selecter->activate();

		jgl::Server_manager<Test>::instanciate(this);
		jgl::Client_manager<Test>::instanciate(this);
	}
};

int main(int argc, char **argv)
{
	jgl::Application app("JGL_tester", jgl::Vector2Int(600, 600), jgl::Color(80, 80, 80));
	app.set_default_font("ressource/font/karma suture.ttf");
	app.active_multithread();

	Tiledmap map;

	Chunk* tmp = map.request_chunk(jgl::Vector2Int(0, 0));
	tmp->bake(&map, false);

	Main_application app_main(nullptr);
	app_main.set_geometry(0, app.size());
	app_main.activate();

	return (app.run());
}