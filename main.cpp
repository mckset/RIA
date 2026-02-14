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
	glfwWindowHint(GLFW_SAMPLES, QUALITY);
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
	int err = Main.Init();
	if (err) return err;

	// Set window handler events
	//glfwSetScrollCallback(Main.w, GetScrollWheel);
	glfwSetWindowMaximizeCallback(Main.w, Maximize);
	glfwSetDropCallback(Main.w, DragDrop);

	

	// Window variables
	Main.visible = true;
	Main.Render = &DrawMain;
	Main.Input = &MainInput;

	//
	// Tag Window
	//

	// Window flags
	glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	TagWin.w = glfwCreateWindow(TAG_COLOR_SELECTOR_SIZE+PADDING*3.5, TAG_COLOR_SELECTOR_SIZE+FONT_SIZE*3+PADDING*2, "Tag", NULL, Main.w);
	err = TagWin.Init(TAG_COLOR_SELECTOR_SIZE+PADDING*3.5, TAG_COLOR_SELECTOR_SIZE+FONT_SIZE*3+PADDING*2);
	if (err) return err;

	// Hide window instead of closing it
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

	Import.w = glfwCreateWindow(360, 480, "Import/Create", NULL, Main.w);
	err = Import.Init(360, 480);
	if (err) return err;

	// Hide window instead of closing it
	glfwSetWindowCloseCallback(Import.w, SubClose);
	//glfwSetScrollCallback(Import.w, GetScrollWheel);

	// Window variables
	Import.Render = &DrawImportMain;
	Import.Input = &MainImportInput;
	Import.Hide();
	
	//
	// Download Window
	//

	// Window flags
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

	DownloadWin.w = glfwCreateWindow(480, 160, "Download Image", NULL, Main.w);
	err = DownloadWin.Init(480, 160);
	if (err) return err;

	// Hide window instead of closing it
	glfwSetWindowCloseCallback(DownloadWin.w, SubClose);

	// Window variables
	DownloadWin.Render = &DrawDownloader;
	DownloadWin.Input = &DownloaderInput;
	DownloadWin.Hide();


	//
	// Warning window
	//

	// Window flags
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

	WarningWin.w = glfwCreateWindow(480, 200, "Warning", NULL, Main.w);
	err = WarningWin.Init(480, 200);
	if (err) return err;

	// Hide window instead of closing it
	glfwSetWindowCloseCallback(WarningWin.w, SubClose);

	// Window variables
	WarningWin.Render = &DrawWarning;
	WarningWin.Input = &WarningInput;
	WarningWin.Hide();

	//
	// Board window
	//

	// Window flags
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

	BoardWin.w = glfwCreateWindow(480, 160, "Image Board", NULL, Main.w);
	err = BoardWin.Init(480, 160);
	if (err) return err;

	// Hide window instead of closing it
	glfwSetWindowCloseCallback(BoardWin.w, SubClose);

	// Window variables
	BoardWin.Render = &DrawBoardWindow;
	BoardWin.Input = &BoardWindowInput;
	BoardWin.Hide();

	return 0;
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

	FindBoards();

	// User is running a wayland desktop
	if (LINUX && getenv("WAYLAND_DISPLAY"))
		WAYLAND = true;

	FPSLimit = 120;

	cursors["Normal"] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
	cursors["Point"] = glfwCreateStandardCursor(GLFW_POINTING_HAND_CURSOR);
	cursors["Text"] = glfwCreateStandardCursor(GLFW_IBEAM_CURSOR);

	tutorialLocation.folders = {
		Location{"Sub folder 1", "", true, true, 0},
		Location{"Sub folder 2", "", false, true}
	};


	tutorialLocation.folders[0].files = {
		File{"Sub folder file", ""}
	};

	tutorialLocation.files = {
		File{"File 1"},
		File{"File 2"}
	};

	tutorialTag.files = {
		File{"Tagged image 1"},
		File{"Tagged image 2"}
	};

	tutorialTag.subTags = {
		Tag{"Sub tag", Blue, true}
	};

	tutorialTag.subTags[0].files = {File{"Tagged image 1"}};

	boardScreenshot = (unsigned char*)malloc(480*480*3);

	for (int i = 0; i < 480*480*3; i++){
		boardScreenshot[i] = (unsigned char)51;
	}
	tutorialBoard.img.width = 480;
	tutorialBoard.img.height = 480;

	SetTextureRBG(boardScreenshot, &tutorialBoard.img);

}

int main(int argCount, char** argValues){

	// Get application directory
	if (LINUX){
		if (WAYLAND) putenv("XDG_SESSION_TYPE=x11");
		path = usrHome;

		if (argCount > 1){
			if (strcmp(argValues[1], "-l") != 0)
				path += "/.local/share/RIA/";
			else if (strcmp(argValues[1], "-p") && argCount > 2){
				if (stat(argValues[2], &st) == 0){
					path = argValues[2];
				}else if (stat((path + argValues[2]).c_str(), &st) == 0)
					path += argValues[2];
				else{
					printf("Invalid path: %s\n", (path + argValues[2]).data());
					return -1;
				}
			}else if (strcmp(argValues[1], "-f")){
				printf("Usage: RIA [option]\n\n");
				printf("\t-h\t\tShow this message\n");
				printf("\t-l\t\tUse the current folder\n");
				printf("\t-p [path]\tUse the path\n");
			}
		}else
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
	int err = CreateWindows();
	if (err){
		printf("Error creating application windows: %d\n", err);
		return -1;	
	}

	// Initialize variables
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
		Main.Draw(backgroundColor);
		TagWin.Draw(backgroundColor);
		Import.Draw(backgroundColor);
		DownloadWin.Draw(backgroundColor);
		WarningWin.Draw(backgroundColor);
		BoardWin.Draw(backgroundColor);

		// Loads after drawing the window to show the application did open
		if (!loadedSave){
			loadThread = new thread(Load);
			string folder = "downloads";
			if (!stat(folder.c_str(), &st) == 0){
				if(!fs::create_directory(folder))
					if (DEBUG) printf("Failed to create downloads folder");
			}
		}

		if (loadedSave == LOAD_FINISHED && loadThread && loadThread->joinable()){
			loadThread->join();
			free(loadThread);
			loadThread = nullptr;
			for (int i = 0; i < textCache.size(); i++)
				CheckString(textCache[i]);
			textCache.clear();
		}

		if (loadedImages.size() && loadedImages[0].loaded){
			imgs.push_back(loadedImages[0]);
			imgs.back().SetTexture();
			loadedImages.erase(loadedImages.begin());
		}

		if (imageBoardThread && loadedSave == LOAD_BOARDS_FINISHED && imageBoardThread->joinable()){
			imageBoardThread->join();
			free(imageBoardThread);
			imageBoardThread = nullptr;
		}

		GetFPS();
	}

	return 0;
}

