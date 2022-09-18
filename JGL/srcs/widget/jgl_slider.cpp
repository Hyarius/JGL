#include "widget/jgl_slider.h"
#include "application/jgl_application.h"

namespace jgl
{
	void Slider::Slidder_bar::_render()
	{
		_box.render();
	}

	void Slider::Slidder_bar::_on_geometry_change()
	{
		_box.set_geometry(_anchor, _area, _depth);
	}

	jgl::Bool Slider::Slidder_bar::_update()
	{
		return (false);
	}

	jgl::Bool Slider::Slidder_bar::_fixed_update()
	{
		return (false);
	}

	Slider::Slidder_bar::Slidder_bar(jgl::Widget* p_parent) : jgl::Widget(p_parent)
	{
		_box = jgl::Widget_component::Box(this);
		_box.set_color(jgl::Color(150, 150, 150), jgl::Color(100, 100, 100));
		_box.set_border_size(2);
	}

	void Slider::_render()
	{

	}

	void Slider::_on_geometry_change()
	{
		jgl::Float button_size = _area.x;

		_up_button->set_geometry(0, button_size);
		_slidder_bar->set_geometry(jgl::Vector2Int(0, button_size), jgl::Vector2Int(button_size, _area.y - button_size * 2));
		_down_button->set_geometry(jgl::Vector2Int(0, _area.y - button_size), button_size);
	}

	jgl::Bool Slider::_update()
	{
		if (_parent->is_pointed() == true)
		{
			if (jgl::Application::active_application()->mouse().wheel() != 0 && _motion_timer.timeout() != jgl::Timer::State::Running)
			{
				_move_widget(jgl::Vector2Int(0, -30) * static_cast<jgl::Int>(jgl::Application::active_application()->mouse().wheel()));
				_motion_timer.start();
			}
		}
		return (false);
	}

	jgl::Bool Slider::_fixed_update()
	{
		return (false);
	}

	void Slider::_move_widget(jgl::Vector2Int p_delta)
	{
		_delta.x = jgl::clamp(_min.x, _delta.x + p_delta.x, _max.x);
		_delta.y = jgl::clamp(_min.y, _delta.y + p_delta.y, _max.y);

		for (jgl::Size_t i = 0; i < _subscribed_widget_base_anchor.size(); i++)
		{
			_subscribed_widget[i]->set_geometry(_subscribed_widget_base_anchor[i] - (_delta - _min), _subscribed_widget[i]->area());
		}
	}

	Slider::Slider(jgl::Widget* p_parent) : jgl::Widget(p_parent), _motion_timer(4)
	{
		_slidder_bar = new Slidder_bar(this);
		_slidder_bar->activate();

		_up_button = new jgl::Button([&](jgl::Data_contener& p_param) {
			_move_widget(jgl::Vector2Int(0, -10)); }, this);
		_up_button->label().set_text("-");
		for (jgl::Size_t i = 0; i < 2; i++)
			_up_button->boxes()[i].set_border_size(2);
		_up_button->activate();

		_down_button = new jgl::Button([&](jgl::Data_contener& p_param) {
			_move_widget(jgl::Vector2Int(0, 10)); }, this);
		_down_button->label().set_text("+");
		for (jgl::Size_t i = 0; i < 2; i++)
			_down_button->boxes()[i].set_border_size(2);
		_down_button->activate();
	}
		
	void Slider::subscribe(jgl::Widget* p_widget)
	{
		_subscribed_widget.push_back(p_widget);
		_subscribed_widget_base_anchor.push_back(p_widget->anchor());
	}

	void Slider::update_anchor()
	{
		_min = INT_MAX;
		_max = INT_MIN;

		for (jgl::Size_t i = 0; i < _subscribed_widget_base_anchor.size(); i++)
		{
			_subscribed_widget_base_anchor[i] = _subscribed_widget[i]->anchor();
			_min = jgl::Vector2Int::compose_min(_min, _subscribed_widget[i]->anchor());
			_max = jgl::Vector2Int::compose_max(_max, _subscribed_widget[i]->anchor() + _subscribed_widget[i]->area());
		}

		_max -= _window_size;

		if (_max.x < _min.x)
			_max.x = _min.x;
		if (_max.y < _min.y)
			_max.y = _min.y;

		_delta = _min;
	}
}