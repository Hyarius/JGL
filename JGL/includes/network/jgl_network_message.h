#ifndef JGL_NETWORK_MESSAGE_H
#define JGL_NETWORK_MESSAGE_H

#include "jgl_basic_types.h"
#include "structure/jgl_string.h"

namespace jgl
{
	/*
		Message header :

		Id		enum class (mandatory)	4 bytes
		Size	uint16_t				4 bytes
	*/
	template <typename T, typename std::enable_if<std::is_enum<T>::value == true>::type* = nullptr >
	struct Message_header
	{
		T id{};
		jgl::Size_t size = 0;
		jgl::Size_t readed = 0;

		Message_header()
		{

		}
		Message_header(T type)
		{
			id = type;
			size = 0;
			readed = 0;
		}
	};

	template <typename T, typename std::enable_if<std::is_enum<T>::value == true>::type* = nullptr >
	struct Message
	{
		Message_header<T> header{};
		std::vector<jgl::Uchar> content;

		/*
			Return the type of the message, stored in the header
		*/
		T type() const
		{
			return (header.id);
		}

		/*
			Return the size of the message, stored in the header
		*/
		jgl::Uint size() const
		{
			return (static_cast<jgl::Uint>(header.size - header.readed));
		}

		/*
			Check if the message is empty
		*/
		jgl::Bool empty()
		{
			return (header.size == header.readed);
		}

		/*
			Create a new empty message with undefined ID
		*/
		Message()
		{
			header = Message_header<T>();
			content.clear();
		}

		/*
			Create a new message of ID [type]
		*/
		Message(T type)
		{
			header = Message_header<T>(type);
			content.clear();
		}

		/*
			Clear message content and reset the size in header
		*/
		void clear()
		{
			header.size = 0;
			header.readed = 0;
			content.clear();
		}

		/*
			Skip a number of bytes inside the message
		*/
		void skip(jgl::Size_t p_nb_to_skip)
		{
			header.readed += p_nb_to_skip;
		}

		/*
			Print the header data of the message
		*/
		friend std::ostream& operator << (std::ostream& os, const Message<T>& msg)
		{
			os << "ID:" << int(msg.header.id) << " Size:" << msg.header.size;
			return os;
		}

		/*
			Add an array of data into the message
		*/
		void add_in_array(jgl::Uchar* p_array, jgl::Size_t p_length)
		{
			jgl::Size_t old_size = content.size();

			content.resize(content.size() + p_length);

			std::memcpy(content.data() + old_size, p_array, p_length);

			header.size = content.size();
		}

		/*
			Load an array of data from the message
		*/
		void load_from_array(void* p_address, jgl::Size_t p_length)
		{
			jgl::Size_t next_size = header.readed;// content.size() - sizeof(DataType);

			std::memcpy(p_address, content.data() + next_size, p_length);

			header.readed += p_length;
		}

		void print(jgl::String p_text, jgl::Bool p_binary_mode = true)
		{
			jgl::cout << p_text << " : ";
			for (jgl::Size_t i = 0; i < content.size(); i++)
			{
				if (i != 0)
					jgl::cout << "-";
				if (p_binary_mode == true)
				{
					std::bitset<8> y(content[i]);
					jgl::cout << "[" << y << "]";
				}
				else
				{
					jgl::cout << "[" << content[i] << "]";
				}
			}
			jgl::cout << jgl::endl;
		}

		/*
			Add a data to the message, storing bytes in content for transmition
		*/
		template<typename DataType>
		Message<T>& operator << (const DataType& data)
		{
			static_assert(std::is_standard_layout<DataType>::value, "Data is too complex to be pushed into vector");

			jgl::Size_t old_size = (*this).content.size();

			(*this).content.resize((*this).content.size() + sizeof(DataType));

			std::memcpy((*this).content.data() + old_size, &data, sizeof(DataType));

			(*this).header.size = (*this).content.size();

			return (*this);
		}

		/*
			Take out a set of bytes, and store it in the data variable
		*/
		template<typename DataType>
		Message<T>& operator >> (DataType& data)
		{
			static_assert(std::is_standard_layout<DataType>::value, "Data is too complex to be pulled from vector");

			jgl::Size_t next_size = (*this).header.readed;// (*this).content.size() - sizeof(DataType);

			std::memcpy(&data, (*this).content.data() + next_size, sizeof(DataType));

			(*this).header.readed += sizeof(DataType);

			return (*this);
		}

		template<>
		Message<T>& operator << <jgl::String>	(const jgl::String& text)
		{
			*this << text.size();
			for (jgl::Uint i = 0; i < text.size(); i++)
				*this << text[i];
			return *this;
		}

		template<>
		Message<T>& operator >> <jgl::String>	(jgl::String& text)
		{
			jgl::Size_t size;
			jgl::Uint i = 0;

			*this >> size;
			while (empty() == false && i < size)
			{
				jgl::Glyph c;
				*this >> c;
				text.push_back(c);
				i++;
			}

			return *this;
		}
	};

	/*
		Forward declaration of a connection

		CF jgl_network_connection.h for more information
	*/
	template <typename T, typename std::enable_if<std::is_enum<T>::value == true>::type*>
	class Connection;

	/*
		A message linked to a desired connection
	*/
	template <typename T, typename std::enable_if<std::is_enum<T>::value == true>::type* = nullptr>
	struct Input_message
	{
		Connection<T, nullptr>* remote = nullptr;
		Message<T> msg;
		jgl::Ulong time;

		/*
			Return the type of message stored in input message
		*/
		T type()
		{
			return (msg.type());
		}

		/*
			Print message header
		*/
		friend std::ostream& operator<<(std::ostream& os, const Input_message<T>& msg)
		{
			os << msg.msg;
			return os;
		}
	};
}

#endif
