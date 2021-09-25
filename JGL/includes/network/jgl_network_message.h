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
		Message_header()
		{

		}
		Message_header(T type)
		{
			id = type;
			size = 0;
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
			return (static_cast<jgl::Uint>(content.size()));
		}

		/*
			Check if the message is empty
		*/
		jgl::Bool empty()
		{
			return (header.size == 0);
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
			content.clear();
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
			Take out a string stored in the message
		*/
		jgl::String get_string()
		{
			jgl::String text;
			jgl::Uint size;
			jgl::Uint i = 0;

			*this >> size;
			while (empty() == false && i < size)
			{
				jgl::Glyph c;
				*this >> c;
				text.insert(c, 0);
				i++;
			}

			return (text);
		}

		/*
			Add a string in the message
		*/
		void add_string(jgl::String text)
		{
			for (jgl::Uint i = 0; i < text.size(); i++)
				*this << text[i];
			*this << text.size();
		}

		/*
			Add a data to the message, storing bytes in content for transmition
		*/
		template<typename DataType>
		friend Message<T>& operator << (Message<T>& msg, const DataType& data)
		{
			static_assert(std::is_standard_layout<DataType>::value, "Data is too complex to be pushed into vector");
#if _WIN64
			size_t i;
#else
			jgl::Size_t i;
#endif
			i = msg.content.size();

			msg.content.resize(msg.content.size() + sizeof(DataType));

			std::memcpy(msg.content.data() + i, &data, sizeof(DataType));

			msg.header.size = msg.size();

			return msg;
		}

		/*
			Take out a set of bytes, and store it in the data variable
		*/
		template<typename DataType>
		friend Message<T>& operator >> (Message<T>& msg, DataType& data)
		{
			static_assert(std::is_standard_layout<DataType>::value, "Data is too complex to be pulled from vector");
#if _WIN64
			size_t i;
#else
			jgl::Size_t i;
#endif

			i = msg.content.size() - sizeof(DataType);

			std::memcpy(&data, msg.content.data() + i, sizeof(DataType));

			msg.content.resize(i);

			msg.header.size = msg.size();

			return msg;
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
