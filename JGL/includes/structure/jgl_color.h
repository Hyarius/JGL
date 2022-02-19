#pragma once

#include "jgl_basic_types.h"

namespace jgl
{
	struct Color
	{
		jgl::Float r;
		jgl::Float g;
		jgl::Float b;
		jgl::Float a;

		/*
			Default constructor, set as black + fully transparent
		*/
		Color() : r(0), g(0), b(0), a(0) {}

		/*
			jgl::Int constructor
			Every color should be represented by an jgl::Int from 0 (none) to 255 (full)
			The color model is RGBA
		*/
		Color(const jgl::Int p_r, const jgl::Int p_g, const jgl::Int p_b, const jgl::Int p_a = 255) :
			r(static_cast<jgl::Float>(p_r % 256) / 255.0f),
			g(static_cast<jgl::Float>(p_g % 256) / 255.0f),
			b(static_cast<jgl::Float>(p_b % 256) / 255.0f),
			a(static_cast<jgl::Float>(p_a % 256) / 255.0f)
		{
		}

		/*
			jgl::Uint constructor
			Every color should be represented by an jgl::Uint from 0 (none) to 255 (full)
			The color model is RGBA
		*/
		Color(const jgl::Uint p_r, const jgl::Uint p_g, const jgl::Uint p_b, const jgl::Uint p_a = 255) : r(static_cast<jgl::Float>(p_r % 256) / 255.0f), g(static_cast<jgl::Float>(p_g % 256) / 255.0f), b(static_cast<jgl::Float>(p_b % 256) / 255.0f), a(static_cast<jgl::Float>(p_a % 256) / 255.0f) {}

		/*
			jgl::Float constructor
			Every color should be represented by an jgl::Float from 0 (none) to 1 (full)
			The color model is RGBA
		*/
		Color(const jgl::Float p_r, const jgl::Float p_g, const jgl::Float p_b, const jgl::Float p_a = 1.0f) : 
			r(p_r > 1.0f ? 1.0f : (p_r < 0 ? 0.0f : p_r)),
			g(p_g > 1.0f ? 1.0f : (p_g < 0 ? 0.0f : p_g)),
			b(p_b > 1.0f ? 1.0f : (p_b < 0 ? 0.0f : p_b)),
			a(p_a > 1.0f ? 1.0f : (p_a < 0 ? 0.0f : p_a)) {}

		/*
			Create a color defined to represent a white color
		*/
		static jgl::Color white() { return (jgl::Color(255, 255, 255)); }

		/*
			Create a color defined to represent a black color
		*/
		static jgl::Color black() { return (jgl::Color(0, 0, 0)); }

		/*
			Create a color defined to represent a full red color
		*/
		static jgl::Color red() { return (jgl::Color(255, 0, 0)); }

		/*
			Create a color defined to represent a full green color
		*/
		static jgl::Color green() { return (jgl::Color(0, 255, 0)); }

		/*
			Create a color defined to represent a full blue color
		*/
		static jgl::Color blue() { return (jgl::Color(0, 0, 255)); }

		/*
			Fuse two color together, returning the result of the operation
		*/
		Color operator + (const Color delta) const
		{
			Color	result;
			float	base_alpha;
			float	inv_alpha;

			base_alpha = delta.a;
			inv_alpha = 1.0f - base_alpha;
			result.r = delta.r * base_alpha + inv_alpha * this->r;
			result.g = delta.g * base_alpha + inv_alpha * this->g;
			result.b = delta.b * base_alpha + inv_alpha * this->b;
			result.a = delta.a * base_alpha + inv_alpha * this->a;

			return (result);
		}
		
		/*
			Print the color
		*/
		friend jgl::Stream& operator<<(jgl::Stream& os, const jgl::Color value)
		{
			os << jgl::itoa(value.r * 255, 0) << "/" << jgl::itoa(value.g * 255, 0) << "/" << jgl::itoa(value.b * 255, 0) << "/" << jgl::itoa(value.a * 255, 0);
			return os;
		}
	};
}