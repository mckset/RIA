//
// The PDC namespace is used to contain everything engine and UI wise without having to reimport each type.
// Define FONTTYPE to 1 to use actual fonts in place of an image map
//

#ifndef PDC

#define		PDC		1 // Has been imported

#ifndef FONTTYPE
	#define		FONTTYPE	0 // Set to 1 to use true type font or set leave at 0 for image based fonts
#endif

#define GLFW_INCLUDE_NONE
#define GLAD_GL_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION


#include <algorithm>
#include <cmath>
#include <chrono>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <vector>
#include <map>

#include "../outsourced/stb_image.h"

#ifdef _WIN32
	#define LINUX 0
	#define NTDDI_VERSION 0x0A000006
	#define _WIN32_WINNT 0x0A00
	#include <windows.h>
	#include <fstream>
	#include "../outsourced/windows/glfw/include/GLFW/glfw3.h"
	#include "../outsourced/windows/glad/include/glad/glad.h"
	#include "../outsourced/windows/explorer.hpp"
	std::string slash = "\\";

#else
	#define LINUX 1
	#include <GLFW/glfw3.h>
	#include <unistd.h>
	#include "../outsourced/linux/glad/include/glad/glad.h"
	#include "../outsourced/linux/explorer.hpp"
	std::string slash = "/";

#endif

using namespace std;
#include "macros.hpp"
typedef unsigned int uint;

// Handler functions are declared here as window.hpp relies on them for the Init() function
static void Error(int, const char*);
void GetScrollWheel(GLFWwindow*, double, double);
void On_Resize(GLFWwindow*, int, int);
void SetCursorPosition(GLFWwindow*, double, double);
void SetKeyboardState(GLFWwindow*, int, int, int, int);
void SetMouseState(GLFWwindow*, int, int, int);
void SubClose(GLFWwindow*);

namespace pdc{

	// Global Window Variables
	int Width = 640, Height = 480; // Window width and height
	float fWidth = 640, fHeight = 480; // Window width and height as a float
	uint vertexArray, vertexBuffer, elementBuffer; // Buffers used to draw to the screen
	const GLFWvidmode* mode;

	float *Scale; // Scale of the current window

	uint ShaderID; // ID of the currently used shader

	//
	// UI Classes
	//

	// Common classes and headers
	#include "classes/color.hpp"
	#include "classes/shader.hpp"
	#include "classes/vector.hpp"

	// Input classes
	#include "classes/keyboard.hpp"
	#include "classes/mouse.hpp"
	// Add gamepad stuff here

	// Windows that will be created by the app
	#include "classes/window.hpp"

	// UI classes
	#include "classes/shape.hpp"
	#include "classes/image.hpp" // Relies on shape.hpp

	#if FONTTYPE
		#include "classes/truefont.hpp" // Full font support
	#else
		#include "classes/font.hpp" // Relies on image.hpp
	#endif

	#include "classes/field.hpp" // Relies on font.hpp and shape.hpp
	#include "classes/button.hpp" // Relies on image.hpp
	#include "classes/scrollbar.hpp"
}

#endif