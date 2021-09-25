#pragma once

#include "jgl_basic_types.h"

namespace jgl
{
	class Reference_count
	{
	private:
		jgl::Int _value;
	public:
		/*
			Default constructor
		*/
		Reference_count() : _value(0) {}

		/*
			Increment the reference counter
		*/
		void increment() { _value++; }

		/*
			Decrement the reference counter
		*/
		void decrement() { _value--; }

		/*
			Returns the counter value
		*/
		const jgl::Int value() const { return (_value); }

		/*
			Set the value to a defined value
		*/
		void set_value(jgl::Int p_value) { _value = p_value; }

		/*
			Print the reference counter
		*/
		friend std::ostream& operator<<(std::ostream& os, const jgl::Reference_count& value)
		{
			os << value.value();
			return (os);
		}
	};

	template <typename T>
	class Pool
	{
	private:
		std::mutex _mutex;
		jgl::Array<T*>_content;
	public:
		/*
			Default constructor
		*/
		Pool() {}

		/*
			Destructor

			/!\ delete memory allocated inside the pool /!\
			/!\ DO NOT DELETE IT YOURSELF /!\
		*/
		~Pool()
		{
			for (uint32_t i = 0; i < _content.size(); i++)
			{
				if (_content[i] != nullptr)
				{
					delete _content[i];
				}
				_content[i] = nullptr;
			}
		}
		/*
			return the list of element inside the pool
		*/
		const jgl::Array<T*> content() const { return (_content); }

		/*
			Return the number of element in the pool
		*/
		const uint32_t size() const { return (_content.size()); }

		/*
			Obtain one element stored in the pool

			If there is no more element, generate one and returns it
		*/
		T* obtain()
		{
			_mutex.lock();
			if (_content.size() == 0)
			{
				T* new_element = new T();
				_mutex.unlock();
				return (new_element);
			}
			else
			{
				auto tmp = _content.back();
				T* result = _content[tmp.index()];
				_content.pop_back();
				_mutex.unlock();
				return (result);
			}
			_mutex.unlock();
		}

		/*
			Put back an element in the pool
		*/
		void release(T* object)
		{
			_mutex.lock();
			_content.push_back(object);
			_mutex.unlock();
		}

		/*
			Accessor operator
		*/
		T* operator[](uint32_t index) { return (_content[index]); }
	};


	template<typename T>
	class Pool_object
	{
	protected:
		static inline jgl::Pool<T> _elem_pool = jgl::Pool<T>();

		T* _element = nullptr;
		jgl::Reference_count* _reference = nullptr;
		void _create_reference()
		{
			_reference = new Reference_count();
		}
		void _obtain()
		{
			_release();
			_element = _elem_pool.obtain();
			_create_reference();
			_reference->increment();
		}
		void _release()
		{
			if (_reference != nullptr)
			{
				_reference->decrement();
				if (_reference->value() == 0 && _element != nullptr)
				{
					_elem_pool.release(_element);
					delete _reference;
					_reference = nullptr;
				}
			}
			_element = nullptr;
		}

	public:
		/*
			Return the element pool corresponding at the template
		*/
		static const jgl::Pool<T>& elem_pool() { return (_elem_pool); }

		/*
			Default constructor
		*/
		Pool_object()
		{
			_obtain();
		}

		/*
			Constructor with copy content
		*/
		Pool_object(T value)
		{
			_obtain();
			*_element = value;
		}

		/*
			Copy constructor
		*/
		Pool_object(const Pool_object& value)
		{
			_release();

			_element = value._element;
			_reference = value._reference;
			_reference->increment();
		}

		/*
			Destructor
		*/
		~Pool_object() { _release(); }

		/*
			Return the content of the pool object
		*/
		const T* element() const { return (_element); }

		/*
			Return the reference count of the pool object
		*/
		const jgl::Reference_count* reference() const { return (_reference); }

		/*
			Compare both pool object and return true if theme content is equal
		*/
		jgl::Pool_object<T>& operator = (const jgl::Pool_object<T>& other)
		{
			if (this != &other)
			{
				_release();
				_element = other._element;
				_reference = other._reference;
				_reference->increment();
			}
			return (*this);
		}

		/*
			Indirection operator
		*/
		T& operator* () { return (*_element); }

		/*
			Dereference operator
		*/
		T* operator-> () { return (_element); }

		/*
			Const indirection operator
		*/
		const T& operator* () const { return (*_element); }

		/*
			Const dereference operator
		*/
		const T* operator-> () const { return (_element); }
	};
}