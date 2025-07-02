/*
	Changes steps in the tutorial depending on the action
	tStage stores the tutorial step and mState stores the substep
	mStage usually only counts to 1 or 2
*/

/*
tStage
0 - Main board
1 - Locations
2 - Tags
3 - Boards
4 - Image packs
*/

void TutorialInput(){
	if (keyboard.newKey == KEY_ESCAPE){
		showTutorial = false;
		Main.Render = &DrawApp;
		Main.Input = &MainInput;
		lMenu = false;
		rMenu = false;
		*Scale = oldScale;
		*View = oldView;
	}

	if (keyboard.newKey == KEY_RIGHT){
		if (tStage == 0 && mStage == 1){
			tStage = 1;
			mStage = 0;
			lMenu = true;
			tagView = false;
		}else if (tStage == 1 && mStage == 1){
			tStage = 2;
			mStage = 0;
			lMenu = true;
			tagView = true;
		}else if (tStage == 2 && mStage == 1){
			tStage = 3;
			mStage = 0;
			lMenu = false;
			rMenu = true;
		}else if (tStage == 3 && mStage == 1){
			tStage = 4;
			mStage = 0;
			rMenu = false;
		}else
			mStage++;
	}

	if (keyboard.newKey == KEY_TAB){
		if (tStage == 0 && mStage == 1){
			tStage = 1;
			mStage = 0;
			lMenu = true;
			tagView = false;
		}
	}
	if (keyboard.newKey == KEY_TILDE){
		tStage = 2;
		mStage = 0;
		lMenu = true;
		tagView = true;
	}

	if (openLoc.Hover() && mouse.Click()){
		if (tStage == 0 && mStage == 1){
			tStage = 1;
			mStage = 0;
			lMenu = true;
			tagView = false;
			mouse.prevState = mouse.state;
		}
	}

	if (openTags.Hover() && mouse.Click()){
		if (tStage == 1 && mStage == 1){
			tStage = 2;
			mStage = 0;
			lMenu = true;
			tagView = true;
			mouse.prevState = mouse.state;
		}
	}
	if ((bBoards.Hover() && mouse.Click()) || keyboard.newKey == KEY_1){
		if (tStage == 2 && mStage == 1){
			tStage = 3;
			mStage = 0;
			lMenu = false;
			rMenu = true;
			mouse.prevState = mouse.state;
		}
	}

	// Moving back
	if (keyboard.newKey == KEY_LEFT){
		if (tStage == 1 && mStage == 0){
			tStage = 0; 
			mStage = 1;
			lMenu = false;
		}else if (tStage == 2 && mStage == 0){
			tStage = 1; 
			mStage = 1;
			tagView = false;
		}else if (tStage == 3 && mStage == 0){
			tStage = 2; 
			mStage = 1;
			tagView = true;
			lMenu = true;
			rMenu = false;
			fCurrentBoard.Reset();
		}else if (tStage == 4 && mStage == 0){
			tStage = 3; 
			mStage = 1;
			rMenu = true;
			fCurrentBoard.Reset();
		}else if (mStage != 0)
			mStage--;
	}

	if (lMenu && mouse.position.x < menuWidth){
		if (add.Hover() && mouse.Click() && mStage == 0 && (tStage == 1 || tStage == 2)){
			if (tagView){
				editTag = -1;
				TagWin.Show();
			}else{
				string folder = GetFolder();
				if (folder.length() && !((stat(folder.c_str(), &st) == 0) == 0)){
					locations.push_back(Table{GetName(folder), folder});
					sort(locations.begin(), locations.end(), locations[0].SortTable);
				}
			}
		}
    }
}