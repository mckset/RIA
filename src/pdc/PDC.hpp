//
// The PDC namespace is used to contain everything engine and UI wise without having to reimport each type.
// Define FONTTYPE to 1 to use actual fonts in place of an image map
// Define PDC_DEFAULT as 0 to declare game values such as gravity before importing PDC
// 
//

#ifndef PDC
#define		PDC		1 // Prevent double import

#ifndef PDC_DEBUG
	#define 	PDC_DEBUG	0 // Enable for debug information
#endif

#ifndef	PDC_VSYNC
	#define		PDC_VSYNC	1 // Enables vsync for windows
#endif

#ifndef PDC_GAME
	#define PDC_GAME		0 // Set to 1 to use game classes and variables
#endif

#ifndef PDC_MACROS
	#define PDC_MACROS		0 // Set to 1 to import verbose macros
#endif

#ifndef FONTTYPE
	#define		FONTTYPE	1 // Set to 1 to use true type font or set leave at 0 for image based fonts
#endif

#ifndef PDC_ANIMTION_UPDATE
	#define		PDC_ANIMATION_UPDATE	1 // Auto updates object animations after they have been drawn
#endif

#ifndef PDC_COLLISIONS
	#define		PDC_COLLISIONS	1 // Auto updates object animations after they have been drawn
#endif

// PDC window event function macros
#ifndef PDC_HANDLE_RESIZE
	#define		PDC_HANDLE_RESIZE	1
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

#include "stb_image.h"

#ifdef _WIN32
	#define LINUX 0
	#define NTDDI_VERSION 0x0A000006
	#define _WIN32_WINNT 0x0A00
	#include <windows.h>
	#include <fstream>
	#include "windows/glfw/include/GLFW/glfw3.h"
	#include "windows/glad/include/glad/glad.h"
	#include "windows/os.hpp"
	std::string slash = "\\";

#else
	#define LINUX 1
	#include <GLFW/glfw3.h>
	#include <unistd.h>
	#include "linux/glad/include/glad/glad.h"
	#include "linux/os.hpp"
	std::string slash = "/";

#endif

using namespace std;
#include "macros.hpp"
typedef unsigned int uint;

// A window handler function needs to have these
static void Error(int e, const char* desc){
	if (e != 65548) fprintf(stderr, "Error #%d: %s\n", e, desc);
}
//void GetScrollWheel(GLFWwindow* w, double x, double y);
void OnResize(GLFWwindow* w, int width, int height);
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

	map<string, GLFWcursor*> cursors;
	string cursorType = "Normal";

	bool runGame = false; // If user is running a game
	
	float *Scale; // Scale of the current window
	class Window;
	Window* CurrentWindow = nullptr; // Pointer to the window being rendered
	Window* FocusedWindow = nullptr;
	uint ShaderID; // ID of the currently used shader

	float infinity = std::numeric_limits<float>::infinity();

	//
	// UI Classes
	//

	// Common classes and headers
	#include "classes/basic/color.hpp"
	#include "classes/basic/shader.hpp"
	#include "src/shaders.hpp"
	#include "classes/basic/vector2.hpp"
	float deltaTime = 0; // Time since last screen draw

	#if PDC_GAME
		#include "src/fps.cpp"
	#endif

	// Input classes
	#include "classes/basic/keyboard.hpp"
	#include "classes/basic/mouse.hpp"
	// Add gamepad stuff here
    
	// Windows that will be created by the app
	#include "classes/basic/window.hpp"

	// UI classes
	#include "classes/basic/shape.hpp"
	#include "classes/basic/image.hpp" // Relies on shape.hpp

	#if FONTTYPE
		#include "classes/basic/truefont.hpp" // Full font support
	#else
		#include "classes/basic/font.hpp" // Relies on image.hpp
	#endif

	// UI

	#include "classes/UI/field.hpp" // Relies on font.hpp and shape.hpp
	#include "classes/UI/button.hpp" // Relies on image.hpp
	#include "classes/UI/scrollbar.hpp"
	#include "classes/UI/colorselector.hpp"
	#include "classes/UI/dropdown.hpp"
	#include "classes/UI/checkbox.hpp"

	//
	// Game classes
	//
	#if PDC_GAME

		// Global physics values
		float gravity = 1;
		float dragBase = 0;
		float frictionBase = 0;
		float maxSlope = 60;
		float collisionRadius = 256;

		#include "classes/basic/variable.hpp" // Unlimited power and segmentation faults
		#include "classes/game/action.hpp"
		#include "classes/game/input_action.hpp"
		#include "classes/game/controller.hpp"
		//#include "classes/game/tile.hpp"

		//
		// Source files (what?)
		//
		#include "src/collisions.cpp"
	#endif
}

#endif

#if PDC_HANDLE_RESIZE
void OnResize(GLFWwindow* w, int width, int height){
	pdc::FocusedWindow->Use();
	pdc::FocusedWindow->width = width;
	pdc::FocusedWindow->height = height;

	glViewport(0, 0, width, height);
}
#endif

void SetCursorPosition(GLFWwindow* w, double x, double y){
	int winX, winY;
	pdc::FocusedWindow->Use();
	glfwGetWindowPos(w, &winX, &winY);
	pdc::mouse.prevPosition = pdc::mouse.position;
	pdc::mouse.position.x = (float)x;
	pdc::mouse.position.y = pdc::Height - (float)y;
}