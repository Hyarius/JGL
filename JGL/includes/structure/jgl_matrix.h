#pragma once

#include "jgl_includes.h"
#include "jgl_vector.h"

namespace jgl
{
	enum class Matrix_type {
		rotation = 0,
		translation = 1,
		scale = 2,
		look_at = 3,
		count
	};

	struct Matrix2x2
	{
		jgl::Float value[3][3];

		/*
			Base matrix 2x2 constructor
			Matrix form :
			a0 / a1
			b0 / b1
		*/
		Matrix2x2(const jgl::Float a0 = 1.0F, const jgl::Float a1 = 0.0f,
				const jgl::Float b0 = 0.0f, const jgl::Float b1 = 1.0F)
		{
			value[0][0] = a0; value[1][0] = a1;
			value[0][1] = b0; value[1][1] = b1;
		}

		/*
			Calculate the determinant of the matrix
		*/
		jgl::Float determinant() const
		{
			return (value[0][0] * value[1][1] - value[1][0] * value[0][1]);
		}

		/*
			Returns the reversed matrix
			Input matrix form :
			a0 / a1
			b0 / b1

			Result matrix form :
			a0 / b0
			a1 / b1
		*/
		Matrix2x2 reverse() const
		{
			Matrix2x2 result;

			for (jgl::Uint i = 0; i < 2; i++)
				for (jgl::Uint j = 0; j < 2; j++)
				{
					result.value[i][j] = value[j][i];
				}
			return (result);
		}

		/*
			Return the inverse matrix
		*/
		Matrix2x2 inverse_matrix() const
		{
			jgl::Float tmp = determinant();
			return (Matrix2x2(
				value[1][1] / tmp, -(value[1][0] / tmp),
				-(value[0][1] / tmp), value[0][0] / tmp
			));
		}

		/*
			Matrix 2x2 x Matrix 2x2 multiplication operator
		*/
		Matrix2x2 operator * (const Matrix2x2 p_matrix) const
		{
			jgl::Int i;
			jgl::Int j;
			Matrix2x2	m = Matrix2x2();

			i = 0;
			while (i < 4)
			{
				j = 0;
				while (j < 4)
				{
					m.value[i][j] = (this->value[0][i] * p_matrix.value[j][0]) +
						(this->value[1][i] * p_matrix.value[j][1]);
					j++;
				}
				i++;
			}
			return (m);
		}

		/*
			Matrix 2x2 x Vector2 multiplication operator
		*/
		Vector2 operator * (const Vector2 vertex) const
		{
			return (Vector2(
				value[0][0] * vertex.x + value[1][0] * vertex.y,
				value[0][1] * vertex.x + value[1][1] * vertex.y
			));
		}
	};

	struct Matrix3x3
	{
		jgl::Float value[3][3];

		/*
			Default constructor
		*/
		Matrix3x3(const jgl::Float a0 = 1.0F, const jgl::Float a1 = 0.0f, const jgl::Float a2 = 0.0f,
			const jgl::Float b0 = 0.0f, const jgl::Float b1 = 1.0F, const jgl::Float b2 = 0.0f,
			const jgl::Float c0 = 0.0f, const jgl::Float c1 = 0.0f, const jgl::Float c2 = 1.0F)
		{
			value[0][0] = a0; value[1][0] = a1; value[2][0] = a2;
			value[0][1] = b0; value[1][1] = b1; value[2][1] = b2;
			value[0][2] = c0; value[1][2] = c1; value[2][2] = c2;
		}

		/*
			Returns the reversed matrix
			Input matrix form :
			a0 / a1 / a2
			b0 / b1 / b2
			c0 / c1 / c2

			Result matrix form :
			a0 / b0 / c0
			a1 / b1 / c1
			a2 / b2 / c2
		*/
		Matrix3x3 reverse() const
		{
			Matrix3x3 result;

			for (jgl::Uint i = 0; i < 3; i++)
				for (jgl::Uint j = 0; j < 3; j++)
				{
					result.value[i][j] = value[j][i];
				}
			return (result);
		}

		/*
			Compute the cofactor of the matrix and returns it
		*/
		jgl::Float cofactor(const jgl::Int x, const jgl::Int y) const
		{
			jgl::Float tmp_value[4]{ 1, 1, 1, 1 };

			jgl::Uint index = 0;

			for (jgl::Uint i = 0; i < 3; i++)
				for (jgl::Uint j = 0; j < 3; j++)
				{
					if (i != x && j != y)
					{
						tmp_value[index] = value[i][j];
						index++;
					}
				}
			Matrix2x2 tmp_matrix(tmp_value[0], tmp_value[1], tmp_value[2], tmp_value[3]);
			return (tmp_matrix.determinant());
		}

		/*
			Calculate the determinant of the matrix
		*/
		jgl::Float determinant() const
		{
			return (value[0][0] * cofactor(0, 0) - value[1][0] * cofactor(1, 0) + value[2][0] * cofactor(2, 0));
		}

		/*
			Compute the cofactor matrix and returns it
		*/
		Matrix3x3 cofactor_matrix() const
		{
			Matrix3x3 result = Matrix3x3(
				+cofactor(0, 0), -cofactor(1, 0), +cofactor(2, 0),
				-cofactor(0, 1), +cofactor(1, 1), -cofactor(2, 1),
				+cofactor(0, 2), -cofactor(1, 2), +cofactor(2, 2)
			);

			return (result);
		}

		/*
			Return the adjucate matrix
		*/
		Matrix3x3 adjucate_matrix() const
		{
			return (cofactor_matrix().reverse());
		}

		/*
			Return the inverse matrix
		*/
		Matrix3x3 inverse_matrix() const
		{
			Matrix3x3 tmp = adjucate_matrix();
			Matrix3x3 result = Matrix3x3();
			jgl::Float deter = determinant();

			if (deter != 0)
			{
				for (jgl::Uint i = 0; i < 3; i++)
					for (jgl::Uint j = 0; j < 3; j++)
					{
						result.value[i][j] = tmp.value[i][j] / deter;
					}
			}
			return (result);

			return (result);
		}

		/*
			Matrix 3x3 x Matrix 3x3 multiplication operator
		*/
		Matrix3x3 operator * (const Matrix3x3 p_matrix) const
		{
			jgl::Int i;
			jgl::Int j;
			Matrix3x3 m = Matrix3x3();

			i = 0;
			while (i < 4)
			{
				j = 0;
				while (j < 4)
				{
					m.value[i][j] = (this->value[0][i] * p_matrix.value[j][0])
						+ (this->value[1][i] * p_matrix.value[j][1])
						+ (this->value[2][i] * p_matrix.value[j][2]);
					j++;
				}
				i++;
			}
			return (m);
		}

		/*
			Matrix 3x3 x Vector3 multiplication operator
		*/
		Vector3 operator * (const Vector3 vertex) const
		{
			return (Vector3(
				value[0][0] * vertex.x + value[1][0] * vertex.y + value[2][0] * vertex.z,
				value[0][1] * vertex.x + value[1][1] * vertex.y + value[2][1] * vertex.z,
				value[0][2] * vertex.x + value[1][2] * vertex.y + value[2][2] * vertex.z
			));
		}
	};

	struct Matrix4x4
	{
		jgl::Float value[4][4];

		/*
			Create a rotation matrix around a custom axis, for [angle] value
		*/
		static Matrix4x4 matrix_custom_rotation(const jgl::Float angle, const Vector3 base_axis)
		{
			Vector3 axis = base_axis.normalize();
			jgl::Float u = axis.x;
			jgl::Float v = axis.y;
			jgl::Float w = axis.z;
			jgl::Float len = axis.length();
			jgl::Float root_len = sqrt(len);
			jgl::Float theta = degree_to_radian(angle);
			jgl::Float cos_theta = std::cos(theta);
			jgl::Float sin_theta = std::sin(theta);

			jgl::Float a0 = static_cast<jgl::Float>(pow(u, 2) + (pow(v, 2) + pow(w, 2)) * cos_theta);
			jgl::Float a1 = u * v * (1 - cos_theta) - w * root_len * sin_theta;
			jgl::Float a2 = u * w * (1 - cos_theta) + v * root_len * sin_theta;
			jgl::Float a3 = 0;

			jgl::Float b0 = v * u * (1 - cos_theta) + w * root_len * sin_theta;
			jgl::Float b1 = static_cast<jgl::Float>(pow(v, 2) + (pow(u, 2) + pow(w, 2)) * cos_theta);
			jgl::Float b2 = v * w * (1 - cos_theta) - u * root_len * sin_theta;
			jgl::Float b3 = 0;

			jgl::Float c0 = u * w * (1 - cos_theta) - v * root_len * sin_theta;
			jgl::Float c1 = v * w * (1 - cos_theta) + u * root_len * sin_theta;
			jgl::Float c2 = static_cast<jgl::Float>(pow(w, 2) + (pow(u, 2) + pow(v, 2)) * cos_theta);
			jgl::Float c3 = 0;

			jgl::Float d0 = 0;
			jgl::Float d1 = 0;
			jgl::Float d2 = 0;
			jgl::Float d3 = 1;

			Matrix4x4 result(
				a0, a1, a2, a3,
				b0, b1, b2, b3,
				c0, c1, c2, c3,
				d0, d1, d2, d3
			);

			for (jgl::Uint i = 0; i < 4; i++)
				for (jgl::Uint j = 0; j < 3; j++)
					result.value[i][j] /= len;

			return (result);
		}

		/*
			Create a matrix look at to the [target] point from [pos] origin

			Optional
			jgl::Float angle - Rotation around the Y axis
		*/
		static Matrix4x4 matrix_look_at(const Vector3 pos, const Vector3 target, const Vector3 up_dir, jgl::Float angle = 0)
		{
			Matrix4x4 result;

			Vector3 forward = (pos - target).normalize();
			Vector3 up = Vector3::up();
			if (forward.dot(up) == 1 || forward.dot(up) == -1)
				up = Vector3::front();
			Vector3 right = (up.cross(forward)).normalize();
			right = Matrix4x4::matrix_rotation_y(angle) * right;
			up = (forward.cross(right)).normalize();

			result.value[0][0] = right.x;
			result.value[1][0] = right.y;
			result.value[2][0] = right.z;
			result.value[3][0] = -(right.dot(pos) / 2);
			result.value[0][1] = up.x;
			result.value[1][1] = up.y;
			result.value[2][1] = up.z;
			result.value[3][1] = -(up.dot(pos) / 2);
			result.value[0][2] = forward.x;
			result.value[1][2] = forward.y;
			result.value[2][2] = forward.z;
			result.value[3][2] = -(forward.dot(pos) / 2);
			result.value[0][3] = 0;
			result.value[1][3] = 0;
			result.value[2][3] = 0;
			result.value[3][3] = 1;

			return (result);
		}


		/*
			Returns a rotation matrix around the X axis
		*/
		static Matrix4x4 matrix_rotation_x(const jgl::Float angle)
		{
			Matrix4x4 result = Matrix4x4();
			jgl::Float		radian = degree_to_radian(angle);
			result.value[0][0] = 1.0f; result.value[1][0] = 0.0f;       	result.value[2][0] = 0.0f; result.value[3][0] = 0;
			result.value[0][1] = 0.0f; result.value[1][1] = cos(radian); 	result.value[2][1] = sin(radian);	result.value[3][1] = 0;
			result.value[0][2] = 0.0f; result.value[1][2] = -sin(radian); 	result.value[2][2] = cos(radian);	result.value[3][2] = 0;
			result.value[0][3] = 0; result.value[1][3] = 0; result.value[2][3] = 0; result.value[3][3] = 1;
			return (result);
		}

		/*
			Returns a rotation matrix around the Y axis
		*/
		static Matrix4x4 matrix_rotation_y(const jgl::Float angle)
		{
			Matrix4x4 result = Matrix4x4();
			jgl::Float		radian = degree_to_radian(angle);

			result.value[0][0] = cos(radian);	result.value[1][0] = 0.0f; result.value[2][0] = -sin(radian);	result.value[3][0] = 0;
			result.value[0][1] = 0.0f;      	result.value[1][1] = 1.0f; result.value[2][1] = 0.0f; result.value[3][1] = 0;
			result.value[0][2] = sin(radian);	result.value[1][2] = 0.0f; result.value[2][2] = cos(radian);	result.value[3][2] = 0;
			result.value[0][3] = 0; result.value[1][3] = 0; result.value[2][3] = 0; result.value[3][3] = 1;
			return (result);
		}

		/*
			Returns a rotation matrix around the Z axis
		*/
		static Matrix4x4 matrix_rotation_z(const jgl::Float angle)
		{
			Matrix4x4 result = Matrix4x4();
			jgl::Float		radian = degree_to_radian(angle);

			result.value[0][0] = cos(radian);	result.value[1][0] = sin(radian);	result.value[2][0] = 0.0f; result.value[3][0] = 0;
			result.value[0][1] = -sin(radian);	result.value[1][1] = cos(radian);	result.value[2][1] = 0.0f; result.value[3][1] = 0;
			result.value[0][2] = 0.0f; result.value[1][2] = 0.0f; result.value[2][2] = 1.0f; result.value[3][2] = 0;
			result.value[0][3] = 0; result.value[1][3] = 0; result.value[2][3] = 0; result.value[3][3] = 1;
			return (result);
		}

	private:
		/*
			Compose a rotation matrix
		*/
		void _matrix_rotation(const jgl::Float x, const jgl::Float y, const jgl::Float z)
		{
			Matrix4x4 rotx = matrix_rotation_x(x);
			Matrix4x4 roty = matrix_rotation_y(y);
			Matrix4x4 rotz = matrix_rotation_z(z);

			*this = rotx * roty * rotz;
		}
		/*
			Compose a translation matrix
		*/
		void _matrix_translation(const jgl::Float x, const jgl::Float y, const jgl::Float z)
		{
			value[0][0] = 1.0f; value[1][0] = 0.0f; value[2][0] = 0.0f; value[3][0] = x;
			value[0][1] = 0.0f; value[1][1] = 1.0f; value[2][1] = 0.0f; value[3][1] = y;
			value[0][2] = 0.0f; value[1][2] = 0.0f; value[2][2] = 1.0f; value[3][2] = z;
			value[0][3] = 0; value[1][3] = 0; value[2][3] = 0; value[3][3] = 1;
		}
		/*
			Compose a scaling matrix
		*/
		void _matrix_scale(const jgl::Float x, const jgl::Float y, const jgl::Float z)
		{
			value[0][0] = x; value[1][0] = 0.0f; value[2][0] = 0.0f; value[3][0] = 0.0f;
			value[0][1] = 0.0f; value[1][1] = y; value[2][1] = 0.0f; value[3][1] = 0.0f;
			value[0][2] = 0.0f; value[1][2] = 0.0f; value[2][2] = z; value[3][2] = 0.0f;
			value[0][3] = 0; value[1][3] = 0; value[2][3] = 0; value[3][3] = 1;
		}

		public:

		/*
			Default constructor
		*/
		Matrix4x4(const jgl::Float a0 = 1.0F, const jgl::Float a1 = 0.0f, const jgl::Float a2 = 0.0f, const jgl::Float a3 = 0.0f,
			const jgl::Float b0 = 0.0f, const jgl::Float b1 = 1.0F, const jgl::Float b2 = 0.0f, const jgl::Float b3 = 0.0f,
			const jgl::Float c0 = 0.0f, const jgl::Float c1 = 0.0f, const jgl::Float c2 = 1.0F, const jgl::Float c3 = 0.0f,
			const jgl::Float d0 = 0.0f, const jgl::Float d1 = 0.0f, const jgl::Float d2 = 0.0f, const jgl::Float d3 = 1.0F)
		{
			value[0][0] = a0; value[1][0] = a1; value[2][0] = a2; value[3][0] = a3;
			value[0][1] = b0; value[1][1] = b1; value[2][1] = b2; value[3][1] = b3;
			value[0][2] = c0; value[1][2] = c1; value[2][2] = c2; value[3][2] = c3;
			value[0][3] = d0; value[1][3] = d1; value[2][3] = d2; value[3][3] = d3;
		}

		/*
			Rotation matrix constructor with 3 jgl::Float as descriptor for each rotation angle
		*/
		Matrix4x4(const Matrix_type type, const jgl::Float x, const jgl::Float y, const jgl::Float z)
		{
			if (type == Matrix_type::rotation)
				_matrix_rotation(x, y, z);
			else if (type == Matrix_type::translation)
				_matrix_translation(x, y, z);
			else if (type == Matrix_type::scale)
				_matrix_scale(x, y, z);
			else
				*this = Matrix4x4();
		}

		/*
			Rotation matrix constructor with jgl::Vector3 as angle
		*/
		Matrix4x4(const Matrix_type type, const Vector3 angle)
		{
			*this = Matrix4x4(type, angle.x, angle.y, angle.z);
		}

		/*
			Look at matrix constructor
		*/
		Matrix4x4(const Matrix_type type, const Vector3 pos, const Vector3 target, const Vector3 up)
		{
			if (type == Matrix_type::look_at)
				*this = matrix_look_at(pos, target, up);
			else
				*this = Matrix4x4();
		}


		/*
			Returns the reversed matrix
			Input matrix form :
			a0 / a1 / a2 / a3
			b0 / b1 / b2 / b3
			c0 / c1 / c2 / c3
			d0 / d1 / d2 / d3

			Result matrix form :
			a0 / b0 / c0 / d0
			a1 / b1 / c1 / d1
			a2 / b2 / c2 / d2
			a3 / b3 / c3 / d3
		*/
		Matrix4x4 reverse() const
		{
			Matrix4x4 result;

			for (jgl::Uint i = 0; i < 4; i++)
				for (jgl::Uint j = 0; j < 4; j++)
				{
					result.value[i][j] = value[j][i];
				}
			return (result);
		}

		/*
			Compute the cofactor of the matrix and returns it
		*/
		jgl::Float cofactor(const jgl::Int x, const jgl::Int y) const
		{
			jgl::Float tmp_value[9]{ 1, 1, 1, 1, 1, 1, 1, 1, 1 };

			jgl::Uint index = 0;

			for (jgl::Uint i = 0; i < 4; i++)
				for (jgl::Uint j = 0; j < 4; j++)
				{
					if (i != x && j != y)
					{
						tmp_value[index] = value[i][j];
						index++;
					}
				}
			Matrix3x3 tmp_matrix(
				tmp_value[0], tmp_value[1], tmp_value[2],
				tmp_value[3], tmp_value[4], tmp_value[5],
				tmp_value[6], tmp_value[7], tmp_value[8]
			);
			jgl::Float result = tmp_matrix.determinant();
			return (result);
		}

		/*
			Calculate the determinant of the matrix
		*/
		jgl::Float determinant() const
		{
			return (value[0][0] * cofactor(0, 0) - value[1][0] * cofactor(1, 0) + value[2][0] * cofactor(2, 0) - value[3][0] * cofactor(3, 0));
		}

		/*
			Compute the cofactor matrix and returns it
		*/
		Matrix4x4 cofactor_matrix() const
		{
			Matrix4x4 result = Matrix4x4(
				+cofactor(0, 0), -cofactor(1, 0), +cofactor(2, 0), -cofactor(3, 0),
				-cofactor(0, 1), +cofactor(1, 1), -cofactor(2, 1), +cofactor(3, 1),
				+cofactor(0, 2), -cofactor(1, 2), +cofactor(2, 2), -cofactor(3, 2),
				-cofactor(0, 3), +cofactor(1, 3), -cofactor(2, 3), +cofactor(3, 3)
			);

			return (result);
		}

		/*
			Compute the adjucate matrix and returns it
		*/
		Matrix4x4 adjucate_matrix() const
		{
			return (cofactor_matrix().reverse());
		}

		/*
			Return the inverse matrix
		*/
		Matrix4x4 inverse_matrix() const
		{
			Matrix4x4 tmp = adjucate_matrix();
			Matrix4x4 result = Matrix4x4();
			jgl::Float deter = determinant();

			if (deter != 0)
			{
				for (jgl::Uint i = 0; i < 4; i++)
					for (jgl::Uint j = 0; j < 4; j++)
					{
						result.value[i][j] = tmp.value[i][j] / deter;
					}
			}
			return (result);
		}

		/*
			Matrix 4x4 x Matrix 4x4 multiplication operator
		*/
		Matrix4x4 operator * (const Matrix4x4 p_matrix) const
		{
			jgl::Int i;
			jgl::Int j;
			Matrix4x4	m = Matrix4x4();

			i = 0;
			while (i < 4)
			{
				j = 0;
				while (j < 4)
				{
					m.value[i][j] = (this->value[0][i] * p_matrix.value[j][0])
						+ (this->value[1][i] * p_matrix.value[j][1])
						+ (this->value[2][i] * p_matrix.value[j][2])
						+ (this->value[3][i] * p_matrix.value[j][3]);
					j++;
				}
				i++;
			}
			return (m);
		}

		/*
			Matrix 4x4 x Vector3 multiplication operator
		*/
		Vector3 operator * (const Vector3 vertex) const
		{
			return (Vector3(
				value[0][0] * vertex.x + value[1][0] * vertex.y + value[2][0] * vertex.z + value[3][0] * value[3][3],
				value[0][1] * vertex.x + value[1][1] * vertex.y + value[2][1] * vertex.z + value[3][1] * value[3][3],
				value[0][2] * vertex.x + value[1][2] * vertex.y + value[2][2] * vertex.z + value[3][2] * value[3][3]
			));
		}

		/*
			Matrix 4x4 x Vector4 multiplication operator
		*/
		Vector4 operator * (const Vector4 vertex) const
		{
			return (Vector4(
				value[0][0] * vertex.x + value[1][0] * vertex.y + value[2][0] * vertex.z + value[3][0] * vertex.w,
				value[0][1] * vertex.x + value[1][1] * vertex.y + value[2][1] * vertex.z + value[3][1] * vertex.w,
				value[0][2] * vertex.x + value[1][2] * vertex.y + value[2][2] * vertex.z + value[3][2] * vertex.w,
				value[3][0] * vertex.x + value[1][3] * vertex.y + value[2][3] * vertex.z + value[3][3] * vertex.w)
				);
		}
	};
}