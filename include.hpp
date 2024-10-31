#ifndef INCLUDE
#define INCLUDE

#define		FONTTYPE 	1

#include "src/pdc/pdc.hpp"
using namespace pdc;


#include <webp/decode.h>
#include <thread> // Multi threading is used for saving
#include <chrono>

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

thread *saveThread = nullptr;

void Save();
void Load();
void ResetImport();
void Maximize(GLFWwindow*, int);

#include "src/pdc/pdc.hpp"

// Common Classes
#include "config.hpp"

#include "src/functions/shaders.hpp"
Window Main;
Window TagWin;
Window Import;

#include "src/classes/object.hpp"
Object previewImg;
vector<Object> imgs; // Image board
vector<uint> selImgs; // Index of selected images on the image board

Button add = Button{"+", Transparent, highlight, White, fontSize};

float menuWidth = 0;

string SortFormat(string s);
Image LoadWebp(string);

#include "src/classes/tag.hpp"
vector<Tag> tags;

#include "src/classes/table.hpp"
vector<Table> locations;

#include "src/functions/sort.cpp"
#ifdef _WIN32
	#include "src/functions/os_win.cpp"
#else
	#include "src/functions/os_linux.cpp"
#endif
#include "src/classes/mouseMenu.hpp"

#include "src/functions/handler.cpp"
#include "src/windows/main/render.cpp"
#include "src/windows/main/input.cpp"
#include "src/functions/tag.cpp"
#include "src/functions/save.cpp"
#include "src/functions/config.cpp"
#include "src/windows/import/render.cpp"
#include "src/windows/import/input.cpp"


#endif
