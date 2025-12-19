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
bool pastedFile = false; // If files had already been pasted
bool rotateImages = false;
bool scaleImages = false; // If the program should scale images
bool showImageSelector = false;
bool showLeftMenu = false;
bool showOrigin = true;
bool showMouseMenu = false; // Draws the right click menu when true
bool showRightMenu = false;
bool showTutorial = false;
bool showTags = false;
bool maximize = false;

//int editedTagIndex = -1;
//int editedSubTagIndex = -1;

float sideMenuWidth = 0;
int zoomTextTimer = 0;
float statusTextTimer = 0;
int loadedSave = 0; // True if the app tried to load the save file
int downloadedImageType = 0;
float sideMenuRatio =	0.25f; // Ratio of the menu width to the window width
struct stat st;

map<string, bool> fileTagMap;

thread *saveThread = nullptr;
thread *loadThread = nullptr;
thread *imageBoardThread = nullptr;

Window Main;
Window Import;
Window TagWin;
Window DownloadWin;

vector<ImageContainer> imgs; // Images on the board
vector<uint> selectedImgs; // Indexes of selected images on the image board
vector<string> internalClipboard; // Paths to images that are copied from RIA
vector<ImageContainer> loadedImages;
vector<string> textCache; // Used to load new text characters on the main thread

Button openTags_Button = Button{"Tags", menuBackgroundColor, highlightColor, White, SMALL_FONT_SIZE};
Button openLocations_Button = Button{"Locations", menuBackgroundColor, highlightColor, White, SMALL_FONT_SIZE};
Button leftClose_Button = Button{"Close", menuBackgroundColor, highlightColor, White, SMALL_FONT_SIZE};
Button help_Button = Button{"Help", menuBackgroundColor, highlightColor, White, SMALL_FONT_SIZE};

Button imagePack_Button = Button{"Image Packs", menuBackgroundColor, highlightColor, White, SMALL_FONT_SIZE};
Button imageBoards_Button = Button{"Boards", menuBackgroundColor, highlightColor, White, SMALL_FONT_SIZE};
Button save_Button = Button{"Save", menuBackgroundColor, highlightColor, White, SMALL_FONT_SIZE};

Button add_Button = Button{"+", Transparent, highlightColor, White, FONT_SIZE};
Button addBoard_Button = Button{"+", Transparent, highlightColor, White, FONT_SIZE};

Button downloadImport_Button = Button{"Import Image", menuBackgroundColor, highlightColor, White, FONT_SIZE};
Button downloadCancel_Button = Button{"Cancel", menuBackgroundColor, highlightColor, White, FONT_SIZE};

Scrollbar board_Scrollbar = Scrollbar{scrollbarBackgroundColor, scrollbarNotchColor};
Scrollbar tags_Scrollbar = Scrollbar{scrollbarBackgroundColor, scrollbarNotchColor};
Scrollbar locations_Scrollbar = Scrollbar{scrollbarBackgroundColor, scrollbarNotchColor};

Field currentBoard_Field = Field{"default", Transparent, Transparent, fontColor, SMALL_FONT_SIZE};
Field newTagName_Field = Field{"Tag Name", fieldBackgroundColor, highlightColor, fontColor, FONT_SIZE};

Field downloadedImageName_Field = {"File Name", fieldBackgroundColor, highlightColor, fontColor, FONT_SIZE};

static milliseconds startTime; // Time the next frame started
static milliseconds endTime; // Time the last frame ended

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