#pragma once

#include "jgl_includes.h"
#include "jgl_basic_types.h"
#include "structure/jgl_vector.h"
#include "structure/jgl_exception.h"
#include "structure/jgl_array.h"
#include "structure/jgl_string.h"

namespace jgl
{
	void check_opengl(jgl::String msg);

	/*
		Initialize internal data for jgl
	*/
	jgl::Int start();

	/*
		Release data created by the start() function
	*/
	jgl::Int stop();

	/*
		Convert an angle (in degree) to an angle (in radian)
	*/
	jgl::Float degree_to_radian(const jgl::Float angle);

	/*
		Convert an angle (in radian) to an angle (in degree)
	*/
	jgl::Float radian_to_degree(const jgl::Float radian);

	/*
		Return the floor value of the float [value]
	*/
	jgl::Int floor(jgl::Float value);

	/*
		Return the round value of the float [value]
	*/
	jgl::Int round(jgl::Float value);

	/*
		Return the ceiling value of the float [value]
	*/
	jgl::Int ceil(jgl::Float value);

	/*
		Add at the beggining of the string [str] the glyph [c] until the resulting string reach a size of [size] glyph than return it
	*/
	jgl::String normalize_string(const jgl::String& str, const jgl::Glyph c, const jgl::Size_t size);

	/*
		return the string equivalent of the jgl::Int [num], with a minimal len of [size] glyph.
		If the string is too small, add the glyph [c] at the beggining until it reach [size] glyph in total
	*/
	jgl::String normalize_int(const jgl::Int num, const jgl::Glyph c, const jgl::Size_t size);

	/*
		return the string equivalent of the jgl::Uint [num], with a minimal len of [size] glyph.
		If the string is too small, add the glyph [c] at the beggining until it reach [size] glyph in total
	*/
	jgl::String normalize_uint(const jgl::Uint num, const jgl::Glyph c, const jgl::Size_t size);

	/*
		return the string equivalent of the jgl::Float [num], with a minimal len of [size] glyph.
		If the string is too small, add the glyph [c] at the beggining until it reach [size] glyph in total
	*/
	jgl::String normalize_float(const jgl::Float num, const jgl::Int after_point, const jgl::Glyph c, const jgl::Size_t size);

	/*
		Return the reverse version of the string
	*/
	void reverse(jgl::String& base);

	/*
		Remove every char contain in [src] who can be found in [to_remove]
	*/
	void remove_char(jgl::String& src, jgl::String& to_remove);

	/*
		Convert a jgl::Int into a string
	*/
	jgl::String itoa(const jgl::Int x, const jgl::Int d = 0);

	/*
		Convert a jgl::Float into a string
	*/
	jgl::String ftoa(const jgl::Float n, const jgl::Int afterpoint = 2, const jgl::Int length = -1);

	/*
		Convert a jgl::Double into a string
	*/
	jgl::String ftoa(const jgl::Double n, const jgl::Int afterpoint = 2, const jgl::Int length = -1);

	/*
		Convert a string to a jgl::Float value
	*/
	jgl::Float stof(jgl::String text);

	/*
		Convert a string to an jgl::Int value
	*/
	jgl::Int stoi(jgl::String text);

	/*
		Check if [value] is between [min] and [max]. Return true if yes, false if not
	*/
	jgl::Bool is_middle(const jgl::Int min, const jgl::Int value, const jgl::Int max);

	/*
		Check if [value] is between [min] and [max]. Return true if yes, false if not
	*/
	jgl::Bool is_middle(const jgl::Float min, const jgl::Float value, const jgl::Float max);

	/*
		Check if [value] is between [min] and [max]. Return true if yes, false if not
	*/
	jgl::Bool is_middle(const jgl::Vector2 min, const jgl::Vector2 value, const jgl::Vector2 max);

	/*
		Check if [value] is between [min] and [max]. Return true if yes, false if not
	*/
	jgl::Bool is_middle(const jgl::Vector3 min, const jgl::Vector3 value, const jgl::Vector3 max);

	/*
		Generate a pseudo-random value from a seed, with two values [x] and [y].
		If the seed is the same, will always return the same value
	*/
	jgl::Int generate_nbr_from_2D(const jgl::Long seed, const jgl::Int x, const jgl::Int y);

	/*
		Return random value from [min] to [max]
	*/
	jgl::Int generate_nbr(const jgl::Int min, const jgl::Int max);

	/*
		Return a string corresponding to the textual name of the GLenum [type]
	*/
	jgl::String glGetTypeString(GLenum type);

	/*
		Convert a screen coord (in pixel) to an OpenGL coord (from -1 to 1 in both axis)
	*/
	Vector3 convert_screen_to_opengl(const Vector2Int source, jgl::Float level);

	/*
		Convert a OpenGL coord (from -1 to 1 in both axis) to a screen coord (in pixel)
	*/
	Vector2Int convert_opengl_to_screen(const Vector2Int source);
}