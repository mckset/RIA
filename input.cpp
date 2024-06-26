//
// Responsible for handling mouse and keyboard events in the main window
//

void MainInput();
void Paste();
void ReorderImages();
void ResetImages();
Vector2 MouseToScreenSpace(Vector2);

void MainInput(){
	shift = keyboard.GetKey(KEY_LEFT_SHIFT);

	// Toggle origin
	if (keyboard.newKey == KEY_SPACE)
		drawOrigin = !drawOrigin;
	

	// Saving
	if (keyboard.GetKey(KEY_S) && keyboard.GetKey(KEY_LEFT_CONTROL)){
		previewImg.loaded = false;
		for (auto img : selImgs)
			imgs[img].angle = imgs[img].prevAngle;
		rot = false;
		ResetImages();
		TagWin.Hide();
		rmMenu.Reset();
		Save();
		importTime = 0;
	}

	// Reset selected list
	if (keyboard.newKey == KEY_ESCAPE){
		previewImg.loaded = false;
		for (auto img : selImgs)
			imgs[img].angle = imgs[img].prevAngle;
		rot = false;
		ResetImages();
		TagWin.Hide();
		rmMenu.Reset();
		importTime = 0;
	}

	// Menu toggle
	if ((keyboard.newKey == KEY_TAB) || mouse.Click(LM_DOWN) && openLoc.Hover()){
		if (!menu)
			menu = true;
		else if (menu && !tagView)
			menu = false;
		tagView = false;
		rmMenu.Reset();
		mouse.prevState = mouse.state;
	}

	// Tag toggle
	if ((keyboard.newKey == KEY_TILDE) || mouse.Click(LM_DOWN) && openTags.Hover()){
		if (!menu && !tagView)
			menu = true;
		else if (menu && tagView)
			menu = false;
		tagView = !tagView;
		rmMenu.Reset();
		mouse.prevState = mouse.state;
	}

	// Close Menu
	if (mouse.Click(LM_DOWN) && menu && closeB.Hover()){
		menu = false;
		rmMenu.Reset();
		mouse.prevState = mouse.state;
	}
	
	// Help button
	if (mouse.Click(LM_DOWN) && !menu && helpB.Hover()){
		OpenHelp();
		mouse.prevState = mouse.state;
	}
	

	// Movement
	if (keyboard.GetKey(KEY_W) || keyboard.GetKey(KEY_UP))
		View->y += viewSpeed;
	if ((keyboard.GetKey(KEY_S) && !keyboard.GetKey(KEY_LEFT_CONTROL)) || keyboard.GetKey(KEY_DOWN))
		View->y -= viewSpeed;
	if (keyboard.GetKey(KEY_A) || keyboard.GetKey(KEY_LEFT))
		View->x -= viewSpeed;
	if (keyboard.GetKey(KEY_D) || keyboard.GetKey(KEY_RIGHT))
		View->x += viewSpeed;


	// Scaling
	if (keyboard.newKey == KEY_KP_ADD || keyboard.newKey == KEY_EQUAL)
		GetScrollWheel(Main.w, 0, 1);
	else if (keyboard.newKey == KEY_MINUS || keyboard.newKey == KEY_KP_SUBTRACT)
		GetScrollWheel(Main.w, 0, -1);


	//
	// Side bar stuff (Most input for the side bar is in table.hpp and tag.hpp)
	//
	if (menu && mouse.position.x < Width/3){
		if (keyboard.newKey == KEY_KP_ADD || keyboard.newKey == KEY_EQUAL || add.Hover() * mouse.Click(LM_DOWN)){
			if (tagView){
				editTag = -1;
				TagWin.Show();
			}else{
				string folder = GetFolder();
				if (folder.length() && !((stat(folder.c_str(), &s) == 0) == 0)){
					locations.push_back(Table{GetName(folder), folder});
					sort(locations.begin(), locations.end(), locations[0].SortTable);
				}
			}
		}

	//
	// Main input events
	//
	}else{

		// Pasting from clipboard
		if (keyboard.GetKey(KEY_V) && keyboard.GetKey(KEY_LEFT_CONTROL) && !pasted){
			Paste();
		}else if (!(keyboard.GetKey(KEY_V) && keyboard.GetKey(KEY_LEFT_CONTROL)) && pasted)
			pasted = false;


		// Import menu
		if (mouse.Click(LM_DOWN) && import.Hover()){
			ResetImport();
			Import.Show();
			keyboard.newKey = -1;
			mouse.prevState = mouse.state;
			return;
		
		// Load image board
		} else if (mouse.Click(LM_DOWN) && loadBoard.Hover()){
			GetBoard();

		// New image board
		}else if (mouse.Click(LM_DOWN) && newBoard.Hover()){
			imgs.clear();
			board = "";
			*Scale = 1;
			*View = Vector2{0,0};
		}

		//
		// Deleting	
		//
		if (keyboard.newKey == KEY_DELETE && selImgs.size()){
			sort(selImgs.begin(), selImgs.end(), SortUint);
			int off = 0;
			for (int i = 0; i < selImgs.size(); i++){
				imgs.erase(imgs.begin() + selImgs[i]-off);
				off++;
			}
			ResetImages();
		}

		//
		// Image flip
		//
		if (selImgs.size()){
			if (keyboard.newKey == KEY_H)
				for (auto img : selImgs)
					imgs[img].hFlip = !imgs[img].hFlip;
			if (keyboard.newKey == KEY_V && !keyboard.GetKey(KEY_LEFT_CONTROL))
				for (auto img : selImgs)
					imgs[img].vFlip = !imgs[img].vFlip;
		}

		//
		// Image rotation
		//
		if (rot){
			// Get center of selection
			Vector2 avg = imgs[selImgs[0]].position + Vector2{0, imgs[selImgs[0]].size.y/2};
			for (int i = 1; i < selImgs.size(); i++)
				avg += imgs[selImgs[i]].position + Vector2{0, imgs[selImgs[i]].size.y/2};
			avg /= (int)selImgs.size();

			// Rotate
			for (auto img : selImgs){
				imgs[img].angle = (mouse.position.y >= avg.y ? 1 : -1) * (avg + Vector2{128, 0}).Angle(avg, mouse.position)*RADIANS;
			}
		}
		if (selImgs.size())
			if (keyboard.newKey == KEY_R){
				rot = true;
				mouse.dragOff = mouse.position;
			}
		
		
		//
		// Image board mouse events
		//
		if (mouse.Click(LM_DOWN)){
			if (imgScale)
				imgScale = false;

			//
			// Rotation, Scaling, Selecting, and such
			//

			if (!previewImg.loaded){

				mouse.drag = true;
				mouse.dragOff = mouse.position;

				// Selecting an image on the board
				if (!selImgs.size()){
					ResetImages();

					for (int i = imgs.size()-1; i >= 0; i--){
						if (mouse.position.Within(ScreenSpace(imgs[i].position), imgs[i].size.Multiply(*Scale))){
								selImgs.push_back(i);
								imgs[i].selected  = true;
								ReorderImages();
								break;
						}
					}

					// Selector window
					if (!selImgs.size()){
						mouse.dragOff = mouse.position;
						selector = true;
					}

				// Clicking on a selected image/no image
				}else if (!shift && !rot){
					bool reset = true;
					for (int i = imgs.size()-1; i >= 0; i--)
						if (mouse.position.Within(ScreenSpace(imgs[i].position), imgs[i].size.Multiply(*Scale))){
							reset = false;
							if (!imgs[i].selected){
								ResetImages();
								imgs[i].selected = true;
								selImgs.push_back(i);
								ReorderImages();
							}
							break;
						}
					
					// No image was clicked
					if (reset)
						ResetImages();

				// Selecting multiple images with shift
				}else if (!rot){
					for (int i = imgs.size()-1; i >= 0; i--)
						if (mouse.position.Within(ScreenSpace(imgs[i].position), imgs[i].size.Multiply(*Scale))){\
						
							// Check if the image was already selected
							bool selected = false;
							for (vector<uint>:: iterator img = selImgs.begin(); img != selImgs.end(); img++){

								// Already selected
								if (*img == i){
									imgs[i].selected = false;
									selected = true;
									selImgs.erase(img);
									break;
								}
							}

							// Not selected
							if (!selected){
								imgs[i].selected = true;
								selImgs.push_back(i);
							}
							break;
						}
				
				// Ending the rotation command
				}else{
					rot = false;
					for (auto img : selImgs)
						imgs[img].prevAngle = imgs[img].angle;
				}

			//
			// Placing a new image on the board
			//
			}else{
				previewImg.size = Vector2{(float)previewImg.width, (float)previewImg.height};
				previewImg.position = MouseToScreenSpace(mouse.position).Subtract(previewImg.size.Divide(2));
				previewImg.selected = true;
				ResetImages();
				imgs.push_back(previewImg);
				previewImg.loaded = false;
				selImgs.push_back(imgs.size()-1);
			}
			
		}else if (mouse.state == LM_DOWN && mouse.drag && !shift & !rot){

			// Moving images
			if (!selector && selImgs.size()){
				for (auto img : selImgs)
					imgs[img].position = imgs[img].position.Add((mouse.position.x - mouse.dragOff.x) / *Scale, (mouse.position.y - mouse.dragOff.y) / *Scale);
					
				mouse.dragOff = mouse.position;
			}

		//
		// RMB
		//
		}else if (mouse.Click(RM_DOWN)){
			
			// Prepare for scaling
			if (!rot && !imgScale){
				mouse.dragOff = Vector2{mouse.position.x, mouse.position.y};
				mouse.drag = true;
			}else if (imgScale)
				imgScale = false;
			else{
				for (auto img : selImgs)
					imgs[img].prevAngle = imgs[img].angle;
				rot = false;
				mouse.dragOff = mouse.position;
				mouse.state = -1;
			}

		}else if (mouse.state == RM_DOWN && selImgs.size() && abs(mouse.position.x - mouse.dragOff.x) > 16)
			imgScale = true;

		if (imgScale){
			// Get corner of the group of images
			Vector2 min = imgs[selImgs[0]].position;
			for (auto img : selImgs){
				if (imgs[img].position.x < min.x)
					min.x = imgs[img].position.x;
				if (imgs[img].position.y < min.y)
					min.y = imgs[img].position.y;
			}
			for (auto img : selImgs){
				if (!shift){
					imgs[img].size.x += (mouse.position.x-mouse.dragOff.x) / *Scale;
					imgs[img].size.y += (mouse.position.y-mouse.dragOff.y) / *Scale;
				}else{
					imgs[img].size.x += (mouse.position.x-mouse.dragOff.x) / *Scale;
					imgs[img].size.y = imgs[img].height * imgs[img].size.x/imgs[img].width;
				}
				if (imgs[img].size.x <= minSize)
					imgs[img].size.x = minSize+1;
				if (imgs[img].size.y <= minSize)
					imgs[img].size.y = minSize+1;
			}
			mouse.dragOff = mouse.position;

		//
		// Middle Mouse
		//
		}else if (mouse.Click(MM_DOWN)){
			mouse.dragOff = mouse.position;
		}else if (mouse.state == MM_DOWN){
			*View = View->Add((mouse.dragOff.x - mouse.position.x) / *Scale, + (mouse.dragOff.y - mouse.position.y) / *Scale);
			mouse.dragOff = mouse.position;
		}
	}
	// 
	// LMB Release
	//
	if (mouse.state == LM_UP && selector){
		selector = false;
		//shape.Draw(mouse.position, mouse.dragOff.Subtract(mouse.position), cSelector, true);
		if (!shift)
			ResetImages();

		for (int i = 0; i < imgs.size(); i++)
			if (ScreenSpace(imgs[i].position).Within(mouse.position, mouse.dragOff.Subtract(mouse.position))){
				selImgs.push_back(i);
				imgs[i].selected = true;
			}
	}

	//
	// RM Menu
	//
	if (mouse.state == RM_UP && mouse.prevState == RM_DOWN && !imgScale){
		// Check that the mouse is not in the menu and a preview image is not loaded
		if ((!menu || mouse.position.x > (float)Width/3) && !previewImg.loaded){
			rmMenu.Reset();
			drawMouseMenu = !drawMouseMenu;
			rmMenu.position = mouse.position;
			mouse.state = -1;
		}
	}else if (mouse.state == RM_UP && !mouse.drag){
		imgScale = false;
		mouse.state = -1;
	}
	
	

	keyboard.newKey = -1;
	mouse.prevState = mouse.state;
}

Vector2 MouseToScreenSpace(Vector2 position){
	return position.Subtract(Width/2, Height/2)
					.Divide(*Scale)
					.Add(Width/2, Height/2) 
					.Add(*View);
}

void Paste(){
	pasted = true;
	const char* cb = glfwGetClipboardString(Main.w);
	if (cb == NULL){
		printf("Failed to read clipbord\n");
		return;
	}
	int s = 0;
	string path = "";
	for (int i = 0; i < strlen(cb); i++){
		if (cb[i] == '\n'){
			if (IsImage(path)){
				Image img;
				img.LoadImage(path.data());
				img.position = MouseToScreenSpace(mouse.position).Subtract(img.size.Divide(2));
				imgs.push_back(img);
			}
			s=i;
			path = "";
		}else if (i-s > 6) // Removes "file://" from the beginning of the path
			path += cb[i];
	}
}

void ResetImages(){
	selImgs.clear();
	for (int i = 0; i < imgs.size(); i++)
		imgs[i].selected = false;
}

void ReorderImages(){
	int i = selImgs[selImgs.size()-1];
	Image temp = imgs[i];

	vector<Image>:: iterator img = imgs.begin();
	advance(img, i);
	imgs.erase(img);

	imgs.push_back(temp);
	selImgs[selImgs.size()-1] = imgs.size()-1;
}