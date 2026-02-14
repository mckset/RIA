/*
	Defines global variables
	Vector variables of a class are defined with that class
*/

using namespace std::chrono;

class ImageContainer;

#define		FONT_SIZE					24.0f
#define 	MEDIUM_FONT_SIZE			18.0f
#define 	SMALL_FONT_SIZE				12.0f
#define		TINY_FONT_SIZE				8.0f // Used for the edit button on tags
#define		PADDING						24.0f
#define		MOUSE_MENU_PADDING			8.0f
#define		BOARD_BUTTON_PADDING		8.0f
#define		BOARD_PADDING				8.0f
#define		BOARD_BUTTON_SIZE			(FONT_SIZE*6)
#define		GRID_SIZE					128.0f
#define		QUALITY						8 // 1-8 (8 being the highest)
#define		SCROLLBAR_SIZE				32.0f
#define		VIEW_SPEED					16.0f
#define		SHADOW_OFFSET				2.0f
#define		TAG_COLOR_SELECTOR_SIZE		256.0f
#define		MOUSE_MENU_SIZE				(FONT_SIZE*9)
#define		SCALE_THRESHOLD				16.0f // How far the mouse has to move before the image gets scaled
#define		FILE_PADDING				16.0f

#define		IMAGE_BORDER_SIZE			4.0f
#define		IMAGE_SELECTOR_BORDER_SIZE	2.0f
#define		MIN_IMAGE_SIZE				32.0f
#define 	PREVIEW_IMAGE_SIZE			360.0f

#define		STATUS_TIME					5.0f


bool WAYLAND = false; // Used by the clipboard to change copying commands on wayland devices

string path = "";
string board = "Default.brd";
string exe = "";
string statusText = "";

const string downloadFile = "_downloaded.tmp";


bool closeThread = false;
bool maximizeWindow = true; 
bool oldSave = false; // True if an old save file
bool pastedFile = false; // If files had already been pasted

bool saving = false;

bool maximize = false;

float statusTextTimer = 0;
int loadedSave = 0; // True if the app tried to load the save file
int downloadedImageType = 0;

struct stat st;

ifstream f;


map<string, bool> fileTagMap;

thread *saveThread = nullptr;
thread *loadThread = nullptr;
thread *imageBoardThread = nullptr;

Window Import;
Window TagWin;
Window DownloadWin;

vector<ImageContainer> imgs; // Images on the board
vector<uint> selectedImgs; // Indexes of selected images on the image board
vector<string> internalClipboard; // Paths to images that are copied from RIA
vector<ImageContainer> loadedImages;
vector<string> textCache; // Used to load new text characters on the main thread

Button add_Button = Button{"+", Transparent, highlightColor, White, FONT_SIZE};


Button downloadImport_Button = Button{"Import Image", menuBackgroundColor, highlightColor, White, FONT_SIZE};
Button downloadCancel_Button = Button{"Cancel", menuBackgroundColor, highlightColor, White, FONT_SIZE};

Field newTagName_Field = Field{"Tag Name", fieldBackgroundColor, highlightColor, fontColor, FONT_SIZE};

Field downloadedImageName_Field = {"File Name", fieldBackgroundColor, highlightColor, fontColor, FONT_SIZE};

Vector2 downloadDropPosition;

#ifdef _WIN32
	string altFonts = {""};
	string fontPath = "C:\\Windows\\Fonts\\tahoma.ttf";
	string fonts[] = {};
#else
	string fontPath = "";
	string fonts[] = {
		"/usr/share/fonts/noto/NotoSansMono-Light.ttf",
		"/usr/share/fonts/truetype/noto/NotoSansMono-Regular.ttf",
		"/usr/share/fonts/open-sans/OpenSans-Light.ttf",
		"/usr/share/fonts/truetype/freefont/FreeMono.ttf"
		};
#endif