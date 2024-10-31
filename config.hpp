//
// Contains the default values for colors and fonts
//

#define 	UBUNTU		0 // Set to 1 to use older filesystem libraries
#define		DEBUG		0 // Set to 1 to print debug information to a terminal

Color backing = Color {.2, .2, .2, 1};
Color locationHeading = {.05f, .05f, .05f, .5};
Color locationFile = Color {.7, .7, .7, .25};
Color fieldBack = Color{.1, .1, .1, 1};
Color fontColor = White;
Color grid = Color {.25f, .25f, .25f, 1};
Color highlight = Color {.7, .7, .7, .75};
Color imageBorder = Color{.1, .1, .1, 1};
Color cOrigin = Color {1, 1, 1, .5};
Color menuBackground = Color {0, 0, 0, .5};

Color RMMenuBacking = Color {0, 0, 0, .5};

// Scroll bar
Color scrollbarBacking = Color {.2, .2, .2, .5};
Color scrollbarNotch = Color {.5, .5, .5, .5};
Color cSelector = Color{0,1,1,.1f};

// Import menu
Color importBacking = Color {.27, .27, .3, 1};
Color importButton = Color {.1, .1, .1, .9};

int borderSize = 6; // Size of the border on selected images
float fontSize = 24;
int gridSize = 128; // The grid in the background
const int minSize = 32; // Minimum image size
const int scrollbarSize = 32; 
const int sampleSize = 8; // Anti-Aliasing level
const int viewSpeed = 16; // How fast to move the view

bool drawOrigin = true;
bool maximize = true;

#ifdef _WIN32
	string fontPath = "C:\\Windows\\Fonts\\tahoma.ttf";
#else
	string fontPath = "/usr/share/fonts/truetype/freefont/FreeMono.ttf";
#endif
