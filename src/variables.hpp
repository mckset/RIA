/*
	Defines global variables
	Vector variables of a class are defined with that class
*/

bool WAYLAND = false; // Used by the clipboard to change copying commands on wayland devices

string path = "";
string board = "Default.brd";
string exe = "";

bool drawOrigin = true; // Show origin
bool DnD = false; // If a file was added via Drag and Drop
bool loaded = false; // True if the app tried to load the save file
bool lMenu = false; // If the left side menu is open
bool maximize = true; // Maximize window
bool pasted = false; // If files had already been pasted
bool rMenu = false; // If the right side menu is open
bool rot = false; // Rotate name was already taken. Rotate images
bool showTutorial = false; // If RIA is in tutorial mode
bool tagView = false; // If the side menu is view tags

const int minSize = 32; // Minimum image size
const int scrollbarSize = 32; 
const int sampleSize = 8; // Anti-Aliasing level
const int viewSpeed = 16; // How fast to move the view

int borderSize = 4; // Size of the border on selected images
int editTag = -1; // Index of an edited tag
int editSub = -1; // Index of an edited sub tag
float fontSize = 24;
int gridSize = 128; // The grid in the background
float menuWidth = 0;
int save = 0; // Duration to show save messages
int showZoom = 0; // Duration to show zoom percent

struct stat st;

thread *saveThread = nullptr;

Window Main;
Window Import;
Window TagWin;

Object previewImg;
vector<Object> imgs; // Image board
vector<uint> selImgs; // Index of selected images on the image board
vector<string> internalClipboard; // Paths to images that are copied from RIA

// Default values for colors and fonts
Color backing = Color {.2, .2, .2, 1};
Color cOrigin = Color {1, 1, 1, .5};
Color fieldBack = Color{.1, .1, .1, 1};
Color fontColor = White;
Color grid = Color {.25f, .25f, .25f, 1};
Color highlight = Color {.7, .7, .7, .5};
Color imageBorder = Color{.1, .1, .1, 1};
Color locationHeading = {.05f, .05f, .05f, .5};
Color locationFile = Color {.7, .7, .7, .25};
Color menuBackground = Color {0, 0, 0, .5};
Color RMMenuBacking = Color {0, 0, 0, .5};

// Scroll bar
Color cSelector = Color{0,1,1,.1f};
Color scrollbarBacking = Color {.2, .2, .2, .5};
Color scrollbarNotch = Color {.5, .5, .5, .5};
Scrollbar boardScroll = Scrollbar{scrollbarBacking, scrollbarNotch, highlight, Transparent};

// Import menu
Color importBacking = Color {.27, .27, .3, 1};
Color importButton = Color {.1, .1, .1, .9};

Button add = Button{"+", Transparent, highlight, White, fontSize};
Button addBoard = Button{"+", Transparent, highlight, White, fontSize};

Field fCurrentBoard = Field{"default", Transparent, Transparent, fontColor, Black, 0, fontSize/2};

Vector2 csPicker = Vector2{256,256};
Vector2 csSize = Vector2{256, 256};

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