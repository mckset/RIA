#define 	UBUNTU		0 // Set to 1 to use older filesystem libraries

Color backing = Color {.2, .2, .2, 1};
Color locationHeading = {.05f, .05f, .05f, .5};
Color fieldBack = Color{.1, .1, .1, 1};
Color fontColor = White;
Color grid = Color {.25f, .25f, .25f, 1};
Color highlight = Color {.5, .5, .5, .75};
Color imageBorder = Color{.1, .1, .1, 1};
Color cOrigin = Color {1, 1, 1, .5};
Color menuBackground = Color {0, 0, 0, .5};
Color scrollbarBacking = Color {.2, .2, .2, .5};
Color scrollbarNotch = Color {.5, .5, .5, .5};
Color cSelector = Color{0,1,1,.1f};

// Import menu
Color importBacking = Color {.27, .27, .3, 1};
Color importButton = Color {.1, .1, .1, .9};

const int borderSize = 6; // Size of the border on selected images
const int fontSize = 32;
const int gridSize = 128; // The grid in the background
const int minSize = 32; // Minimum image size
const int scrollbarSize = 32; 
const int sampleSize = 8; // Anti-Aliasing level

bool drawOrigin = true;