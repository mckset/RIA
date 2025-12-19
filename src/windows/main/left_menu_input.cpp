/*
    Handles all the input for the left side menu and adjacent button functions other than what is covered in the classes
*/

void LeftMenuInput(){
    // Menu toggle
	if ((keyboard.newKey == KEY_TAB) || openLocations_Button.pressed){
		if (!showLeftMenu)
			showLeftMenu = true;
		else if (showLeftMenu && !showTags)
			showLeftMenu = false;
		showTags = false;
		mouseMenu.Reset();
		mouse.state = INPUT_NULL;
		openLocations_Button.pressed = false;
	}

	// Tag toggle
	if ((keyboard.newKey == KEY_TILDE) || openTags_Button.pressed){
		if (!showLeftMenu && !showTags)
			showLeftMenu = true;
		else if (showLeftMenu && showTags)
			showLeftMenu = false;
		showTags = !showTags;
		mouseMenu.Reset();
		mouse.state = INPUT_NULL;
		openTags_Button.pressed = false;
	}

	// Close Menu
	if (showLeftMenu && leftClose_Button.pressed){
		showLeftMenu = false;
		mouseMenu.Reset();
		mouse.state = INPUT_NULL;
		leftClose_Button.pressed = false;
	}
	
	// Help button
	if (!showLeftMenu && help_Button.pressed){
		OpenTutorial();
		mouse.state = INPUT_NULL;
	}

    //
	// Side bar stuff (Most input for the side bar is in table.hpp and tag.hpp)
	//
	if (showLeftMenu && mouse.position.x < sideMenuWidth){
		if (keyboard.newKey == KEY_KP_ADD || keyboard.newKey == KEY_EQUAL || add_Button.pressed){
			if (showTags){
				editTag = nullptr;
				parentTag = nullptr;
				newTag_ColorSelector.SetColor(Red);
				deleteTag_Button.text = "Cancel";
				newTagName_Field.text = "";
				TagWin.Show();
			}else{
				string folder = GetFolder();
				if (folder.length() && !((stat(folder.c_str(), &st) == 0) == 0)){
					locations.push_back(Location{GetName(folder), folder});
					sort(locations.begin(), locations.end(), locations[0].SortLocations);
				}
			}
		}
    }
}
