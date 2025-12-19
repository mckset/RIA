/*
	MOUSE.HPP

	The mouse class contains important information about the state of the mouse and the app runs.
	Most notably the position and click states of the mouse.
	ToScreeSpace() Is defined to provide an easy way to turn mouse coordinates to world coordinates as they relate to the view and scale of the window.
*/

class Mouse{
	public:	
		Vector2 position; // Cursor position
		Vector2 prevPosition; // Previous cursor position
		Vector2 dragOffset; // Offset from the mouse to the dragged image
		bool drag; // If the mouse is held down
		int state, prevState;
		bool hidden = false;
		int scrollX = 0; // Holds scrollwheel values to be used by a window
		int scrollY = 0; // Holds scrollwheel values to be used by a window

		bool Click(int s = LM_DOWN){return (state == s && prevState != s);}
		void Hide(GLFWwindow *w){glfwSetInputMode(w, GLFW_CURSOR, GLFW_CURSOR_HIDDEN); hidden = true;}
		void Show(GLFWwindow *w){glfwSetInputMode(w, GLFW_CURSOR, GLFW_CURSOR_NORMAL); hidden = false;}

		// Converts the mouses position to the worlds position offsetting for the view and scale of the window
		Vector2 ToScreenSpace(){
			return (position - Vector2{fWidth/2, fHeight/2}) / (*Scale) + Vector2{fWidth/2, fHeight/2} + (*View);
		}

		bool Within(Vector2 p, Vector2 s) {return position.Within(p, s);}
};
Mouse mouse; 

void SetMouseState(GLFWwindow* w, int button, int action, int mod){
	mouse.state = button*10 + action;
	if ((mouse.state != LM_DOWN && mouse.state != RM_DOWN) && mouse.drag)
		mouse.drag = false;
}