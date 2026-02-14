#ifndef INCLUDE
#define INCLUDE

#define		FONTTYPE 	1
#define		PDC_DEBUG	1
#define		PDC_VSYNC	0
#define 	UBUNTU		0 // Set to 1 to use older filesystem libraries
#define		DEBUG		1 // Set to 1 to print debug information to a terminal

#if UBUNTU
	#include <experimental/filesystem>
	namespace fs = std::experimental::filesystem;
#else
	#include <filesystem>
	namespace fs = std::filesystem;
#endif

#include "src/pdc/PDC.hpp"
#include "src/pdc/classes/basic/variable.hpp"
using namespace pdc;

#include "src/pdc/src/fps.cpp" // Wayland vsync freezing work around


#include <webp/decode.h>
#include <thread> // Multi threading is used for saving and loading
#include <sys/stat.h>
#include <map>

// Common Classes
//#include "src/functions/shaders.hpp"

bool showTutorial = false;


// Functions and variables
#include "src/colors.hpp"
#include "src/functions.hpp"
#include "src/variables.hpp"

#include "src/classes/image_container.hpp"


ImageContainer previewImg;


#include "src/classes/file.hpp"
#include "src/functions/sort.cpp"

enum Warnings{
	WARNING_LOCATION,
	WARNING_TAG,
	WARNING_SUBTAG,
	WARNING_BOARD
};

#include "src/classes/tag.hpp"
#include "src/classes/location.hpp"


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

#include "src/windows/main/main_window.hpp"
#include "src/classes/board.hpp"

#include "src/classes/mouse_menu.hpp"


#include "src/functions/window_events.cpp"
#include "src/windows/warning.hpp"
#include "src/windows/board_window.hpp"


// Functions are declared here because they rely on classes
void DrawFolders(float, float*, Vector2, Location*);
bool Duplicate(string img, vector<File>* list);
Tag FilterTag(Tag, vector<File>);
void ImportTag(Tag, Tag*);
vector<Tag> LoadTags(ifstream*, bool, bool);
void SaveTags(ofstream*, vector<Tag>, bool);
bool SortTag(Tag t1, Tag t2){return SortString(t1.name, t2.name);}


#include "src/windows/tag.cpp"
#include "src/functions/clipboard.cpp"
#include "src/windows/main/boards.cpp"
#include "src/windows/main/main_render.cpp"
#include "src/windows/main/main_input.cpp"
#include "src/windows/main/left_menu_input.cpp"
#include "src/windows/main/right_menu_input.cpp"

#include "src/windows/import/import_render.cpp"
#include "src/windows/import/import_input.cpp"

#include "src/windows/main/tutorial_render.cpp"
#include "src/windows/main/tutorial_input.cpp"


#include "src/functions/save.cpp"
#include "src/functions/load.cpp"

#include "src/windows/downloader/downloader_render.cpp"
#include "src/windows/downloader/downloader_input.cpp"



#endif
