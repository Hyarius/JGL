#pragma once

#include "jgl.h"

namespace jgl
{
	class Updater_widget : public jgl::Widget
	{
	protected:
		void _render() final;

		jgl::Bool _update() = 0;

	public:
		Updater_widget(jgl::Widget* p_parent);
	};
}