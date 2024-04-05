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
	Image img;
	img.LoadImage((char*)"images/icon.png");
	if (!img.loaded)
		return 0;
	GLFWimage i[1]; 
	stbi_set_flip_vertically_on_load(false);
	i[0].pixels = stbi_load("images/icon.png", &i[0].width, &i[0].height, 0, 4); //rgba channels 
	glfwSetWindowIcon(Main.w, 1, i); 
	stbi_image_free(i[0].pixels);
	glfwSetScrollCallback(Main.w, GetScrollWheel);
	glfwSetWindowMaximizeCallback(Main.w, Maximize);
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
	getcwd(p, sizeof(p));
	path = p;

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
	printf("[Initializing]\n");

	if (!CreateWindows()){
		printf("%s\n", path.data());
		printf("image/icon.png not missing!\n");	
		return -1;
	}
	Init();
	font.Load((char*)"images/font.png");
	if (!font.fontImg.loaded){
		printf("image/font.png not missing!\n");
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

		if (Main.Focus()){
			Main.Use(backing); // Main window
			DrawApp(); // Draw
			glfwSwapBuffers(Main.w);// Swap buffer
			MainInput();
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

