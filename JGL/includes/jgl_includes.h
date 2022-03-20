#pragma once

#include <ios>
#include <fstream>
#include <stdint.h>
#include <iostream>
#include <string>
#include <vector>
#include <mutex>
#include <thread>
#include <deque>
#include <random>
#include <stdlib.h>
#include <iomanip>
#include <sstream>
#include <map>
#include <direct.h>
#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <bitset>
#include <deque>
#include <cstdint>
#include <type_traits>

#include "jgl_dirent.h"

#include <windows.h>
#define GLEW_STATIC
#include "GL/glew.h"
#include "GL/glut.h"
#include <GL/GL.h>
#include <GL/wglew.h>
#pragma comment(lib, "opengl32.lib")

#define ASIO_STANDALONE
#include <external_lib/asio.hpp>
#include <external_lib/asio/ts/buffer.hpp>
#include <external_lib/asio/ts/internet.hpp>