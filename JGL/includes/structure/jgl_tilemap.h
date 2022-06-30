#pragma once

#include "application/jgl_application.h"
#include "structure/jgl_vector.h"
#include "structure/jgl_string.h"
#include "application/jgl_shader.h"
#include "structure/jgl_sprite_sheet.h"

extern jgl::String vertex_tilemap_shader_code;
extern jgl::String fragment_tilemap_shader_code;

namespace jgl
{
	struct INode
	{
		static jgl::Vector2Int SIZE;
		static jgl::Vector3 UNIT;
		jgl::Ushort id;
		jgl::Vector2 sprite;
		jgl::Char animation_size;
		jgl::Bool autotiled;

		INode(jgl::Ushort p_id = 0, jgl::Bool p_autotiled = false) :
			id(p_id),
			sprite(0),
			animation_size(0),
			autotiled(p_autotiled)
		{

		}
	};

	template <typename TNodeType, const jgl::Size_t NChunkSize, const jgl::Size_t NChunkDepth = 1>
	class IChunk
	{
	protected:
		static inline jgl::Array<TNodeType*> _nodes = {};

		jgl::Vector2Int _pos;
		jgl::Short _content[NChunkSize][NChunkSize][NChunkDepth];

	public:
		static void add_node(TNodeType* p_node)
		{
			_nodes.push_back(p_node);
		}

		IChunk(jgl::Vector2Int p_pos) :
			_pos(p_pos)
		{
			static_assert(NChunkDepth != 0, "Chunk can't have a depth of 0");
			static_assert(std::is_base_of<INode, TNodeType>::value, "Chunk can only allow INode inherenced template");
			for (jgl::Size_t i = 0; i < NChunkSize; i++)
			{
				for (jgl::Size_t j = 0; j < NChunkSize; j++)
				{
					for (jgl::Size_t h = 0; h < NChunkDepth; h++)
					{
						_content[i][j][h] = -1;
					}
				}
			}
		}

		jgl::Short content(jgl::Vector2Int p_pos)
		{
			static_assert(NChunkDepth == 1, "Chunk depth > 1 : Use content(Vector3Int)");
			return (_content[p_pos.x][p_pos.y][0]);
		}

		jgl::Short content(jgl::Vector2Int p_pos, jgl::Int p_z)
		{
			static_assert(NChunkDepth != 1, "Chunk depth == 1 : Use content(Vector2Int)");
			return (_content[p_pos.x][p_pos.y][p_z]);
		}

		jgl::Short content(jgl::Vector3Int p_pos)
		{
			static_assert(NChunkDepth != 1, "Chunk depth == 1 : Use content(Vector2Int)");
			return (_content[p_pos.x][p_pos.y][p_pos.z]);
		}

		void set_content(jgl::Vector2Int p_pos, jgl::Short p_value)
		{
			static_assert(NChunkDepth == 1, "Chunk depth > 1 : Use set_content(Vector3Int, jgl::Short)");
			_content[p_pos.x][p_pos.y][0] = p_value;
		}

		void set_content(jgl::Vector3Int p_pos, jgl::Short p_value)
		{
			static_assert(NChunkDepth != 1, "Chunk depth == 1 : Use set_content(Vector2Int, jgl::Short)");
			_content[p_pos.x][p_pos.y][p_pos.z] = p_value;
		}
	};

	template<typename TBakableChunkType>
	class Tilemap;

	template <typename TNodeType, const jgl::Size_t NChunkSize, const jgl::Size_t NChunkDepth = 1>
	class IBakableChunk : public IChunk<TNodeType, NChunkSize, NChunkDepth>
	{
	protected:
		struct Shader_data
		{
			static inline jgl::Shader* shader = nullptr;

			jgl::Bool generated = false;

			jgl::Buffer* model_space_buffer = nullptr;
			jgl::Buffer* model_uvs_buffer = nullptr;
			jgl::Buffer* animation_sprite_delta_buffer = nullptr;
			jgl::Buffer* indexes_buffer = nullptr;

			jgl::Uniform* delta_model_uniform = nullptr;
			jgl::Uniform* animation_state_uniform = nullptr;
			jgl::Uniform* uvs_unit_uniform = nullptr;
			jgl::Uniform* texture_uniform = nullptr;

			void generate()
			{
				if (shader == nullptr)
				{
					shader = jgl::Application::active_application()->shader("Chunk shader");
					if (shader == nullptr)
						shader = jgl::Application::active_application()->add_shader("Chunk shader", vertex_tilemap_shader_code, fragment_tilemap_shader_code);
				}

				if (model_space_buffer == nullptr)
					model_space_buffer = shader->buffer("model_space")->copy();
				if (model_uvs_buffer == nullptr)
					model_uvs_buffer = shader->buffer("model_uvs")->copy();
				if (animation_sprite_delta_buffer == nullptr)
					animation_sprite_delta_buffer = shader->buffer("animation_sprite_delta")->copy();
				if (indexes_buffer == nullptr)
					indexes_buffer = shader->indexes_buffer()->copy();

				if (delta_model_uniform == nullptr)
					delta_model_uniform = shader->uniform("delta_model")->copy();
				if (animation_state_uniform == nullptr)
					animation_state_uniform = shader->uniform("animation_state")->copy();
				if (uvs_unit_uniform == nullptr)
					uvs_unit_uniform = shader->uniform("uvs_unit")->copy();
				if (texture_uniform == nullptr)
					texture_uniform = shader->uniform("textureID")->copy();

				generated = true;
			}

			void cast(jgl::Vector3 p_offset, jgl::Int p_animation_state)
			{
				shader->activate();

				delta_model_uniform->send(p_offset);
				animation_state_uniform->send(p_animation_state);
				uvs_unit_uniform->send(_node_texture->unit());
				texture_uniform->send(0);

				if (indexes_buffer->size() != 0 && _node_texture != nullptr)
				{
					_node_texture->activate();

					model_space_buffer->activate();
					model_uvs_buffer->activate();
					animation_sprite_delta_buffer->activate();
					indexes_buffer->activate();

					shader->cast(jgl::Shader::Mode::Triangle, indexes_buffer->size() / sizeof(jgl::Uint));
				}
			}
		};
		std::recursive_mutex _mutex;
		Shader_data _shader_data;
		jgl::Bool _baked;

		IBakableChunk<TNodeType, NChunkSize, NChunkDepth>* _neightbour_chunks[3][3];

		static inline jgl::Vector2Int _baking_neightbour_node[4][3] = {
		{
			jgl::Vector2Int(-1, 0),
			jgl::Vector2Int(-1, -1),
			jgl::Vector2Int(0, -1),
		},
		{
			jgl::Vector2Int(0, 1),
			jgl::Vector2Int(-1, 1),
			jgl::Vector2Int(-1, 0),
		},
		{
			jgl::Vector2Int(0, -1),
			jgl::Vector2Int(1, -1),
			jgl::Vector2Int(1, 0),
		},
		{
			jgl::Vector2Int(1, 0),
			jgl::Vector2Int(1, 1),
			jgl::Vector2Int(0, 1),
		},
		};
		static inline jgl::Vector2Int _baking_delta_node_sprite[4][2][2][2] = {
			/*
				if next node == actual -> 0
				else -> 1
			*/
			{// Passe 1
				{// A = 1
					{// B = 1
						jgl::Vector2Int(1, 3), // ABC
						jgl::Vector2Int(1, 2)  // AB
					},
					{// B = 0
						jgl::Vector2Int(2, 0), // A C
						jgl::Vector2Int(1, 2)  // A
					}
				},
				{// A = 0
					{// B = 1
						jgl::Vector2Int(0, 3), //  BC
						jgl::Vector2Int(0, 0)  //  B
					},
					{// B = 0
						jgl::Vector2Int(0, 3), //   C
						jgl::Vector2Int(0, 0)  // 
					}
				}
			},
			{// Passe 2
				{// A = 1
					{// B = 1
						jgl::Vector2Int(1, 4), // ABC
						jgl::Vector2Int(0, 4)  // AB
					},
					{// B = 0
						jgl::Vector2Int(2, 1), // A C
						jgl::Vector2Int(0, 4)  // A
					}
				},
				{// A = 0
					{// B = 1
						jgl::Vector2Int(1, 5), //  BC
						jgl::Vector2Int(0, 1)  //  B
					},
					{// B = 0
						jgl::Vector2Int(1, 5), //   C
						jgl::Vector2Int(0, 1)  // 
					}
				}
			},
			{// Passe 3
				{// A = 1
					{// B = 1
						jgl::Vector2Int(2, 3), // ABC
						jgl::Vector2Int(3, 3)  // AB
					},
					{// B = 0
						jgl::Vector2Int(3, 0), // A C
						jgl::Vector2Int(3, 3)  // A
					}
				},
				{// A = 0
					{// B = 1
						jgl::Vector2Int(2, 2), //  BC
						jgl::Vector2Int(1, 0)  //  B
					},
					{// B = 0
						jgl::Vector2Int(2, 2), //   C
						jgl::Vector2Int(1, 0)  // 
					}
				}
			},
			{// Passe 4
				{// A = 1
					{// B = 1
						jgl::Vector2Int(2, 4), // ABC
						jgl::Vector2Int(2, 5)  // AB
					},
					{// B = 0
						jgl::Vector2Int(3, 1), // A C
						jgl::Vector2Int(2, 5)  // A
					}
				},
				{// A = 0
					{// B = 1
						jgl::Vector2Int(3, 4), //  BC
						jgl::Vector2Int(1, 1)  //  B
					},
					{// B = 0
						jgl::Vector2Int(3, 4), //   C
						jgl::Vector2Int(1, 1)  // 
					}
				}
			},
		};
		static inline jgl::Vector3 _delta_autotile_position[4] = {
			jgl::Vector3(0.0f, 0.0f, 0.0f),
			jgl::Vector3(0.0f, 0.5f, 0.0f),
			jgl::Vector3(0.5f, 0.0f, 0.0f),
			jgl::Vector3(0.5f, 0.5f, 0.0f)
		};
		static inline jgl::Vector3 _delta_tile_position[4] = {
			jgl::Vector3(0.0f, 0.0f, 0.0f),
			jgl::Vector3(0.0f, 1.0f, 0.0f),
			jgl::Vector3(1.0f, 0.0f, 0.0f),
			jgl::Vector3(1.0f, 1.0f, 0.0f)
		};
		static inline jgl::Vector2 _delta_uvs[4] = {
			jgl::Vector2(0, 0),
			jgl::Vector2(1, 0),
			jgl::Vector2(0, 1),
			jgl::Vector2(1, 1)
		};
		static inline jgl::Uint _element_index[6] = {
			0, 3, 1, 2, 3, 0
		};
		static inline jgl::Sprite_sheet* _node_texture = nullptr;

	protected:
		void _initialize_opengl_data()
		{
			if (TNodeType::UNIT == 0.0f)
			{
				TNodeType::UNIT = jgl::convert_screen_to_opengl(jgl::Vector2Int(TNodeType::UNIT.x, TNodeType::UNIT.y), 1) - jgl::convert_screen_to_opengl(0, 0);
			}
			if (_shader_data.generated == false)
				_shader_data.generate();
		}

		jgl::Vector2Int _calc_sub_part_sprite(jgl::Int p_x, jgl::Int p_y, jgl::Int p_z, jgl::Size_t p_sub_part)
		{
			jgl::Int first_value;

			first_value = this->content(jgl::Vector3Int(p_x, p_y, p_z));

			jgl::Bool values[3] = { false, false, false };
			for (jgl::Size_t j = 0; j < 3; j++)
			{
				jgl::Vector2Int next_pos = jgl::Vector2Int(p_x + _baking_neightbour_node[p_sub_part][j].x, p_y + _baking_neightbour_node[p_sub_part][j].y);
				jgl::Int chunk_x = (next_pos.x == -1 ? 0 : (next_pos.x != NChunkSize ? 1 : 2));
				jgl::Int chunk_y = (next_pos.y == -1 ? 0 : (next_pos.y != NChunkSize ? 1 : 2));

				if (next_pos.x == -1)
					next_pos.x = NChunkSize - 1;
				if (next_pos.y == -1)
					next_pos.y = NChunkSize - 1;

				if (next_pos.x == NChunkSize)
					next_pos.x = 0;
				if (next_pos.y == NChunkSize)
					next_pos.y = 0;

				jgl::Int next_value;

				if (_neightbour_chunks[chunk_x][chunk_y] == nullptr)
				{
					next_value = -1;
				}
				else
				{
					next_value = _neightbour_chunks[chunk_x][chunk_y]->content(next_pos, p_z);
				}
				if (next_value == -1)
				{

				}
				else
				{
					if (next_value == first_value)
					{
						values[j] = false;
					}
					else
						values[j] = true;
				}
			}

			return (_baking_delta_node_sprite[p_sub_part][values[0]][values[1]][values[2]]);
		}


		void _bake_autotile(jgl::Array<jgl::Vector3>& p_vertex_array, jgl::Array<jgl::Vector2>& p_uvs_array, jgl::Array<jgl::Float>& p_animation_sprite_delta_array, jgl::Array<jgl::Uint>& p_element_array,
			TNodeType* p_node,
			jgl::Int p_x, jgl::Int p_y, jgl::Int p_z)
		{
			for (jgl::Size_t face = 0; face < 4; face++)
			{
				jgl::Vector2Int sprite_value = p_node->sprite + _calc_sub_part_sprite(p_x, p_y, p_z, face);
				jgl::Uint sprite_id = (_node_texture->size().x * sprite_value.y) + sprite_value.x;
				jgl::Vector3 node_pos = jgl::Vector3(p_x, p_y, p_z) + _delta_autotile_position[face];
				jgl::Vector2 sprite = _node_texture->sprite(sprite_id);
				jgl::Vector2 unit = _node_texture->unit();

				jgl::Size_t vertex_array_entry_size = p_vertex_array.size();

				for (size_t i = 0; i < 4; i++)
				{
					p_vertex_array.push_back(TNodeType::UNIT * (node_pos + _delta_autotile_position[i]));
					p_uvs_array.push_back(sprite + unit * _delta_uvs[i]);
				}
				for (jgl::Size_t i = 0; i < 6; i++)
				{
					p_element_array.push_back(_element_index[i] + vertex_array_entry_size);
				}

				for (jgl::Size_t i = 0; i < 4; i++)
					p_animation_sprite_delta_array.push_back(static_cast<jgl::Float>(p_node->animation_size));
			}

		}

		void _bake_tile(jgl::Array<jgl::Vector3>& p_vertex_array, jgl::Array<jgl::Vector2>& p_uvs_array, jgl::Array<jgl::Float>& p_animation_sprite_delta_array, jgl::Array<jgl::Uint>& p_element_array,
			TNodeType* p_node,
			jgl::Int p_x, jgl::Int p_y, jgl::Int p_z)
		{
			jgl::Uint sprite_id = (_node_texture->size().x * p_node->sprite.y) + p_node->sprite.x;
			jgl::Vector3 node_pos = jgl::Vector3(p_x, p_y, p_z);
			jgl::Vector2 sprite = _node_texture->sprite(sprite_id);
			jgl::Vector2 unit = _node_texture->unit();

			jgl::Size_t vertex_array_entry_size = p_vertex_array.size();

			for (size_t i = 0; i < 4; i++)
			{
				p_vertex_array.push_back(TNodeType::UNIT * (node_pos + _delta_tile_position[i]));
				p_uvs_array.push_back(sprite + unit * _delta_uvs[i]);
			}
			for (jgl::Size_t i = 0; i < 6; i++)
			{
				p_element_array.push_back(_element_index[i] + vertex_array_entry_size);
			}
			for (jgl::Size_t i = 0; i < 4; i++)
				p_animation_sprite_delta_array.push_back(static_cast<jgl::Float>(p_node->animation_size));
		}

		void _bake_content(jgl::Array<jgl::Vector3>& p_vertex_array, jgl::Array<jgl::Vector2>& p_uvs_array, jgl::Array<jgl::Float>& p_animation_sprite_delta_array, jgl::Array<jgl::Uint>& p_element_array, jgl::Int p_x, jgl::Int p_y, jgl::Int p_z)
		{
			jgl::Short value = this->content(jgl::Vector3Int(p_x, p_y, p_z));

			if (value != -1 && static_cast<jgl::Size_t>(value) < this->_nodes.size())
			{
				TNodeType* tmp_node = this->_nodes[value];

				if (tmp_node->autotiled == true)
				{
					_bake_autotile(p_vertex_array, p_uvs_array, p_animation_sprite_delta_array, p_element_array, tmp_node, p_x, p_y, p_z);
				}
				else
				{
					_bake_tile(p_vertex_array, p_uvs_array, p_animation_sprite_delta_array, p_element_array, tmp_node, p_x, p_y, p_z);
				}
			}
		}

	public:
		IBakableChunk(jgl::Vector2Int p_pos) : IChunk<TNodeType, NChunkSize, NChunkDepth>(p_pos),
			_baked(false)
		{

		}

		void unbake()
		{
			_mutex.lock();
			for (jgl::Int i = -1; i <= 1; i++)
			{
				for (jgl::Int j = -1; j <= 1; j++)
				{
					_neightbour_chunks[i + 1][j + 1] = nullptr;
				}
			}
			_baked = false;
			_mutex.unlock();
		}

		void bake(Tilemap<IBakableChunk>* p_map, jgl::Bool rebake)
		{
			_mutex.lock();
			_initialize_opengl_data();

			if (_node_texture == nullptr)
				return;

			for (jgl::Int i = -1; i <= 1; i++)
			{
				for (jgl::Int j = -1; j <= 1; j++)
				{
					if (_neightbour_chunks[i + 1][j + 1] == nullptr)
						_neightbour_chunks[i + 1][j + 1] = p_map->chunk(jgl::Vector2Int(i, j) + this->_pos);
				}
			}

			static jgl::Array<jgl::Vector3> vertex_array;
			static jgl::Array<jgl::Vector2> uvs_array;
			static jgl::Array<jgl::Float> animation_sprite_delta_array;
			static jgl::Array<jgl::Uint> element_array;

			vertex_array.clear();
			uvs_array.clear();
			animation_sprite_delta_array.clear();
			element_array.clear();

			for (jgl::Size_t i = 0; i < NChunkSize; i++)
				for (jgl::Size_t j = 0; j < NChunkSize; j++)
					for (jgl::Size_t h = 0; h < NChunkDepth; h++)
					{
						_bake_content(vertex_array, uvs_array, animation_sprite_delta_array, element_array, i, j, h);
					}

			_shader_data.model_space_buffer->send(vertex_array.all(), vertex_array.size());
			_shader_data.model_uvs_buffer->send(uvs_array.all(), uvs_array.size());
			_shader_data.animation_sprite_delta_buffer->send(animation_sprite_delta_array.all(), animation_sprite_delta_array.size());
			_shader_data.indexes_buffer->send(element_array.all(), element_array.size());

			_baked = true;
			_mutex.unlock();

			if (rebake == true)
			{
				for (jgl::Size_t i = 0; i < 3; i++)
				{
					for (jgl::Size_t j = 0; j < 3; j++)
					{
						if ((i != 1 || j != 1) && _neightbour_chunks[i][j] != nullptr)
							_neightbour_chunks[i][j]->bake(p_map, false);
					}
				}
			}
		}

		void render(jgl::Vector3 p_offset, jgl::Int p_animation_state)
		{
			if (_node_texture != nullptr)
			{
				if (_shader_data.generated == true)
					_shader_data.cast(p_offset, p_animation_state);
			}
		}
	};

	template<typename TBakableChunkType>
	class Tilemap
	{
	protected:
		jgl::Map<jgl::Vector2Int, TBakableChunkType*> _chunks;

	public:
		Tilemap()
		{

		}

		void unbake()
		{
			for (auto tmp : _chunks)
			{
				tmp.second->unbake();
			}
		}

		TBakableChunkType* request_chunk(jgl::Vector2Int p_pos)
		{
			if (_chunks.count(p_pos) == 0)
				_chunks[p_pos] = new TBakableChunkType(p_pos);
			return (_chunks[p_pos]);
		}

		TBakableChunkType* chunk(jgl::Vector2Int p_pos)
		{
			if (_chunks.count(p_pos) == 0)
				return (nullptr);
			return (_chunks[p_pos]);
		}

		void add_chunk(jgl::Vector2Int p_pos, TBakableChunkType* p_chunk)
		{
			if (_chunks.count(p_pos) != 0)
				delete _chunks[p_pos];
			_chunks[p_pos] = p_chunk;
		}
	};

}