/*
	Displays the steps of the tutorial
	tutorialStage stores the tutorial step and tutorialPage stores the substep
	tutorialPage usually only counts to 1 or 2
*/

enum Tutorial_Stages{
	MAIN_BOARD,
	LOCATIONS,
	TAGS,
	BOARDS,
	IMAGE_PACKS
};

int tutorialStage = 0;
int tutorialPage = 0;
float oldScale = 0;
Vector2 oldView;


void MainWindow::OpenTutorial(){
	showTutorial = true;
	display = MAIN_TUTORIAL;
	oldScale = *Scale;
	oldView = *View;
	*Scale = 1;
	*View = {0,0};
	showLeftMenu = false;
	showRightMenu = false;
	tutorialPage = 0;
	tutorialStage = 0;
	TagWin.Hide();
	Import.Hide();
}

Location tutorialLocation{
	"Folder",
	"",
	true,
	true
};

Tag tutorialTag{
	"Tag",
	Red
};

Board tutorialBoard{
	"Default",
	"",
};

void MainWindow::DrawEmpty(){
	//
	// Draw grid
	//
	for (int w = -2; w < (Width/GRID_SIZE)/(*Scale) + 1; w++){
		shape.Draw({0, ((float)w*GRID_SIZE*(*Scale) - ((int)(View->y/5) % (int)GRID_SIZE*(*Scale)))*2}, {fWidth*2, 3}, gridColor, POSITION_FIXED);
	}
	for (int h = -2; h < (Height/GRID_SIZE)/(*Scale) + 1; h++){
		shape.Draw({((float)h*GRID_SIZE*(*Scale) - ((int)(View->x/5) % (int)GRID_SIZE*(*Scale)))*2, 0}, {3, fHeight*2}, gridColor, POSITION_FIXED);
	}

	// Origin
	if (showOrigin){
		shape.Draw({0,View->y/2 - Height*2/(*Scale)}, {2/(*Scale), fHeight*16/(*Scale)}, gridOriginColor);
		shape.Draw({View->x/2 - Width*2/(*Scale), 0}, {fWidth*16/(*Scale), 2/(*Scale)}, gridOriginColor);
	}

	//
	// Selector box
	//
	if (showImageSelector){
		shape.DrawBox(mouse.position, mouse.dragOffset - mouse.position, imageSelectorColor, IMAGE_SELECTOR_BORDER_SIZE, POSITION_FIXED);
		shape.Draw(mouse.position, mouse.dragOffset - mouse.position, imageSelectorColor, POSITION_FIXED);
	}

	// Scale text
	if (zoomTextTimer){
		char buf[64];
		int perScale = (int)((*Scale)*100+1)/10;
		snprintf(buf, sizeof(buf), "%d", perScale);
		if (perScale >= 10){
			buf[2] = '0';
			buf[3] = '%';
			buf[4] = '\0';
		}else{
			buf[1] = '0';
			buf[2] = '%';
			buf[3] = '\0';
		}
		shape.Draw({fWidth/2-FONT_SIZE*2-PADDING,fHeight-FONT_SIZE*2}, {FONT_SIZE*4+PADDING*2, FONT_SIZE*4}, menuBackgroundColor, POSITION_FIXED);
		font.Write(buf, {0,fHeight-FONT_SIZE*2}, FONT_SIZE, White, POSITION_FIXED, Width, ALIGN_CENTER);
		zoomTextTimer--;
	}

	// Preview image
	if (tutorialStage == TAGS && tutorialPage == 2)
		shape.Draw({fWidth-(PREVIEW_IMAGE_SIZE+PADDING*4) - (showRightMenu ? sideMenuWidth+SCROLLBAR_SIZE : 0), 0}, {PREVIEW_IMAGE_SIZE+PADDING*4, PREVIEW_IMAGE_SIZE+PADDING*2+FONT_SIZE*3}, menuBackgroundColor, POSITION_FIXED);
	

	// Left menu
	if (showLeftMenu){
		shape.Draw({0}, {sideMenuWidth, fHeight}, menuBackgroundColor, POSITION_FIXED);

		if (showTags){
			float y = Height - FONT_SIZE*2;

			shape.Draw({0, y}, {sideMenuWidth, FONT_SIZE*2}, menuBackgroundColor, POSITION_FIXED);
			font.Write("Tags", {FONT_SIZE*2, y}, FONT_SIZE, fontColor, POSITION_FIXED, sideMenuWidth-SCROLLBAR_SIZE-FONT_SIZE*3, ALIGN_CENTER);
			add_Button.Draw({sideMenuWidth-FONT_SIZE*2, y}, {FONT_SIZE*2, FONT_SIZE*2}, ALIGN_CENTER);

			y -= FONT_SIZE;

			tutorialTag.Draw({0, y}, {sideMenuWidth - SCROLLBAR_SIZE, FONT_SIZE});
		
		}else{
			float y = Height - FONT_SIZE*2;

			shape.Draw({0, y}, {sideMenuWidth, FONT_SIZE*2}, menuBackgroundColor, POSITION_FIXED);
			font.Write("Folders", {FONT_SIZE*2, y}, FONT_SIZE, fontColor, POSITION_FIXED, sideMenuWidth-SCROLLBAR_SIZE-FONT_SIZE*3, ALIGN_CENTER);
			add_Button.Draw({sideMenuWidth-FONT_SIZE*2, y}, {FONT_SIZE*2, FONT_SIZE*2}, ALIGN_CENTER);
			refreshLocations_Button.Draw({0, y}, {sideMenuWidth-SCROLLBAR_SIZE-FONT_SIZE, FONT_SIZE*2}, ALIGN_CENTER);

			y -= FONT_SIZE;

			tutorialLocation.Draw({0, y}, {sideMenuWidth - SCROLLBAR_SIZE, FONT_SIZE});

		}

		if (!((tutorialStage == LOCATIONS || tutorialStage == TAGS) && tutorialPage <= 1)){
			if (showTags){
				openLocations_Button.Draw({sideMenuWidth-FONT_SIZE+SCROLLBAR_SIZE, fHeight-FONT_SIZE-BOARD_BUTTON_PADDING}, {BOARD_BUTTON_SIZE, FONT_SIZE*1});
			}else{
				openTags_Button.Draw({sideMenuWidth-FONT_SIZE+SCROLLBAR_SIZE, fHeight-FONT_SIZE-BOARD_BUTTON_PADDING}, {BOARD_BUTTON_SIZE, FONT_SIZE*1});
			}
			
			leftClose_Button.Draw({sideMenuWidth-FONT_SIZE+SCROLLBAR_SIZE, fHeight-FONT_SIZE*2-BOARD_BUTTON_PADDING}, {BOARD_BUTTON_SIZE, FONT_SIZE*1});
		}

	}else{
		openTags_Button.Draw({BOARD_BUTTON_PADDING, fHeight-FONT_SIZE-BOARD_BUTTON_PADDING}, {BOARD_BUTTON_SIZE, FONT_SIZE*1});
		openLocations_Button.Draw({BOARD_BUTTON_PADDING, fHeight-FONT_SIZE*2-BOARD_BUTTON_PADDING}, {BOARD_BUTTON_SIZE, FONT_SIZE*1});
		help_Button.Draw({BOARD_BUTTON_PADDING, fHeight-FONT_SIZE*3-BOARD_BUTTON_PADDING}, {BOARD_BUTTON_SIZE, FONT_SIZE*1});
	}

	// Right Menu
	if (showRightMenu){
		shape.Draw({fWidth, 0}, {-sideMenuWidth-SCROLLBAR_SIZE, fHeight}, menuBackgroundColor, POSITION_FIXED);

		float y = fHeight;
		shape.Draw({fWidth, y}, {-(sideMenuWidth+SCROLLBAR_SIZE), -FONT_SIZE*2}, menuBackgroundColor, POSITION_FIXED);
		font.Write("Boards", {fWidth-sideMenuWidth+FONT_SIZE*2, y-FONT_SIZE*2}, FONT_SIZE, fontColor, POSITION_FIXED, sideMenuWidth-FONT_SIZE*4, ALIGN_CENTER);
		addBoard_Button.Draw({fWidth-FONT_SIZE*2, y-FONT_SIZE*2}, {FONT_SIZE*2, FONT_SIZE*2}, ALIGN_CENTER);

		y -= FONT_SIZE;

		// Scroll bar end position
		y -=  sideMenuWidth+FONT_SIZE*3+BOARD_PADDING;
		tutorialBoard.Draw({fWidth-sideMenuWidth, y}, {sideMenuWidth, sideMenuWidth+boardNamePlate});

		if (!(tutorialStage == BOARDS && tutorialPage < 2)){
			imagePack_Button.Draw({fWidth - BOARD_BUTTON_SIZE-BOARD_BUTTON_PADDING-sideMenuWidth-SCROLLBAR_SIZE, fHeight - FONT_SIZE-BOARD_BUTTON_PADDING}, {BOARD_BUTTON_SIZE, FONT_SIZE});
			imageBoards_Button.text = "Close";
			imageBoards_Button.Draw({fWidth - BOARD_BUTTON_SIZE-BOARD_BUTTON_PADDING-sideMenuWidth-SCROLLBAR_SIZE, fHeight - FONT_SIZE*2-BOARD_BUTTON_PADDING}, {BOARD_BUTTON_SIZE, FONT_SIZE});
			save_Button.Draw({fWidth - BOARD_BUTTON_SIZE-BOARD_BUTTON_PADDING-sideMenuWidth-SCROLLBAR_SIZE, fHeight - FONT_SIZE*3-BOARD_BUTTON_PADDING}, {BOARD_BUTTON_SIZE, FONT_SIZE});
		}
	}else{
		imagePack_Button.Draw({fWidth - BOARD_BUTTON_SIZE-BOARD_BUTTON_PADDING, fHeight - FONT_SIZE-BOARD_BUTTON_PADDING}, {BOARD_BUTTON_SIZE, FONT_SIZE});
		imageBoards_Button.text = "Boards";
		imageBoards_Button.Draw({fWidth - BOARD_BUTTON_SIZE-BOARD_BUTTON_PADDING, fHeight - FONT_SIZE*2-BOARD_BUTTON_PADDING}, {BOARD_BUTTON_SIZE, FONT_SIZE});
		save_Button.Draw({fWidth - BOARD_BUTTON_SIZE-BOARD_BUTTON_PADDING, fHeight - FONT_SIZE*3-BOARD_BUTTON_PADDING}, {BOARD_BUTTON_SIZE, FONT_SIZE});
	}
	tags_Scrollbar.scroll = 0;
	locations_Scrollbar.scroll = 0;
	board_Scrollbar.scroll = 0;
}

void MainWindow::DrawStage(){
	if (tutorialStage == 0){

		// First message
		if (tutorialPage == MAIN_BOARD){
			font.Write("Use the left and right arrow keys", {0,fHeight/2+FONT_SIZE+8}, MEDIUM_FONT_SIZE, White, POSITION_FIXED, Width, ALIGN_CENTER);
			font.Write("to advance through the tutorial pages.", {0,fHeight/2}, MEDIUM_FONT_SIZE, White, POSITION_FIXED, Width, ALIGN_CENTER);
			font.Write("Press ESCAPE to leave.", {0,fHeight/2-FONT_SIZE-8}, MEDIUM_FONT_SIZE, White, POSITION_FIXED, Width, ALIGN_CENTER);
		}else if (tutorialPage == 1){
			font.Write("This area is the board", {0,fHeight/2+FONT_SIZE}, MEDIUM_FONT_SIZE, White, POSITION_FIXED, Width, ALIGN_CENTER);
			font.Write("You can move around with w/a/s/d,", {0,fHeight/2-8}, MEDIUM_FONT_SIZE, White, POSITION_FIXED, Width, ALIGN_CENTER);
			font.Write("the arrow keys, or by dragging", {0,fHeight/2-FONT_SIZE-16}, MEDIUM_FONT_SIZE, White, POSITION_FIXED, Width, ALIGN_CENTER);
			font.Write("with the middle mouse held down.", {0,fHeight/2-FONT_SIZE*2-28}, MEDIUM_FONT_SIZE, White, POSITION_FIXED, Width, ALIGN_CENTER);
			zoomTextTimer = 1;

			mouseMenu.position = {640, 264};
			mouseMenu.Draw();

			font.Write("If you right click an image or", {572,264+FONT_SIZE*3}, SMALL_FONT_SIZE, White, POSITION_FIXED);
			font.Write("a group of selected images, you", {572,264+FONT_SIZE*2}, SMALL_FONT_SIZE, White, POSITION_FIXED);
			font.Write("can alter them or add tags.", {572,264+FONT_SIZE}, SMALL_FONT_SIZE, White, POSITION_FIXED);

			font.Write("You can click and drag to select", {128,256+FONT_SIZE*2}, SMALL_FONT_SIZE, White, POSITION_FIXED);
			font.Write("images in a window.", {128,256+FONT_SIZE}, SMALL_FONT_SIZE, White, POSITION_FIXED);
			shape.DrawBox({128, 128}, {256,128}, imageSelectorColor, 2, POSITION_FIXED);
			shape.Draw({128, 128}, {256,128}, imageSelectorColor, POSITION_FIXED);

			//shape.DrawLine({});
			font.Write("Zoom percent", {0,fHeight-FONT_SIZE*4}, MEDIUM_FONT_SIZE, White, POSITION_FIXED, Width, ALIGN_CENTER);
			font.Write("Scrolling or pressing +/- will zoom in and out", {0,fHeight-FONT_SIZE*5-8}, MEDIUM_FONT_SIZE, White, POSITION_FIXED, Width, ALIGN_CENTER);

			shape.DrawBox({8, fHeight-FONT_SIZE*2-8}, {BOARD_BUTTON_SIZE, FONT_SIZE}, White, 2);
			font.Write("Click to open the folders menu", Vector2{24, fHeight-FONT_SIZE*2-8}+Vector2{BOARD_BUTTON_SIZE, FONT_SIZE/2}, SMALL_FONT_SIZE, White, POSITION_FIXED);
			font.Write("Or press TAB", Vector2{24, fHeight-FONT_SIZE*2-8}+Vector2{BOARD_BUTTON_SIZE, -FONT_SIZE/2}, SMALL_FONT_SIZE, White, POSITION_FIXED);
		}

	// Locations
	}else if (tutorialStage == LOCATIONS){

		if (tutorialPage == 0){
			shape.DrawBox({0, 0}, {sideMenuWidth-SCROLLBAR_SIZE, fHeight-FONT_SIZE*2}, White, 4);
			shape.DrawBox({sideMenuWidth-FONT_SIZE*2-4, fHeight-FONT_SIZE*2}, {FONT_SIZE*2, FONT_SIZE*2-4}, White, 4);
			font.Write("Click to add a new folder", {sideMenuWidth+8, fHeight-FONT_SIZE*1.5f}, SMALL_FONT_SIZE, White, POSITION_FIXED, sideMenuWidth*1.5f, ALIGN_LEFT, TEXT_WRAP);
			
			font.Write(
				"All saved folders are displayed here. Clicking on a folder will display all it's contents including images and subfolders.\n\n"
				"Files that have been tagged will have a white box to the left of their name."
				, Vector2{sideMenuWidth+8.0f, fHeight-FONT_SIZE*6}, MEDIUM_FONT_SIZE, White, POSITION_FIXED, sideMenuWidth*1.5f, ALIGN_LEFT, TEXT_WRAP);

		}else if (tutorialPage == 1){
			shape.DrawBox({0, fHeight-FONT_SIZE*2}, {sideMenuWidth-FONT_SIZE*2-4, FONT_SIZE*2-4}, White, 4);
			shape.DrawBox({sideMenuWidth-FONT_SIZE*2-12, fHeight-FONT_SIZE*3-4}, {FONT_SIZE+4, FONT_SIZE+4}, White, 4);

			font.Write("Click the header to refresh the folders", {sideMenuWidth+8, fHeight-FONT_SIZE*1.5f}, SMALL_FONT_SIZE, White, POSITION_FIXED, sideMenuWidth*1.5f, ALIGN_LEFT, TEXT_WRAP);

			font.Write("Pressing the \"x\" button or delete key while hovering over the main folder will delete it from the list.", 
				Vector2{sideMenuWidth+8.0f, fHeight-FONT_SIZE*6+8}, MEDIUM_FONT_SIZE, White, POSITION_FIXED, sideMenuWidth*1.5f, ALIGN_LEFT, TEXT_WRAP);

		}else if (tutorialPage == 2){
			shape.DrawBox({8+sideMenuWidth, fHeight-FONT_SIZE-8}, {BOARD_BUTTON_SIZE, FONT_SIZE}, White, 2);
			font.Write("Click to open the tag menu", Vector2{24+sideMenuWidth, fHeight-FONT_SIZE-12}+Vector2{BOARD_BUTTON_SIZE, FONT_SIZE/2}, SMALL_FONT_SIZE, White, POSITION_FIXED);
			font.Write("Or press ~", Vector2{24+sideMenuWidth, fHeight-FONT_SIZE-12}+Vector2{BOARD_BUTTON_SIZE, -FONT_SIZE/2}, SMALL_FONT_SIZE, White, POSITION_FIXED);
		}
	
	// Tags
	}else if (tutorialStage == TAGS){
		if (tutorialPage == 0){
			shape.DrawBox({0, 0}, {sideMenuWidth-SCROLLBAR_SIZE, fHeight-FONT_SIZE*2}, White, 4);
			shape.DrawBox({sideMenuWidth-FONT_SIZE*2-4, fHeight-FONT_SIZE*2}, {FONT_SIZE*2+4, FONT_SIZE*2-4}, White, 4);
			font.Write("Click to add a new tag", {sideMenuWidth+8, fHeight-FONT_SIZE*1.5f}, SMALL_FONT_SIZE, White, POSITION_FIXED);

			font.Write(
				"All saved tags are displayed here. Clicking on a tag will display its images and sub tags. Tags can only handle one level of sub tags.",				
				Vector2{sideMenuWidth+8.0f, fHeight-FONT_SIZE*6}, MEDIUM_FONT_SIZE, White, POSITION_FIXED, sideMenuWidth*1.5f, ALIGN_LEFT, TEXT_WRAP);

			}else if (tutorialPage == 1){

				tutorialTag.expanded = true;
				shape.DrawBox({0, fHeight-FONT_SIZE*3}, {FONT_SIZE-4, FONT_SIZE-4}, White, 4);
				shape.DrawBox({sideMenuWidth-SCROLLBAR_SIZE-FONT_SIZE*2, fHeight-FONT_SIZE*3}, {FONT_SIZE*2-4, FONT_SIZE-4}, White, 4);
				font.Write(
					"While viewing a tag, 2 buttons will appear. The edit button on the right of the tag and the \"+\" or add button on the left.\n\n"
					"Pressing the edit button or right clicking the tag will allow you to edit the tags name and color or delete it.\n\n"
					"You can also press the delete key while hovering over a tag to delete it.\n\n"
					"Pressing the \"+\" button will add a new sub tag.",				
					Vector2{sideMenuWidth+8.0f, fHeight-FONT_SIZE*6}, MEDIUM_FONT_SIZE, White, POSITION_FIXED, sideMenuWidth*1.5f, ALIGN_LEFT, TEXT_WRAP);
		
		}else if (tutorialPage == 2){
			font.Write("Clicking on an image from a tag", {fWidth-(PREVIEW_IMAGE_SIZE+PADDING*4)-FONT_SIZE*5, PREVIEW_IMAGE_SIZE+FONT_SIZE*4 + FONT_SIZE*6+16}, MEDIUM_FONT_SIZE, White, POSITION_FIXED);
			font.Write("or location will load it into",  {fWidth-(PREVIEW_IMAGE_SIZE+PADDING*4)-FONT_SIZE*5, PREVIEW_IMAGE_SIZE+FONT_SIZE*4 + FONT_SIZE*5+8}, MEDIUM_FONT_SIZE, White, POSITION_FIXED);
			font.Write("this preview window. Clicking",  {fWidth-(PREVIEW_IMAGE_SIZE+PADDING*4)-FONT_SIZE*5, PREVIEW_IMAGE_SIZE+FONT_SIZE*4 + FONT_SIZE*4}, MEDIUM_FONT_SIZE, White, POSITION_FIXED);
			font.Write("on the board will place the",  {fWidth-(PREVIEW_IMAGE_SIZE+PADDING*4)-FONT_SIZE*5, PREVIEW_IMAGE_SIZE+FONT_SIZE*4 + FONT_SIZE*3-8}, MEDIUM_FONT_SIZE, White, POSITION_FIXED);
			font.Write("image at the mouse position.",  {fWidth-(PREVIEW_IMAGE_SIZE+PADDING*4)-FONT_SIZE*5, PREVIEW_IMAGE_SIZE+FONT_SIZE*4 + FONT_SIZE*2-16}, MEDIUM_FONT_SIZE, White, POSITION_FIXED);

			font.Write(
				"While an image is in the preview window, right clicking a tag or sub tag will add or remove the image from the tag depending on it already being tag or not.\n\n"
				"You can also edit an images tag in a similar way with the right mouse menu.\n\n"
				"If an image has a tag, a white circle will appear to the left of the tags the image has.\n\n",
				Vector2{sideMenuWidth+8.0f, fHeight-FONT_SIZE*6}, MEDIUM_FONT_SIZE, White, POSITION_FIXED, sideMenuWidth*1.5f, ALIGN_LEFT, TEXT_WRAP);

			shape.DrawBox({fWidth - FONT_SIZE*6-12, fHeight - FONT_SIZE*2-8}, {FONT_SIZE*6+4, FONT_SIZE}, White, 2);
			font.Write("Click to open the boards menu", {fWidth - FONT_SIZE*22, fHeight - FONT_SIZE*2-8}, SMALL_FONT_SIZE, White, POSITION_FIXED);
			font.Write("or press escape", {fWidth - FONT_SIZE*22, fHeight - FONT_SIZE*3-8}, SMALL_FONT_SIZE, White, POSITION_FIXED);		
		}

	// Boards
	}else if (tutorialStage == BOARDS){
		if (tutorialPage == 0){
			shape.DrawBox({fWidth - sideMenuWidth, 0}, {sideMenuWidth-4, fHeight-FONT_SIZE*2-4}, White, 4);
			shape.DrawBox({fWidth - FONT_SIZE*2, fHeight-FONT_SIZE*2}, {FONT_SIZE*2-4,FONT_SIZE*2-4}, White, 4);

			font.Write("Pressing the \"+\" button will let you add or copy an image board.", 
				{fWidth - FONT_SIZE*23 - sideMenuWidth, fHeight - FONT_SIZE*3}, SMALL_FONT_SIZE, White, POSITION_FIXED, sideMenuWidth*1.0f, ALIGN_LEFT, TEXT_WRAP);

			font.Write("All saved image boards are displayed here. Clicking on one will load it.", 
				{fWidth - FONT_SIZE*24 - sideMenuWidth, fHeight/2 + FONT_SIZE*2-8}, MEDIUM_FONT_SIZE, White, POSITION_FIXED, sideMenuWidth*1.0f, ALIGN_RIGHT, TEXT_WRAP);
		}else if (tutorialPage == 1){
			shape.DrawBox({fWidth - FONT_SIZE*2-BOARD_PADDING, fHeight-FONT_SIZE*4-BOARD_PADDING*2}, {FONT_SIZE*2-4,FONT_SIZE*2-4}, White, 4);
			shape.DrawBox({fWidth - sideMenuWidth, fHeight-FONT_SIZE*2-sideMenuWidth-boardNamePlate-BOARD_PADDING}, {sideMenuWidth-4,FONT_SIZE*2-4}, White, 4);

			font.Write(
				"Pressing the \"x\" button or the delete key while hovering over a board will delete it. This action is permanent.\n\n"
				"The name of a board is displayed under it's screenshot. A lighter background means that the board is the one currently in use. "
				"Clicking on the name will let you rename a board.", 
				{fWidth - FONT_SIZE*21 - sideMenuWidth, fHeight - FONT_SIZE*3}, MEDIUM_FONT_SIZE, White, POSITION_FIXED, sideMenuWidth*1.0f, ALIGN_RIGHT, TEXT_WRAP);
		}
	
	// Image packs
	}else if (tutorialStage == IMAGE_PACKS){
		if (tutorialPage == 0){
			shape.DrawBox({fWidth - FONT_SIZE*6-10, fHeight - FONT_SIZE-8}, {FONT_SIZE*6, FONT_SIZE}, White, 2);
			shape.DrawBox({fWidth - FONT_SIZE*6-10, fHeight - FONT_SIZE*3-8}, {FONT_SIZE*6, FONT_SIZE}, White, 2);

			font.Write("Click to open the image pack window", {fWidth - FONT_SIZE*26, fHeight - FONT_SIZE-8}, SMALL_FONT_SIZE, White, POSITION_FIXED);

			font.Write("RIA does not save information or", {fWidth - FONT_SIZE*26, fHeight - FONT_SIZE*3-8}, SMALL_FONT_SIZE, White, POSITION_FIXED);
			font.Write("boards when closed. Be sure to", {fWidth - FONT_SIZE*26, fHeight - FONT_SIZE*4-8}, SMALL_FONT_SIZE, White, POSITION_FIXED);
			font.Write("save frequently", {fWidth - FONT_SIZE*26, fHeight - FONT_SIZE*5-8}, SMALL_FONT_SIZE, White, POSITION_FIXED);

			font.Write("You can drag and drop images,", {0,fHeight/2+FONT_SIZE}, MEDIUM_FONT_SIZE, White, POSITION_FIXED, Width, ALIGN_CENTER);
			font.Write("folders, and boards into RIA. Images", {0,fHeight/2-8}, MEDIUM_FONT_SIZE, White, POSITION_FIXED, Width, ALIGN_CENTER);
			font.Write("from the internet will be downloaded", {0,fHeight/2-FONT_SIZE-16}, MEDIUM_FONT_SIZE, White, POSITION_FIXED, Width, ALIGN_CENTER);
			font.Write("to RIA's downloads folder.", {0,fHeight/2-FONT_SIZE*2-28}, MEDIUM_FONT_SIZE, White, POSITION_FIXED, Width, ALIGN_CENTER);

		//256 360
		}else if (tutorialPage == 1){
			showTutorial = false;
			display = MAIN_APP;
			showLeftMenu = false;
			showRightMenu = false;
			*Scale = oldScale;
			*View = oldView;
		}
	}
}

void MainWindow::DrawTutorial(){
	DrawEmpty();
	DrawStage();
}