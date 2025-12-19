/*
	Stores functions for handling window events such as keyboard/mouse events and scaling
*/

extern Scrollbar import_Scrollbar;

// Handles drag and drop functions
void DragDrop(GLFWwindow* w, int c, const char** paths){
	struct stat st;

	for (c; c > 0; c--){
		string p = paths[c-1];

		// Adds folders
		if (stat(paths[c-1], &st) == 0){ // Is valid

			// Append folder to location menu
			if (st.st_mode & S_IFDIR && showLeftMenu && !showTags){
				locations.push_back(Location{GetName(p), p});
				sort(locations.begin(), locations.end(), Location{}.SortLocations);
				
			// Load image
			}else if (st.st_mode & S_IFREG && IsImage(p)){
				ImageContainer img;
				img.Load(p);

				if (!img.img.loaded){
					statusText = "Unable to add: " + p;
					statusText = STATUS_TIME;
					continue;
				}

				// Magic math that puts the mouse into screen space
				img.size = Vector2{(float)img.img.width, (float)img.img.height};
				img.position = mouse.ToScreenSpace() - img.size/2;
				img.path = p;

				imgs.push_back(img);
			
			// Load image board
			}else if (st.st_mode & S_IFREG && p.substr(p.length()-4) == ".brd" && p.length() > path.length()+8){
				board = p.substr(path.length()+8);
				if (loadThread || imageBoardThread){
					closeThread = true;

					if (loadThread){
						loadThread->join();
						free(loadThread);
						loadThread = nullptr;
						for (int i = 0; i < textCache.size(); i++)
							CheckString(textCache[i]);
						textCache.clear();
					}else{
						imageBoardThread->join();
						free(imageBoardThread);
					}
					closeThread = false;
				}
				loadedImages.clear();
                imageBoardThread = new thread(LoadImageBoard);
				return;
			}
		}else{
			string fixedURL = "";

			for (char c : p)
				if (c == ' ')
					fixedURL += "%20";
				else
					fixedURL += c;

			if (DEBUG) printf("Downloading from: %s\n", fixedURL.data());
			string cmd = "curl -s \"" + fixedURL + "\" > \"" + path + "downloads" + slash[0] + "_downloaded.tmp\"";
			system(cmd.c_str());
			GetDownloadedFileType();

			if (downloadedImageType != INVALID)
				DownloadWin.Show();
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

	if (w == Import.w){
		if (y < 0){
			import_Scrollbar.scroll += 32;
			if (import_Scrollbar.scroll > import_Scrollbar.end)
				import_Scrollbar.scroll = import_Scrollbar.end;
		}else{
			import_Scrollbar.scroll -= 32;
			if (import_Scrollbar.scroll < 0)
				import_Scrollbar.scroll = 0;
		}
		return;
	}

	// Board zoom
	if ((!showLeftMenu || mouse.position.x > sideMenuWidth+SCROLLBAR_SIZE) && (!showRightMenu || mouse.position.x < fWidth-sideMenuWidth-SCROLLBAR_SIZE)){
		zoomTextTimer = 60;
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

	// Side menu scrolling
	}else{
		if (y < 0){
			// Left menu
			if (mouse.position.x <= sideMenuWidth+SCROLLBAR_SIZE)
				if (showTags){
					tags_Scrollbar.scroll += 32;
					if (tags_Scrollbar.scroll > tags_Scrollbar.end)
						tags_Scrollbar.scroll = tags_Scrollbar.end;
					return;

				}else{
					locations_Scrollbar.scroll += 32;
					if (locations_Scrollbar.scroll > locations_Scrollbar.end)
						locations_Scrollbar.scroll = locations_Scrollbar.end;
					return;
				}
			
			// Right menu
			board_Scrollbar.scroll += 64;
			if (board_Scrollbar.scroll > board_Scrollbar.end)
				board_Scrollbar.scroll = board_Scrollbar.end;
			

		}else{
			// Left menu
			if (mouse.position.x <= sideMenuWidth+SCROLLBAR_SIZE)
				if (showTags){
					tags_Scrollbar.scroll -= 32;
					if (tags_Scrollbar.scroll < 0)
						tags_Scrollbar.scroll = 0;
					return;

				}else{
					locations_Scrollbar.scroll -= 32;
					if (locations_Scrollbar.scroll < 0)
						locations_Scrollbar.scroll = 0;
					return;
				}

			// Right menu
			board_Scrollbar.scroll -= 64;
			if (board_Scrollbar.scroll < 0)
				board_Scrollbar.scroll = 0;
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
void Maximize(GLFWwindow* w, int max){  }

// Handles resize events
void OnResize(GLFWwindow* w, int width, int height){
	if (w == Main.w){
		Main.Use();
		Main.width = width;
		Main.height = height;
		sideMenuWidth = width*sideMenuRatio;
	}else if (w == TagWin.w){
		TagWin.Use();
		TagWin.width = width;
		TagWin.height = height;
	}else if (w == Import.w){
		Import.Use();
		Import.width = width;
		Import.height = height;
	}else if (w == DownloadWin.w){
		DownloadWin.Use();
		DownloadWin.width = width;
		DownloadWin.height = height;
	}
	glViewport(0, 0, width, height);
}

// Updates mouse cursor position
void SetCursorPosition(GLFWwindow* w, double x, double y){
	if (Main.w == w)
		Main.Use();
	else if (TagWin.w == w)
		TagWin.Use();
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
	
	if (w == DownloadWin.w)
		DownloadWin.Hide();
}