#pragma once

#include "jgl_includes.h"
#include "Widget/jgl_widget.h"

namespace jgl
{
	template<typename T>
	class Singleton_widget
	{
		/*
		* Don't forget to add 'friend class Singleton_widget<T>;' to T definition
		*/
	protected:
		static inline T* _instance = nullptr;

		Singleton_widget()
		{
			static_assert(std::is_base_of<jgl::Widget, T>::value, "Singleton_widget can only allow jgl::Widget enherenced object");
		}

	public:
		static T* instanciate(jgl::Widget* p_parent)
		{
			if (_instance == nullptr)
			{
				_instance = new T(p_parent);
				_instance->activate();
			}
			return (_instance);
		}
		static T* instance()
		{
			return (_instance);
		}
	};
}