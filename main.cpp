#include "include.hpp"

int CreateWindows();
void Init();

int CreateWindows(){
	// Initialize GLFW
	glfwInit();

	// Set window flags
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, sampleSize);
	glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);

	// Get monitor resolution
	mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	// Windowed fullscreen
	Width = mode->width;
	Height = mode->height-42;

	//
	// Main Window
	//
	Main.w = glfwCreateWindow(Width, Height-42, "RIA", NULL, NULL);
	Main.Init();

	// Set window handler events
	glfwSetScrollCallback(Main.w, GetScrollWheel);
	glfwSetWindowMaximizeCallback(Main.w, Maximize);
	glfwSetDropCallback(Main.w, DragDrop);

	// Window variables
	Main.visible = true;
	Main.Render = &DrawApp;
	Main.Input = &MainInput;

	//
	// Tag Window
	//

	// Window flags
	glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	TagWin.w = glfwCreateWindow(360, 256+fontSize*3, "Tag", NULL, Main.w);
	TagWin.Init(360, 256+fontSize*3);

	// Hides window instead of closing it
	glfwSetWindowCloseCallback(TagWin.w, SubClose);

	// Window variables
	TagWin.Render = &DrawTag;
	TagWin.Input = &TagInput;
	TagWin.Hide();

	//
	// Import Window
	//

	// Window flags
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

	Import.w = glfwCreateWindow(256, 360, "Import/Create", NULL, Main.w);
	Import.Init(256, 360);
	
	// Hides window instead of closing it
	glfwSetWindowCloseCallback(Import.w, SubClose);
	glfwSetScrollCallback(Import.w, GetScrollWheel);

	// Window variables
	Import.Render = &DrawImportMain;
	Import.Input = &MainImportInput;
	Import.Hide();

	return 1;
}


void Init(){
	// Images are loaded flipped otherwise
	stbi_set_flip_vertically_on_load(true);

	// Create shared folder in the application directory
	string folder = "shared";
	if (!stat(folder.c_str(), &st) == 0)
		if(!fs::create_directory(folder))
			if (DEBUG) printf("Failded to create shared folder");

	// Create boards folder in the application directory
	folder = "boards";
	if (!stat(folder.c_str(), &st) == 0)
		if(!fs::create_directory(folder))
			if (DEBUG) printf("Failded to create boards folder");
	
	// Load shaders
	sImage.Init(imageV, imageF);
	sShape.Init(shapeV, shapeF);
	sFont.Init(fontV, fontF);
	sColorSelector.Init(shapeV, colorSelectorF);
	sHueSelector.Init(shapeV, hueSelectorF);
	sCircle.Init(shapeV, circleF);

	// Check for valid fonts on different Linux distros
	if (LINUX){
		fontPath = "";
		for (int f = 0; f < 4; f++)
	    if (stat(fonts[f].c_str(), &st) == 0){
			fontPath = fonts[f];
			break;
		}

		if (!fontPath.length())
			if (DEBUG) printf("Font could not be loaded\n");
	}

	// Loads font
	if (FT_Init_FreeType(&ft))
		printf("Error: Unable to load font library");
	if (FT_New_Face(ft, fontPath.data(), 0, &face))
		printf("Error: Unable to load font %s\n", fontPath.data());
	else
		font.loaded = true;
	InitFont();

	// Create the right click menu
	InitMenu();

	// User is running a wayland desktop
	if (LINUX && getenv("WAYLAND_DISPLAY"))
		WAYLAND = true;
}

int main(int argCount, char** argValues){

	// Get application directory
	if (LINUX){
		path = usrHome;
		path += "/.local/share/RIA/";
		if (!stat(path.c_str(), &st) == 0)
			if(!fs::create_directory(path)){
				if (DEBUG) printf("Failded to create app directoty");
				return -1;
			}
		fs::current_path(path);
	}else{
		path = argValues[0];
		string exe = GetName(path);
		path = path.substr(0,path.length()-exe.length());
	}
	
	if (DEBUG) printf("Application path: %s\n", path.data());

	// Hides the scary console on Windows devices
	if (!LINUX && !DEBUG) HideConsole();
	if (DEBUG) printf("[Initializing]\n");

	// Creates the 3 windows (main, tag edit, import) used by RIA
	if (!CreateWindows() && DEBUG){
		printf("%s\n", path.data());
		printf("image/icon.png missing!\n");	
	}

	// Initialize other variables
	Init();

	// Set rendering flags
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Main loop until main window requests to close
	while(!glfwWindowShouldClose(Main.w)){

		// Get mouse and keyboard events
		glfwPollEvents();

		// Sets GPU variables
		vertexArray = Main.vA;
		vertexBuffer = Main.vB;
		elementBuffer = Main.eB;

		// Draws each window only if they are visible
		Main.Draw(backing);
		TagWin.Draw(backing);
		Import.Draw(backing);

		// Loads after drawing the window to show the application did open
		if (!loaded){
			Load();
			string folder = "downloads";
			if (!stat(folder.c_str(), &st) == 0){
				if(!fs::create_directory(folder))
					if (DEBUG) printf("Failded to create downloads folder");
			}
		}
	}

	return 0;
}

