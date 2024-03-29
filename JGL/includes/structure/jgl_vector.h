#pragma once

#include "jgl_includes.h"
#include "jgl_basic_types.h"
#include "jgl_exception.h"

namespace jgl
{
	template <typename T>
	void clamp(const T& p_min, T& p_value, const T& p_max);

	float degree_to_radian(const float angle);
	float radian_to_degree(const float radian);
	jgl::String ftoa(const jgl::Float n, const jgl::Int afterpoint, const jgl::Int length);

	template <typename T, typename std::enable_if<std::is_arithmetic<T>::value == true>::type* = nullptr>
	struct Vector2_base
	{
		T x;
		T y;

		Vector2_base()
		{
			x = {};
			y = {};
		}
		Vector2_base(const T& value)
		{
			x = value;
			y = value;
		}
		Vector2_base(const T& p_x, const T& p_y)
		{
			x = p_x;
			y = p_y;
		}

		//Convert non-float to float
		template <typename U = T, typename std::enable_if<std::is_floating_point<U>::value == false>::type* = nullptr>
		operator Vector2_base<jgl::Float>() const
		{
			return (Vector2_base<jgl::Float>(static_cast<jgl::Float>(x), static_cast<jgl::Float>(y)));
		}
		//Convert float to int
		template <typename U = T, typename std::enable_if<std::is_floating_point<U>::value == true>::type* = nullptr>
		operator Vector2_base<jgl::Int>() const
		{
			return (Vector2_base<jgl::Int>(static_cast<jgl::Int>(x), static_cast<jgl::Int>(y)));
		}
		//Convert float to uint
		template <typename U = T, typename std::enable_if<std::is_floating_point<U>::value == true>::type* = nullptr>
		operator Vector2_base<jgl::Uint>() const
		{
			return (Vector2_base<jgl::Uint>(static_cast<jgl::Uint>(x), static_cast<jgl::Uint>(y)));
		}

		//Convert non-float and signed to unsigned
		template <typename U = T, typename std::enable_if<std::is_floating_point<U>::value == false>::type* = nullptr, typename std::enable_if<std::is_unsigned<U>::value == false>::type* = nullptr>
		operator Vector2_base<jgl::Uint>() const
		{
			return (Vector2_base<jgl::Uint>(static_cast<jgl::Uint>(x), static_cast<jgl::Uint>(y)));
		}
		//Convert non-float and unsigned to signed
		template <typename U = T, typename std::enable_if<std::is_floating_point<U>::value == false>::type* = nullptr, typename std::enable_if<std::is_unsigned<U>::value == true>::type* = nullptr>
		operator Vector2_base<jgl::Int>() const
		{
			return (Vector2_base<jgl::Int>(static_cast<jgl::Int>(x), static_cast<jgl::Int>(y)));
		}

		template <typename U = T, typename std::enable_if<std::is_floating_point<U>::value == true>::type* = nullptr>
		jgl::Int ix() const {
			return (static_cast<jgl::Int>(x));
		}
		template <typename U = T, typename std::enable_if<std::is_floating_point<U>::value == true>::type* = nullptr>
		jgl::Int iy() const {
			return (static_cast<jgl::Int>(y));
		}

		template <typename U = T, typename std::enable_if<std::is_unsigned<U>::value == true>::type* = nullptr>
		jgl::Int ix() const {
			return (static_cast<jgl::Int>(x));
		}
		template <typename U = T, typename std::enable_if<std::is_unsigned<U>::value == true>::type* = nullptr>
		jgl::Int iy() const {
			return (static_cast<jgl::Int>(y));
		}

		template <typename U = T, typename std::enable_if<std::is_floating_point<U>::value == true>::type* = nullptr>
		jgl::Uint ux() const {
			return (static_cast<jgl::Int>(x));
		}
		template <typename U = T, typename std::enable_if<std::is_floating_point<U>::value == true>::type* = nullptr>
		jgl::Uint uy() const {
			return (static_cast<jgl::Int>(y));
		}

		template <typename U = T, typename std::enable_if<std::is_signed<U>::value == true>::type* = nullptr>
		jgl::Uint ux() const {
			return (static_cast<jgl::Int>(x));
		}
		template <typename U = T, typename std::enable_if<std::is_signed<U>::value == true>::type* = nullptr>
		jgl::Uint uy() const {
			return (static_cast<jgl::Int>(y));
		}

		const T* decompose() const
		{
			return (&x);
		}
		jgl::Float length() const
		{
			return (sqrt(pow(x, 2.0f)
				+ pow(y, 2.0f)));
		}
		jgl::Float distance(const Vector2_base& other) const
		{
			return (std::sqrt(pow(other.x - x, 2) + pow(other.y - y, 2)));
		}
		Vector2_base<jgl::Float> normalize() const
		{
			jgl::Float tmp_length;

			tmp_length = length();

			/*
			std::cout << "Length : " << length << std::endl;
			std::cout << "X : " << static_cast<jgl::Float>(x) / length << std::endl;
			std::cout << "Y : " << static_cast<jgl::Float>(y) / length << std::endl;
			*/
			if (tmp_length == 0)
				THROW_EXCEPTION(jgl::Error_level::Error, -3, "Trying to divide by 0");
			return (Vector2_base<jgl::Float>(static_cast<jgl::Float>(x) / tmp_length, static_cast<jgl::Float>(y) / tmp_length));
		}
		Vector2_base cross(const Vector2_base other = 0) const
		{
			Vector2_base result;

			result = Vector2_base(-(other.y - y), other.x - x);

			return (result);
		}
		jgl::Float dot(const Vector2_base other, const Vector2_base center = 0) const
		{
			jgl::Float result;

			result = (x - center.x) * (other.x - center.x) + (y - center.y) * (other.y - center.y);

			return (result);
		}
		jgl::Float angle(const Vector2_base other, const Vector2_base center = 0) const
		{
			jgl::Float rdot = dot(other, center);

			rdot = (rdot < -1.0f ? -1.0f : (rdot > 1.0f ? 1.0f : rdot));

			jgl::Float angle = std::acos(rdot);

			return (jgl::radian_to_degree(angle));
		}
		jgl::Float angle() const
		{
			jgl::Float degree = jgl::radian_to_degree(std::atan(static_cast<jgl::Float>(y) / static_cast<jgl::Float>(x)));

			if (x >= 0 && y >= 0)
				return (degree);
			else if (x >= 0 && y < 0)
				return (360 + degree);
			else if (x < 0 && y >= 0)
				return (180 + degree);
			else
				return (180 + degree);
		}
		Vector2_base<T> invert() const
		{
			return (Vector2_base<T>(x * -1, y * -1));
		}
		Vector2_base rotate(Vector2_base center, jgl::Float angle) const
		{
			float theta = jgl::degree_to_radian(angle);

			float pcos = cos(theta);
			float psin = sin(theta);

			jgl::Vector2_base<T> result;
			result.x = (x - center.x) * pcos - (y - center.y) * psin;
			result.y = (x - center.x) * psin + (y - center.y) * pcos;

			return (result + center);
		}

		Vector2_base<T> operator + (const Vector2_base& other) const
		{
			return (Vector2_base<T>(x + other.x, y + other.y));
		}
		Vector2_base<T> operator - (const Vector2_base& other) const
		{
			return (Vector2_base<T>(x - other.x, y - other.y));
		}
		Vector2_base<T> operator * (const Vector2_base& other) const
		{
			return (Vector2_base<T>(x * other.x, y * other.y));
		}
		Vector2_base<T> operator / (const Vector2_base& other) const
		{
			if (other.x == 0 || other.y == 0)
				THROW_EXCEPTION(jgl::Error_level::Error, -3, "Trying to divide by 0");
			return (Vector2_base<T>(x / other.x, y / other.y));
		}
		void operator += (const Vector2_base& other)
		{
			x += other.x;
			y += other.y;
		}
		void operator -= (const Vector2_base& other)
		{
			x -= other.x;
			y -= other.y;
		}
		void operator *= (const Vector2_base& other)
		{
			x *= other.x;
			y *= other.y;
		}
		void operator /= (const Vector2_base& other)
		{
			if (other.x == 0 || other.y == 0)
				THROW_EXCEPTION(jgl::Error_level::Error, -3, "Trying to divide by 0");
			x /= other.x;
			y /= other.y;
		}

		template <typename U = T, typename std::enable_if<!std::is_floating_point<U>::value == true>::type* = nullptr>
		Vector2_base<T> operator % (const Vector2_base& delta) const {
			return (Vector2_base<T>(
				x % delta.x,
				y % delta.y)
				);
		}

		bool operator == (const Vector2_base& delta) const
		{
			return ((x == delta.x && y == delta.y) ? true : false);
		}
		bool operator != (const Vector2_base& delta) const
		{
			return ((x == delta.x && y == delta.y) ? false : true);
		}
		bool operator < (const Vector2_base& other) const
		{
			if (y < other.y)
				return (true);
			if (y == other.y && x < other.x)
				return (true);
			return (false);
		}
		bool operator > (const Vector2_base& other) const
		{
			if (y < other.y || x < other.x)
				return (false);
			return (true);
		}

		jgl::String text() const
		{
			jgl::String tmp;

			tmp = jgl::ftoa(x) + " / " + jgl::ftoa(y);
			return (tmp);
		}

		friend std::ostream& operator<<(std::ostream& os, const Vector2_base<T>& point)
		{
			os << point.x << " / " << point.y;
			return os;
		}
		template <typename U = T, typename std::enable_if<std::is_floating_point<U>::value == true>::type* = nullptr>
		Vector2_base<T> floor()
		{
			return (Vector2_base<T>(std::floor(x), std::floor(y)));
		}
		template <typename U = T, typename std::enable_if<std::is_floating_point<U>::value == true>::type* = nullptr>
		Vector2_base<T> ceiling()
		{
			return (Vector2_base<T>(std::ceil(x), std::ceil(y)));
		}
		template <typename U = T, typename std::enable_if<std::is_floating_point<U>::value == true>::type* = nullptr>
		Vector2_base<T> round()
		{
			return (Vector2_base<T>(std::round(x), std::round(y)));
		}
		bool is_middle(const Vector2_base& min, const Vector2_base& max) const
		{
			return (x >= min.x && x <= max.x &&
				y >= min.y && y <= max.y);
		}

		static Vector2_base<T> compose_min(const Vector2_base& v1, const Vector2_base& v2)
		{
			return (Vector2_base<T>(
				v1.x < v2.x ? v1.x : v2.x,
				v1.y < v2.y ? v1.y : v2.y
			));
		}

		static Vector2_base<T> compose_max(const Vector2_base& v1, const Vector2_base& v2)
		{
			return (Vector2_base<T>(
				v1.x > v2.x ? v1.x : v2.x,
				v1.y > v2.y ? v1.y : v2.y
			));
		}

		Vector2_base<T> clamp(const Vector2_base<T>& p_min, const Vector2_base<T>& p_max)
		{
			jgl::Vector2_base<T> result = *this;

			jgl::clamp<T>(p_min.x, result.x, p_max.x);
			jgl::clamp<T>(p_min.y, result.y, p_max.y);

			return (result);
		}
	};

	template <typename T, typename std::enable_if<std::is_arithmetic<T>::value>::type* = nullptr>
	struct Vector3_base
	{
		T x;
		T y;
		T z;

		Vector3_base()
		{
			x = {};
			y = {};
			z = {};
		}
		Vector3_base(T value)
		{
			x = value;
			y = value;
			z = value;
		}
		Vector3_base(T p_x, T p_y, T p_z)
		{
			x = p_x;
			y = p_y;
			z = p_z;
		}

		static Vector3_base up() { return (Vector3_base(0, 1, 0)); }
		static Vector3_base right() { return (Vector3_base(0, 0, 1)); }
		static Vector3_base front() { return (Vector3_base(1, 0, 0)); }
		static Vector3_base zero() { return (0); }

		template <typename U = T, typename std::enable_if<std::is_floating_point<U>::value == true>::type* = nullptr>
		jgl::Int ix() const {
			return (static_cast<jgl::Int>(x));
		}
		template <typename U = T, typename std::enable_if<std::is_floating_point<U>::value == true>::type* = nullptr>
		jgl::Int iy() const {
			return (static_cast<jgl::Int>(y));
		}
		template <typename U = T, typename std::enable_if<std::is_floating_point<U>::value == true>::type* = nullptr>
		jgl::Int iz() const {
			return (static_cast<jgl::Int>(z));
		}

		template <typename U = T, typename std::enable_if<std::is_unsigned<U>::value == true>::type* = nullptr>
		jgl::Int ix() const {
			return (static_cast<jgl::Int>(x));
		}
		template <typename U = T, typename std::enable_if<std::is_unsigned<U>::value == true>::type* = nullptr>
		jgl::Int iy() const {
			return (static_cast<jgl::Int>(y));
		}
		template <typename U = T, typename std::enable_if<std::is_unsigned<U>::value == true>::type* = nullptr>
		jgl::Int iz() const {
			return (static_cast<jgl::Int>(z));
		}

		template <typename U = T, typename std::enable_if<std::is_signed<U>::value == true>::type* = nullptr>
		jgl::Uint ix() const {
			return (static_cast<jgl::Uint>(x));
		}
		template <typename U = T, typename std::enable_if<std::is_signed<U>::value == true>::type* = nullptr>
		jgl::Uint iy() const {
			return (static_cast<jgl::Uint>(y));
		}
		template <typename U = T, typename std::enable_if<std::is_signed<U>::value == true>::type* = nullptr>
		jgl::Uint iz() const {
			return (static_cast<jgl::Uint>(z));
		}

		const T* decompose() const
		{
			return (&x);
		}
		jgl::Float length() const
		{
			return (sqrt(pow(x, 2.0f)
				+ pow(y, 2.0f)
				+ pow(z, 2.0f)));
		}
		jgl::Float distance(const Vector3_base& other) const
		{
			return (std::sqrt(pow(other.x - x, 2) + pow(other.y - y, 2) + pow(other.z - z, 2)));
		}
		Vector3_base normalize() const
		{
			float		length;

			length = sqrt((x * x)
				+ (y * y)
				+ (z * z));

			if (length == 0)
				THROW_EXCEPTION(jgl::Error_level::Error, -3, "Trying to divide by 0");
			return (Vector3_base(x / length, y / length, z / length));
		}
		Vector3_base cross(const Vector3_base& other) const //Produit vectoriel / cross product
		{
			Vector3_base	result = Vector3_base(
				y * other.z - z * other.y,
				z * other.x - x * other.z,
				x * other.y - y * other.x
			);

			return (result);
		}
		jgl::Float dot(const Vector3_base& other, const Vector3_base& center = 0) const
		{
			float		result;

			result = (x - center.x) * (other.x - center.x)
				+ (y - center.y) * (other.y - center.y)
				+ (z - center.z) * (other.z - center.z);

			return (result);
		}
		jgl::Float angle(const Vector3_base& other, const Vector3_base center = 0) const
		{
			jgl::Float rdot = dot(other, center);

			rdot = (rdot < -1.0f ? -1.0f : (rdot > 1.0f ? 1.0f : rdot));

			jgl::Float angle = std::acos(rdot);

			return (jgl::radian_to_degree(angle));
		}
		Vector3_base invert() const
		{
			return (Vector3_base(x * -1, y * -1, z * -1));
		}
		bool is_middle(const Vector3_base& min, const Vector3_base& max) const
		{
			return (x >= min.x && x <= max.x &&
				y >= min.y && y <= max.y &&
				z >= min.z && z <= max.z);
		}

		Vector3_base operator + (const Vector3_base& other) const
		{
			return (Vector3_base(x + other.x, y + other.y, z + other.z));
		}
		Vector3_base operator - (const Vector3_base& other) const
		{
			return (Vector3_base(x - other.x, y - other.y, z - other.z));
		}
		Vector3_base operator * (const Vector3_base& other) const
		{
			return (Vector3_base(x * other.x, y * other.y, z * other.z));
		}
		Vector3_base operator / (const Vector3_base& other) const
		{
			if (other.x == 0 || other.y == 0 || other.z == 0)
				THROW_EXCEPTION(jgl::Error_level::Error, -3, "Trying to divide by 0");
			return (Vector3_base(x / other.x, y / other.y, z / other.z));
		}
		void operator += (const Vector3_base& other)
		{
			x += other.x;
			y += other.y;
			z += other.z;
		}
		void operator -= (const Vector3_base& other)
		{
			x -= other.x;
			y -= other.y;
			z -= other.z;
		}
		void operator *= (const Vector3_base& other)
		{
			x *= other.x;
			y *= other.y;
			z *= other.z;
		}
		void operator /= (const Vector3_base& other)
		{
			if (other.x == 0 || other.y == 0 || other.z == 0)
				THROW_EXCEPTION(jgl::Error_level::Error, -3, "Trying to divide by 0");
			x /= other.x;
			y /= other.y;
			z /= other.z;
		}

		template <typename U = T, typename std::enable_if<!std::is_floating_point<U>::value == true>::type* = nullptr>
		Vector3_base operator % (const Vector3_base& delta) const {
			return (Vector3_base(
				x % delta.x,
				y % delta.y,
				z % delta.z)
				);
		}

		bool operator == (const Vector3_base& delta) const {
			return ((x == delta.x && y == delta.y && z == delta.z) ?
				true : false);
		}
		bool operator != (const Vector3_base& delta) const {
			return ((x == delta.x && y == delta.y && z == delta.z) ?
				false : true);
		}
		bool operator < (const Vector3_base& other) const
		{
			if (z < other.z)
				return (true);
			if (z == other.z && y < other.y)
				return (true);
			if (z == other.z && y == other.y && x < other.x)
				return (true);
			return (false);
		}
		bool operator > (const Vector3_base& other) const
		{
			if (z < other.z || y < other.y || x < other.x)
				return (false);
			return (true);
		}

		jgl::String text() const
		{
			jgl::String tmp;

			tmp = jgl::ftoa(x) + " / " + jgl::ftoa(y) + " / " + jgl::ftoa(z);
			return (tmp);
		}

		friend std::ostream& operator<<(std::ostream& os, const Vector3_base<T>& point)
		{
			os << point.x << " / " << point.y << " / " << point.z;
			return os;
		}

		template <typename U = T, typename std::enable_if<std::is_floating_point<U>::value == true>::type* = nullptr>
		Vector3_base floor()
		{
			return (Vector3_base<T>(std::floor(x), std::floor(y), std::floor(z)));
		}
		template <typename U = T, typename std::enable_if<std::is_floating_point<U>::value == true>::type* = nullptr>
		Vector3_base ceiling()
		{
			return (Vector3_base<T>(std::ceil(x), std::ceil(y), std::ceil(z)));
		}
		template <typename U = T, typename std::enable_if<std::is_floating_point<U>::value == true>::type* = nullptr>
		Vector3_base round()
		{
			return (Vector3_base<T>(std::round(x), std::round(y), std::round(z)));
		}

		static Vector3_base compose_min(const Vector3_base& v1, const Vector3_base& v2)
		{
			return (Vector3_base(
				v1.x < v2.x ? v1.x : v2.x,
				v1.y < v2.y ? v1.y : v2.y,
				v1.z < v2.z ? v1.z : v2.z
			));
		}

		static Vector3_base compose_max(const Vector3_base& v1, const Vector3_base& v2)
		{
			return (Vector3_base(
				v1.x > v2.x ? v1.x : v2.x,
				v1.y > v2.y ? v1.y : v2.y,
				v1.z > v2.z ? v1.z : v2.z
			));
		}
	};

	template <typename T, typename std::enable_if<std::is_arithmetic<T>::value == true>::type* = nullptr>
	struct Vector4_base
	{
		T x;
		T y;
		T z;
		T w;

		Vector4_base()
		{
			x = {};
			y = {};
			z = {};
			w = {};
		}
		Vector4_base(T value)
		{
			x = value;
			y = value;
			z = value;
			w = value;
		}
		Vector4_base(T p_x, T p_y, T p_z, T p_w)
		{
			x = p_x;
			y = p_y;
			z = p_z;
			w = p_w;
		}

		static Vector4_base up() { return (Vector4_base(0, 1, 0)); }
		static Vector4_base right() { return (Vector4_base(0, 0, 1)); }
		static Vector4_base front() { return (Vector4_base(1, 0, 0)); }
		static Vector4_base zero() { return (0); }

		template <typename U = T, typename std::enable_if<std::is_floating_point<U>::value == true>::type* = nullptr>
		jgl::Int ix() const {
			return (static_cast<jgl::Int>(x));
		}
		template <typename U = T, typename std::enable_if<std::is_floating_point<U>::value == true>::type* = nullptr>
		jgl::Int iy() const {
			return (static_cast<jgl::Int>(y));
		}
		template <typename U = T, typename std::enable_if<std::is_floating_point<U>::value == true>::type* = nullptr>
		jgl::Int iz() const {
			return (static_cast<jgl::Int>(z));
		}
		template <typename U = T, typename std::enable_if<std::is_floating_point<U>::value == true>::type* = nullptr>
		jgl::Int iw() const {
			return (static_cast<jgl::Int>(w));
		}

		const T* decompose() const
		{
			return (&x);
		}
		jgl::Float length() const
		{
			return (sqrt(pow(x, 2.0f)
				+ pow(y, 2.0f)
				+ pow(z, 2.0f)));
		}
		jgl::Float distance(const Vector4_base& point) const
		{
			return (sqrt(pow(point.x - x, 2.0f)
				+ pow(point.y - y, 2.0f)
				+ pow(point.z - z, 2.0f)
			));
		}
		Vector4_base normalize() const
		{
			float		length;

			length = sqrt((x * x)
				+ (y * y)
				+ (z * z));

			if (length == 0)
				THROW_EXCEPTION(jgl::Error_level::Error, -3, "Trying to divide by 0");
			return (Vector4_base(x / length, y / length, z / length, 1.0f));
		}
		Vector4_base cross(const Vector4_base& other) const //Produit vectoriel / cross product
		{
			return (Vector4_base(
				y * other.z - z * other.y,
				z * other.x - x * other.z,
				x * other.y - y * other.x,
				1.0f
			));
		}
		jgl::Float dot(const Vector4_base& b) const
		{
			return (x * b.x + y * b.y + z * b.z);
		}
		jgl::Float angle(const Vector4_base& other, const Vector4_base& center = 0) const
		{
			jgl::Float rdot = dot(other, center);

			rdot = (rdot < -1.0f ? -1.0f : (rdot > 1.0f ? 1.0f : rdot));

			jgl::Float angle = std::acos(rdot);

			return (jgl::radian_to_degree(angle));
		}
		Vector4_base invert() const
		{
			return (Vector4_base(x * -1, y * -1, z * -1, w));
		}
		bool is_middle(const Vector4_base& min, const Vector4_base& max) const
		{
			return (x >= min.x && x <= max.x &&
				y >= min.y && y <= max.y &&
				z >= min.z && z <= max.z);
		}

		Vector4_base operator + (const Vector4_base& other) const
		{
			return (Vector4_base(x + other.x, y + other.y, z + other.z, w + other.w));
		}
		Vector4_base operator - (const Vector4_base& other) const
		{
			return (Vector4_base(x - other.x, y - other.y, z - other.z, w - other.w));
		}
		Vector4_base operator * (const Vector4_base& other) const
		{
			return (Vector4_base(x * other.x, y * other.y, z * other.z, w * other.w));
		}
		Vector4_base operator / (const Vector4_base& other) const
		{
			if (other.x == 0 || other.y == 0 || other.z == 0 || other.w == 0)
				THROW_EXCEPTION(jgl::Error_level::Error, -3, "Trying to divide by 0");
			return (Vector4_base(x / other.x, y / other.y, z / other.z, w / other.w));
		}
		void operator += (const Vector4_base& other)
		{
			x += other.x;
			y += other.y;
			z += other.z;
			w += other.w;
		}
		void operator -= (const Vector4_base& other)
		{
			x -= other.x;
			y -= other.y;
			z -= other.z;
			w -= other.w;
		}
		void operator *= (const Vector4_base& other)
		{
			x *= other.x;
			y *= other.y;
			z *= other.z;
			w *= other.w;
		}
		void operator /= (const Vector4_base& other)
		{
			if (other.x == 0 || other.y == 0 || other.z == 0 || other.w == 0)
				THROW_EXCEPTION(jgl::Error_level::Error, -3, "Trying to divide by 0");
			x /= other.x;
			y /= other.y;
			z /= other.z;
			w /= other.w;
		}

		template <typename U = T, typename std::enable_if<std::is_floating_point<U>::value == false>::type* = nullptr>
		Vector4_base operator % (const Vector4_base& delta) const {
			return (Vector4_base(
				x % delta.x,
				y % delta.y,
				z % delta.z,
				w % delta.w)
				);
		}

		bool operator == (const Vector4_base& delta) const {
			return ((x == delta.x && y == delta.y && z == delta.z) ?
				true : false);
		}
		bool operator != (const Vector4_base& delta) const {
			return ((x == delta.x && y == delta.y && z == delta.z) ?
				false : true);
		}
		bool operator < (const Vector4_base& other) const
		{
			if (z < other.z)
				return (true);
			if (z == other.z && y < other.y)
				return (true);
			if (z == other.z && y == other.y && x < other.x)
				return (true);
			return (false);
		}
		bool operator > (const Vector4_base& other) const
		{
			if (z < other.z || y < other.y || x < other.x)
				return (false);
			return (true);
		}

		jgl::String text() const
		{
			jgl::String tmp;

			tmp = jgl::ftoa(x) + " / " + jgl::ftoa(y) + " / " + jgl::ftoa(z) + " / " + jgl::ftoa(w);
			return (tmp);
		}

		friend std::ostream& operator<<(std::ostream& os, const Vector4_base<T>& point)
		{
			os << point.x << " / " << point.y << " / " << point.z << " / " << point.w;
			return os;
		}

		template <typename U = T, typename std::enable_if<std::is_floating_point<U>::value == true>::type* = nullptr>
		Vector4_base floor() const
		{
			return (Vector4_base<T>(std::floor(x), std::floor(y), std::floor(z), std::floor(w)));
		}
		template <typename U = T, typename std::enable_if<std::is_floating_point<U>::value == true>::type* = nullptr>
		Vector4_base ceiling() const
		{
			return (Vector4_base<T>(std::ceil(x), std::ceil(y), std::ceil(z), std::ceil(w)));
		}
		template <typename U = T, typename std::enable_if<std::is_floating_point<U>::value == true>::type* = nullptr>
		Vector4_base round() const
		{
			return (Vector4_base<T>(std::round(x), std::round(y), std::round(z), std::round(w)));
		}

		static Vector4_base compose_min(const Vector4_base& v1, const Vector4_base& v2)
		{
			return (Vector4_base(
				v1.x < v2.x ? v1.x : v2.x,
				v1.y < v2.y ? v1.y : v2.y,
				v1.z < v2.z ? v1.z : v2.z,
				v1.w < v2.w ? v1.w : v2.w
			));
		}

		static Vector4_base compose_max(const Vector4_base& v1, const Vector4_base& v2)
		{
			return (Vector4_base(
				v1.x > v2.x ? v1.x : v2.x,
				v1.y > v2.y ? v1.y : v2.y,
				v1.z > v2.z ? v1.z : v2.z,
				v1.w > v2.w ? v1.w : v2.w
			));
		}
	};

	using Vector2 = Vector2_base<jgl::Float>;
	using Vector2Int = Vector2_base<jgl::Int>;
	using Vector2Uint = Vector2_base<jgl::Uint>;
	using Vector3 = Vector3_base<jgl::Float>;
	using Vector3Int = Vector3_base<jgl::Int>;
	using Vector3Uint = Vector3_base<jgl::Uint>;
	using Vector4 = Vector4_base<jgl::Float>;
	using Vector4Int = Vector4_base<jgl::Int>;
	using Vector4Uint = Vector4_base<jgl::Uint>;
};