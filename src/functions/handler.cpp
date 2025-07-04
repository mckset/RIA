/*
	Stores functions for handling window events such as keyboard/mouse events and scaling
*/

extern Scrollbar importScroll;

// Handles drag and drop functions
void DragDrop(GLFWwindow* w, int c, const char** paths){
	struct stat st;

	DnD = true;

	for (c; c > 0; c--){
		string p = paths[c-1];

		if (stat(paths[c-1], &st) == 0){ // Is valid

			// Append folder to location menu
			if (st.st_mode & S_IFDIR && lMenu && !tagView){
				locations.push_back(Table{GetName(p), p});
				sort(locations.begin(), locations.end(), Table{}.SortTable);
			// Load image
			}else if (st.st_mode & S_IFREG && IsImage(p)){
				Object img;
				img.img.LoadImage((char*) paths[c-1]);

				if (!img.img.loaded && Lower(p.substr(p.length()-4)) == "webp")
					img.img = LoadWebp(p);
				if (!img.img.loaded)
					return;

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
		}else{
			string fixedURL = "";
			string url = p;

			string formats[] = {
				"fm=jpg",
				"f=jpg",
				"format=jpg",
				"fm=jpeg",
				"f=jpeg",
				"format=jpeg",
				"f=webp",
				"fm=webp",
				"format=webp",
				"fm=png",
				"f=png",
				"format=png",
			};
			string f[] = {
				".jpg",
				".jpeg",
				".webp",
				".png"
			};

			for (char c : p)
				if (c == ' ')
					fixedURL += "%20";
				else
					fixedURL += c;
			string base = GetName(url);

			for (int i = 0; i < 12; i++){
				int x = base.find(formats[i], 0);
				if (x != string::npos){
					base = base.substr(0,x-1) + f[i/3];
					break;
				}
			}

			for (int i = base.length()-1; i > 0; i--)
				if (base[i] == '.')
					if (base.substr(i+1,4) != "webp" && base.substr(i+1,4) != "jpeg"){
						base = base.substr(0,i+4);
						break;
					}else{
						base = base.substr(0,i+5);
						break;
					}
			string file = base;
			string name = RemoveExt(base);
			
			if (file.length() < 5 || !IsImage(base.substr(name.length()))) return;
			int i = 1;
			while (stat((path + "downloads" + slash[0] + file).c_str(), &st) == 0){
				file = name + "_" + to_string(i) + base.substr(name.length());
				i++;
			}
			if (DEBUG) printf("Dropped: %s from %s\n", file.data(), fixedURL.data());
			string cmd = "curl -s \"" + fixedURL + "\" > \"" + path + "downloads" + slash[0] + file + "\"";
			system(cmd.c_str());
			Object img;
			if (file.substr(file.length()-4) != "webp")
				img.img.LoadImage(path + "downloads" + slash[0] + file);
			else
				img.img = LoadWebp(path + "downloads" + slash[0] + file);

			img.size = Vector2{(float)img.img.width, (float)img.img.height};
			img.position = mouse.ToScreenSpace() - img.size/2;
			img.path = p;
			imgs.push_back(img);
		}
	}
}

// Reports GLFW errors
static void Error(int e, const char* desc){
	if (DEBUG) fprintf(stderr, "Error #%d: %s\n", e, desc);
}

// Handles scrollwheel events
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

// Shifts a character to upper case when shift is held
char KeyToChar(int key){
	int c = key-32;
	if (keyboard.GetKey(KEY_LEFT_SHIFT) || keyboard.GetKey(KEY_RIGHT_SHIFT))
		return UpperMap[c];
	else
		return LowerMap[c];
}

// Maximizes a window
void Maximize(GLFWwindow* w, int max){ maximize = max; }

// Handles resize events
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

// Updates mouse cursor position
void SetCursorPosition(GLFWwindow* w, double x, double y){
	if (Main.Focus()) Main.Use();
	else if (TagWin.Focus()) TagWin.Use();
	else if (Import.Focus()) Import.Use();
	mouse.position.x = (float)x;
	mouse.position.y = Height - (float)y;

}

// Handles keyboard events
void SetKeyboardState(GLFWwindow* w, int key, int code, int action, int mod){
	if (action == 1)
		keyboard.AddKey(key);
	else if (action == 0)
		keyboard.DelKey(key);
}

// Get mouse button events
void SetMouseState(GLFWwindow* w, int button, int action, int mod){
	mouse.state = button*10 + action;
	if (mouse.state != LM_DOWN && mouse.state != RM_DOWN && mouse.drag)
		mouse.drag = false;
}

// Hides sub windows
void SubClose(GLFWwindow *w){
	glfwSetWindowShouldClose(w, GL_FALSE);
	if (w == TagWin.w)
		TagWin.Hide();
	if (w == Import.w)
		Import.Hide();
}