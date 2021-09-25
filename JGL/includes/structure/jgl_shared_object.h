#pragma once

#include "jgl_pool.h"

namespace jgl
{
	template<typename T>
	class share_object
	{
	public:
		T* _element[1] = { nullptr };
		jgl::Reference_count* _reference = nullptr;
		void create_reference()
		{
			_reference = new Reference_count();
		}

	public:
		jgl::Reference_count* reference() { return (_reference); }

		share_object()
		{
			obtain();
			*_element = new T;
		}
		share_object(T* value)
		{
			obtain();
			*_element = value;
		}
		share_object(const share_object& other)
		{
			release();
			_element[0] = other._element[0];
			_reference = other._reference;
			_reference->increment();
		}
		~share_object()
		{
			release();
		}
		void obtain()
		{
			release();
			*_element = nullptr;
			create_reference();
			_reference->increment();
		}
		void release()
		{
			if (_reference != nullptr)
			{
				_reference->decrement();
				if (_reference->value() == 0)
				{
					if (*_element != nullptr)
						delete* _element;
					*_element = nullptr;
					delete _reference;
					_reference = nullptr;
				}
			}
		}
		share_object<T> copy()
		{
			if (*_element == nullptr)
				return (nullptr);
			else
				return (new T(**_element));
		}
		jgl::share_object<T>& operator = (T* value)
		{
			obtain();
			*_element = value;
			return (*this);
		}
		jgl::share_object<T>& operator = (const jgl::share_object<T>& other)
		{
			release();
			_element[0] = other._element[0];
			_reference = other._reference;
			_reference->increment();
			return (*this);
		}
		T& operator* ()
		{
			if (_element == nullptr)
				THROW_EXCEPTION(jgl::Error_level::Critical, 1, "Segfault : invalid acces to share_object while equal to nullptr 1");
			if (*_element == nullptr)
				THROW_EXCEPTION(jgl::Error_level::Critical, 1, "Segfault : invalid acces to share_object while equal to nullptr 2");
			return (**_element);
		}
		T* operator-> ()
		{
			if (_element == nullptr)
				return (nullptr);
			return (*_element);
		}
		const T& operator* () const
		{
			if (_element == nullptr)
				THROW_EXCEPTION(jgl::Error_level::Critical, 1, "Segfault : invalid acces to share_object while equal to nullptr 1");
			if (*_element == nullptr)
				THROW_EXCEPTION(jgl::Error_level::Critical, 1, "Segfault : invalid acces to share_object while equal to nullptr 2");
			return (**_element);
		}
		const T* operator-> () const
		{
			if (_element == nullptr)
				return (nullptr);
			return (*_element);
		}
		bool operator == (const share_object<T>& other) const
		{
			T* first = (this->_element)[0];
			T* second = (other._element)[0];

			return (first == second);
		}
		bool operator != (const share_object<T>& other) const
		{
			T* first = (this->_element)[0];
			T* second = (other._element)[0];

			return (first != second);
		}
		T* extract()
		{
			if (*_element == nullptr)
				return (nullptr);
			T* result = *_element;
			delete _reference;
			*_element = nullptr;
			return (result);
		}
		friend std::ostream& operator<<(std::ostream& os, const jgl::share_object<T> value)
		{
			if (value == nullptr)
				os << "(nullptr)";
			else
				os << *value;
			return (os);
		}
	};
}