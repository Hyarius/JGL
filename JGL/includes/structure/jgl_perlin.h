#pragma once

#include "jgl_basic_types.h"

namespace jgl
{
	class Perlin {
	private:
		jgl::Uchar p[512];

		jgl::Float fade(jgl::Float t);
		jgl::Float lerp(jgl::Float t, jgl::Float a, jgl::Float b);
		jgl::Float grad(jgl::Uchar hash, jgl::Float x, jgl::Float y, jgl::Float z);
		jgl::Float weight(jgl::Int octaves);

	public:
		/*
			Default constructor
		*/
		Perlin(jgl::Int seed = std::default_random_engine::default_seed);
		/*
			Reseed the perlin generator
		*/
		void reseed(jgl::Uint seed);

		/*
			Generate a single pass value on coord X

			Optional
			jgl::Float Y - 2D value for more complex result
			jgl::Float Z - 3D value for more complex result
		*/
		jgl::Float noise(jgl::Float x, jgl::Float y = 0, jgl::Float z = 0);

		/*
			Generate a n-pass value on coord X

			Recommended value :
			octaves : 3

			Optional :
			jgl::Float Y - 2D value for more complex result
			jgl::Float Z - 3D value for more complex result
		*/
		jgl::Float accumulated_noise(jgl::Int  octaves, jgl::Float x, jgl::Float y = 0, jgl::Float z = 0);
	};
}