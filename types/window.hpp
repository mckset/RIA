static void Error(int, const char*);
void GetScrollWheel(GLFWwindow*, double, double);
void On_Resize(GLFWwindow*, int, int);
void SetCursorPosition(GLFWwindow*, double, double);
void SetKeyboardState(GLFWwindow*, int, int, int, int);
void SetMouseState(GLFWwindow*, int, int, int);
void SubClose(GLFWwindow*);

class Window{
	public:
		bool visible = true;
		unsigned int width, height; // Window width and height
		unsigned int vA, vB, eB; // Buffers used to draw to the screen
		GLFWwindow* w; // Window pointer

		bool Focus(){return glfwGetWindowAttrib(w, GLFW_FOCUSED);}
		void Show(){glfwShowWindow(w); visible = true;}
		void Hide(){glfwHideWindow(w); visible = false;}
		void Resize(int Width, int Height){glfwSetWindowSize(w, Width, Height);}

		void Use(Color clear = Color{0,0,0,1}){
			glfwMakeContextCurrent(w);
			Width = width;
			Height = height;
			glViewport(0,0, Width, Height);
			// Background clear
			glClearColor(clear.r,clear.g,clear.b,clear.a);

			// Set to draw buffer
			glClear(GL_COLOR_BUFFER_BIT);
		}

		int Init(unsigned int wi=Width, unsigned int h=Height){
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

			// Sets OpenGL viewport
			glViewport(0, 0, wi, h);
			glfwSwapInterval(1); // Enables vSync

			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			// Set window flags
			glfwSetWindowAttrib(w, GLFW_RESIZABLE, GLFW_TRUE);

			// Set event callbacks
			glfwSetErrorCallback(Error);
			glfwSetFramebufferSizeCallback(w, On_Resize);

			// Bind keyboard events
			glfwSetKeyCallback(w, SetKeyboardState);

			// Bind mouse events
			glfwSetMouseButtonCallback(w, SetMouseState);
			glfwSetCursorPosCallback(w, SetCursorPosition);

			// Generate vertex array
			glGenVertexArrays(1, &vA);

			// Generate vertex buffer
			glGenBuffers(1, &vB);

			// Generate element buffer
			glGenBuffers(1, &eB);

			visible = false;

			width = wi;
			height = h;
			glfwSetWindowSize(w, width, height);
			return 0;
		}
};