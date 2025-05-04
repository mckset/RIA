/*
	handler.cpp

	Stores functions for handling window events such as keyboard/mouse events and scaling
*/

char KeyToChar(int);

extern Scrollbar importScroll;

//
// Functions to handle window actions
//
void DragDrop(GLFWwindow* w, int c, const char** paths){
	struct stat st;

	DnD = true;

	for (c; c > 0; c--){
		string p = paths[c-1];
		if (stat(paths[c-1], &st) == 0){ // Is valid

			// Append folder to location menu
			if (st.st_mode & S_IFDIR && lMenu && !tagView)
				locations.push_back(Table{GetName(p), p});

			// Load image
			else if (st.st_mode & S_IFREG && IsImage(p)){
				Object img;
				img.img.LoadImage((char*) paths[c-1]);

				// Magic math that puts the mouse into screen space
				img.size = Vector2{(float)img.img.width, (float)img.img.height};
				img.position = mouse.ToScreenSpace() - img.size/2;
				img.path = p;

				imgs.push_back(img);
			
			// Load image board
			}else if (st.st_mode & S_IFREG && !strcmp(p.substr(p.length()-4).data(), ".brd")){
				board = p.substr(path.length()+8);
				LoadImageBoard();
				return;
			}
		}
	}
	sort(locations.begin(), locations.end(), Table{}.SortTable);
}

static void Error(int e, const char* desc){
	if (DEBUG) fprintf(stderr, "Error #%d: %s\n", e, desc);
}

void Maximize(GLFWwindow* w, int max){
	maximize = max;
}

void On_Resize(GLFWwindow* w, int width, int height){
	if (w == Main.w){
		Main.Use();
		Main.width = width;
		Main.height = height;
		menuWidth = width/6;
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

void SubClose(GLFWwindow *w){
	glfwSetWindowShouldClose(w, GL_FALSE);
	if (w == TagWin.w)
		TagWin.Hide();
	if (w == Import.w)
		Import.Hide();
}


//
// Functions to handle mouse actions
//

void GetScrollWheel(GLFWwindow* w, double x, double y){
	if (showTutorial){
		*Scale = 1;
		return;
	}
	if (w != Import.w){
		if ((!lMenu || mouse.position.x > menuWidth+scrollbarSize) && (!rMenu || mouse.position.x < fWidth-menuWidth-scrollbarSize)){
			showZoom = 60;
			if (y < 0){ // Zoom out
				if (*Scale > 1.5f)
					*Scale -= .3f;
				else if (*Scale > .11f)
					*Scale -= .1f;
			}else{ // Zoom in
				if (*Scale < 1.5f)
					*Scale+=.1f;
				else if (*Scale < 3)
					*Scale += .3f;
			}
		}else{
			if (y < 0){
				// Left menu
				if (mouse.position.x <= menuWidth+scrollbarSize)
					if (tagView){
						tagScroll.scroll += 32;
						if (tagScroll.scroll > tagScroll.end)
							tagScroll.scroll = tagScroll.end;
					}else{
						locScroll.scroll += 32;
						if (locScroll.scroll > locScroll.end)
							locScroll.scroll = locScroll.end;
					}

				// Right menu
				else{
					boardScroll.scroll += 32;
					if (boardScroll.scroll > boardScroll.end)
						boardScroll.scroll = boardScroll.end;
				}
			}else{
				// Left menu
				if (mouse.position.x <= menuWidth+scrollbarSize)
					if (tagView){
						tagScroll.scroll -= 32;
						if (tagScroll.scroll < 0)
							tagScroll.scroll = 0;
					}else{
						locScroll.scroll -= 32;
						if (locScroll.scroll < 0)
							locScroll.scroll = 0;
					}
				
				// Right menu
				else{
					boardScroll.scroll -= 32;
					if (boardScroll.scroll < 0)
						boardScroll.scroll = 0;
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


void SetCursorPosition(GLFWwindow* w, double x, double y){
	if (Main.Focus()) Main.Use();
	else if (TagWin.Focus()) TagWin.Use();
	else if (Import.Focus()) Import.Use();
	mouse.position.x = (float)x;
	mouse.position.y = Height - (float)y;

}

void SetMouseState(GLFWwindow* w, int button, int action, int mod){
	mouse.state = button*10 + action;
	if (mouse.state != LM_DOWN && mouse.state != RM_DOWN && mouse.drag)
		mouse.drag = false;
}

//
// Functions to handle keyboard actions
//

// Mostly used to apply upperchar shifting
char KeyToChar(int key){
	int c = key-32;
	if (keyboard.GetKey(KEY_LEFT_SHIFT))
		return UpperMap[c];
	else
		return LowerMap[c];
}


void SetKeyboardState(GLFWwindow* w, int key, int code, int action, int mod){
	if (action == 1)
		keyboard.AddKey(key);
	else if (action == 0)
		keyboard.DelKey(key);
}




