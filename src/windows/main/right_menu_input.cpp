/*
    Handles all the input for the right side menu and adjacent button functions other than what is covered in the classes
*/

void RightMenuInput(){
	// Menu toggle
	if ((keyboard.newKey == KEY_ESCAPE && !selectedImgs.size() && statusTextTimer <= 0 && !previewImg.loaded) || imageBoards_Button.pressed){
		if (!showRightMenu){
			showRightMenu = true;
			imageBoards_Button.text = "Close";
		}else if (showRightMenu){
			showRightMenu = false;
			imageBoards_Button.text = "Boards";
		}
		mouseMenu.Reset();
		mouse.state = INPUT_NULL;
	}

	// Image pack button
	if (imagePack_Button.pressed){
		ResetImport();
		Import.Show();
		keyboard.newKey = INPUT_NULL;
		mouse.state = INPUT_NULL;
	}

	// Checks if the saving thread is finished
	if (saveThread && saveThread->joinable() && !saving){
		saveThread->join();
		FindBoards();
	}

	// Save
	if ((keyboard.newKey == KEY_S && keyboard.ctrl) || save_Button.pressed){
		// If saved while loading images, unloaded images will be deleted
		if (loadThread || imageBoardThread) return;
		previewImg.loaded = false;
		for (auto i : selectedImgs)
			imgs[i].angle = imgs[i].prevAngle;
		rotateImages = false;
		ResetImages();
		TagWin.Hide();
		mouseMenu.Reset();

		// Detach saving thread if it is hanging (most likely crashed but rare)
		if (saveThread && saving)
			saveThread->detach();
		
		TakeBoardScreenshot();

		free(saveThread);
		saveThread = new thread(Save);
	}
}
