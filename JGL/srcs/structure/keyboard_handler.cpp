#include "jgl.h"

namespace jgl
{
    jgl::String jgl::Keyboard::_unknow_key = "Unknow input";
	jgl::Map<jgl::Key, jgl::String> jgl::Keyboard::_input_name = {
        {jgl::Key::Backspace, "Backspace"},
        {jgl::Key::Tab, "Tab"},
        {jgl::Key::Clear, "Clear"},
        {jgl::Key::Return, "Return"},
        {jgl::Key::Shift, "Shift"},
        {jgl::Key::Control, "Control"},
        {jgl::Key::Alt, "Alt"},
        {jgl::Key::Pause, "Pause"},
        {jgl::Key::Capslock, "Capslock"},
        {jgl::Key::Escape, "Escape"},
        {jgl::Key::Convert, "Convert"},
        {jgl::Key::Non_convert, "Non_convert"},
        {jgl::Key::Accept, "Accept"},
        {jgl::Key::Mode_change, "Mode_change"},
        {jgl::Key::Space, "Space"},
        {jgl::Key::Prior, "Prior"},
        {jgl::Key::Next, "Next"},
        {jgl::Key::End, "End"},
        {jgl::Key::Home, "Home"},
        {jgl::Key::Left_arrow, "Left_arrow"},
        {jgl::Key::Up_arrow, "Up_arrow"},
        {jgl::Key::Right_arrow, "Right_arrow"},
        {jgl::Key::Down_arrow, "Down_arrow"},
        {jgl::Key::Select, "Select"},
        {jgl::Key::Print, "Print"},
        {jgl::Key::Execute, "Execute"},
        {jgl::Key::Snapshot, "Snapshot"},
        {jgl::Key::Insert, "Insert"},
        {jgl::Key::Delete, "Delete"},
        {jgl::Key::Help, "Help"},
        {jgl::Key::Key_0, "Key_0"},
        {jgl::Key::Key_1, "Key_1"},
        {jgl::Key::Key_2, "Key_2"},
        {jgl::Key::Key_3, "Key_3"},
        {jgl::Key::Key_4, "Key_4"},
        {jgl::Key::Key_5, "Key_5"},
        {jgl::Key::Key_6, "Key_6"},
        {jgl::Key::Key_7, "Key_7"},
        {jgl::Key::Key_8, "Key_8"},
        {jgl::Key::Key_9, "Key_9"},
        {jgl::Key::A, "A"},
        {jgl::Key::B, "B"},
        {jgl::Key::C, "C"},
        {jgl::Key::D, "D"},
        {jgl::Key::E, "E"},
        {jgl::Key::F, "F"},
        {jgl::Key::G, "G"},
        {jgl::Key::H, "H"},
        {jgl::Key::I, "I"},
        {jgl::Key::J, "J"},
        {jgl::Key::K, "K"},
        {jgl::Key::L, "L"},
        {jgl::Key::M, "M"},
        {jgl::Key::N, "N"},
        {jgl::Key::O, "O"},
        {jgl::Key::P, "P"},
        {jgl::Key::Q, "Q"},
        {jgl::Key::R, "R"},
        {jgl::Key::S, "S"},
        {jgl::Key::T, "T"},
        {jgl::Key::U, "U"},
        {jgl::Key::V, "V"},
        {jgl::Key::W, "W"},
        {jgl::Key::X, "X"},
        {jgl::Key::Y, "Y"},
        {jgl::Key::Z, "Z"},
        {jgl::Key::Left_windows, "Left_windows"},
        {jgl::Key::Right_windows, "Right_windows"},
        {jgl::Key::App, "App"},
        {jgl::Key::Sleep, "Sleep"},
        {jgl::Key::Numpad_0, "Numpad_0"},
        {jgl::Key::Numpad_1, "Numpad_1"},
        {jgl::Key::Numpad_2, "Numpad_2"},
        {jgl::Key::Numpad_3, "Numpad_3"},
        {jgl::Key::Numpad_4, "Numpad_4"},
        {jgl::Key::Numpad_5, "Numpad_5"},
        {jgl::Key::Numpad_6, "Numpad_6"},
        {jgl::Key::Numpad_7, "Numpad_7"},
        {jgl::Key::Numpad_8, "Numpad_8"},
        {jgl::Key::Numpad_9, "Numpad_9"},
        {jgl::Key::Numpad_multiply, "Numpad_multiply"},
        {jgl::Key::Numpad_plus, "Numpad_plus"},
        {jgl::Key::Numpad_separator, "Numpad_separator"},
        {jgl::Key::Numpad_minus, "Numpad_minus"},
        {jgl::Key::Numpad_decimal, "Numpad_decimal"},
        {jgl::Key::Numpad_divide, "Numpad_divide"},
        {jgl::Key::F1, "F1"},
        {jgl::Key::F2, "F2"},
        {jgl::Key::F3, "F3"},
        {jgl::Key::F4, "F4"},
        {jgl::Key::F5, "F5"},
        {jgl::Key::F6, "F6"},
        {jgl::Key::F7, "F7"},
        {jgl::Key::F8, "F8"},
        {jgl::Key::F9, "F9"},
        {jgl::Key::F10, "F10"},
        {jgl::Key::F11, "F11"},
        {jgl::Key::F12, "F12"},
        {jgl::Key::F13, "F13"},
        {jgl::Key::F14, "F14"},
        {jgl::Key::F15, "F15"},
        {jgl::Key::F16, "F16"},
        {jgl::Key::F17, "F17"},
        {jgl::Key::F18, "F18"},
        {jgl::Key::F19, "F19"},
        {jgl::Key::F20, "F20"},
        {jgl::Key::F21, "F21"},
        {jgl::Key::F22, "F22"},
        {jgl::Key::F23, "F23"},
        {jgl::Key::F24, "F24"},
        {jgl::Key::Num_lock, "Num_lock"},
        {jgl::Key::Scroll, "Scroll"},
        {jgl::Key::Left_shift, "Left_shift"},
        {jgl::Key::Right_shift, "Right_shift"},
        {jgl::Key::Left_control, "Left_control"},
        {jgl::Key::Right_control, "Right_control"},
        {jgl::Key::Left_menu, "Left_menu"},
        {jgl::Key::Right_menu, "Right_menu"},
        {jgl::Key::Semi_colon, "Semi_colon"},
        {jgl::Key::Plus, "Plus"},
        {jgl::Key::Comma, "Comma"},
        {jgl::Key::Minus, "Minus"},
        {jgl::Key::Period, "Period"},
        {jgl::Key::Question_mark, "Question_mark"},
        {jgl::Key::Tilde, "Tilde"},
        {jgl::Key::Left_bracket, "Left_bracket"},
        {jgl::Key::Vertical_line, "Vertical_line"},
        {jgl::Key::Right_bracket, "Right_bracket"},
        {jgl::Key::Quote, "Quote"},
        {jgl::Key::Unknow, "Unknow"},
        {jgl::Key::Angle_bracket, "Angle_bracket"},
        {jgl::Key::Process, "Process"},
	};

	jgl::Keyboard::Keyboard()
	{
		for (jgl::Size_t i = 0; i < KEY_COUNT; i++)
			_data[i] = jgl::Input_status::Up;
	}
}