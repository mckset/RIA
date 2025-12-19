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


void OpenTutorial(){
	showTutorial = true;
	Main.Render = &DrawTutorial;
	Main.Input = &TutorialInput;
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


void DrawEmpty(){
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
	if (previewImg.loaded){
		shape.Draw({fWidth-(PREVIEW_IMAGE_SIZE+PADDING*4) - (showRightMenu ? sideMenuWidth+SCROLLBAR_SIZE : 0), 0}, {PREVIEW_IMAGE_SIZE+PADDING*4, PREVIEW_IMAGE_SIZE+PADDING*2+FONT_SIZE*3}, menuBackgroundColor, POSITION_FIXED);

		// Draw image centered 
		previewImg.Draw({fWidth - (PREVIEW_IMAGE_SIZE+PADDING*2) + (PREVIEW_IMAGE_SIZE-previewImg.size.x)/2 - (showRightMenu ? sideMenuWidth+SCROLLBAR_SIZE : 0), (PREVIEW_IMAGE_SIZE-previewImg.size.y)/2 + PADDING},
			previewImg.size, White, POSITION_FIXED);

		font.Write("Preview", 
			{fWidth-(PREVIEW_IMAGE_SIZE+PADDING*3) - (showRightMenu ? sideMenuWidth+SCROLLBAR_SIZE : 0), PREVIEW_IMAGE_SIZE+PADDING*2}, 
			FONT_SIZE, White, POSITION_FIXED, PREVIEW_IMAGE_SIZE+PADDING*2, ALIGN_CENTER);
		
		// Mouse 
		if (mouse.position.x > sideMenuWidth || !showLeftMenu){
			shape.Draw(
				mouse.position - (Vector2{(float)previewImg.img.width, (float)previewImg.img.height} * (*Scale)/2), 
				Vector2{(float)previewImg.img.width, (float)previewImg.img.height} * (*Scale), 
				menuBackgroundColor, POSITION_FIXED);
		}
	}else if (tutorialStage == TAGS && tutorialPage == 1)
		shape.Draw({fWidth-(PREVIEW_IMAGE_SIZE+PADDING*4) - (showRightMenu ? sideMenuWidth+SCROLLBAR_SIZE : 0), 0}, {PREVIEW_IMAGE_SIZE+PADDING*4, PREVIEW_IMAGE_SIZE+PADDING*2+FONT_SIZE*3}, menuBackgroundColor, POSITION_FIXED);
	

	// Left menu
	if (showLeftMenu){
		shape.Draw({0}, {sideMenuWidth, fHeight}, menuBackgroundColor, POSITION_FIXED);

		if (showTags)
			DrawTags();
		else
			DrawLocations();

		if (!((tutorialStage == LOCATIONS || tutorialStage == TAGS) && tutorialPage == 0)){
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
		DrawBoards();

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

void DrawStage(){
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
			font.Write("Click to open the locations menu", Vector2{24, fHeight-FONT_SIZE*2-8}+Vector2{BOARD_BUTTON_SIZE, FONT_SIZE/2}, SMALL_FONT_SIZE, White, POSITION_FIXED);
			font.Write("Or press TAB", Vector2{24, fHeight-FONT_SIZE*2-8}+Vector2{BOARD_BUTTON_SIZE, -FONT_SIZE/2}, SMALL_FONT_SIZE, White, POSITION_FIXED);
		}

	// Locations
	}else if (tutorialStage == LOCATIONS){

		if (tutorialPage == 0){
			shape.DrawBox({0, 0}, {sideMenuWidth-SCROLLBAR_SIZE, fHeight-FONT_SIZE*2}, White, 4);
			shape.DrawBox({sideMenuWidth-FONT_SIZE*2-SCROLLBAR_SIZE-4, fHeight-FONT_SIZE*2}, {FONT_SIZE*2+4, FONT_SIZE*2-4}, White, 4);
			font.Write("Click to add a new location", {sideMenuWidth+8, fHeight-FONT_SIZE*1.5f}, SMALL_FONT_SIZE, White, POSITION_FIXED);
			font.Write("All saved folders are displayed here.", Vector2{sideMenuWidth+8.0f, fHeight-FONT_SIZE*6}, MEDIUM_FONT_SIZE, White, POSITION_FIXED);
			font.Write("Clicking on a folder will display all it's", Vector2{sideMenuWidth+8.0f, fHeight-FONT_SIZE*7-8}, MEDIUM_FONT_SIZE, White, POSITION_FIXED);
			font.Write("contents including images and subfolders.", Vector2{sideMenuWidth+8.0f, fHeight-FONT_SIZE*8-16}, MEDIUM_FONT_SIZE, White, POSITION_FIXED);

			font.Write("Files that have been tagged will", Vector2{sideMenuWidth+8.0f, fHeight-FONT_SIZE*11+8}, MEDIUM_FONT_SIZE, White, POSITION_FIXED);
			font.Write("have a white box to the left of", Vector2{sideMenuWidth+8.0f, fHeight-FONT_SIZE*12}, MEDIUM_FONT_SIZE, White, POSITION_FIXED);
			font.Write("their name.", Vector2{sideMenuWidth+8.0f, fHeight-FONT_SIZE*13-8}, MEDIUM_FONT_SIZE, White, POSITION_FIXED);

			font.Write("Pressing delete while hovering over", Vector2{sideMenuWidth+8.0f, fHeight-FONT_SIZE*16+8}, MEDIUM_FONT_SIZE, White, POSITION_FIXED);
			font.Write("the main folder will delete it from", Vector2{sideMenuWidth+8.0f, fHeight-FONT_SIZE*17}, MEDIUM_FONT_SIZE, White, POSITION_FIXED);
			font.Write("the list.", Vector2{sideMenuWidth+8.0f, fHeight-FONT_SIZE*18-8}, MEDIUM_FONT_SIZE, White, POSITION_FIXED);

		}else if (tutorialPage == 1){
			shape.DrawBox({8+sideMenuWidth, fHeight-FONT_SIZE-8}, {BOARD_BUTTON_SIZE, FONT_SIZE}, White, 2);
			font.Write("Click to open the tag menu", Vector2{24+sideMenuWidth, fHeight-FONT_SIZE-12}+Vector2{BOARD_BUTTON_SIZE, FONT_SIZE/2}, SMALL_FONT_SIZE, White, POSITION_FIXED);
			font.Write("Or press ~", Vector2{24+sideMenuWidth, fHeight-FONT_SIZE-12}+Vector2{BOARD_BUTTON_SIZE, -FONT_SIZE/2}, SMALL_FONT_SIZE, White, POSITION_FIXED);
		}
	
	// Tags
	}else if (tutorialStage == TAGS){
		if (tutorialPage == 0){
			shape.DrawBox({0, 0}, {sideMenuWidth-SCROLLBAR_SIZE, fHeight-FONT_SIZE*2}, White, 4);
			shape.DrawBox({sideMenuWidth-FONT_SIZE*2-SCROLLBAR_SIZE-4, fHeight-FONT_SIZE*2}, {FONT_SIZE*2+4, FONT_SIZE*2-4}, White, 4);
			font.Write("Click to add a new tag", {sideMenuWidth+8, fHeight-FONT_SIZE*1.5f}, SMALL_FONT_SIZE, White, POSITION_FIXED);
			font.Write("All saved tags are displayed here.", Vector2{sideMenuWidth+8.0f, fHeight-FONT_SIZE*6}, MEDIUM_FONT_SIZE, White, POSITION_FIXED);
			font.Write("Clicking on a tag will display its", Vector2{sideMenuWidth+8.0f, fHeight-FONT_SIZE*7-8}, MEDIUM_FONT_SIZE, White, POSITION_FIXED);
			font.Write("images and sub tags. Tags can only", Vector2{sideMenuWidth+8.0f, fHeight-FONT_SIZE*8-16}, MEDIUM_FONT_SIZE, White, POSITION_FIXED);
			font.Write("handle one level of sub tags.", Vector2{sideMenuWidth+8.0f, fHeight-FONT_SIZE*9-24}, MEDIUM_FONT_SIZE, White, POSITION_FIXED);

			font.Write("While viewing a tag, 2 buttons", Vector2{sideMenuWidth+8.0f, fHeight-FONT_SIZE*12}, MEDIUM_FONT_SIZE, White, POSITION_FIXED);
			font.Write("will appear. The edit button", Vector2{sideMenuWidth+8.0f, fHeight-FONT_SIZE*13-8}, MEDIUM_FONT_SIZE, White, POSITION_FIXED);
			font.Write("on the right of the tag and the", Vector2{sideMenuWidth+8.0f, fHeight-FONT_SIZE*14-16}, MEDIUM_FONT_SIZE, White, POSITION_FIXED);
			font.Write("\"+\" or add button on the left.", Vector2{sideMenuWidth+8.0f, fHeight-FONT_SIZE*15-24}, MEDIUM_FONT_SIZE, White, POSITION_FIXED);

			font.Write("Pressing the edit button or right", Vector2{sideMenuWidth+8.0f, fHeight-FONT_SIZE*18}, MEDIUM_FONT_SIZE, White, POSITION_FIXED);
			font.Write("clicking the tag will allow you to", Vector2{sideMenuWidth+8.0f, fHeight-FONT_SIZE*19-8}, MEDIUM_FONT_SIZE, White, POSITION_FIXED);
			font.Write("edit the tags name and color.", Vector2{sideMenuWidth+8.0f, fHeight-FONT_SIZE*20-16}, MEDIUM_FONT_SIZE, White, POSITION_FIXED);
			
			font.Write("Pressing the \"+\" button will", Vector2{sideMenuWidth+8.0f, fHeight-FONT_SIZE*23}, MEDIUM_FONT_SIZE, White, POSITION_FIXED);
			font.Write("add a new sub tag.", Vector2{sideMenuWidth+8.0f, fHeight-FONT_SIZE*24-8}, MEDIUM_FONT_SIZE, White, POSITION_FIXED);

			font.Write("Pressing delete while hovering", Vector2{sideMenuWidth+8.0f, fHeight-FONT_SIZE*27}, MEDIUM_FONT_SIZE, White, POSITION_FIXED);
			font.Write("over a tag or sub tag will ", Vector2{sideMenuWidth+8.0f, fHeight-FONT_SIZE*28-8}, MEDIUM_FONT_SIZE, White, POSITION_FIXED);
			font.Write("delete it from the list.", Vector2{sideMenuWidth+8.0f, fHeight-FONT_SIZE*29-16}, MEDIUM_FONT_SIZE, White, POSITION_FIXED);
		
		}else if (tutorialPage == 1){

			font.Write("Clicking on an image from a tag", {fWidth-(PREVIEW_IMAGE_SIZE+PADDING*4)-FONT_SIZE*5, PREVIEW_IMAGE_SIZE+FONT_SIZE*4 + FONT_SIZE*6+16}, MEDIUM_FONT_SIZE, White, POSITION_FIXED);
			font.Write("or location will load it into",  {fWidth-(PREVIEW_IMAGE_SIZE+PADDING*4)-FONT_SIZE*5, PREVIEW_IMAGE_SIZE+FONT_SIZE*4 + FONT_SIZE*5+8}, MEDIUM_FONT_SIZE, White, POSITION_FIXED);
			font.Write("this preview window. Clicking",  {fWidth-(PREVIEW_IMAGE_SIZE+PADDING*4)-FONT_SIZE*5, PREVIEW_IMAGE_SIZE+FONT_SIZE*4 + FONT_SIZE*4}, MEDIUM_FONT_SIZE, White, POSITION_FIXED);
			font.Write("on the board will place the",  {fWidth-(PREVIEW_IMAGE_SIZE+PADDING*4)-FONT_SIZE*5, PREVIEW_IMAGE_SIZE+FONT_SIZE*4 + FONT_SIZE*3-8}, MEDIUM_FONT_SIZE, White, POSITION_FIXED);
			font.Write("image at the mouse position.",  {fWidth-(PREVIEW_IMAGE_SIZE+PADDING*4)-FONT_SIZE*5, PREVIEW_IMAGE_SIZE+FONT_SIZE*4 + FONT_SIZE*2-16}, MEDIUM_FONT_SIZE, White, POSITION_FIXED);

			font.Write("While an image is in the preview", Vector2{sideMenuWidth+8.0f, fHeight-FONT_SIZE*5+8}, MEDIUM_FONT_SIZE, White, POSITION_FIXED);
			font.Write("window, right clicking a tag or sub", Vector2{sideMenuWidth+8.0f, fHeight-FONT_SIZE*6}, MEDIUM_FONT_SIZE, White, POSITION_FIXED);
			font.Write("tag will add or remove the image", Vector2{sideMenuWidth+8.0f, fHeight-FONT_SIZE*7-8}, MEDIUM_FONT_SIZE, White, POSITION_FIXED);
			font.Write("from the tag depending on if it", Vector2{sideMenuWidth+8.0f, fHeight-FONT_SIZE*8-16}, MEDIUM_FONT_SIZE, White, POSITION_FIXED);
			font.Write("already has the tag.", Vector2{sideMenuWidth+8.0f, fHeight-FONT_SIZE*9-24}, MEDIUM_FONT_SIZE, White, POSITION_FIXED);

			font.Write("If an image has a tag, a white", Vector2{sideMenuWidth+8.0f, fHeight-FONT_SIZE*12}, MEDIUM_FONT_SIZE, White, POSITION_FIXED);
			font.Write("circle will appear to the left of", Vector2{sideMenuWidth+8.0f, fHeight-FONT_SIZE*13-8}, MEDIUM_FONT_SIZE, White, POSITION_FIXED);
			font.Write("the tags the image has.", Vector2{sideMenuWidth+8.0f, fHeight-FONT_SIZE*14-16}, MEDIUM_FONT_SIZE, White, POSITION_FIXED);

			shape.DrawBox({fWidth - FONT_SIZE*6-12, fHeight - FONT_SIZE*2-8}, {FONT_SIZE*6, FONT_SIZE}, White, 2);
			font.Write("Click to open the boards menu", {fWidth - FONT_SIZE*22, fHeight - FONT_SIZE*2-8}, SMALL_FONT_SIZE, White, POSITION_FIXED);
			font.Write("or press escape", {fWidth - FONT_SIZE*22, fHeight - FONT_SIZE*3-8}, SMALL_FONT_SIZE, White, POSITION_FIXED);		
		}

	// Boards
	}else if (tutorialStage == BOARDS){
		if (tutorialPage == 0){
			shape.DrawBox({fWidth - sideMenuWidth, 0}, {sideMenuWidth, fHeight-FONT_SIZE*3}, White, 4);
			shape.DrawBox({fWidth - sideMenuWidth, fHeight-FONT_SIZE*3}, {sideMenuWidth,FONT_SIZE}, White, 4);
			font.Write("Shows the name of the current image board.", {fWidth - FONT_SIZE*23 - sideMenuWidth, fHeight - FONT_SIZE*3}, SMALL_FONT_SIZE, White, POSITION_FIXED);
			font.Write("Clicking it will let you edit the name.", {fWidth - FONT_SIZE*23 - sideMenuWidth, fHeight - FONT_SIZE*4}, SMALL_FONT_SIZE, White, POSITION_FIXED);

			font.Write("All saved image boards are", {fWidth - FONT_SIZE*24 - sideMenuWidth, fHeight/2 + FONT_SIZE*2-8}, MEDIUM_FONT_SIZE, White, POSITION_FIXED);
			font.Write("displayed here. Pressing ", {fWidth - FONT_SIZE*24 - sideMenuWidth, fHeight/2 + FONT_SIZE-16}, MEDIUM_FONT_SIZE, White, POSITION_FIXED);	
			font.Write("delete while hovering over", {fWidth - FONT_SIZE*24 - sideMenuWidth, fHeight/2 -24}, MEDIUM_FONT_SIZE, White, POSITION_FIXED);
			font.Write("an image board will delete it.", {fWidth - FONT_SIZE*24 - sideMenuWidth, fHeight/2 - FONT_SIZE-32}, MEDIUM_FONT_SIZE, White, POSITION_FIXED);	

			font.Write("You can also delete an image", {fWidth - FONT_SIZE*24 - sideMenuWidth, fHeight/2 - FONT_SIZE*4-8}, MEDIUM_FONT_SIZE, White, POSITION_FIXED);
			font.Write("board from the boards folder.", {fWidth - FONT_SIZE*24 - sideMenuWidth, fHeight/2 - BOARD_BUTTON_SIZE-16}, MEDIUM_FONT_SIZE, White, POSITION_FIXED);	
		}else if (tutorialPage == 1){
			shape.DrawBox({fWidth - sideMenuWidth, fHeight-FONT_SIZE*3}, {sideMenuWidth,FONT_SIZE}, White, 4);
			font.Write("When adding a new board, the", {fWidth - FONT_SIZE*21 - sideMenuWidth, fHeight - FONT_SIZE*3}, SMALL_FONT_SIZE, White, POSITION_FIXED);
			font.Write("text will clear for you to type", {fWidth - FONT_SIZE*21 - sideMenuWidth, fHeight - FONT_SIZE*4}, SMALL_FONT_SIZE, White, POSITION_FIXED);
			font.Write("the name of your new board.", {fWidth - FONT_SIZE*21 - sideMenuWidth, fHeight - BOARD_BUTTON_SIZE}, SMALL_FONT_SIZE, White, POSITION_FIXED);

			font.Write("Pressing enter will create the", {fWidth - FONT_SIZE*21 - sideMenuWidth, fHeight - FONT_SIZE*8}, SMALL_FONT_SIZE, White, POSITION_FIXED);
			font.Write("board and save all the images", {fWidth - FONT_SIZE*21 - sideMenuWidth, fHeight - FONT_SIZE*9}, SMALL_FONT_SIZE, White, POSITION_FIXED);
			font.Write("in the view to the new board.", {fWidth - FONT_SIZE*21 - sideMenuWidth, fHeight - FONT_SIZE*10}, SMALL_FONT_SIZE, White, POSITION_FIXED);

			font.Write("Pressing escape will cancel it.", {fWidth - FONT_SIZE*21 - sideMenuWidth, fHeight - FONT_SIZE*11}, SMALL_FONT_SIZE, White, POSITION_FIXED);
		}
	
	// Image packs
	}else if (tutorialStage == IMAGE_PACKS){
		if (tutorialPage == 0){
			shape.DrawBox({fWidth - FONT_SIZE*6-12, fHeight - FONT_SIZE-8}, {FONT_SIZE*6, FONT_SIZE}, White, 2);
			shape.DrawBox({fWidth - FONT_SIZE*6-12, fHeight - FONT_SIZE*3-8}, {FONT_SIZE*6, FONT_SIZE}, White, 2);

			font.Write("Click to open the image pack window", {fWidth - FONT_SIZE*26, fHeight - FONT_SIZE-8}, SMALL_FONT_SIZE, White, POSITION_FIXED);

			font.Write("RIA does not save information or", {fWidth - FONT_SIZE*26, fHeight - FONT_SIZE*3-8}, SMALL_FONT_SIZE, White, POSITION_FIXED);
			font.Write("boards when closed. All changes", {fWidth - FONT_SIZE*26, fHeight - FONT_SIZE*4-8}, SMALL_FONT_SIZE, White, POSITION_FIXED);
			font.Write("must be saved by the user.", {fWidth - FONT_SIZE*26, fHeight - FONT_SIZE*5-8}, SMALL_FONT_SIZE, White, POSITION_FIXED);

		//256 360
		}else if (tutorialPage == 1){
			showTutorial = false;
			Main.Render = &DrawApp;
			Main.Input = &MainInput;
			showLeftMenu = false;
			showRightMenu = false;
			*Scale = oldScale;
			*View = oldView;
		}
	}
}

void DrawTutorial(){
	DrawEmpty();
	DrawStage();
}