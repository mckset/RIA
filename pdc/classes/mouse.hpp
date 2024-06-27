/*
	MOUSE.HPP

	The mouse class contains important information about the state of the mouse and the app runs.
	Most notably the position and click states of the mouse.
	ToScreeSpace() Is defined to provide an easy way to turn mouse coordinates to world coordinates as they relate to the view and scale of the window.
*/

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

		// Converts the mouses position to the worlds position offsetting for the view and scale of the window
		Vector2 ToScreenSpace(){
			return position.Subtract(Width/2, Height/2) // Removes 1/2 of the window size because window (0, 0) is center of the window
				.Divide(*Scale)
				.Add(Width/2, Height/2) 
				.Add(*View);}
};
Mouse mouse; 
