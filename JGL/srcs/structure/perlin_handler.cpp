#include "jgl.h"

namespace jgl
{
	jgl::Float jgl::Perlin::fade(jgl::Float t)
	{
		return t * t * t * (t * (t * 6 - 15) + 10);
	}

	jgl::Float jgl::Perlin::lerp(jgl::Float t, jgl::Float a, jgl::Float b)
	{
		return a + t * (b - a);
	}

	jgl::Float jgl::Perlin::grad(jgl::Uchar hash, jgl::Float x, jgl::Float y, jgl::Float z)
	{
		const jgl::Uchar h = hash & 15;
		const jgl::Float u = h < 8 ? x : y;
		const jgl::Float v = h < 4 ? y : h == 12 || h == 14 ? x : z;
		return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
	}

	jgl::Float jgl::Perlin::weight(jgl::Int octaves)
	{
		jgl::Float amp = 1;
		jgl::Float value = 0;

		for (jgl::Int i = 0; i < octaves; ++i)
		{
			value += amp;
			amp /= 2;
		}

		return value;
	}

	jgl::Perlin::Perlin(jgl::Int seed)
	{
		reseed(seed);
	}

	void jgl::Perlin::reseed(jgl::Uint seed)
	{
		for (jgl::Int i = 0; i < 256; ++i)
		{
			p[i] = static_cast<jgl::Uchar>(i);
		}

		std::shuffle(std::begin(p), std::begin(p) + 256, std::default_random_engine(seed));

		for (jgl::Int i = 0; i < 256; ++i)
		{
			p[256 + i] = p[i];
		}
	}

	//Noise [0, 1]
	jgl::Float jgl::Perlin::noise(jgl::Float x, jgl::Float y, jgl::Float z)
	{
		const jgl::Int X = static_cast<jgl::Int>(std::floor(x)) & 255;
		const jgl::Int Y = static_cast<jgl::Int>(std::floor(y)) & 255;
		const jgl::Int Z = static_cast<jgl::Int>(std::floor(z)) & 255;

		x -= std::floor(x);
		y -= std::floor(y);
		z -= std::floor(z);

		const jgl::Float u = fade(x);
		const jgl::Float v = fade(y);
		const jgl::Float w = fade(z);

		const jgl::Int A = p[X] + Y, AA = p[A] + Z, AB = p[A + 1] + Z;
		const jgl::Int B = p[X + 1] + Y, BA = p[B] + Z, BB = p[B + 1] + Z;

		return (lerp(w, lerp(v, lerp(u, grad(p[AA], x, y, z),
			grad(p[BA], x - 1, y, z)),
			lerp(u, grad(p[AB], x, y - 1, z),
				grad(p[BB], x - 1, y - 1, z))),
			lerp(v, lerp(u, grad(p[AA + 1], x, y, z - 1),
				grad(p[BA + 1], x - 1, y, z - 1)),
				lerp(u, grad(p[AB + 1], x, y - 1, z - 1),
					grad(p[BB + 1], x - 1, y - 1, z - 1)))) + 1) / 2;
	}

	jgl::Float jgl::Perlin::accumulated_noise(jgl::Int octaves, jgl::Float x, jgl::Float y, jgl::Float z)
	{
		jgl::Float result = 0;
		jgl::Float amp = 1;

		for (jgl::Int i = 0; i < octaves; ++i)
		{
			result += noise(x, y, z) * amp;
			x *= 2.0;
			y *= 2.0;
			z *= 2.0;
			amp /= 2.0;
		}

		return (result / weight(octaves));
	}
}