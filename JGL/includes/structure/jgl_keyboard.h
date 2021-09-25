#pragma once

#include "jgl_enum.h"
#include "jgl_basic_types.h"
#include "structure/jgl_exception.h"

namespace jgl
{
	struct Keyboard
	{
	private:
		friend class Application;
		static jgl::String _unknow_key;
		static jgl::Map<jgl::Key, jgl::String> _input_name;
		jgl::Input_status _data[KEY_COUNT];
		jgl::Glyph _char_entry;

		void _update()
		{
			for (jgl::Size_t i = 0; i < KEY_COUNT; i++)
			{
				if (_data[i] == jgl::Input_status::Pressed)
					_data[i] = jgl::Input_status::Down;
				else if (_data[i] == jgl::Input_status::Release)
					_data[i] = jgl::Input_status::Up;
			}
		}

		void _actualize_entry(jgl::Glyph entry)
		{
			_char_entry = entry;
		}

	public:
		/*
			Method used by the WinProc function
			/!\ DO NOT OVERLOAD :!\
		*/
		friend void actualize_key(jgl::Keyboard& keyboard, jgl::Key p_key, jgl::Input_status p_status);
		/*
			Method used by the WinProc function
			/!\ DO NOT OVERLOAD :!\
		*/
		friend void actualize_entry(jgl::Keyboard& keyboard, jgl::Glyph entry);

		/*
			Keyboard constructor
		*/
		Keyboard();

		/*
			Returns the status of the desired key
		*/
		const jgl::Input_status& get_key(jgl::Key p_key) const { return (_data[static_cast<jgl::Int>(p_key)]); }

		/*
			Return the text entry of the user as a single glyph
		*/
		const jgl::Glyph& get_text_entry() const { return (_char_entry); }

		/*
			Returns the name of the key [p_key]
		*/
		static const jgl::String& key_name(jgl::Key p_key) { return (_input_name[p_key]); }
		/*
			Returns the name of the key [p_key] (as jgl::Int -> Will be casted)
			If the value isn't in acceptable range, return "Unknow input"
		*/
		static const jgl::String& key_name(jgl::Int p_key) { if (p_key >= static_cast<jgl::Int>(jgl::Key::Backspace) && p_key <= static_cast<jgl::Int>(jgl::Key::Process))return (_input_name[static_cast<jgl::Key>(p_key)]); else return (_unknow_key); }
	};
}