#include "jgl.h"

jgl::Color jgl::Color::to_color(jgl::String p_str, jgl::String p_delim)
{
	static jgl::Array<jgl::String> tab;

	p_str.split(tab, p_delim);

	jgl::Int R = jgl::stoi(tab[0]);
	jgl::Int G = jgl::stoi(tab[1]);
	jgl::Int B = jgl::stoi(tab[2]);
	jgl::Int A = (tab.size() == 4 ? jgl::stoi(tab[3]) : 255);

	return (jgl::Color(R, G, B, A));
}