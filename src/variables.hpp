/*
	Defines global variables
	Vector variables of a class are defined with that class
*/

struct stat s;

string path = "";
string board = "Default.brd";

bool loaded = false; // True if the app tried to load the save file
bool DnD = false; // If a file was added via Drag and Drop
bool lMenu = false; // If the left side menu is open
bool rMenu = false; // If the right side menu is open
bool pasted = false; // If files had already been pasted
bool rot = false; // Rotate name was already taken. Rotate images
bool tagView = false; // If the side menu is view tags

int save = 0;
int editTag = -1; // Index of an edited tag
int editSub = -1; // Index of an edited sub tag

thread *saveThread = nullptr;

Window Main;
Window TagWin;
Window Import;

Object previewImg;
vector<Object> imgs; // Image board
vector<uint> selImgs; // Index of selected images on the image board
vector<string> internalClipboard; // Paths to images that are copied from RIA

//
// Default values for colors and fonts
//

Color backing = Color {.2, .2, .2, 1};
Color locationHeading = {.05f, .05f, .05f, .5};
Color locationFile = Color {.7, .7, .7, .25};
Color fieldBack = Color{.1, .1, .1, 1};
Color fontColor = White;
Color grid = Color {.25f, .25f, .25f, 1};
Color highlight = Color {.7, .7, .7, .5};
Color imageBorder = Color{.1, .1, .1, 1};
Color cOrigin = Color {1, 1, 1, .5};
Color menuBackground = Color {0, 0, 0, .5};

Color RMMenuBacking = Color {0, 0, 0, .5};

// Scroll bar
Color scrollbarBacking = Color {.2, .2, .2, .5};
Color scrollbarNotch = Color {.5, .5, .5, .5};
Color cSelector = Color{0,1,1,.1f};
Scrollbar boardScroll = Scrollbar{scrollbarBacking, scrollbarNotch, highlight, Transparent};

// Import menu
Color importBacking = Color {.27, .27, .3, 1};
Color importButton = Color {.1, .1, .1, .9};


bool showTutorial = false;
int borderSize = 6; // Size of the border on selected images
float fontSize = 24;
int gridSize = 128; // The grid in the background
const int minSize = 32; // Minimum image size
const int scrollbarSize = 32; 
const int sampleSize = 8; // Anti-Aliasing level
const int viewSpeed = 16; // How fast to move the view
int showZoom = 0;

bool drawOrigin = true;
bool maximize = true;
struct stat st;



#ifdef _WIN32
	string altFonts = {""};
	string fontPath = "C:\\Windows\\Fonts\\tahoma.ttf";
#else
	string altFonts = {"/usr/share/fonts/truetype/freefont/FreeMono.ttf"};
	string fontPath = "/usr/share/fonts/noto/NotoSansMono-Light.ttf";
#endif


Button add = Button{"+", Transparent, highlight, White, fontSize};
Button addBoard = Button{"+", Transparent, highlight, White, fontSize};

Vector2 csPicker = Vector2{256,256};
Vector2 csSize = Vector2{256, 256};

Field fCurrentBoard = Field{"default", Transparent, Transparent, fontColor, Black, 0, fontSize/2};


float menuWidth = 0;
