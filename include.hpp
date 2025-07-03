#ifndef INCLUDE
#define INCLUDE

#define		FONTTYPE 	1
#define		PDC_DEBUG	1
#define 	UBUNTU		1 // Set to 1 to use older filesystem libraries
#define		DEBUG		0 // Set to 1 to print debug information to a terminal

#include "src/pdc/pdc.hpp"
using namespace pdc;

#include <webp/decode.h>
#include <thread> // Multi threading is used for saving
#include <sys/stat.h>

// Common Classes
#include "src/functions/shaders.hpp"
#include "src/classes/object.hpp"


// Functions and variables
#include "src/functions.hpp"
#include "src/variables.hpp"

#include "src/classes/board.hpp"
#include "src/classes/tag.hpp"
#include "src/classes/table.hpp"

// Functions are declared here because they rely on classes
void DrawFolders(float, float*, Vector2, Table*);
bool Duplicate(string img, vector<File>* list);
Tag FilterTag(Tag, vector<File>);
void ImportTag(Tag, Tag*);
vector<Tag> LoadTags(ifstream*, bool, bool);
void SaveTags(ofstream*, vector<Tag>, bool);
bool SortTag(Tag, Tag);

#include "src/functions/sort.cpp"
#ifdef _WIN32
	#include "src/functions/os_win.cpp"
	const char *usrHome = "";
#else
	#include "src/functions/os_linux.cpp"
	#include <sys/types.h>
	#include <pwd.h>

	struct passwd *pw = getpwuid(getuid());
	const char *usrHome = pw->pw_dir;
#endif
#include "src/classes/mouseMenu.hpp"

#include "src/functions/handler.cpp"
#include "src/functions/clipboard.cpp"

#include "src/windows/main/boards.cpp"
#include "src/windows/main/main_render.cpp"
#include "src/windows/main/right_menu_input.cpp"
#include "src/windows/main/left_menu_input.cpp"
#include "src/windows/main/main_input.cpp"
#include "src/windows/main/tutorial_render.cpp"
#include "src/windows/main/tutorial_input.cpp"

#include "src/functions/tag.cpp"

#include "src/functions/save.cpp"
#include "src/functions/load.cpp"

#include "src/windows/import/import_render.cpp"
#include "src/windows/import/import_input.cpp"


#endif
