/*
	Displays the steps of the tutorial
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
int tStage = 0;
int mStage = 0;
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
	lMenu = false;
	rMenu = false;
	mStage = 0;
	tStage = 0;
	TagWin.Hide();
	Import.Hide();
}


void DrawEmpty(){
	//
	// Draw grid
	//
	for (int w = -2; w < (Width/gridSize)/(*Scale) + 1; w++){
		shape.Draw({0, ((float)w*gridSize*(*Scale) - ((int)(View->y/5) % (int)gridSize*(*Scale)))*2}, {fWidth*2, 3}, grid, true);
	}
	for (int h = -2; h < (Height/gridSize)/(*Scale) + 1; h++){
		shape.Draw({((float)h*gridSize*(*Scale) - ((int)(View->x/5) % (int)gridSize*(*Scale)))*2, 0}, {3, fHeight*2}, grid, true);
	}

	// Origin
	if (drawOrigin){
		shape.Draw({0,View->y/2 - Height*2/(*Scale)}, {2/(*Scale), fHeight*16/(*Scale)}, cOrigin, false);
		shape.Draw({View->x/2 - Width*2/(*Scale), 0}, {fWidth*16/(*Scale), 2/(*Scale)}, cOrigin, false);
	}

	//
	// Selector box
	//
	if (selector){
		shape.DrawBox(mouse.position, mouse.dragOff.Subtract(mouse.position), cSelector, 2, true, true);
		shape.Draw(mouse.position, mouse.dragOff.Subtract(mouse.position), cSelector, true);
	}

	// Scale text
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
	if (showZoom){
		shape.Draw({fWidth/2-fontSize*1.5f-8,fHeight-fontSize*1.5f}, {fontSize*3+16, fontSize*1.5f}, menuBackground, true);
		font.Write(buf, {0,fHeight-fontSize*1.5f}, fontSize*.75f, White, true, Width, 1);
		showZoom--;
	}

	//
	// Image preview
	//
	if (previewImg.img.loaded || (tStage == 2 && mStage == 1)){
		shape.Draw({fWidth/5*4-fontSize*4 - (rMenu ? menuWidth+scrollbarSize : 0), 0}, {fWidth/5+fontSize*4, fWidth/5+fontSize*4}, menuBackground, true);

		// Draw image centered 
		previewImg.Draw(
			{fWidth/5*4+(Width/5-previewImg.size.x)/2-fontSize*2 - (rMenu ? menuWidth+scrollbarSize : 0), (fWidth/5 - previewImg.size.y)/2 + fontSize/2}, 
			previewImg.size, White, true);

		font.Write("Preview", {fWidth/5*4-fontSize*4, fWidth/5+fontSize}, fontSize, White, true, Width/5+fontSize*4, 1);
		
		// Mouse 
		if (mouse.position.x > menuWidth || !lMenu){
			shape.Draw(
				mouse.position.Subtract(Vector2{(float)previewImg.img.width, (float)previewImg.img.height}.Multiply((*Scale)).Divide(2)), 
				Vector2{(float)previewImg.img.width, (float)previewImg.img.height}.Multiply((*Scale)), 
				menuBackground, true);
		}
	}


	// Left buttons
	if (lMenu){
		shape.Draw({0}, {menuWidth, fHeight}, menuBackground, true);
		if (tagView)
			DrawTags();
		else
			DrawLocations();

		// Menu Buttons
		if (!tagView){
			if (!(tStage == 1 && mStage == 0)) openTags.Draw({menuWidth-fontSize+scrollbarSize, fHeight-fontSize-8}, {fontSize*5, fontSize*1});
			openLoc.visible = false;
		}else{
			if (!(tStage == 2 && mStage == 0)) openLoc.Draw({menuWidth-fontSize+scrollbarSize, fHeight-fontSize-8}, {fontSize*5, fontSize*1});
			openTags.visible = false;
		}
		if (!((tStage == 1 || tStage == 2) && mStage == 0)) closeLB.Draw({menuWidth-fontSize+scrollbarSize, fHeight-fontSize*2-8}, {fontSize*5, fontSize*1});
		
	}else{
		openTags.Draw({8, fHeight-fontSize-8}, {fontSize*5, fontSize*1});
		openLoc.Draw({8, fHeight-fontSize*2-8}, {fontSize*5, fontSize*1});
		helpB.Draw({8, fHeight-fontSize*3-8}, {fontSize*5, fontSize*1});
		add.visible = false;
		closeLB.visible = false;
	}

	// Right buttons
	if (rMenu) {
		shape.Draw({fWidth, 0}, {-menuWidth-scrollbarSize, fHeight}, menuBackground, true);
		DrawBoards();
		if (tStage != 3) import.Draw({fWidth - fontSize*6-12-menuWidth-scrollbarSize, fHeight - fontSize-8}, {fontSize*6, fontSize});
		if (tStage != 3) bBoards.Draw({fWidth - fontSize*6-12-menuWidth-scrollbarSize, fHeight - fontSize*2-8}, {fontSize*6, fontSize});
		if (tStage != 3) bSave.Draw({fWidth - fontSize*6-12-menuWidth-scrollbarSize, fHeight - fontSize*3-8}, {fontSize*6, fontSize});
	}else{
		import.Draw({fWidth - fontSize*6-12, fHeight - fontSize-8}, {fontSize*6, fontSize});
		bBoards.Draw({fWidth - fontSize*6-12, fHeight - fontSize*2-8}, {fontSize*6, fontSize});
		bSave.Draw({fWidth - fontSize*6-12, fHeight - fontSize*3-8}, {fontSize*6, fontSize});
	}
}

void DrawStage(){
	if (tStage == 0){

		// First message
		if (mStage == 0){
			font.Write("Use the left and right arrow keys", {0,fHeight/2+fontSize+8}, fontSize*.75f, White, true, Width, 1);
			font.Write("to advance through the tutorial pages.", {0,fHeight/2}, fontSize*.75f, White, true, Width, 1);
			font.Write("Press ESCAPE to leave.", {0,fHeight/2-fontSize-8}, fontSize*.75f, White, true, Width, 1);
		}else if (mStage == 1){
			font.Write("This area is the board", {0,fHeight/2+fontSize}, fontSize*.75f, White, true, Width, 1);
			font.Write("You can move around with w/a/s/d,", {0,fHeight/2-8}, fontSize*.75f, White, true, Width, 1);
			font.Write("the arrow keys, or by dragging", {0,fHeight/2-fontSize-16}, fontSize*.75f, White, true, Width, 1);
			font.Write("with the middle mouse held down.", {0,fHeight/2-fontSize*2-28}, fontSize*.75f, White, true, Width, 1);
			showZoom = 1;

			rmMenu.position = {640, 264};
			rmMenu.Draw();

			font.Write("If you right click an image or", {572,264+fontSize*3}, fontSize*.5f, White, true, Width, 0);
			font.Write("a group of selected images, you", {572,264+fontSize*2}, fontSize*.5f, White, true, Width, 0);
			font.Write("can alter them or add tags.", {572,264+fontSize}, fontSize*.5f, White, true, Width, 0);

			font.Write("You can click and drag to select", {128,256+fontSize*2}, fontSize*.5f, White, true, Width, 0);
			font.Write("images in a window.", {128,256+fontSize}, fontSize*.5f, White, true, Width, 0);
			shape.DrawBox({128, 128}, {256,128}, cSelector, 2, true, true);
			shape.Draw({128, 128}, {256,128}, cSelector, true);

			//shape.DrawLine({});
			font.Write("Zoom percent", {0,fHeight-fontSize*3}, fontSize*.75f, White, true, Width, 1);
			font.Write("Scrolling or pressing +/- will zoom in and out", {0,fHeight-fontSize*4-8}, fontSize*.75f, White, true, Width, 1);

			shape.DrawBox({8, fHeight-fontSize*2-8}, {fontSize*5, fontSize}, White, 2);
			font.Write("Click to open the locations menu", Vector2{8, fHeight-fontSize*2-8}+Vector2{fontSize*6, fontSize/2}, fontSize*.5f, White, true, Width, 0);
			font.Write("Or press TAB", Vector2{8, fHeight-fontSize*2-8}+Vector2{fontSize*6, -fontSize/2}, fontSize*.5f, White, true, Width, 0);
		}

	// Locations
	}else if (tStage == 1){

		if (mStage == 0){
			shape.DrawBox({0, 0}, {menuWidth-scrollbarSize, fHeight-fontSize*2}, White, 4);
			shape.DrawBox({menuWidth-fontSize*2-scrollbarSize, fHeight-fontSize*2}, {fontSize*2, fontSize*2}, White, 4);
			font.Write("Click to add a new location", {menuWidth+8, fHeight-fontSize*1.5f}, fontSize*.5f, White, true, Width, 0);
			font.Write("All saved folders are displayed here.", Vector2{menuWidth+8.0f, fHeight-fontSize*6}, fontSize*.75f, White, true, Width, 0);
			font.Write("Clicking on a folder will display all it's", Vector2{menuWidth+8.0f, fHeight-fontSize*7-8}, fontSize*.75f, White, true, Width, 0);
			font.Write("contents including images and subfolders.", Vector2{menuWidth+8.0f, fHeight-fontSize*8-16}, fontSize*.75f, White, true, Width, 0);

			font.Write("Files that have been tagged will", Vector2{menuWidth+8.0f, fHeight-fontSize*11+8}, fontSize*.75f, White, true, Width, 0);
			font.Write("have a white box to the left of", Vector2{menuWidth+8.0f, fHeight-fontSize*12}, fontSize*.75f, White, true, Width, 0);
			font.Write("their name.", Vector2{menuWidth+8.0f, fHeight-fontSize*13-8}, fontSize*.75f, White, true, Width, 0);

			font.Write("Pressing delete while hovering over", Vector2{menuWidth+8.0f, fHeight-fontSize*16+8}, fontSize*.75f, White, true, Width, 0);
			font.Write("the main folder will delete it from", Vector2{menuWidth+8.0f, fHeight-fontSize*17}, fontSize*.75f, White, true, Width, 0);
			font.Write("the list.", Vector2{menuWidth+8.0f, fHeight-fontSize*18-8}, fontSize*.75f, White, true, Width, 0);

		}else if (mStage == 1){
			shape.DrawBox({8+menuWidth, fHeight-fontSize-8}, {fontSize*5, fontSize}, White, 2);
			font.Write("Click to open the tag menu", Vector2{8+menuWidth, fHeight-fontSize-12}+Vector2{fontSize*6, fontSize/2}, fontSize*.5f, White, true, Width, 0);
			font.Write("Or press ~", Vector2{8+menuWidth, fHeight-fontSize-12}+Vector2{fontSize*6, -fontSize/2}, fontSize*.5f, White, true, Width, 0);
		}
	
	// Tags
	}else if (tStage == 2){
		if (mStage == 0){
			shape.DrawBox({0, 0}, {menuWidth-scrollbarSize, fHeight-fontSize*2}, White, 4);
			shape.DrawBox({menuWidth-fontSize*2-scrollbarSize, fHeight-fontSize*2}, {fontSize*2, fontSize*2}, White, 4);
			font.Write("Click to add a new tag", {menuWidth+8, fHeight-fontSize*1.5f}, fontSize*.5f, White, true, Width, 0);
			font.Write("All saved tags are displayed here.", Vector2{menuWidth+8.0f, fHeight-fontSize*6}, fontSize*.75f, White, true, Width, 0);
			font.Write("Clicking on a tag will display its", Vector2{menuWidth+8.0f, fHeight-fontSize*7-8}, fontSize*.75f, White, true, Width, 0);
			font.Write("images and sub tags. Tags can only", Vector2{menuWidth+8.0f, fHeight-fontSize*8-16}, fontSize*.75f, White, true, Width, 0);
			font.Write("handle one level of sub tags.", Vector2{menuWidth+8.0f, fHeight-fontSize*9-24}, fontSize*.75f, White, true, Width, 0);

			font.Write("While viewing a tag, 2 buttons", Vector2{menuWidth+8.0f, fHeight-fontSize*12}, fontSize*.75f, White, true, Width, 0);
			font.Write("will appear. The edit button", Vector2{menuWidth+8.0f, fHeight-fontSize*13-8}, fontSize*.75f, White, true, Width, 0);
			font.Write("on the right of the tag and the", Vector2{menuWidth+8.0f, fHeight-fontSize*14-16}, fontSize*.75f, White, true, Width, 0);
			font.Write("\"+\" or add button on the left.", Vector2{menuWidth+8.0f, fHeight-fontSize*15-24}, fontSize*.75f, White, true, Width, 0);

			font.Write("Pressing the edit button or right", Vector2{menuWidth+8.0f, fHeight-fontSize*18}, fontSize*.75f, White, true, Width, 0);
			font.Write("clicking the tag will allow you to", Vector2{menuWidth+8.0f, fHeight-fontSize*19-8}, fontSize*.75f, White, true, Width, 0);
			font.Write("edit the tags name and color.", Vector2{menuWidth+8.0f, fHeight-fontSize*20-16}, fontSize*.75f, White, true, Width, 0);
			
			font.Write("Pressing the \"+\" button will", Vector2{menuWidth+8.0f, fHeight-fontSize*23}, fontSize*.75f, White, true, Width, 0);
			font.Write("add a new sub tag.", Vector2{menuWidth+8.0f, fHeight-fontSize*24-8}, fontSize*.75f, White, true, Width, 0);

			font.Write("Pressing delete while hovering", Vector2{menuWidth+8.0f, fHeight-fontSize*27}, fontSize*.75f, White, true, Width, 0);
			font.Write("over a tag or sub tag will ", Vector2{menuWidth+8.0f, fHeight-fontSize*28-8}, fontSize*.75f, White, true, Width, 0);
			font.Write("delete it from the list.", Vector2{menuWidth+8.0f, fHeight-fontSize*29-16}, fontSize*.75f, White, true, Width, 0);
		
		}else if (mStage == 1){
			font.Write("Clicking on an image from a tag", {fWidth/5*4-fontSize*4, fWidth/5+fontSize*4 + fontSize*6+16}, fontSize*.75f, White, true, Width, 0);
			font.Write("or location will load it into",  {fWidth/5*4-fontSize*4, fWidth/5+fontSize*4 + fontSize*5+8}, fontSize*.75f, White, true, Width, 0);
			font.Write("this preview window. Clicking",  {fWidth/5*4-fontSize*4, fWidth/5+fontSize*4 + fontSize*4}, fontSize*.75f, White, true, Width, 0);
			font.Write("on the board will place the",  {fWidth/5*4-fontSize*4, fWidth/5+fontSize*4 + fontSize*3-8}, fontSize*.75f, White, true, Width, 0);
			font.Write("image at the mouse position.",  {fWidth/5*4-fontSize*4, fWidth/5+fontSize*4 + fontSize*2-16}, fontSize*.75f, White, true, Width, 0);

			font.Write("While an image is in the preview", Vector2{menuWidth+8.0f, fHeight-fontSize*5+8}, fontSize*.75f, White, true, Width, 0);
			font.Write("window, right clicking a tag or sub", Vector2{menuWidth+8.0f, fHeight-fontSize*6}, fontSize*.75f, White, true, Width, 0);
			font.Write("tag will add or remove the image", Vector2{menuWidth+8.0f, fHeight-fontSize*7-8}, fontSize*.75f, White, true, Width, 0);
			font.Write("from the tag depending on if it", Vector2{menuWidth+8.0f, fHeight-fontSize*8-16}, fontSize*.75f, White, true, Width, 0);
			font.Write("already has the tag.", Vector2{menuWidth+8.0f, fHeight-fontSize*9-24}, fontSize*.75f, White, true, Width, 0);

			font.Write("If an image has a tag, a white", Vector2{menuWidth+8.0f, fHeight-fontSize*12}, fontSize*.75f, White, true, Width, 0);
			font.Write("circle will appear to the left of", Vector2{menuWidth+8.0f, fHeight-fontSize*13-8}, fontSize*.75f, White, true, Width, 0);
			font.Write("the tags the image has.", Vector2{menuWidth+8.0f, fHeight-fontSize*14-16}, fontSize*.75f, White, true, Width, 0);

			shape.DrawBox({fWidth - fontSize*6-12, fHeight - fontSize*2-8}, {fontSize*6, fontSize}, White, 2);
			font.Write("Click to open the boards menu", {fWidth - fontSize*22, fHeight - fontSize*2-8}, fontSize*.5f, White, true, Width, 0);
			font.Write("or press escape", {fWidth - fontSize*22, fHeight - fontSize*3-8}, fontSize*.5f, White, true, Width, 0);		
		}

	// Boards
	}else if (tStage == 3){
		if (mStage == 0){
			shape.DrawBox({fWidth - menuWidth, 0}, {menuWidth, fHeight-fontSize*3}, White, 4);
			shape.DrawBox({fWidth - menuWidth, fHeight-fontSize*3}, {menuWidth,fontSize}, White, 4);
			font.Write("Shows the name of the current image board.", {fWidth - fontSize*23 - menuWidth, fHeight - fontSize*3}, fontSize*.5f, White, true, Width, 0);
			font.Write("Clicking it will let you edit the name.", {fWidth - fontSize*23 - menuWidth, fHeight - fontSize*4}, fontSize*.5f, White, true, Width, 0);

			font.Write("All saved image boards are", {fWidth - fontSize*24 - menuWidth, fHeight/2 + fontSize*2-8}, fontSize*.75f, White, true, Width, 0);
			font.Write("displayed here. Pressing ", {fWidth - fontSize*24 - menuWidth, fHeight/2 + fontSize-16}, fontSize*.75f, White, true, Width, 0);	
			font.Write("delete while hovering over", {fWidth - fontSize*24 - menuWidth, fHeight/2 -24}, fontSize*.75f, White, true, Width, 0);
			font.Write("an image board will delete it.", {fWidth - fontSize*24 - menuWidth, fHeight/2 - fontSize-32}, fontSize*.75f, White, true, Width, 0);	

			font.Write("You can also delete an image", {fWidth - fontSize*24 - menuWidth, fHeight/2 - fontSize*4-8}, fontSize*.75f, White, true, Width, 0);
			font.Write("board from the boards folder.", {fWidth - fontSize*24 - menuWidth, fHeight/2 - fontSize*5-16}, fontSize*.75f, White, true, Width, 0);	
		}else if (mStage == 1){
			shape.DrawBox({fWidth - menuWidth, fHeight-fontSize*3}, {menuWidth,fontSize}, White, 4);
			font.Write("When adding a new board, the", {fWidth - fontSize*21 - menuWidth, fHeight - fontSize*3}, fontSize*.5f, White, true, Width, 0);
			font.Write("text will clear for you to type", {fWidth - fontSize*21 - menuWidth, fHeight - fontSize*4}, fontSize*.5f, White, true, Width, 0);
			font.Write("the name of your new board.", {fWidth - fontSize*21 - menuWidth, fHeight - fontSize*5}, fontSize*.5f, White, true, Width, 0);

			font.Write("Pressing enter will create the", {fWidth - fontSize*21 - menuWidth, fHeight - fontSize*8}, fontSize*.5f, White, true, Width, 0);
			font.Write("board and save all the images", {fWidth - fontSize*21 - menuWidth, fHeight - fontSize*9}, fontSize*.5f, White, true, Width, 0);
			font.Write("in the view to the new board.", {fWidth - fontSize*21 - menuWidth, fHeight - fontSize*10}, fontSize*.5f, White, true, Width, 0);

			font.Write("Pressing escape will cancel it.", {fWidth - fontSize*21 - menuWidth, fHeight - fontSize*11}, fontSize*.5f, White, true, Width, 0);
		}
	
	// Image packs
	}else if (tStage == 4){
		if (mStage == 0){
			shape.DrawBox({fWidth - fontSize*6-12, fHeight - fontSize-8}, {fontSize*6, fontSize}, White, 2);
			shape.DrawBox({fWidth - fontSize*6-12, fHeight - fontSize*3-8}, {fontSize*6, fontSize}, White, 2);

			font.Write("Click to open the image pack window", {fWidth - fontSize*26, fHeight - fontSize-8}, fontSize*.5f, White, true, Width, 0);

			font.Write("RIA does not save information or", {fWidth - fontSize*26, fHeight - fontSize*3-8}, fontSize*.5f, White, true, Width, 0);
			font.Write("boards on exit. All changes must be", {fWidth - fontSize*26, fHeight - fontSize*4-8}, fontSize*.5f, White, true, Width, 0);
			font.Write("saved by the user.", {fWidth - fontSize*26, fHeight - fontSize*5-8}, fontSize*.5f, White, true, Width, 0);

		//256 360
		}else if (mStage == 1){
			showTutorial = false;
			Main.Render = &DrawApp;
			Main.Input = &MainInput;
			lMenu = false;
			rMenu = false;
			*Scale = oldScale;
			*View = oldView;
		}
	}
}

void DrawTutorial(){
	DrawEmpty();
	DrawStage();
}