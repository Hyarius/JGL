#include "jgl.h"

jgl::String color_model_shader_vert = {
	"#version 330 core\n"
	//	---	Array data ---
	"layout(location = 0) in vec3 model_space;"
	"layout(location = 1) in vec3 model_normal;"
	//	---	Model information ---
	"uniform vec3 pos;"
	"uniform mat4 rot;"
	"uniform vec3 size;"
	//	---	Camera information ---
	"uniform mat4 MVP;"
	"uniform mat4 view_matrix;"
	"uniform mat4 model_matrix;"
	//	---	tmp variable ---
	"vec4 tmp_pos;"
	"vec4 tmp_normale;"
	//	--- output value ---
	"out vec4 computed_normal;"
	""
	"void main()"
	"{"
	// Calcul of the world position
	"tmp_pos = rot * vec4(model_space * size, 1);"
	"tmp_pos = tmp_pos + vec4(pos, 1);"
	"gl_Position = MVP * tmp_pos;"
	// Light computing
	"computed_normal = rot * vec4(model_normal, 1);"
"}"
};

jgl::String color_model_shader_frag = {
	"#version 330 core\n"
	//	---	Input variable ---
	"in vec4 computed_normal;"
	//	--- Model information ---
	"uniform float alpha;"
	//	---	Camera information ---
	"uniform vec3 light_pos;"
	"uniform vec4 light_dir;"
	"uniform vec4 light_color;"
	"uniform vec3 camera_dir;"
	//	---	Material information ---
	"uniform vec4 material_ka;"
	"uniform vec4 material_kd;"
	"uniform vec4 material_ks;"
	"uniform vec4 material_ke;"
	"uniform float material_ns;"
	"uniform float material_ni;"
	"uniform float material_d;"
	"uniform int material_illum;"
	//	---	Output variable ---
	"layout(location = 0) out vec4 color;"
	""
	"void main()"
	"{"
		"float value = (((dot(computed_normal, light_dir) + 1) / 2) - 1) * -1;"
		"value = clamp(value, 0.3f, 1.0f);"
		"vec4 source_color = material_ka;"
		"color = source_color * value;"
		"color.a = source_color.a * alpha;"
	"}"
};

jgl::String texture_model_shader_vert = {
	"#version 330 core\n"
	//	---	Array data ---
	"layout(location = 0) in vec3 model_space;"
	"layout(location = 1) in vec2 model_uv;"
	"layout(location = 2) in vec3 model_normal;"
	//	---	Model information ---
	"uniform vec3 pos;"
	"uniform mat4 rot;"
	"uniform vec3 size;"
	//	---	Camera information ---
	"uniform mat4 MVP;"
	"uniform mat4 view_matrix;"
	"uniform mat4 model_matrix;"
	//	---	tmp variable ---
	"vec4 tmp_pos;"
	"vec4 tmp_normale;"
	//	--- output value ---
	"out vec4 result_position;"
	"out vec4 computed_normal;"
	"out vec2 UV;"
	""
	"void main()"
	"{"
		// Calcul of the world position
		"tmp_pos = rot * vec4(model_space * size, 1);"
		"tmp_pos = tmp_pos + vec4(pos, 1);"
		"result_position = MVP * tmp_pos;"
		"gl_Position = result_position;"
		// Light computing
		"computed_normal = rot * vec4(model_normal, 1);"
		"UV = model_uv;"
	"}"
};

jgl::String texture_model_shader_frag = {
	"#version 330 core\n"
	//	---	Input variable ---
	"in vec4 result_position;"
	"in vec4 computed_normal;"
	"in vec2 UV;"
	//	--- Model information ---
	"uniform float alpha;"
	//	---	Camera information ---
	"uniform vec3 light_pos;"
	"uniform vec4 light_dir;"
	"uniform vec4 light_color;"
	"uniform vec3 camera_dir;"
	"float near = 0.1;"
	"float far = 100.0;"
	//	---	Material information ---
	"uniform vec4 material_ka;"
	"uniform vec4 material_kd;"
	"uniform vec4 material_ks;"
	"uniform vec4 material_ke;"
	"uniform float material_ns;"
	"uniform float material_ni;"
	"uniform float material_d;"
	"uniform int material_illum;"
	"uniform sampler2D ambiant_texture;"
	"uniform sampler2D diffuse_texture;"
	"uniform sampler2D specular_texture;"
	"uniform sampler2D specular_hight_light;"
	"uniform sampler2D alpha_texture;"
	"uniform sampler2D bump_texture;"
	//	---	Output variable ---
	"layout(location = 0) out vec4 color;"
	""
	"float linearize_depth(float depth)"
	"{"
		"float z = depth * 2.0 - 1.0;" // back to NDC
		"return (2.0 * near * far) / (far + near - z * (far - near));"
	"}"
	"float calc_depth(float depth)"
	"{"
		"return (linearize_depth(depth) / far);"
	"}"
	"void main()"
	"{"
		"if (UV.x < 0 || UV.x > 1 || UV.y < 0 || UV.y > 1)"
			"discard;"
		"float value = (((dot(computed_normal, light_dir) + 1) / 2) - 1) * -1;"
		"value = clamp(value, 0.3f, 1.0f);"
		"vec4 source_color = texture(diffuse_texture, UV).rgba;"
		"color = source_color * value;"
		"color.a = source_color.a * alpha;"
	"}"
};