//
// Responsible for handling mouse and keyboard events in the main window view
//

enum Selection_Type{
	SELECT_NONE,
	SELECT_NEW,
	SELECT_OLD,
	SELECT_DELETE
};

void MainInput(){
	BoardInput();

	LeftMenuInput();
	if (showLeftMenu && mouse.position.x < sideMenuWidth+SCROLLBAR_SIZE) return;

	RightMenuInput();
	if (showRightMenu && mouse.position.x > fWidth-sideMenuWidth-SCROLLBAR_SIZE) return;

	ImageInput();
}


void BoardInput(){
	// Drag view around
	if (mouse.Click(MM_DOWN))
		mouse.dragOffset = mouse.position;

	else if (mouse.state == MM_DOWN){
		*View = View->Add((mouse.dragOffset.x - mouse.position.x) / *Scale, + (mouse.dragOffset.y - mouse.position.y) / *Scale);
		mouse.dragOffset = mouse.position;
	}

	if (currentBoard_Field.active) return;

	// Toggle origin
	if (keyboard.newKey == KEY_SPACE)
		showOrigin = !showOrigin;

	// View movement
	if (keyboard.GetKey(KEY_W) || keyboard.GetKey(KEY_UP))
		View->y += VIEW_SPEED/(*Scale);
	if ((keyboard.GetKey(KEY_S) && !keyboard.ctrl) || keyboard.GetKey(KEY_DOWN))
		View->y -= VIEW_SPEED/(*Scale);
	if ((keyboard.GetKey(KEY_A) && !keyboard.ctrl) || keyboard.GetKey(KEY_LEFT))
		View->x -= VIEW_SPEED/(*Scale);
	if (keyboard.GetKey(KEY_D) || keyboard.GetKey(KEY_RIGHT))
		View->x += VIEW_SPEED/(*Scale);

	// Scaling
	if (keyboard.newKey == KEY_KP_ADD || keyboard.newKey == KEY_EQUAL)
		GetScrollWheel(Main.w, 0, 1);
	else if (keyboard.newKey == KEY_MINUS || keyboard.newKey == KEY_KP_SUBTRACT)
		GetScrollWheel(Main.w, 0, -1);

	// Copy to clipboard
	if (keyboard.newKey == KEY_C && keyboard.ctrl) Copy();

	// Pasting from clipboard
	if (keyboard.GetKey(KEY_V) && keyboard.ctrl && !pastedFile)
		Paste();
	else if (!(keyboard.GetKey(KEY_V) && keyboard.ctrl) && pastedFile)
		pastedFile = false;
}

void ImageInput(){
	// Select all
	if (keyboard.newKey == KEY_A && keyboard.ctrl){
		selectedImgs.clear();
		for (uint i = 0; i < imgs.size(); i++){
			selectedImgs.push_back(i);
			imgs[i].selected = true;
		}
	}

	// Reset selected list
	if (keyboard.newKey == KEY_ESCAPE){
		previewImg.loaded = false;
		if (rotateImages)
			for (auto img : selectedImgs)
				imgs[img].angle = imgs[img].prevAngle;
		rotateImages = false;
		ResetImages();
		TagWin.Hide();
		mouseMenu.Reset();
		statusTextTimer = 0;
	}

	//
	// Deleting	
	//
	if (keyboard.newKey == KEY_DELETE && selectedImgs.size()){
		sort(selectedImgs.begin(), selectedImgs.end(), SortUint);
		int off = 0;
		for (int i = 0; i < selectedImgs.size(); i++){
			imgs.erase(imgs.begin() + selectedImgs[i]-off);
			off++;
		}
		ResetImages();
	}

	//
	// Flip image
	//
	if (selectedImgs.size()){
		if (keyboard.newKey == KEY_H)
			for (auto img : selectedImgs)
				imgs[img].hFlip = !imgs[img].hFlip;
		else if (keyboard.newKey == KEY_V && !keyboard.ctrl)
			for (auto img : selectedImgs)
				imgs[img].vFlip = !imgs[img].vFlip;
	}

	//
	// Rotate image
	//
	if (rotateImages){
		// Get center of selection
		Vector2 avg = {0, 0};
		for (int i = 0; i < selectedImgs.size(); i++)
			avg += imgs[selectedImgs[i]].position + imgs[selectedImgs[i]].size/2;
		
		avg /= (int)selectedImgs.size();

		// 128 is a random number used to create a triangle to get the angle from the images to the mouse
		float a = (mouse.ToScreenSpace().y >= avg.y ? 1 : -1) * (avg + Vector2{128, 0}).Angle(avg, mouse.ToScreenSpace());

		// Snap rotate
		if (keyboard.shift)
			a = ((int)a/15)*15;

		// Rotate
		for (auto img : selectedImgs)
			imgs[img].angle = a;
	}

	if (selectedImgs.size())
		if (keyboard.newKey == KEY_R){
			rotateImages = true;
			mouse.dragOffset = mouse.position;
		}

	//
	// Left mouse events
	//
	if (mouse.Click()){

		if (scaleImages){
			scaleImages = false;
			return;
		}

		if (rotateImages){
			// Apply image rotations
			for (auto i : selectedImgs)
				imgs[i].prevAngle = imgs[i].angle;

			rotateImages = false;
			mouse.dragOffset = mouse.position;
			return;
		}

		// Add image to board
		if (previewImg.loaded){
			ResetImages();
			imgs.push_back(previewImg);
			imgs.back().size = Vector2{(float)previewImg.img.width, (float)previewImg.img.height};
			imgs.back().position = mouse.ToScreenSpace() - imgs.back().size/2;
			imgs.back().selected = true;
			previewImg.loaded = false;
			selectedImgs.push_back(imgs.size()-1);
			mouse.dragOffset = mouse.position;
			return;
		}

		// Select images
		mouse.drag = true;
		mouse.dragOffset = mouse.position;

		int selectionType = SelectImage();

		// Selecting an image
		if (selectionType == SELECT_NEW || selectionType == SELECT_OLD)
			ReorderImages();
		
		// Selector window activate
		else if (selectionType == SELECT_NONE){
			if (!keyboard.shift) ResetImages();
			mouse.dragOffset = mouse.position;
			showImageSelector = true;
		}
		
		return;

	}else if (mouse.state == LM_DOWN && mouse.drag && !keyboard.shift && !rotateImages){

		// Move images
		if (!showImageSelector && selectedImgs.size()){
			for (auto img : selectedImgs)
				imgs[img].position = imgs[img].position + Vector2{(mouse.position.x - mouse.dragOffset.x) / *Scale, (mouse.position.y - mouse.dragOffset.y) / *Scale};
		
			mouse.dragOffset = mouse.position;
		}

	// Image selector
	}else if (mouse.state == LM_UP && showImageSelector){
		showImageSelector = false;
		if (!keyboard.shift) ResetImages();

		for (int i = 0; i < imgs.size(); i++){
			if (imgs[i].WithinWindow(mouse.position, mouse.dragOffset) && !imgs[i].selected){
				selectedImgs.push_back(i);
				imgs[i].selected = true;
			}
		}

	//
	// Right mouse events
	//

	}else if (mouse.Click(RM_DOWN)){

		// Prepare to scale images
		if (!rotateImages && !scaleImages){
			mouse.dragOffset = mouse.position;
			mouse.drag = true;
			return;
		
		// Scaled by mouse menu
		}else if (scaleImages){
			scaleImages = false;
			return;
		}

		// Apply image rotations
		for (auto i : selectedImgs)
			imgs[i].prevAngle = imgs[i].angle;

		rotateImages = false;
		mouse.dragOffset = mouse.position;
		return;
	
	}else if (mouse.state == RM_DOWN && selectedImgs.size() && abs(mouse.position.x - mouse.dragOffset.x) > SCALE_THRESHOLD){
		scaleImages = true;

	}else if (mouse.state == RM_UP && mouse.prevState == RM_DOWN && !scaleImages){
		mouseMenu.Reset();
		mouseMenu.position = mouse.position;
		showMouseMenu = true;

		// Select the image if it is not selected
		for (int i = imgs.size()-1; i  > -1; i--){
			if (mouse.Within(ScreenSpace(imgs[i].position), imgs[i].size*(*Scale))){
				if (imgs[i].selected) break;
				if (!keyboard.shift) ResetImages();
				selectedImgs.push_back(i);
				imgs[i].selected = true;
				break;
			}
		}
		return;

	}else if (mouse.state == RM_UP && !mouse.drag)
		scaleImages = false;

	if (scaleImages){
		for (auto i : selectedImgs){
			if (!keyboard.shift){
				imgs[i].size.x += (mouse.position.x-mouse.dragOffset.x) / *Scale;
				imgs[i].size.y += (mouse.position.y-mouse.dragOffset.y) / *Scale;
			}else{
				imgs[i].size.x += (mouse.position.x-mouse.dragOffset.x) / *Scale;
				imgs[i].size.y = imgs[i].img.height * imgs[i].size.x/imgs[i].img.width;
			}
			if (imgs[i].size.x <= MIN_IMAGE_SIZE)
				imgs[i].size.x = MIN_IMAGE_SIZE+1;
			if (imgs[i].size.y <= MIN_IMAGE_SIZE)
				imgs[i].size.y = MIN_IMAGE_SIZE+1;
		}
		mouse.dragOffset = mouse.position;
	}
}

int SelectImage(){
	for (int i = imgs.size()-1; i > -1; i--){
		if (mouse.Within(ScreenSpace(imgs[i].position), imgs[i].size*(*Scale))){

			// Selecting multiple images
			if (keyboard.shift){

				// Deselect already selected image
				if (imgs[i].selected){
					imgs[i].selected = false;
					for (int s = 0; s < selectedImgs.size(); s++)
						if (selectedImgs[s] == i){
							selectedImgs.erase(selectedImgs.begin()+s);
							break;
						}
					return SELECT_DELETE;
				}

				// Adding a new image 
				selectedImgs.push_back(i);
				imgs[i].selected = true;
				return SELECT_NEW;
			}

			// Check if image was already selected
			if (selectedImgs.size()){
				if (imgs[i].selected)
					return SELECT_OLD;

				ResetImages();
				selectedImgs.push_back(i);
				imgs[i].selected = true;
				return SELECT_NEW;
			}

			// Selecting a single image
			selectedImgs.push_back(i);
			imgs[i].selected = true;
			return SELECT_NEW;
		}
	}

	return SELECT_NONE;
}

// Clears selection
void ResetImages(){
	selectedImgs.clear();
	for (int i = 0; i < imgs.size(); i++)
		imgs[i].selected = false;
}

// Moves newest selection to the top of the image order
void ReorderImages(){
	int i = selectedImgs[selectedImgs.size()-1];
	ImageContainer temp = imgs[i];

	imgs.erase(imgs.begin() + i);

	imgs.push_back(temp);

	for (int s = 0; s < selectedImgs.size(); s++)
		if (selectedImgs[s] > i)
			selectedImgs[s]--;

	selectedImgs[selectedImgs.size()-1] = imgs.size()-1;
}