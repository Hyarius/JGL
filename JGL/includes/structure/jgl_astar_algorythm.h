#pragma once

#include "structure/jgl_array.h"
#include "structure/jgl_vector.h"
#include "jgl_basic_types.h"
#include <set>

namespace jgl
{
	namespace Abstract
	{
		template <typename TBoardType>
		class IAStar_algorithm
		{
		public:
			struct AStar_node
			{
				static inline jgl::Vector2Int C_SOURCE = -1;
				static inline jgl::Vector2Int C_DESTINATION = -1;

				jgl::Vector2Int pos = -1;
				jgl::Float local = INFINITY;
				jgl::Float global = INFINITY;
				jgl::Float cumuled = INFINITY;
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
					cumuled = global + local;
				}

				void reset()
				{
					local = INFINITY;
					global = INFINITY;
					cumuled = INFINITY;
					parent = nullptr;
				}

				jgl::Bool operator < (const AStar_node& p_other) const
				{
					return (global < p_other.global);
				}

			};
		protected:
			
			TBoardType* _board;
			jgl::Size_t _max_path_length = 50;
			jgl::Bool _diagonal_allowed = true;
			jgl::Int _distance_from_walls = 0;
			std::vector<AStar_node*> _to_calc;
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


			virtual jgl::Bool _count(jgl::Vector2Int p_pos) = 0;
			virtual AStar_node& _nodes(jgl::Vector2Int p_pos) = 0;
			virtual void _insert_node(AStar_node new_node) = 0;
			virtual void _reset_node_data() = 0;

			void _add_node(jgl::Vector2Int p_pos, AStar_node* p_parent)
			{
				_insert_node(AStar_node(p_pos, p_parent));
				
				_to_calc.push_back(&_nodes(p_pos));
			}

			AStar_node* _closest_node()
			{
				AStar_node* result = nullptr;
				jgl::Size_t index = 0;

				for (jgl::Size_t i = 0; i < _to_calc.size(); i++)
				{
					if (result == nullptr || *_to_calc[i] < *result)
					{
						result = _to_calc[i];
						index = i;
					}
				}
				if (result != nullptr)
				{
					_to_calc.erase(_to_calc.begin() + index);
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
			IAStar_algorithm()
			{
				set_board(nullptr);
			}
			IAStar_algorithm(TBoardType* p_board)
			{
				set_board(p_board);
			}
			void set_max_path_length(jgl::Size_t p_max_path_length)
			{
				_max_path_length = p_max_path_length;
			}
			void set_distance_from_walls(jgl::Int p_distance_from_walls)
			{
				_distance_from_walls = p_distance_from_walls;
			}
			jgl::Int distance_from_walls()
			{
				return (_distance_from_walls);
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
			virtual jgl::Bool iterate()
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
					if (_check_distance_from_walls(_last_node->pos + direction_value[i]) == true)
					{
						if (_count(_last_node->pos + direction_value[i]) == 0)
						{
							_add_node(_last_node->pos + direction_value[i], _last_node);
						}
						else if (_nodes(_last_node->pos + direction_value[i]).local > _last_node->local + direction_value[i].length() && _last_node->local + direction_value[i].length() < _max_path_length)
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
				p_path.clear();
				reset();

				while (iterate() == false) {}

				compute_result(p_path, _last_node);
			}

			void reset()
			{
				
				_last_node = nullptr;
				
				_reset_node_data();
				
				_to_calc.clear();
				
				_add_node(AStar_node::C_SOURCE, nullptr);
				
			}
		};
	}

	template <typename TBoardType>
	class AStar_algorithm : public jgl::Abstract::IAStar_algorithm<TBoardType>
	{
	private:
		jgl::Map<jgl::Vector2Int, jgl::Abstract::IAStar_algorithm<TBoardType>::AStar_node> _nodes_data;

		jgl::Bool _count(jgl::Vector2Int p_pos)
		{
			return (_nodes_data.count(p_pos));
		}
		jgl::Abstract::IAStar_algorithm<TBoardType>::AStar_node& _nodes(jgl::Vector2Int p_pos)
		{
			return (_nodes_data.find(p_pos)->second);
		}
		void _insert_node(jgl::Abstract::IAStar_algorithm<TBoardType>::AStar_node new_node)
		{
			_nodes_data[new_node.pos] = new_node;
		}
		void _reset_node_data()
		{
			_nodes_data.clear();
		}

	public:
		AStar_algorithm() : jgl::Abstract::IAStar_algorithm<TBoardType>(){}
		AStar_algorithm(TBoardType* p_board) : jgl::Abstract::IAStar_algorithm<TBoardType>(p_board){}
	};

	template <typename TBoardType, const jgl::Size_t NSize_x, const jgl::Size_t NSize_y>
	class Fixed_AStar_algorithm : public jgl::Abstract::IAStar_algorithm<TBoardType>
	{
	private:
		jgl::Abstract::IAStar_algorithm<TBoardType>::AStar_node _nodes_data[NSize_x][NSize_y];

		jgl::Bool _count(jgl::Vector2Int p_pos)
		{
			return (true);
		}
		jgl::Abstract::IAStar_algorithm<TBoardType>::AStar_node& _nodes(jgl::Vector2Int p_pos)
		{
			return (_nodes_data[p_pos.x][p_pos.y]);
		}
		void _insert_node(jgl::Abstract::IAStar_algorithm<TBoardType>::AStar_node new_node)
		{
			_nodes_data[new_node.pos.x][new_node.pos.y] = new_node;
		}
		void _reset_node_data()
		{
			for (jgl::Size_t i = 0; i < NSize_x; i++)
			{
				for (jgl::Size_t j = 0; j < NSize_y; j++)
				{
					_nodes_data[i][j].reset();
				}
			}
		}

	public:
		Fixed_AStar_algorithm() : jgl::Abstract::IAStar_algorithm<TBoardType>(nullptr){}
		Fixed_AStar_algorithm(TBoardType* p_board) : jgl::Abstract::IAStar_algorithm<TBoardType>(p_board){}
	};
}