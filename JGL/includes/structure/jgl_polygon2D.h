#pragma once

#include "structure/jgl_vector.h"
#include "structure/jgl_array.h"

namespace jgl
{
	class Polygon2D
	{
		friend class Polygon2D;

	private:
		struct Segment_data
		{
			jgl::Size_t a;
			jgl::Size_t b;
			jgl::Vector2 norm;
		};

		jgl::Vector2 center;
		jgl::Vector2 size = 1;
		jgl::Array<jgl::Vector2> points;
		jgl::Array<Segment_data> segments;

		struct Axis_projection
		{
			jgl::Float min;
			jgl::Float max;
		};

		Axis_projection _project_point_on_axis(jgl::Vector2 p_point, jgl::Vector2 p_axis, jgl::Vector2 p_delta_pos = 0) const
		{
			Axis_projection result;

			jgl::Float tmp_dot = p_axis.dot(p_point + p_delta_pos);
			result.min = tmp_dot;
			result.max = tmp_dot;

			return (result);
		}

		Axis_projection _projection_on_axis(jgl::Vector2 p_axis, jgl::Vector2 p_delta_pos = 0) const
		{
			Axis_projection result;

			jgl::Float tmp_dot = p_axis.dot(points[0] * size + center + p_delta_pos);
			result.min = tmp_dot;
			result.max = tmp_dot;

			for (jgl::Size_t i = 1; i < points.size(); i++)
			{
				tmp_dot = p_axis.dot(points[i] * size + center + p_delta_pos);
				if (tmp_dot < result.min)
					result.min = tmp_dot;
				else if (tmp_dot > result.max)
					result.max = tmp_dot;
			}

			return (result);
		}

		jgl::Float _calc_distance(Axis_projection p_a, Axis_projection p_b) const
		{
			if (p_a.min < p_b.min)
				return p_b.min - p_a.max;
			else
				return p_a.min - p_b.max;
		}

	public:
		void set_center(jgl::Vector2 p_center)
		{
			center = p_center;
		}
		void set_size(jgl::Vector2 p_size)
		{
			size = p_size;
		}
		void add_point(jgl::Vector2 p_point)
		{
			points.push_back(p_point);
		}
		void add_segment(jgl::Size_t p_a, jgl::Size_t p_b)
		{
			Segment_data result;

			result.a = p_a;
			result.b = p_b;
			result.norm = points[p_a].cross(points[p_b]).normalize();

			segments.push_back(result);
		}

		void reset_points()
		{
			points.clear();
		}
		void reset_indexes()
		{
			segments.clear();
		}

		jgl::Bool collide(const Polygon2D& p_other, jgl::Vector2 p_delta_pos = 0)
		{
			jgl::Bool result = true;

			for (jgl::Size_t i = 0; i < segments.size(); i++)
			{
				Axis_projection projection_a = _projection_on_axis(segments[i].norm);
				Axis_projection projection_b = p_other._projection_on_axis(segments[i].norm, p_delta_pos);

				if (_calc_distance(projection_a, projection_b) > 0)
					result = false;
			}

			return (result);
		}

		jgl::Bool collide(const jgl::Vector2& p_point, jgl::Vector2 p_delta_pos = 0)
		{
			jgl::Bool result = true;

			for (jgl::Size_t i = 0; i < segments.size(); i++)
			{
				Axis_projection projection_a = _projection_on_axis(segments[i].norm);
				Axis_projection projection_b = _project_point_on_axis(p_point, segments[i].norm, p_delta_pos);

				if (_calc_distance(projection_a, projection_b) > 0)
					result = false;
			}

			return (result);
		}
	};
}