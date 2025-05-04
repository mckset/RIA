/*
    Handles all the input for the left side menu and adjacent button functions other than what is covered in the classes
*/

void LeftMenuInput(){
    // Menu toggle
	if ((keyboard.newKey == KEY_TAB) || mouse.Click(LM_DOWN) && openLoc.Hover()){
		if (!lMenu)
			lMenu = true;
		else if (lMenu && !tagView)
			lMenu = false;
		tagView = false;
		rmMenu.Reset();
		mouse.state = -1;
	}

	// Tag toggle
	if ((keyboard.newKey == KEY_TILDE) || mouse.Click(LM_DOWN) && openTags.Hover()){
		if (!lMenu && !tagView)
			lMenu = true;
		else if (lMenu && tagView)
			lMenu = false;
		tagView = !tagView;
		rmMenu.Reset();
		mouse.state = -1;
	}

	// Close Menu
	if (mouse.Click(LM_DOWN) && lMenu && closeLB.Hover()){
		lMenu = false;
		rmMenu.Reset();
		mouse.state = -1;
	}
	
	// Help button
	if (mouse.Click(LM_DOWN) && !lMenu && helpB.Hover()){
		OpenTutorial();
		mouse.state = -1;
	}

    //
	// Side bar stuff (Most input for the side bar is in table.hpp and tag.hpp)
	//
	if (lMenu && mouse.position.x < menuWidth){
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
    }
}
