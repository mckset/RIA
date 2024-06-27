float defScale = 1.0f;

class Window{
	public:
		bool visible = true, scalable = true;
		unsigned int width, height; // Window width and height
		unsigned int vA, vB, eB; // Buffers used to draw to the screen
		GLFWwindow* w; // Window pointer
		Vector2 view; // Window view offset
		float scale = 1.0f; // Window scale

		// Render and input function pointers
		void (*Render)();
		void (*Input)();

		bool Focus(){return glfwGetWindowAttrib(w, GLFW_FOCUSED);}
		void Show(){glfwShowWindow(w); visible = true;}
		void Hide(){glfwHideWindow(w); visible = false;}
		void Toggle(){if (visible) Hide(); else Show();}
		void Resize(int Width, int Height){glfwSetWindowSize(w, Width, Height);}

		void Use(){
			glfwMakeContextCurrent(w);
			Width = width;
			Height = height;
			fWidth = (float)width;
			fHeight = (float)height;
			View = &view;
			Scale = (scalable ? &scale : &defScale);
			glViewport(0,0, Width, Height);
		}

		void Draw(Color clear = Color{0,0,0,1}){
			ShaderID = -1;

			if (!visible)
				return;
			Use();
			// Background clear
			glClearColor(clear.r,clear.g,clear.b,clear.a);

			// Set to draw buffer
			glClear(GL_COLOR_BUFFER_BIT);

			Render();
			if (Focus()){
				if (Input != nullptr) Input();
				mouse.prevState = mouse.state;
				keyboard.newKey = -1;
			}
			glfwSwapBuffers(w);
		}

		int Init(unsigned int wi=Width, unsigned int h=Height, bool vSync = true){
			// Check if window was created
			if (w == NULL){
				printf("Failed to create window\n");
				glfwTerminate();
				return -1;
			}
			glfwMakeContextCurrent(w);
				
			// Initialize GLAD
			if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
				printf("Failed to initialize GLAD\n");
				return -1;
			}
			printf("[Init] Created Window\n");

			// Sets OpenGL viewport
			glViewport(0, 0, wi, h);
			glfwSwapInterval(vSync); // Disable vSync

			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			// Set window flags
			glfwSetWindowAttrib(w, GLFW_RESIZABLE, GLFW_TRUE);

			printf("[Init] Set Flags\n");

			// Set event callbacks
			glfwSetErrorCallback(Error);
			glfwSetFramebufferSizeCallback(w, On_Resize);

			// Bind keyboard events
			glfwSetKeyCallback(w, SetKeyboardState);

			// Bind mouse events
			glfwSetMouseButtonCallback(w, SetMouseState);
			glfwSetCursorPosCallback(w, SetCursorPosition);

			printf("[Init] Bound Events\n");

			// Generate vertex array
			glGenVertexArrays(1, &vA);

			// Generate vertex buffer
			glGenBuffers(1, &vB);

			// Generate element buffer
			glGenBuffers(1, &eB);

			printf("[Init] Bound buffers\n");

			width = wi;
			height = h;
			glfwSetWindowSize(w, width, height);
			return 0;
		}
};