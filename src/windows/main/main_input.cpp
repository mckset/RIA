//
// Responsible for handling mouse and keyboard events in the main window
//


void MainInput(){
	//
	// Basic inputs
	//

	if (fCurrentBoard.active){
		RightMenuInput();
		if (fCurrentBoard.active)
			return;
	}

	// Toggle origin
	if (keyboard.newKey == KEY_SPACE)
		drawOrigin = !drawOrigin;

	// Refresh locations
	if (keyboard.GetKey(KEY_R) && keyboard.ctrl){
		for (int i = 0; i < locations.size(); i++){
			locations[i].RefreshTable();
		}
	}

	// Reset selected list
	if (keyboard.newKey == KEY_ESCAPE){
		previewImg.img.loaded = false;
		for (auto img : selImgs)
			imgs[img].angle = imgs[img].prevAngle;
		rot = false;
		ResetImages();
		TagWin.Hide();
		rmMenu.Reset();
		importTime = 0;
	}

	// View movement
	if (keyboard.GetKey(KEY_W) || keyboard.GetKey(KEY_UP))
		View->y += viewSpeed/(*Scale);
	if ((keyboard.GetKey(KEY_S) && !keyboard.ctrl) || keyboard.GetKey(KEY_DOWN))
		View->y -= viewSpeed/(*Scale);
	if ((keyboard.GetKey(KEY_A) && !keyboard.ctrl) || keyboard.GetKey(KEY_LEFT))
		View->x -= viewSpeed/(*Scale);
	if (keyboard.GetKey(KEY_D) || keyboard.GetKey(KEY_RIGHT))
		View->x += viewSpeed/(*Scale);

	// Select all
	if (keyboard.newKey == KEY_A && keyboard.ctrl){
		selImgs.clear();
		for (uint i = 0; i < imgs.size(); i++){
			selImgs.push_back(i);
			imgs[i].selected = true;
		}
	}


	// Scaling
	if (keyboard.newKey == KEY_KP_ADD || keyboard.newKey == KEY_EQUAL)
		GetScrollWheel(Main.w, 0, 1);
	else if (keyboard.newKey == KEY_MINUS || keyboard.newKey == KEY_KP_SUBTRACT)
		GetScrollWheel(Main.w, 0, -1);

	// Copy to clipboard
	if (keyboard.newKey == KEY_C && keyboard.ctrl) Copy();
		

	// Pasting from clipboard
		if (keyboard.GetKey(KEY_V) && keyboard.ctrl && !pasted)
		Paste();
	else if (!(keyboard.GetKey(KEY_V) && keyboard.ctrl) && pasted)
		pasted = false;

	LeftMenuInput();
	if (lMenu && mouse.position.x < menuWidth+scrollbarSize) return;
	RightMenuInput();
	if (rMenu && mouse.position.x > fWidth-menuWidth-scrollbarSize) return;

	//
	// Main input events
	//

	// Import menu
	if (mouse.Click(LM_DOWN) && import.Hover()){
		ResetImport();
		Import.Toggle();
		keyboard.newKey = -1;
		mouse.prevState = mouse.state;
		return;
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
		if (keyboard.newKey == KEY_V && !keyboard.ctrl)
			for (auto img : selImgs)
				imgs[img].vFlip = !imgs[img].vFlip;
	}

	//
	// Image rotation
	//
	if (rot){
		// Get center of selection
		Vector2 avg = {0, 0};
		for (int i = 0; i < selImgs.size(); i++)
			avg += imgs[selImgs[i]].position + imgs[selImgs[i]].size/2;
		
		avg /= (int)selImgs.size();
		float a = (mouse.ToScreenSpace().y >= avg.y ? 1 : -1) * (avg + Vector2{128, 0}).Angle(avg, mouse.ToScreenSpace());
		if (keyboard.shift)
			a = ((int)a/15)*15;

		// Rotate
		for (auto img : selImgs){
			imgs[img].angle = a;
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

		if (!previewImg.img.loaded){
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
			}else if (!keyboard.shift && !rot){
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

			// Selecting multiple images with shift held down
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
			Object img;
			img.size = Vector2{(float)previewImg.img.width, (float)previewImg.img.height};
			img.position = mouse.ToScreenSpace() - img.size/2;
			img.selected = true;
			img.img = previewImg.img;
			img.path = previewImg.path;
			printf("%s\n", img.path.data());
			ResetImages();
			imgs.push_back(img);
			previewImg.img.loaded = false;
			selImgs.push_back(imgs.size()-1);
			mouse.dragOff = mouse.position;
		}
			
	}else if (mouse.state == LM_DOWN && mouse.drag && !keyboard.shift & !rot){

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
			if (!keyboard.shift){
				imgs[img].size.x += (mouse.position.x-mouse.dragOff.x) / *Scale;
				imgs[img].size.y += (mouse.position.y-mouse.dragOff.y) / *Scale;
			}else{
				imgs[img].size.x += (mouse.position.x-mouse.dragOff.x) / *Scale;
				imgs[img].size.y = imgs[img].img.height * imgs[img].size.x/imgs[img].img.width;
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

	// 
	// LMB Release
	//
	if (mouse.state == LM_UP && selector){
		selector = false;
		if (!keyboard.shift)
			ResetImages();

		for (int i = 0; i < imgs.size(); i++)
			if (imgs[i].WithinWindow(mouse.position, mouse.dragOff)){
				selImgs.push_back(i);
				imgs[i].selected = true;
			}
	}

	//
	// RM Menu
	//
	if (mouse.state == RM_UP && mouse.prevState == RM_DOWN && !imgScale){
		// Check that the mouse is not in the left menu and a preview image is not loaded
		if ((!lMenu || mouse.position.x > menuWidth) && !previewImg.img.loaded){
			rmMenu.Reset();
			drawMouseMenu = !drawMouseMenu;
			rmMenu.position = mouse.position;
			mouse.state = -1;

			// Selected the clicked image if no images are selected
			if (!selImgs.size()){
				for (int i = imgs.size()-1; i >= 0; i--){
					if (mouse.position.Within(ScreenSpace(imgs[i].position), imgs[i].size.Multiply(*Scale))){
							selImgs.push_back(i);
							imgs[i].selected  = true;
							break;
					}
				}
			}
		}
	}else if (mouse.state == RM_UP && !mouse.drag){
		imgScale = false;
		mouse.state = -1;
	}
}

// REMOVE
Vector2 MouseToScreenSpace(Vector2 position){
	return position.Subtract(Width/2, Height/2)
					.Divide(*Scale)
					.Add(Width/2, Height/2) 
					.Add(*View);
}

void ResetImages(){
	selImgs.clear();
	for (int i = 0; i < imgs.size(); i++)
		imgs[i].selected = false;
}

void ReorderImages(){
	int i = selImgs[selImgs.size()-1];
	Object temp = imgs[i];

	imgs.erase(imgs.begin() + i);

	imgs.push_back(temp);
	selImgs[selImgs.size()-1] = imgs.size()-1;
}