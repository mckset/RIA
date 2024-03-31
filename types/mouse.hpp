class Mouse{
	public:	
		Vector2 position; // Cursor position
		Vector2 dragOff; // Offset from the mouse to the dragged image
		bool active = true; // If the mouse is in the window bounds
		bool drag; // If the mouse is held down
		int state, prevState;
		int gridX=0, gridY=0;
		bool clicked;
		bool hidden = false;

		bool Click(int s){return (state == s && prevState != s);}
		void Hide(GLFWwindow *w){glfwSetInputMode(w, GLFW_CURSOR, GLFW_CURSOR_HIDDEN); hidden = true;}
		void Show(GLFWwindow *w){glfwSetInputMode(w, GLFW_CURSOR, GLFW_CURSOR_NORMAL); hidden = false;}
};
Mouse mouse; 
