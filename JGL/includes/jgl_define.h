#pragma once

#ifndef M_PI
#define M_PI 3.14159265358979323846f
#endif

#define JGL_STRING_PUSH_SIZE 100

#define GET_VARIABLE_NAME(Variable) (#Variable)

#include <string>

#if !defined(__PRETTY_FUNCTION__) && !defined(__GNUC__)
#define __PRETTY_FUNCTION__ __FUNCSIG__
#endif

namespace jgl
{
	std::string methodName(const std::string& prettyFunction);
	std::string className(const std::string& prettyFunction);
}
#define __METHOD__ jgl::methodName(__PRETTY_FUNCTION__)
#define __CLASS__ jgl::className(__PRETTY_FUNCTION__)