#pragma once

#include "jgl_basic_types.h"

namespace jgl
{
	template <typename T>
	class Array
	{

	private:
		bool _computed;
		T* _computed_result;
		jgl::Size_t _computed_result_size;

		T** _array_content;
		jgl::Size_t _size;
		jgl::Size_t _max_size;
		jgl::Size_t _push_size;

		void _add_new_line()
		{
			T** old_array_content = _array_content;

			jgl::Size_t nb_line = _max_size / _push_size;
			_max_size += _push_size;
			_array_content = new T * [nb_line + 1];

			for (jgl::Size_t i = 0; i < nb_line; i++)
				_array_content[i] = old_array_content[i];

			delete[] old_array_content;

			_array_content[nb_line] = new T[_push_size];
			for (jgl::Size_t i = 0; i < _push_size; i++)
				_array_content[nb_line][i] = T();

		}

		void _clear_computed()
		{
			_computed = false;
		}

		void _delete_computed_content()
		{
			if (_computed = true)
				delete _computed_result;
			_computed = false;
			_computed_result = nullptr;
		}
		void _delete_array_content()
		{
			if (_max_size == 0)
				return;

			if (_array_content != nullptr)
			{
				for (jgl::Size_t i = 0; i < _max_size / _push_size; i++)
				{
					delete[] _array_content[i];
				}
				delete[] _array_content;
				_array_content = nullptr;
			}
		}

	public:
		/*
			Iterator for the array
		*/
		class Iterator
		{
		private:
			const Array<T>* _parent;
			jgl::Size_t _index;
			const T* _element;

			/*
				Base constructor
				Set the iterator's parent to nullptr
				Set the element describe by the iterator to nullptr and his index to 0
			*/
			Iterator()
			{
				_parent = nullptr;
				_index = 0;
				_element = nullptr;
			}

			void _calc_element()
			{
				if (_index >= _parent->size())
					_element = nullptr;
				else
					_element = &(_parent->operator[](_index));
			}
			void _set_index(jgl::Size_t p_delta) {
				_index = p_delta;
			}
			void _set_element(const T* p_delta) {
				_element = p_delta;
			}
		public:
			Iterator(const Array<T>* p_parent, const jgl::Size_t p_index)
			{
				_parent = p_parent;
				_index = p_index;
				_calc_element();
			}

			const Array<T>* parent() const { return (_parent); }

			const jgl::Size_t index() const { return (_index); }

			const T* element() const { return (_element); }

			Iterator operator + (const Iterator other) const
			{
				Array<T>::Iterator result;

				if (result.index() + other.index() >= result.parent()->size())
					result._set_index(result.parent()->size());
				else
					result._set_index(result.index() + other.index());
				result._calc_element();
				return (result);
			}

			Iterator operator - (const Iterator other) const
			{
				Array<T>::Iterator result;

				if (result.index() <= other.index())
					result._set_index(0);
				else
					result._set_index(result.index() - other.index());
				result._calc_element();
				return (result);
			}

			Iterator operator + (jgl::Int delta) const
			{
				Array<T>::Iterator result;

				if (result.index() + static_cast<jgl::Size_t>(delta) >= result.parent()->size())
					result._set_index(result.parent()->size());
				else
					result._set_index(result.index() + delta);
				result._calc_element();
				return (result);
			}

			Iterator operator - (jgl::Int delta) const
			{
				Array<T>::Iterator result;

				if (result.index() <= static_cast<jgl::Size_t>(delta))
					result._set_index(0);
				else
					result._set_index(result.index() - delta);
				result._calc_element();
				return (result);
			}

			void operator ++ ()
			{
				if (_index + 1 >= _parent->size())
					_index = _parent->size();
				else
					_index++;
				_calc_element();
			}

			void operator -- ()
			{
				if (_index == 0)
					return;
				_index--;
				_calc_element();
			}

			Iterator& operator = (const jgl::Size_t p_value)
			{
				_index = p_value;
				_calc_element();
				return (*this);
			}

			const T& operator*() const
			{
				return (*_element);
			}

			bool operator == (const Iterator other) const
			{
				if (_parent != other.parent())
					return (false);
				return (_index == other.index());
			}

			bool operator != (const Iterator other) const
			{
				if (_parent != other.parent())
					return (false);
				return (_index != other.index());
			}

			bool operator < (const Iterator other) const
			{
				if (_parent != other.parent())
					return (false);
				return (_index < other.index());
			}

			bool operator > (const Iterator other) const
			{
				if (_parent != other.parent())
					return (false);
				return (_index > other.index());
			}

			bool operator <= (const Iterator other) const
			{
				if (_parent != other.parent())
					return (false);
				return (_index <= other.index());
			}

			bool operator >= (const Iterator other) const
			{
				if (_parent != other.parent())
					return (false);
				return (_index >= other.index());
			}
		};

		Array(const std::initializer_list<T> init) : Array<T>()
		{
			for (jgl::Size_t i = 0; i < init.size(); i++)
				push_back(init.begin()[i]);
		}

		Array(const jgl::Size_t p_push_size = 100)
		{
			_array_content = nullptr;
			_push_size = p_push_size;
			_size = 0;
			_max_size = 0;
			_computed = false;
			_computed_result = nullptr;
			_computed_result_size = 0;
		}

		Array(const Array<T>& other) : Array<T>(other._push_size)
		{
			for (jgl::Size_t i = 0; i < other.size(); i++)
				push_back(other[i]);
		}

		~Array()
		{
			_delete_computed_content();
			_delete_array_content();
		}

		T& push_back(const T elem)
		{
			if (_size + 1 >= _max_size)
			{
				_add_new_line();
			}
			jgl::Size_t nb_line = _size / _push_size;
			jgl::Size_t nb_index = _size % _push_size;
			_array_content[nb_line][nb_index] = elem;
			_size++;
			_clear_computed();
			return (_array_content[nb_line][nb_index]);
		}

		T& push_front(const T& elem)
		{
			return (insert(0, elem));
		}

		T& first()
		{
			return (this->operator[](0));
		}

		T& last()
		{
			return (this->operator[](_size - 1));
		}

		const T& first() const
		{
			return (this->operator[](0));
		}

		const T& last() const
		{
			return (this->operator[](_size - 1));
		}

		const Iterator front() const
		{
			return (Iterator(this, 0));
		}

		const Iterator back() const
		{
			if (size() == 0)
				return (end());
			return (Iterator(this, _size - 1));
		}

		jgl::Array<T>& operator = (const jgl::Array<T>& other)
		{
			this->clear();
			for (jgl::Size_t i = 0; i < other.size(); i++)
				push_back(other[i]);

			return (*this);
		}

		T& operator [](const jgl::Size_t index)
		{
			jgl::Size_t nb_line = index / _push_size;
			jgl::Size_t nb_index = index % _push_size;
			return (_array_content[nb_line][nb_index]);
		}

		const T& operator [](const jgl::Size_t index) const
		{
			jgl::Size_t nb_line = index / _push_size;
			jgl::Size_t nb_index = index % _push_size;
			return (_array_content[nb_line][nb_index]);
		}

		T& insert(const jgl::Size_t index, const T elem)
		{
			jgl::Size_t i = size();
			push_back({});
			while (i >= 1 && i > index)
			{
				T tmp = this->operator[](i - 1);
				this->operator[](i) = tmp;
				i--;
			}
			this->operator[](index) = elem;
			_clear_computed();
			return (this->operator[](index));
		}

		T& insert(const Iterator iter, const T elem)
		{
			jgl::Size_t i = size();
			push_back('\0');
			while (i >= 1 && i > iter.index())
			{
				T tmp = this->operator[](i - 1);
				this->operator[](i) = tmp;
				i--;
			}
			this->operator[](iter.index()) = elem;
			_clear_computed();
			return (this->operator[](iter.index()));
		}

		void resize(const jgl::Size_t new_size)
		{
			while (_max_size < new_size)
				_add_new_line();
		}

		void clear()
		{
			_size = 0;
			_clear_computed();
		}

		bool empty() const
		{
			return (_size == 0 ? true : false);
		}

		bool contain(const T to_find) const
		{
			for (jgl::Size_t i = 0; i < size(); i++)
				if (this->operator[](i) == to_find)
					return(true);
			return (false);
		}

		void pop_back()
		{
			if (_size > 0)
				_size--;
			_clear_computed();
		}

		void pop_front()
		{
			erase(0);
		}

		void erase(const jgl::Size_t index)
		{
			if (_size == 0)
				return;
			for (jgl::Size_t i = index; i < _size - 1; i++)
				this->operator[](i) = this->operator[](i + 1);
			_size--;
			_clear_computed();
		}

		void erase(const Iterator iter)
		{
			if (_size == 0)
				return;
			for (jgl::Size_t i = iter.index(); i < _size - 1; i++)
				this->operator[](i) = this->operator[](i + 1);
			_size--;
			_clear_computed();
		}

		void reverse()
		{
			for (jgl::Size_t i = 0; i < _size / 2; i++)
			{
				T tmp = this->operator[](i);
				this->operator[](i) = this->operator[](_size - i - 1);
				this->operator[](_size - i - 1) = tmp;
			}
		}

		const bool computed() const { return (_computed); }

		const T* computed_content() const { return (_computed_result); }

		const jgl::Size_t size() const { return (_size); }

		const T* all()
		{
			if (_size == 0)
				return (nullptr);

			if (_computed == false)
			{
				if (_computed_result == nullptr || _computed_result_size < _size)
				{
					if (_computed_result != nullptr)
						delete _computed_result;

					_computed_result = new T[_size];
					_computed_result_size = _size;
				}

				for (jgl::Size_t i = 0; i < _size; i++)
					_computed_result[i] = this->operator[](i);
				_computed = true;
			}

			return (_computed_result);
		}

		Iterator find(const T to_find) const {
			return (find(to_find, 0));
		}

		Iterator find(const T to_find, const jgl::Size_t start) const {
			for (jgl::Size_t i = start; i < size(); i++)
			{
				if (this->operator[](i) == to_find)
					return (Iterator(this, i));
			}
			return (end());
		}

		Iterator begin() const
		{
			return (Iterator(this, 0));
		}

		Iterator end() const
		{
			return (Iterator(this, size()));
		}

		friend std::ostream& operator<<(std::ostream& os, const jgl::Array<T>& value)
		{
			for (jgl::Size_t i = 0; i < value.size(); i++)
			{
				if (i != 0)
					os << " - ";
				os << "[" << value[i] << "]";
			}
			return os;
		}
	};
}