#include "jgl.h"

jgl::String color_shader_vert =
R"(#version 330 core

layout(location = 0) in vec3 model_space;
layout(location = 1) in vec4 color_space;
out vec4 fragmentColor;

void main()
{
gl_Position = vec4(model_space, 1.0f);
fragmentColor = color_space;
})";

jgl::String color_shader_frag =
R"(#version 330 core

in vec4 fragmentColor;
layout(location = 0) out vec4 color;
void main()
{
	color = fragmentColor;
	if (color.a == 0)
		discard;
})";

jgl::String texture_shader_vert =
R"(#version 330 core

layout(location = 0) in vec3 model_space;
layout(location = 1) in vec2 vertexUV;
layout(location = 2) in float alpha_value;
out vec2 UV;
out float ALPHA;
void main()
{
	gl_Position = vec4(model_space, 1.0f);
	UV = vertexUV;
	ALPHA = alpha_value;
})";

jgl::String texture_shader_frag =
R"(#version 330 core

in vec2 UV;
in float ALPHA;
layout(location = 0) out vec4 color;
uniform sampler2D textureID;
void main()
{
	if (UV.x < 0 || UV.x > 1 || UV.y < 0 || UV.y > 1)
		discard;
	color = texture(textureID, UV).rgba;
	color.a = color.a * ALPHA;
	if (color.a == 0)
		discard;
})";

jgl::String texture_bitmap_shader_vert =
R"(#version 330 core

layout(location = 0) in vec3 model_space;
layout(location = 1) in vec2 vertexUV;
layout(location = 2) in float alpha_value;
layout(location = 3) in vec4 color_space;
layout(location = 4) in vec4 color_outline_space;
out vec2 UV;
out float ALPHA;
out vec4 fragmentColor;
out vec4 fragmentColorOutline;
void main()
{
	gl_Position = vec4(model_space, 1.0f);
	UV = vertexUV;
	ALPHA = alpha_value;
	fragmentColor = color_space;
	fragmentColorOutline = color_outline_space;
})";

jgl::String texture_bitmap_shader_frag =
R"(#version 330 core

in vec2 UV;
in float ALPHA;
layout(location = 0) out vec4 color;
in vec4 fragmentColor;
in vec4 fragmentColorOutline;
uniform sampler2D textureID;
void main()
{
	if (UV.x < 0 || UV.x > 1 || UV.y < 0 || UV.y > 1)
		discard;
	vec4 tmp_color = texture(textureID, UV).rgba;
	vec2 size = 1.0f / textureSize(textureID, 0);
	int outline_size = 1;
	if (tmp_color.r == 0.0f)
		color = vec4(0, 0, 0, 0);
	else {
		//if (tmp_color.r == 1.0f){
			color = fragmentColor;
		//}
		//else{
		//	color = fragmentColorOutline;
		//}
	}
	color.a = color.a * ALPHA;
	if (color.a == 0)
		discard;
})";


jgl::String vertex_tilemap_shader_code = 
R"(#version 330 core

layout(location = 0) in vec3 model_space;
layout(location = 1) in vec2 model_uvs;
layout(location = 2) in float animation_sprite_delta;

uniform vec3 delta_model;
uniform int animation_state;
uniform vec2 uvs_unit;

out vec2 UV;

void main()
{
	gl_Position = vec4(model_space + delta_model, 1.0f);

	UV = model_uvs + (vec2(animation_state * animation_sprite_delta, 0) * uvs_unit);
})";

jgl::String fragment_tilemap_shader_code = R"(#version 330 core

in vec2 UV;

layout(location = 0) out vec4 color;

uniform sampler2D textureID;

void main()
{
	if (UV.x < 0 || UV.x > 1 || UV.y < 0 || UV.y > 1)
	{
		color = vec4(1, 0, 0, 1);
	}
	else
	{
		color = texture(textureID, UV).rgba;

		if (color.a == 0)
			discard;
	}
})";