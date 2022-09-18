#pragma once

#include "jgl.h"

#include "structure/jgl_stat.h"

namespace jgl
{
	template<typename T>
	class IWatchdog_bar : public jgl::Widget
	{
	protected:
		jgl::Widget_component::Box _box;
		jgl::Widget_component::Text_label _label;
		jgl::Color _bar_color;

		jgl::String _prefix;
		jgl::String _suffix;

		T* _watchdog;
		T _last_value;

		virtual jgl::Float _percent() = 0;
		virtual jgl::Bool _on_data_change() = 0;
		virtual jgl::String _compose_text() = 0;

		void _render()
		{
			_box.render();
			if (_watchdog != nullptr)
			{
				jgl::Float percent = _percent();
				if (percent > 0.0f)
				{
					jgl::Vector2 size = (_area - _box.border_size() * 2);
					jgl::draw_rectangle_color(_bar_color, _anchor + _box.border_size(), size * jgl::Vector2(percent, 1.0f), _depth + 3);
				}
				_label.render();
			}
		}
		void _on_geometry_change()
		{
			_box.set_border_size(_area.y / 10);
			_box.set_geometry(_anchor, _area, _depth);
			_label.set_geometry(_anchor + _box.border_size(), _area - _box.border_size() * 2, _depth + 5);
		}

		jgl::Bool _update()
		{
			if (_watchdog != nullptr && _on_data_change() == true)
			{
				_label.set_text(_compose_text());

				_last_value = *_watchdog;
			}
			return (false);
		}
		jgl::Bool _fixed_update()
		{
			return (false);
		}

		IWatchdog_bar(jgl::Widget* p_parent) : jgl::Widget(p_parent)
		{
			_watchdog = nullptr;

			_box = jgl::Widget_component::Box(this);
			_box.set_color(jgl::Color(150, 150, 150), jgl::Color(100, 100, 100));

			_label = jgl::Widget_component::Text_label("", this);
			_label.set_horizontal_align(jgl::Horizontal_alignment::Centred);
			_label.set_vertical_align(jgl::Vertical_alignment::Centred);
			_label.set_text_color(jgl::Color(150, 150, 150));
		}

	public:

		void set_prefix(jgl::String p_prefix) { _prefix = p_prefix; }
		void set_suffix(jgl::String p_suffix) { _suffix = p_suffix; }

		T* watchdog() { return (_watchdog); }
		void set_watchdog(T* p_watchdog) {
			_watchdog = p_watchdog;
			if (_watchdog != nullptr)
				_label.set_text(_compose_text());
		}

		jgl::Widget_component::Box& box() { return (_box); }
		jgl::Widget_component::Text_label& label() { return (_label); }

		void set_bar_color(jgl::Color p_color) { _bar_color = p_color; }
	};
	}
	
	class Stat_watchdog_bar : public jgl::IWatchdog_bar<jgl::Stat>
	{
	protected:
		jgl::Float _percent()
		{
			return (_watchdog->percent());
		}
		jgl::Bool _on_data_change()
		{
			return (_watchdog->value() != _last_value.value() || _watchdog->max_value() != _last_value.max_value());
		}
		jgl::String _compose_text()
		{
			return (_prefix + jgl::itoa(_watchdog->value()) + " / " + jgl::itoa(_watchdog->max_value()) + _suffix);
		}

	public:
		Stat_watchdog_bar(jgl::Widget*  p_parent) : jgl::IWatchdog_bar<jgl::Stat>(p_parent)
		{
			
		}
	};

	class Int_watchdog_bar : public jgl::IWatchdog_bar<jgl::Int>
	{
	protected:
		jgl::Float _percent()
		{
			return (1.0f);
		}
		jgl::Bool _on_data_change()
		{
			return (*_watchdog != _last_value);
		}
		jgl::String _compose_text()
		{
			return (_prefix + jgl::itoa(*_watchdog) + _suffix);
		}

	public:
		Int_watchdog_bar(jgl::Widget* p_parent) : jgl::IWatchdog_bar<jgl::Int>(p_parent)
		{

		}
	};
}