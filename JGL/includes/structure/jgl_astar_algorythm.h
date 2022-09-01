#pragma once

#include "structure/jgl_array.h"
#include "structure/jgl_vector.h"
#include "jgl_basic_types.h"

namespace jgl
{
	template <typename TBoardType>
	class AStar_algorithm
	{
	public:
		struct AStar_node
		{
			static inline jgl::Vector2Int C_SOURCE = -1;
			static inline jgl::Vector2Int C_DESTINATION = -1;

			jgl::Vector2Int pos = -1;
			jgl::Float road_length = INFINITY;
			jgl::Float distance_start = INFINITY;
			jgl::Float distance_end = INFINITY;
			AStar_node* parent = nullptr;

			AStar_node()
			{

			}
			AStar_node(jgl::Vector2Int p_pos, AStar_node* p_parent)
			{
				pos = p_pos;
				parent = p_parent;
				distance_start = pos.distance(C_SOURCE);
				distance_end = pos.distance(C_DESTINATION);
				if (parent == nullptr)
					road_length = 0;
				else
				{
					road_length = parent->road_length + parent->pos.distance(pos);
				}
			}

			jgl::Float cumuled_distance()
			{
				return (distance_end + road_length);
			}

			jgl::Bool is_closer(AStar_node* p_other)
			{
				return (cumuled_distance() < p_other->cumuled_distance());
			}

		};
	private:
		TBoardType* _board;
		jgl::Map<jgl::Vector2Int, AStar_node> _node_map;
		jgl::Array<AStar_node*> _to_calc;

		static inline jgl::Vector2Int direction_value[8] = {
			jgl::Vector2Int(-1, -1),
			jgl::Vector2Int(0, -1),
			jgl::Vector2Int(1, -1),
			jgl::Vector2Int(1, 0),
			jgl::Vector2Int(-1, 0),
			jgl::Vector2Int(-1, 1),
			jgl::Vector2Int(0, 1),
			jgl::Vector2Int(1, 1),
		};

		static inline jgl::Float direction_lengths[8] = {
			1.414f, 1, 1.414f, 1, 1, 1.414f, 1, 1.414f
		};

		void _add_node(jgl::Vector2Int p_pos, AStar_node* p_parent)
		{
			AStar_node new_node = AStar_node(p_pos, p_parent);
			_node_map[p_pos] = new_node;
			_to_calc.push_back(&_node_map[p_pos]);
		}

		AStar_node* _closest_node()
		{
			AStar_node* result = nullptr;

			for (jgl::Size_t i = 0; i < _to_calc.size(); i++)
			{
				if (result == nullptr || _to_calc[i]->is_closer(result) == true)
				{
					result = _to_calc[i];
				}
			}
			if (result != nullptr)
			{
				_to_calc.erase(_to_calc.find(result));
			}

			return (result);
		}
		AStar_node* _last_node;

	public:
		AStar_algorithm(TBoardType* p_board)
		{
			set_board(p_board);
		}
		void set_board(TBoardType* p_board)
		{
			_board = p_board;
		}
		void configure(jgl::Vector2Int p_start, jgl::Vector2Int p_end)
		{
			AStar_node::C_SOURCE = p_start;
			AStar_node::C_DESTINATION = p_end;
			reset();
		}
		AStar_node* last_node()
		{
			return (_last_node);
		}
		jgl::Bool iterate()
		{
			if (_last_node != nullptr && _last_node->pos == AStar_node::C_DESTINATION)
			{
				return (true);
			}

			_last_node = _closest_node();

			if (_last_node == nullptr)
				return (true);

			for (jgl::Size_t i = 0; i < 8; i++)
			{
				if (_board->can_acces(_last_node->pos, direction_value[i]) == true)
				{
					if (_node_map.count(_last_node->pos + direction_value[i]) == 0)
					{
						_add_node(_last_node->pos + direction_value[i], _last_node);
					}
					else if (_node_map[_last_node->pos + direction_value[i]].road_length > _last_node->road_length + direction_value[i].length())
					{
						_add_node(_last_node->pos + direction_value[i], _last_node);
					}
				}
			}

			return (false);
		}

		void compute_result(jgl::Array<jgl::Vector2Int>& p_path, AStar_node* destination_node)
		{
			if (destination_node == nullptr)
				return;

			while (destination_node != nullptr)
			{
				p_path.push_back(destination_node->pos);
				destination_node = destination_node->parent;
			}

			p_path.reverse();
			reset();
		}

		void run(jgl::Array<jgl::Vector2Int>& p_path)
		{
			reset();

			while (iterate() == false) {}

			compute_result(p_path, _last_node);
		}

		jgl::Array<jgl::Vector2Int> run(jgl::Vector2Int p_start, jgl::Vector2Int p_end)
		{
			jgl::Array<jgl::Vector2Int> result;

			run(result, p_start, p_end);

			return (result);
		}

		void reset()
		{
			_last_node = nullptr;
			_node_map.clear();
			_to_calc.clear();
			_add_node(AStar_node::C_SOURCE, nullptr);
		}

		jgl::Bool is_calculated(jgl::Vector2Int p_pos)
		{
			if (_node_map.count(p_pos) == 0)
				return (false);
			return (true);
		}

		jgl::Float road_length(jgl::Vector2Int p_pos)
		{
			if (_node_map.count(p_pos) == 0)
				return (INFINITY);
			return (_node_map[p_pos].road_length);
		}

		jgl::Float distance_start(jgl::Vector2Int p_pos)
		{
			if (_node_map.count(p_pos) == 0)
				return (INFINITY);
			return (_node_map[p_pos].distance_start);
		}

		jgl::Float distance_end(jgl::Vector2Int p_pos)
		{
			if (_node_map.count(p_pos) == 0)
				return (INFINITY);
			return (_node_map[p_pos].distance_end);
		}

		jgl::Float cumuled_distance(jgl::Vector2Int p_pos)
		{
			if (_node_map.count(p_pos) == 0)
				return (INFINITY);
			return (_node_map[p_pos].cumuled_distance());
		}
	};
}