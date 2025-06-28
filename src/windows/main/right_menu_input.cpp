/*
    Handles all the input for the right side menu and adjacent button functions other than what is covered in the classes
*/

void RightMenuInput(){
	// Menu toggle
	if ((keyboard.newKey == KEY_ESCAPE && !fCurrentBoard.active && !selImgs.size()) || mouse.Click() && bBoards.Hover()){
		if (fCurrentBoard.active)
			fCurrentBoard.Reset();
		if (!rMenu)
			rMenu = true;
		else if (rMenu)
			rMenu = false;
		rmMenu.Reset();
		mouse.state = -1;
	}

	// Image pack button
	if (mouse.Click() && import.Hover()){
		ResetImport();
		Import.Toggle();
		keyboard.newKey = -1;
		mouse.state = -1;
	}

	// Checks if the saving thread is finished
	if (saveThread && saveThread->joinable() && !saving){
		saveThread->join();
		FindBoards();
	}

	// Save
	if ((keyboard.newKey == KEY_S && keyboard.ctrl) || mouse.Click() && bSave.Hover()){
		previewImg.img.loaded = false;
		for (auto img : selImgs)
			imgs[img].angle = imgs[img].prevAngle;
		rot = false;
		ResetImages();
		TagWin.Hide();
		rmMenu.Reset();

		// Detach saving thread if it is hanging (most likely crashed but rare)
		if (saveThread && saving)
			saveThread->detach();
		
		TakeBoardScreenshot();

		saveThread = new thread(Save);
		importTime = 0;
	}
}