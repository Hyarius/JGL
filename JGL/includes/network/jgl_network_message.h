#ifndef JGL_NETWORK_MESSAGE_H
#define JGL_NETWORK_MESSAGE_H

#include "jgl_basic_types.h"
#include "structure/jgl_string.h"

namespace jgl
{
	template <typename T, typename std::enable_if<std::is_enum<T>::value == true>::type* = nullptr >
	class Message : public jgl::Data_contener
	{
		T _id{};

	public:
		/*
			Return the type of the message, stored in the header
		*/
		T type() const
		{
			return (_id);
		}

		/*
			Create a new empty message with undefined ID
		*/
		Message() : jgl::Data_contener()
		{
			_content.clear();
		}

		/*
			Create a new message of ID [type]
		*/
		Message(T type) : jgl::Data_contener()
		{
			_id = type;
		}

		/*
			Print the header data of the message
		*/
		friend std::ostream& operator << (std::ostream& os, const Message<T>& msg)
		{
			os << "ID:" << int(_id) << " Size:" << _header.size;
			return os;
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
