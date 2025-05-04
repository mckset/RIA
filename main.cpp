#include "include.hpp"

int CreateWindows();
void Init();

int CreateWindows(){
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, sampleSize);
	glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
	mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	Width = mode->width;
	Height = mode->height-42;

	//
	// Main Window
	//
	Main.w = glfwCreateWindow(Width, Height-42, "RIA", NULL, NULL);
	Main.Init();

	// Checks if the icon image exists
	Image img;
	img.LoadImage("images/icon.png");
	if (!img.loaded)
		if (DEBUG) printf("[Warning] Unable to load icon image\n");
	else{
		GLFWimage i[1]; 
		stbi_set_flip_vertically_on_load(false);
		i[0].pixels = stbi_load("images/icon.png", &i[0].width, &i[0].height, 0, 4); //rgba channels 
		glfwSetWindowIcon(Main.w, 1, i); 
		stbi_image_free(i[0].pixels);
		stbi_set_flip_vertically_on_load(true);
	}
	
	glfwSetScrollCallback(Main.w, GetScrollWheel);
	glfwSetWindowMaximizeCallback(Main.w, Maximize);
	glfwSetDropCallback(Main.w, DragDrop);
	Main.visible = true;
	Main.Render = &DrawApp;
	Main.Input = &MainInput;

	//
	// Tag Window
	//
	glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	TagWin.w = glfwCreateWindow(360, 256+fontSize*3, "Tag", NULL, Main.w);
	TagWin.Init(360, 256+fontSize*3);
	glfwSetWindowCloseCallback(TagWin.w, SubClose);

	TagWin.Render = &DrawTag;
	TagWin.Input = &TagInput;
	TagWin.Hide();

	//
	// Import Window
	//
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
	Import.w = glfwCreateWindow(256, 360, "Import/Create", NULL, Main.w);
	Import.Init(256, 360);
	glfwSetWindowCloseCallback(Import.w, SubClose);
	glfwSetScrollCallback(Import.w, GetScrollWheel);
	Import.Render = &DrawImportMain;
	Import.Input = &MainImportInput;
	Import.Hide();

	return 1;
}


void Init(){
	char p[4096]; // 4096 is the max size path size for Linux, 256 for windows
	getcwd(p, sizeof(p));
	path = p;

	string folder = "shared";
	if (!stat(folder.c_str(), &st) == 0)
		if(!fs::create_directory(folder))
			if (DEBUG) printf("Failded to create shared folder");

	folder = "boards";
	if (!stat(folder.c_str(), &st) == 0)
		if(!fs::create_directory(folder))
			if (DEBUG) printf("Failded to create boards folder");
	
	sImage.Init(imageV, imageF);
	sShape.Init(shapeV, shapeF);

	sFont.Init(fontV, fontF);
	sColorSelector.Init(shapeV, colorSelectorF);
	sHueSelector.Init(shapeV, hueSelectorF);
	sCircle.Init(shapeV, circleF);

	if (FT_Init_FreeType(&ft))
		printf("Error: Unable to load font library");
	if (FT_New_Face(ft, fontPath.data(), 0, &face))
		printf("Error: Unable to load font %s\n", fontPath.data());

	InitFont();
	InitMenu();
	
	Main.width = Width;
	Main.height = Height;
	if (maximize)
		glfwMaximizeWindow(Main.w);
	else
		Main.Resize(Width, Height);
}

int main(){
	if (!LINUX) ShowWindow(GetConsoleWindow(), SW_HIDE);
	if (DEBUG) printf("[Initializing]\n");

	if (!CreateWindows() && DEBUG){
		printf("%s\n", path.data());
		printf("image/icon.png missing!\n");	
	}

	stbi_set_flip_vertically_on_load(true);
	Init();
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	
	while(!glfwWindowShouldClose(Main.w)){
		glfwPollEvents();

		vertexArray = Main.vA;
		vertexBuffer = Main.vB;
		elementBuffer = Main.eB;

		Main.Draw(backing);
		TagWin.Draw(backing);
		Import.Draw(backing);
		if (!loaded)
			Load();
		
		GetDeltaTime();
	}

	return 0;
}

