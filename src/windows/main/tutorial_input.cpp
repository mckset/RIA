/*
	Changes steps in the tutorial depending on the action
	tutorialStage stores the tutorial step and mState stores the substep
	tutorialPage usually only counts to 1 or 2
*/

/*
tutorialStage
0 - Main board
1 - Locations
2 - Tags
3 - Boards
4 - Image packs
*/

void MainWindow::TutorialInput(){
	if (boardName_Field){
		boardName_Field->active = false;
		boardName_Field = nullptr;
	}

	if (keyboard.newKey == KEY_ESCAPE){
		showTutorial = false;
		display = MAIN_APP;
		showLeftMenu = false;
		showRightMenu = false;
		*Scale = oldScale;
		*View = oldView;
	}

	if (keyboard.newKey == KEY_RIGHT){
		if (tutorialStage == MAIN_BOARD && tutorialPage == 1){
			tutorialStage = LOCATIONS;
			tutorialPage = 0;
			showLeftMenu = true;
			showTags = false;
		}else if (tutorialStage == LOCATIONS && tutorialPage == 2){
			tutorialStage = TAGS;
			tutorialPage = 0;
			showLeftMenu = true;
			showTags = true;
		}else if (tutorialStage == TAGS && tutorialPage == 2){
			tutorialStage = BOARDS;
			tutorialPage = 0;
			showLeftMenu = false;
			showRightMenu = true;
		}else if (tutorialStage == BOARDS && tutorialPage == 1){
			tutorialStage = IMAGE_PACKS;
			tutorialPage = 0;
			showRightMenu = false;
		}else
			tutorialPage++;
	}

	if (keyboard.newKey == KEY_TAB){
		if (tutorialStage == MAIN_BOARD && tutorialPage == 1){
			tutorialStage = LOCATIONS;
			tutorialPage = 0;
			showLeftMenu = true;
			showTags = false;
		}
	}
	if (keyboard.newKey == KEY_TILDE){
		if (tutorialStage == LOCATIONS && tutorialPage == 2){
			tutorialStage = TAGS;
			tutorialPage = 0;
			showLeftMenu = true;
			showTags = true;
		}
	}

	if (openLocations_Button.pressed){
		if (tutorialStage == MAIN_BOARD && tutorialPage == 1){
			tutorialStage = LOCATIONS;
			tutorialPage = 0;
			showLeftMenu = true;
			showTags = false;
			mouse.prevState = mouse.state;
		}
	}

	if (openTags_Button.pressed){
		if (tutorialStage == LOCATIONS && tutorialPage == 2){
			tutorialStage = TAGS;
			tutorialPage = 0;
			showLeftMenu = true;
			showTags = true;
			mouse.prevState = mouse.state;
		}
	}
	if (imageBoards_Button.pressed){
		if (tutorialStage == LOCATIONS && tutorialPage == 2){
			tutorialStage = BOARDS;
			tutorialPage = 0;
			showLeftMenu = false;
			showRightMenu = true;
			mouse.prevState = mouse.state;
		}
	}

	// Moving back
	if (keyboard.newKey == KEY_LEFT){
		if (tutorialStage == LOCATIONS && tutorialPage == 0){
			tutorialStage = MAIN_BOARD; 
			tutorialPage = 1;
			showLeftMenu = false;
		}else if (tutorialStage == TAGS && tutorialPage == 0){
			tutorialStage = LOCATIONS; 
			tutorialPage = 2;
			showTags = false;
		}else if (tutorialStage == BOARDS && tutorialPage == 0){
			tutorialStage = TAGS; 
			tutorialPage = 2;
			showTags = true;
			showLeftMenu = true;
			showRightMenu = false;
		}else if (tutorialStage == IMAGE_PACKS && tutorialPage == 0){
			tutorialStage = BOARDS; 
			tutorialPage = 1;
			showRightMenu = true;
		}else if (tutorialPage != 0)
			tutorialPage--;
	}
}