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
			jgl::Float local = INFINITY;
			jgl::Float global = INFINITY;
			AStar_node* parent = nullptr;

			AStar_node()
			{

			}
			AStar_node(jgl::Vector2Int p_pos, AStar_node* p_parent)
			{
				pos = p_pos;
				parent = p_parent;
				global = pos.distance(C_DESTINATION);
				if (parent == nullptr)
					local = 0;
				else
				{
					local = parent->local + parent->pos.distance(pos);
				}
			}

			jgl::Float cumuled_distance()
			{
				return (global + local);
			}

			jgl::Bool is_closer(AStar_node* p_other)
			{
				return (cumuled_distance() < p_other->cumuled_distance());
			}

		};
	private:
		TBoardType* _board;
		jgl::Bool _diagonal_allowed = true;
		jgl::Int _distance_from_walls = 0;
		jgl::Map<jgl::Vector2Int, AStar_node> _node_map;
		jgl::Array<AStar_node*> _to_calc;
		AStar_node* _last_node;

		static inline jgl::Vector2Int direction_value[8] = {
			jgl::Vector2Int(0, -1),
			jgl::Vector2Int(1, 0),
			jgl::Vector2Int(0, 1),
			jgl::Vector2Int(-1, 0),
			jgl::Vector2Int(1, 1),
			jgl::Vector2Int(-1, -1),
			jgl::Vector2Int(1, -1),
			jgl::Vector2Int(-1, 1),
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
			jgl::Size_t index = 0;

			for (jgl::Size_t i = 0; i < _to_calc.size(); i++)
			{
				if (result == nullptr || _to_calc[i]->is_closer(result) == true)
				{
					result = _to_calc[i];
					index = i;
				}
			}
			if (result != nullptr)
			{
				_to_calc.erase(index);
			}

			return (result);
		}
		jgl::Bool _check_distance_from_walls(jgl::Vector2Int p_pos)
		{
			for (jgl::Int i = -_distance_from_walls; i <= _distance_from_walls; i++)
			{
				for (jgl::Int j = -_distance_from_walls; j <= _distance_from_walls; j++)
				{
					jgl::Vector2Int delta = jgl::Vector2Int(i, j);
					if (delta.distance(0) <= _distance_from_walls && _board->can_acces(p_pos + delta, 0) == false)
						return (false);
				}
			}

			return (true);
		}

	public:
		AStar_algorithm()
		{
			set_board(nullptr);
		}
		AStar_algorithm(TBoardType* p_board)
		{
			set_board(p_board);
		}
		void set_distance_from_walls(jgl::Int p_distance_from_walls)
		{
			_distance_from_walls = p_distance_from_walls;
		}
		void set_diagonal_allowed(jgl::Bool p_diagonal_allowed)
		{
			_diagonal_allowed = p_diagonal_allowed;
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

			for (jgl::Size_t i = 0; i < (_diagonal_allowed == true ? 8 : 4); i++)
			{
				if (_board->can_acces(_last_node->pos, direction_value[i]) == true && 
					_check_distance_from_walls(_last_node->pos + direction_value[i]) == true)
				{
					if (_node_map.count(_last_node->pos + direction_value[i]) == 0)
					{
						_add_node(_last_node->pos + direction_value[i], _last_node);
					}
					else if (_node_map[_last_node->pos + direction_value[i]].local > _last_node->local + direction_value[i].length())
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

		jgl::Array<jgl::Vector2Int> run(jgl::Vector2Int p_start, jgl::Vector2Int p_end)
		{
			jgl::Array<jgl::Vector2Int> result;

			configure(p_start, p_end);

			run(result);

			return (result);
		}

		jgl::Array<jgl::Vector2Int> run()
		{
			jgl::Array<jgl::Vector2Int> result;

			run(result);

			return (result);
		}

		void run(jgl::Array<jgl::Vector2Int>& p_path, jgl::Vector2Int p_start, jgl::Vector2Int p_end)
		{
			configure(p_start, p_end);

			run(p_path);
		}

		void run(jgl::Array<jgl::Vector2Int>& p_path)
		{
			reset();

			while (iterate() == false) {}

			compute_result(p_path, _last_node);
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

		jgl::Float local(jgl::Vector2Int p_pos)
		{
			if (_node_map.count(p_pos) == 0)
				return (INFINITY);
			return (_node_map[p_pos].local);
		}

		jgl::Float global(jgl::Vector2Int p_pos)
		{
			if (_node_map.count(p_pos) == 0)
				return (INFINITY);
			return (_node_map[p_pos].global);
		}

		jgl::Float cumuled_distance(jgl::Vector2Int p_pos)
		{
			if (_node_map.count(p_pos) == 0)
				return (INFINITY);
			return (_node_map[p_pos].cumuled_distance());
		}
	};
}