#define GLFW_INCLUDE_NONE

#ifndef INCLUDE
#define INCLUDE

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#ifdef _WIN32
	#define NTDDI_VERSION 0x0A000006
	#define _WIN32_WINNT 0x0A00
	#include <windows.h>
	#include <fstream>
	#include "windows/glfw/include/GLFW/glfw3.h"
	#include "windows/glad/include/glad/glad.h"
	#include "windows/explorer.hpp"
	#define WINDOWS 1
#else
	#define WINDOWS 0
	#include <unistd.h>
	#include <GLFW/glfw3.h>
	#include "linux/glad/include/glad/glad.h"
	#include "linux/explorer.hpp"
#endif

#include <algorithm>
#include <iostream>
#include <string>
#include <filesystem>
#include <sys/stat.h>
#include <vector>

using namespace std;

string GetName(string);

#ifdef _WIN32
	string slash = "\\";
	string opSlash = "/";
	typedef unsigned int uint;
#else
	string slash = "/";
	string opSlash = "\\";
	#include <unistd.h>
#endif

int Width = -1, Height = -1;
uint vertexArray, vertexBuffer, elementBuffer;
const GLFWvidmode* mode;

bool shift = false;
bool menu = false;
bool tagView = false;
bool scroll = true;
int save = 0;
int editTag = -1; // Index of an edited tag
int editSub = -1; // Index of an edited sub tag
string previewPath = "";
string path = "";
char p[4096]; // 4096 is the max size path size for Linux, 256 for windows

void Save();
void Load();
void ResetImport();

#include "macros.hpp"

// Common Classes
#include "types/vector.hpp"
Vector2 View;

#include "types/color.hpp"
#include "colors.hpp"

#include "config.hpp"

#include "types/keyboard.hpp"
#include "types/mouse.hpp"

#include "types/shader.hpp"
#include "modules/shaders.hpp"
#include "types/window.hpp"
Window Main;
Window TagWin;
Window Import;

#include "types/shape.hpp"
#include "types/image.hpp"
Image folder;
Image previewImg;
vector<Image> imgs; // Image board
vector<uint> selImgs; // Index of selected images on the image board

#include "types/font.hpp"
#include "types/button.hpp"
#include "types/field.hpp"
Field tagName = Field{"Tag Name", White, highlight, fontColor}; // Edit tag name
Button add = Button{"+", Transparent, highlight};

#include "types/scrollbar.hpp"
// Edit tag colors
Scrollbar rgb[3] = { 
	Scrollbar{Red, White, highlight, Black, 0, true, 0, 8, 255},
	Scrollbar{Green, White, highlight, Black, 0, true, 0, 8, 255},
	Scrollbar{Blue, White, highlight, Black, 0, true, 0, 8, 255}};

Scrollbar tagScroll = Scrollbar{scrollbarBacking, scrollbarNotch, highlight, Transparent};
Scrollbar locScroll = Scrollbar{scrollbarBacking, scrollbarNotch, highlight, Transparent};
Scrollbar importScroll = Scrollbar{scrollbarBacking, scrollbarNotch, highlight, Transparent};


bool rot = false; // Rotate name was already taken. Rotate images
bool imgScale = false; // If the program should scale images
int drawMouseMenu = 0; // Draws the right click menu when true


#include "types/tag.hpp"
vector<Tag> tags;

#include "types/table.hpp"
vector<Table> locations;

#include "types/mouseMenu.hpp"

#include "modules/sort.cpp"
#include "handler.cpp"
#include "render.cpp"
#include "input.cpp"
#include "modules/tag.cpp"
#include "modules/save.cpp"
#include "modules/import.cpp"


#endif
