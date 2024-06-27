#ifndef INCLUDE
#define INCLUDE

#define		FONTTYPE 	1

#include "pdc/pdc.hpp"
using namespace pdc;


#include <webp/decode.h>

#include <sys/stat.h>
struct stat s;

string GetName(string);
void LoadImageBoard();

string path = "";
string board = "default.brd";


bool DnD = false; // If a file was added via Drag and Drop
bool menu = false; // If the side menu is open
bool pasted = false; // If files had already been pasted
bool scroll = true; // ?
bool tagView = false; // If the side menu is view tags

int save = 0;
int editTag = -1; // Index of an edited tag
int editSub = -1; // Index of an edited sub tag

string previewPath = ""; // Obsolete

void Save();
void Load();
void ResetImport();
void Maximize(GLFWwindow*, int);

#include "macros.hpp"

// Common Classes
#include "config.hpp"

#include "modules/shaders.hpp"
Window Main;
Window TagWin;
Window Import;

Image previewImg;
vector<Image> imgs; // Image board
vector<uint> selImgs; // Index of selected images on the image board

Field tagName = Field{"Tag Name", fieldBack, highlight, fontColor, Black, 0, fontSize}; // Edit tag name

Button add = Button{"+", Transparent, highlight, White, fontSize};

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
float menuWidth = 0;

string SortFormat(string s);
#include "classes/tag.hpp"
vector<Tag> tags;

Image LoadWebp(string);
#include "classes/table.hpp"
vector<Table> locations;


#include "modules/sort.cpp"
#ifdef _WIN32
	#include "modules/os_win.cpp"
#else
	#include "modules/os_linux.cpp"
#endif
#include "classes/mouseMenu.hpp"

#include "modules/handler.cpp"
#include "render.cpp"
#include "input.cpp"
#include "modules/tag.cpp"
#include "modules/save.cpp"
#include "modules/config.cpp"
#include "modules/import.cpp"


#endif
