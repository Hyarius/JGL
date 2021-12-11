#include "jgl.h"

namespace jgl
{
	void String::_resize(Size_t new_desired_size)
	{
		if (_max_size >= new_desired_size)
			return;

		//Recupere l'ancien tableau 2D
		Glyph** old_array_content = _content;
		//Calcul de l'ancien nombre de ligne
		Size_t nb_line = _max_size / JGL_STRING_BUFFER_SIZE;

		//Definition de la nouvelle max size, un multiple de JGL_STRING_BUFFER_SIZE
		_max_size = (new_desired_size / JGL_STRING_BUFFER_SIZE) * JGL_STRING_BUFFER_SIZE;
		//Calcul du nouveau nombre de ligne
		Size_t new_nb_line = (_max_size / JGL_STRING_BUFFER_SIZE) + 1;
		//Reallocation d'un tableau 2D avec le nouveau nombre de ligne
		_content = new Glyph * [new_nb_line];

		//On copie dans le nouveau tableau le contenu de l'ancien tableau
		for (Size_t i = 0; i < nb_line; i++)
		{
			_content[i] = old_array_content[i];
		}


		//On se debarrasse de l'ancien tableau
		delete[] old_array_content;

		//Pour chaque nouvelles lignes a ajouter on cree une ligne vierge
		for (Size_t i = nb_line; i < new_nb_line; i++)
		{
			_content[i] = new Glyph[JGL_STRING_BUFFER_SIZE];
			for (Size_t j = 0; j < JGL_STRING_BUFFER_SIZE; j++)
				_content[i][j] = '\0';
		}
	}

	void String::_add_new_line()
	{
		_resize(_max_size + JGL_STRING_BUFFER_SIZE);
	}

	void String::_delete_array_content()
	{
		//Si la taille max correspond a celle dans le buffer de taille fixe
		if (_max_size == JGL_STRING_BUFFER_SIZE)
			return;

		//Si on a bien alloue un tableau 2D
		if (_content != nullptr)
		{
			//Pour toutes les lignes sauf la premiere
			for (Size_t i = 1; i < (_max_size / JGL_STRING_BUFFER_SIZE); i++)
			{
				//On delete la ligne
				delete[] _content[i];
			}
			//On delete le tableau 2D
			delete[] _content;
			_content = nullptr;
		}
	}

	jgl::Size_t String::_count_word(const String& delim)
	{
		jgl::Size_t tmp = size();
		jgl::Size_t tmp2 = delim.size();
		jgl::Size_t result = 1;

		for (jgl::Size_t i = 0; i < tmp; i++)
		{
			if (this->operator[](i) == delim[0])
			{
				jgl::Size_t j = 0;
				while (j < tmp2 &&
					i + j < tmp &&
					this->operator[](i + j) == delim[j])
					j++;
				if (j == tmp2)
					result++;
				i += j - 1;
			}
		}
		return (result);
	}

	jgl::Size_t String::_count_word(const jgl::Glyph& delim)
	{
		jgl::Size_t tmp = size();
		jgl::Size_t result = 1;

		for (jgl::Size_t i = 0; i < tmp; i++)
		{
			if (this->operator[](i) == delim)
				result++;
		}
		return (result);
	}

	jgl::Size_t String::_count_word_len(const jgl::Glyph& delim, const jgl::Size_t start)
	{
		jgl::Size_t tmp = size();
		jgl::Size_t result = 0;

		for (result = start; result < tmp; result++)
		{
			if (this->operator[](result) == delim)
				return (result - start);
		}
		return (result - start);
	}

	jgl::Size_t String::_count_word_len(const String& delim, const jgl::Size_t start)
	{
		jgl::Size_t tmp = size();
		jgl::Size_t tmp2 = delim.size();
		jgl::Size_t result = 0;

		for (result = start; result < tmp; result++)
		{
			if (this->operator[](result) == delim[0])
			{
				jgl::Size_t j = 0;
				while (j < tmp2 &&
					result + j < tmp &&
					this->operator[](result + j) == delim[j])
					j++;
				if (j == tmp2)
					return (result - start);
				result += j - 1;
			}
		}
		return (result - start);
	}

	String::String()
	{
		_str = nullptr;
		_size = 0u;
		_max_size = JGL_STRING_BUFFER_SIZE;
		_content = new Glyph * [1];
		_content[0] = _buffer;
	}

	String::String(const char* str) : String()
	{
		for (Size_t i = 0; str[i] != '\0'; i++)
			push_back(str[i]);
	}

	String::String(Glyph* str) : String()
	{
		for (Size_t i = 0; str[i] != '\0'; i++)
			push_back(str[i]);
	}

	String::String(Glyph elem, Size_t nb) : String()
	{
		for (Size_t i = 0; i < nb; i++)
			push_back(elem);
	}

	String::String(const std::string& other) : String()
	{
		for (Size_t i = 0; i < other.size(); i++)
			push_back(other[i]);
	}

	String::String(const std::wstring& other) : String()
	{
		for (Size_t i = 0; i < other.size(); i++)
			push_back(jgl::Glyph(other[i]));
	}

	String::String(const String& other) : String()
	{
		for (Size_t i = 0; i < other.size(); i++)
			push_back(other[i]);
	}

	String::~String()
	{
		_delete_array_content();
		
		jgl::Size_t nb_to_remove = _max_size / JGL_STRING_BUFFER_SIZE;

		if (_content != nullptr)
		{
			for (jgl::Size_t i = 1; i < nb_to_remove; i++)
				delete _content[i];
			delete[] _content;
		}
	}

	const Size_t String::size() const
	{
		return (_size);
	}

	const Glyph& String::back() const
	{
		if (_size == 0)
			return (nullchar);
		return (this->operator[](_size - 1));
	}

	void String::push_back(const Glyph& elem)
	{
		//Si la derniere ligne est remplie
		if (_size + 1 >= _max_size)
		{
			//On ajoute une nouvelle ligne vierge
			_add_new_line();
		}
		//Acces a l'element de fin de la chaine puis definition de sa valeur
		this->operator[](_size) = elem;
		//On augmente la taille de la chaine de 1
		_size++;
		_clear_str();
	}

	void String::push_front(const Glyph& elem)
	{
		insert(elem, 0);
	}

	void String::append(const String& other)
	{
		//Pour chaque glyph dans la chaine other, on push_back ce glyph
		for (Size_t i = 0; i < other.size(); i++)
		{
			push_back(other[i]);
		}
	}

	void String::clear()
	{
		_size = 0;
	}

	void String::resize(const Size_t& new_size)
	{
		_resize(new_size);
		//On defini la nouvelle size a celle desirée
		_size = new_size;
		_clear_str();
	}

	const jgl::Bool String::empty() const
	{
		return (_size == 0);
	}

	const Int String::find(const String& to_find) const
	{
		for (Size_t i = 0; i < size(); i++)
		{
			jgl::Bool found = true;
			for (Size_t j = 0; j < to_find.size() && found == true; j++)
			{
				if (this->operator[](i + j) != to_find[j])
					found = false;
			}
			if (found == true)
				return (i);
		}
		return (npos);
	}

	const jgl::Bool String::compose_only(const String& to_find) const
	{
		jgl::Bool result = false;
		for (uint32_t i = 0; i < size(); i++)
		{
			result = false;
			for (uint32_t j = 0; j < to_find.size() && result == false; j++)
				if (this->operator[](i) == to_find[j])
					result = true;
			if (result == false)
				return (false);
		}
		return (true);
	}

	String String::substr(const Size_t& start, const Size_t& size) const
	{
		String result;

		this->substr(result, start, size);

		return (result);
	}

	void String::substr(String& result, const Size_t& start, const Size_t& size) const
	{
		if (start >= _size)
		{
			result.clear();
			return;
		}

		result.clear();
		result.resize(size);
		for (jgl::Size_t i = 0; i < size; i++)
		{
			result[i] = jgl::Glyph(this->operator[](i + start));
		}
	}

	void String::pop_back()
	{
		_size--;
		_clear_str();
	}

	void String::insert(const Glyph& to_add, const Size_t& index)
	{
		if (index > size())
			THROW_EXCEPTION(jgl::Error_level::Critical, 1, "String too short to add a glyph at pos " + jgl::itoa(index));
		push_back(back());
		for (jgl::Size_t i = _size - 1; i > index; i--)
		{
			this->operator[](i) = this->operator[](i - 1);
		}
		this->operator[](index) = to_add;
		_clear_str();
	}

	void String::erase(Size_t index)
	{
		if (index >= size())
			THROW_EXCEPTION(jgl::Error_level::Critical, 1, "String too short to remove a glyph at pos " + jgl::itoa(index));
		_size--;
		while (index <= _size)
		{
			this->operator[](index) = this->operator[](index + 1);
			index++;
		}
		_clear_str();
	}

	const char* String::c_str()
	{
		if (_str == nullptr)
		{
			jgl::Size_t total_size_str = total_size();
			_str = new char[total_size_str + 1];
			jgl::Size_t c = 0;

			for (uint32_t i = 0; i < total_size_str; i++)
			{
				jgl::Glyph tmp = this->operator[](i);
				for (uint32_t j = 0; j < tmp.size(); j++)
				{
					_str[c] = tmp.content()[j];
					c++;
				}
			}
			_str[c] = '\0';
		}

		return (_str);
	}

	const unsigned char* String::uc_str()
	{
		return (reinterpret_cast<const unsigned char*>(c_str()));
	}

	void String::split(jgl::Array<String>& tab, const String& delim, const jgl::Bool& regroup)
	{
		jgl::Size_t tmp = size();
		jgl::Size_t tmp2 = delim.size();
		jgl::Size_t index = 0;
		jgl::Size_t nb_word = 0;
		jgl::Size_t total_nb_word = _count_word(delim);
		
		tab.resize(total_nb_word);
		for (jgl::Size_t i = 0; i < total_nb_word; i++)
			tab[i].clear();
		while (index < tmp)
		{
			jgl::Size_t word_len = _count_word_len(delim, index);
			if (word_len != 0 || regroup == true)
			{
				tab[nb_word] = substr(index, word_len);
				nb_word++;
			}
			index += word_len + tmp2;
		}
	}

	jgl::Array<String> String::split(const String& delim, const jgl::Bool& regroup)
	{
		jgl::Array<String>	tab(40);

		split(tab, delim, regroup);

		return (tab);
	}

	Glyph& String::operator [](const Size_t index)
	{
		//Calcul pour acceder a la ligne desirée et a l'index dans cette ligne
		Size_t nb_line = index / JGL_STRING_BUFFER_SIZE;
		Size_t nb_index = index % JGL_STRING_BUFFER_SIZE;
		return (_content[nb_line][nb_index]);
	}

	const Glyph& String::operator [](const Size_t index) const
	{
		//Calcul pour acceder a la ligne desirée et a l'index dans cette ligne
		Size_t nb_line = index / JGL_STRING_BUFFER_SIZE;
		Size_t nb_index = index % JGL_STRING_BUFFER_SIZE;
		return (_content[nb_line][nb_index]);
	}
	
	String& String::operator = (const String& p_value)
	{
		clear();

		for (uint32_t i = 0; i < p_value.size(); i++)
			push_back(p_value[i]);

		return (*this);
	}

	String& String::operator = (const char* p_value)
	{
		clear();

		for (uint32_t i = 0; p_value[i] != '\0'; i++)
			push_back(p_value[i]);

		return (*this);
	}

	String String::operator + (const String& other) const
	{
		String result;

		// Ajout du contenu de la chaine actuelle
		for (Size_t i = 0; i < size(); i++)
		{
			result.push_back(this->operator[](i));
		}
		// Ajout du contenu de la chaine other
		for (Size_t i = 0; i < other.size(); i++)
		{
			result.push_back(other[i]);
		}

		return (result);
	}

	String operator + (const char* str, const String& delta) 
	{
		String result;

		for (Size_t i = 0; str[i]; i++)
			result.push_back(str[i]);

		for (Size_t i = 0; i < delta.size(); i++)
			result.push_back(delta[i]);

		return (result);
	}

	void String::operator += (const String& other)
	{
		//Ajout dans la chaine actuelle le contenu de la chaine other
		for (Size_t i = 0; i < other.size(); i++)
			push_back(other[i]);
	}

	jgl::Bool String::operator == (const String& other) const
	{
		if (size() != other.size())
			return (false);
		for (jgl::Size_t i = 0; i < size(); i++)
			if (other[i] != this->operator[](i))
				return (false);
		return (true);
	}
	jgl::Bool String::operator != (const String& other) const
	{
		return (!(*this == other));
	}

	jgl::Bool String::operator < (const jgl::String& other) const
	{
		for (uint32_t i = 0; i < size(); i++)
		{
			jgl::Glyph tmp2;
			jgl::Glyph tmp;
			if (other.size() > i)
				tmp = other[i];
			else
				tmp = jgl::Glyph('\0');
			tmp2 = this->operator[](i);

			if (tmp2 < tmp)
			{
				return (true);
			}
			else if (tmp2 > tmp)
			{
				return (false);
			}
		}
		if (other.size() > size())
			return (true);
		return (false);
	}

	jgl::Bool String::operator > (const jgl::String& other) const
	{
		for (uint32_t i = 0; i < size(); i++)
		{
			jgl::Glyph tmp;
			jgl::Glyph tmp2;
			if (other.size() > i)
				tmp = other[i];
			else
				tmp = jgl::Glyph('\0');
			tmp2 = this->operator[](i);

			if (tmp2 > tmp)
			{
				return (true);
			}
			else if (tmp2 < tmp)
			{
				return (false);
			}
		}
		if (other.size() > size())
			return (false);
		return (false);
	}
}