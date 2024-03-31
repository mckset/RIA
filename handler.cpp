//
// handler.cpp
//

/*
	Handles keyboard and mouse events to be used by input functions.
	All of the functions are general functions that happen independent of which window is being used.
*/
char KeyToChar(int);


static void Error(int e, const char* desc){
	fprintf(stderr, "Error #%d: %s\n", e, desc);
}

void GetScrollWheel(GLFWwindow* w, double x, double y){
	if (w != Import.w){
		if (!menu || mouse.position.x > Width/3){
			if (y < 0){ // Zoom out
				if (Scale > .1f)
					Scale -= .1f;
			}else{ // Zoom in
				if (Scale < 1.5f)
					Scale+=.1f;
			}
		}else{
			if (y < 0){
				if (tagView){
					tagScroll.scroll += 32;
					if (tagScroll.scroll > tagScroll.end)
						tagScroll.scroll = tagScroll.end;
				}else{
					locScroll.scroll += 32;
					if (locScroll.scroll > locScroll.end)
						locScroll.scroll = locScroll.end;
				}
			}else{
				if (tagView){
					tagScroll.scroll -= 32;
					if (tagScroll.scroll < 0)
						tagScroll.scroll = 0;
				}else{
					locScroll.scroll -= 32;
					if (locScroll.scroll < 0)
						locScroll.scroll = 0;
				}
			}
		}

	// Import window
	}else{
		if (y < 0){
			importScroll.scroll += 32;
			if (importScroll.scroll > importScroll.end)
				importScroll.scroll = importScroll.end;
		}else{
			importScroll.scroll -= 32;
			if (importScroll.scroll < 0)
				importScroll.scroll = 0;
		}
	}
}

char KeyToChar(int key){
	int c = key-32;
	if (keyboard.GetKey(KEY_LEFT_SHIFT))
		return upperKeyToChar[c];
	else
		return lowerKeyToChar[c];
}

void On_Resize(GLFWwindow* w, int width, int height){
	if (w == Main.w){
		Main.Use();
		Main.width = width;
		Main.height = height;
	}else if (w == TagWin.w){
		TagWin.Use();
		TagWin.width = width;
		TagWin.height = height;
	}else if (w == Import.w){
		Import.Use();
		Import.width = width;
		Import.height = height;
	}
	glViewport(0, 0, width, height);
}

void SetCursorPosition(GLFWwindow* w, double x, double y){
	int winX, winY;
	if (glfwGetWindowAttrib(Main.w, GLFW_FOCUSED))
		Main.Use();
	glfwGetWindowPos(w, &winX, &winY);
	mouse.position.x = (float)x;
	mouse.position.y = Height - (float)y;

}

void SetKeyboardState(GLFWwindow* w, int key, int code, int action, int mod){
	if (action == 1)
		keyboard.AddKey(key);
	else if (action == 0)
		keyboard.DelKey(key);
}

void SetMouseState(GLFWwindow* w, int button, int action, int mod){
	mouse.state = button*10 + action;
	if ((mouse.state != LM_DOWN && mouse.state != RM_DOWN) && mouse.drag)
		mouse.drag = false;
}

void SubClose(GLFWwindow *w){
	glfwSetWindowShouldClose(w, GL_FALSE);
	if (w == TagWin.w)
		TagWin.Hide();
	if (w == Import.w)
		Import.Hide();
}

