/*
    Handles all the input for the right side menu and adjacent button functions other than what is covered in the classes
*/

void RightMenuInput(){
	// Menu toggle
	if ((keyboard.newKey == KEY_1 && !fCurrentBoard.active) || mouse.Click(LM_DOWN) && bBoards.Hover()){
		if (fCurrentBoard.active)
			fCurrentBoard.Reset();
		if (!rMenu)
			rMenu = true;
		else if (rMenu)
			rMenu = false;
		rmMenu.Reset();
		mouse.state = -1;
	}

	// Saving
	if (saveThread && saveThread->joinable() && !saving){
		saveThread->join();
		FindBoards();
	}

	if ((keyboard.newKey == KEY_S && keyboard.ctrl) || mouse.Click(LM_DOWN) && bSave.Hover()){
		previewImg.img.loaded = false;
		for (auto img : selImgs)
			imgs[img].angle = imgs[img].prevAngle;
		rot = false;
		ResetImages();
		TagWin.Hide();
		rmMenu.Reset();

		if (saveThread && saving)
			saveThread->detach();
		
		TakeBoardScreenshot();

		saveThread = new thread(Save);
		importTime = 0;
	}
}
