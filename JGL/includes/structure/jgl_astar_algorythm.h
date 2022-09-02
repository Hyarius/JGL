#pragma once

#include "structure/jgl_array.h"
#include "structure/jgl_vector.h"
#include "jgl_basic_types.h"
#include <unordered_set>

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
			jgl::float local = INFINITY;
			jgl::float global = INFINITY;
			const AStar_node* parent = nullptr;

			AStar_node()
			{

			}
			AStar_node(jgl::Vector2Int p_pos, const AStar_node* p_parent)
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

			jgl::float value() const
			{
				return (global + local);
			}
			
			jgl::Bool operator < (const AStar_node& p_other) const 
			{
				return (value() < p_other.value());
			}
			
			jgl::Bool operator == (const AStar_node& p_other) const 
			{
				return (value() == p_other.value());
			}

			jgl::Size_t hash() const
			{
				jgl::Size_t result = pos.x;
				result *= 37;
				result += pos.y;

				return result;
			}
		
			friend std::ostream& operator<<(std::ostream& os, const AStar_node& node)
			{
				os << node.pos << " - " << node.local << " / " << node.global;
				return os;
			}
		};
	private:
		TBoardType* _board;
		
		static inline auto hash = [](const AStar_node& value){ return value.hash(); };
		static inline auto equal = [](const AStar_node& lhd, const AStar_node& rhd){ return lhd == rhd; };

		std::unordered_set<AStar_node, decltype(hash), decltype(equal)> _node_set;
		
		jgl::Int _range = 1;
		jgl::Array<const AStar_node*> _to_calc;
		const AStar_node* _last_node;

		const AStar_node* _closest_node()
		{
			const AStar_node* result = nullptr;
			jgl::Size_t index = 0;

			for (jgl::Size_t i = 0; i < _to_calc.size(); i++)
			{
				if (result == nullptr || *_to_calc[i] < *result == true)
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
		void _add_node(AStar_node p_node)
		{
			_node_set.insert(p_node);
			auto tmp = _node_set.find(p_node);
			_to_calc.push_back(&(*tmp));
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
		void set_range(jgl::Int p_range)
		{
			_range = p_range;
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
		const AStar_node* last_node()
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

			for (int x = -_range; x <= _range; x++)
			for (int y = -_range; y <= _range; y++)
			{
				jgl::Vector2Int direction = jgl::Vector2Int(x, y);
				if (_board->can_acces(_last_node->pos, direction) == true)
				{
					AStar_node tmp_node = AStar_node(_last_node->pos + direction, _last_node);
					auto founded_element = _node_set.find(tmp_node);

					if (founded_element == _node_set.end() || 
						founded_element->local > tmp_node.local)
					{
						if (founded_element != _node_set.end())
							_node_set.erase(tmp_node);
						_add_node(tmp_node);
					}
				}
			}

			return (false);
		}

		void compute_result(jgl::Array<jgl::Vector2Int>& p_path, const AStar_node* destination_node)
		{
			if (destination_node == nullptr)
				return;

			while (destination_node != nullptr)
			{
				p_path.push_back(destination_node->pos);
				destination_node = destination_node->parent;
			}
			
			std::reverse(p_path.begin(), p_path.end());
			reset();
		}

		jgl::Array<jgl::Vector2Int> run(jgl::Vector2Int p_start, jgl::Vector2Int p_end)
		{
			jgl::Array<jgl::Vector2Int> result;

			run(result, p_start, p_end);

			return (result);
		}

		void run(jgl::Array<jgl::Vector2Int>& p_path, jgl::Vector2Int p_start, jgl::Vector2Int p_end)
		{
			configure(p_start, p_end);
			run(p_path);
		}

		jgl::Array<jgl::Vector2Int> run()
		{
			jgl::Array<jgl::Vector2Int> result;
			
			run(result);
			
			return (result);
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
			_node_set.clear();
			_to_calc.clear();
			
			_add_node(AStar_node(AStar_node::C_SOURCE, nullptr));
		}

		jgl::Bool is_calculated(jgl::Vector2Int p_pos)
		{
			if (_node_set.count(p_pos) == 0)
				return (false);
			return (true);
		}

		jgl::float local(jgl::Vector2Int p_pos)
		{
			if (_node_set.count(p_pos) == 0)
				return (INFINITY);
			return (_node_set[p_pos].local);
		}

		jgl::float global(jgl::Vector2Int p_pos)
		{
			if (_node_set.count(p_pos) == 0)
				return (INFINITY);
			return (_node_set[p_pos].global);
		}

		jgl::float value(jgl::Vector2Int p_pos)
		{
			if (_node_set.count(p_pos) == 0)
				return (INFINITY);
			return (_node_set[p_pos].value());
		}
	};

}