#include "include.hpp"

int CreateWindows();
void Init();

int CreateWindows(){
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, sampleSize);
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
	img.LoadImage((char*)"images/icon.png");
	if (!img.loaded)
		printf("[Warning] Unable to load icon image\n");
	else{
		GLFWimage i[1]; 
		stbi_set_flip_vertically_on_load(false);
		i[0].pixels = stbi_load("images/icon.png", &i[0].width, &i[0].height, 0, 4); //rgba channels 
		stbi_image_free(i[0].pixels);
		glfwSetWindowIcon(Main.w, 1, i); 
		stbi_set_flip_vertically_on_load(true);
	}
	
	glfwSetScrollCallback(Main.w, GetScrollWheel);
	glfwSetWindowMaximizeCallback(Main.w, Maximize);
	glfwSetDropCallback(Main.w, DragDrop);
	Main.visible = true;

	//
	// Tag Window
	//
	glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
	TagWin.w = glfwCreateWindow(Width/2, Height/2-42, "Tag", NULL, Main.w);
	TagWin.Init(Width/2, Height/2);
	glfwSetWindowCloseCallback(TagWin.w, SubClose);

	//
	// Import Window
	//
	glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
	Import.w = glfwCreateWindow(Width/4, Height/2-42, "Import/Create", NULL, Main.w);
	Import.Init(Width/2, Height/2);
	glfwSetWindowCloseCallback(Import.w, SubClose);
	glfwSetScrollCallback(Import.w, GetScrollWheel);
	return 1;
}

void Init(){
	char p[4096]; // 4096 is the max size path size for Linux, 256 for windows
	getcwd(p, sizeof(p));
	path = p;

	// Check for main folders
	struct stat st;

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
	Load();
	InitMenu();
	Main.width = Width;
	Main.height = Height;
	if (maximize)
		glfwMaximizeWindow(Main.w);
	else
		Main.Resize(Width, Height);

		
}

int main(){
	if (DEBUG) printf("[Initializing]\n");

	if (!CreateWindows()){
		if (DEBUG) printf("%s\n", path.data());
		if (DEBUG) printf("image/icon.png missing!\n");	
	}

	stbi_set_flip_vertically_on_load(true);
	Init();
	font.Load((char*)"images/font.png");
	if (!font.fontImg.loaded){
		if (DEBUG) printf("image/font.png missing!\n");
		return -1;	
	}
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	while(!glfwWindowShouldClose(Main.w)){
		glfwPollEvents();

		vertexArray = Main.vA;
		vertexBuffer = Main.vB;
		elementBuffer = Main.eB;

		if (Main.Focus() || DnD){
			Main.Use(backing); // Main window
			DrawApp(); // Draw
			glfwSwapBuffers(Main.w);// Swap buffer
			MainInput();
			if (DnD)
				DnD = false;
		}

		if (TagWin.Focus()){
			TagWin.Use(backing);
			DrawTag();
			glfwSwapBuffers(TagWin.w);
			TagInput();
		}
		if (Import.Focus()){
			Import.Use(importBacking);
			importText = DrawImport();
			glfwSwapBuffers(Import.w);
			keyboard.newKey = -1;
			mouse.prevState = mouse.state;
		}
	}

	return 0;
}

