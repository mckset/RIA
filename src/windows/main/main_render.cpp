//
// Responsible for rendering everything in the main window
//

// Right menu buttons
Button import = Button{"Image Packs", menuBackground, highlight, White, fontSize/2};
Button bBoards = Button{"Boards", menuBackground, highlight, White, fontSize/2};
Button bSave = Button{"Save", menuBackground, highlight, White, fontSize/2};
Button closeRB = Button{"Close", menuBackground, highlight, White, fontSize/2};

// Left menu buttons
Button openTags = Button{"Tags", menuBackground, highlight, White, fontSize/2};
Button openLoc = Button{"Locations", menuBackground, highlight, White, fontSize/2};
Button closeLB = Button{"Close", menuBackground, highlight, White, fontSize/2};
Button helpB = Button{"Help", menuBackground, highlight, White, fontSize/2};

// Displays informations about importing and exporting image packs when the time is greater than 0
string importText = "";
int importTime = 0;

// Displays informations about saving and loading when the time is greater than 0
extern bool saving;
extern bool saveError;
extern string saveText;

// True when the selector window should be shown (mouse1 drag)
bool selector = false;

void DrawApp(){
	// Draw images
	DrawMain();

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

	// Saving text
	if (save > 0){
		save--;
		font.Write(saveText, {2,fontSize-2}, fontSize*.75f, Black, true, Width, 1);
		font.Write(saveText, {0,fontSize}, fontSize*.75f, White, true, Width, 1);
	
	// Failed saving
	}else if (saveError && save == 0){
		saveError = false;
		saveText = "Saving failed. Please try again.";
		save = 60;

	// Import Text
	}else if (importTime > 0){
		importTime--;
		font.Write(importText, {2,fontSize-2}, fontSize*.75f, Black, true, Width, 1);
		font.Write(importText, {0,fontSize}, fontSize*.75f, White, true, Width, 1);
	}

	// Right click menu
	if (drawMouseMenu)
		rmMenu.Draw();

	//
	// Image preview
	//
	if (previewImg.img.loaded){
		shape.Draw({fWidth/5*4-fontSize*4 - (rMenu ? menuWidth+scrollbarSize : 0), 0}, {fWidth/5+fontSize*4, fWidth/5+fontSize*4}, menuBackground, true);

		// Draw image centered 
		previewImg.Draw(
			{fWidth/5*4+(Width/5-previewImg.size.x)/2-fontSize*2 - (rMenu ? menuWidth+scrollbarSize : 0), (fWidth/5 - previewImg.size.y)/2 + fontSize/2}, 
			previewImg.size, White, true);

		font.Write("Preview", {fWidth/5*4-fontSize*4 - (rMenu ? menuWidth+scrollbarSize : 0), fWidth/5+fontSize}, fontSize, White, true, Width/5+fontSize*4, 1);
		
		// Mouse 
		if (mouse.position.x > menuWidth || !lMenu){
			shape.Draw(
				mouse.position.Subtract(Vector2{(float)previewImg.img.width, (float)previewImg.img.height}.Multiply((*Scale)).Divide(2)), 
				Vector2{(float)previewImg.img.width, (float)previewImg.img.height}.Multiply((*Scale)), 
				menuBackground, true);
		}
	}


	// Left menu opened
	if (lMenu){
		// Left menu background
		shape.Draw({0}, {menuWidth, fHeight}, menuBackground, true);

		// Draw tags
		if (tagView)
			DrawTags();

		// Draw locations
		else
			DrawLocations();

		// Enable tag button and disable location button when locations are being shown
		if (!tagView){
			openTags.Draw({menuWidth-fontSize+scrollbarSize, fHeight-fontSize-8}, {fontSize*5, fontSize*1});
			openLoc.visible = false;

		// Enable location button and disable tag button when tags are being shown
		}else{
			openLoc.Draw({menuWidth-fontSize+scrollbarSize, fHeight-fontSize-8}, {fontSize*5, fontSize*1});
			openTags.visible = false;
		}

		// Close button is only shown when the menu is open
		closeLB.Draw({menuWidth-fontSize+scrollbarSize, fHeight-fontSize*2-8}, {fontSize*5, fontSize*1});
	
	// Left menu closed (default)
	}else{
		openTags.Draw({8, fHeight-fontSize-8}, {fontSize*5, fontSize*1});
		openLoc.Draw({8, fHeight-fontSize*2-8}, {fontSize*5, fontSize*1});
		helpB.Draw({8, fHeight-fontSize*3-8}, {fontSize*5, fontSize*1});
	}

	// Right menu opened
	if (rMenu) {
		// Background
		shape.Draw({fWidth, 0}, {-menuWidth-scrollbarSize, fHeight}, menuBackground, true);
		
		DrawBoards();

		// Buttons
		import.Draw({fWidth - fontSize*6-12-menuWidth-scrollbarSize, fHeight - fontSize-8}, {fontSize*6, fontSize});
		bBoards.Draw({fWidth - fontSize*6-12-menuWidth-scrollbarSize, fHeight - fontSize*2-8}, {fontSize*6, fontSize});
		bSave.Draw({fWidth - fontSize*6-12-menuWidth-scrollbarSize, fHeight - fontSize*3-8}, {fontSize*6, fontSize});
	
	// Right menu closed (default)
	}else{
		import.Draw({fWidth - fontSize*6-12, fHeight - fontSize-8}, {fontSize*6, fontSize});
		bBoards.Draw({fWidth - fontSize*6-12, fHeight - fontSize*2-8}, {fontSize*6, fontSize});
		bSave.Draw({fWidth - fontSize*6-12, fHeight - fontSize*3-8}, {fontSize*6, fontSize});
	}

	// Draw loading text
	if (!loaded) font.Write("Loading...", {2,fontSize-2}, fontSize*.75f, White, true, Width, 1);
}


// Draw locations
void DrawLocations(){
	// y is set to the top of the window and is used to render down to the bottom
	float y = Height-fontSize*2+locScroll.scroll;

	// Location heading, button, and title
	shape.Draw({0, y}, {menuWidth-scrollbarSize, fontSize*2}, menuBackground, true);
	font.Write("Locations", {0, y+8}, fontSize*.75, fontColor, true, menuWidth-scrollbarSize-fontSize*2, 1);
	add.Draw({menuWidth-fontSize*2-scrollbarSize, y}, {fontSize*2, fontSize*2}, false, true);

	// Scroll bar end position
	locScroll.end = fontSize;
	y-=fontSize;

	// Locations
	for (int i = 0; i < locations.size(); i++){
		// Draws entries if within the window height
		if (y > -fontSize){
			bool del = locations[i].Draw({0, y}, {menuWidth-scrollbarSize, fontSize});

			// Deletes a location
			if (del){
				locations.erase(locations.begin() + i);
				return;
			}
			y -= locations[i].listSize;
			locScroll.end += locations[i].listSize;
		
		// Prevents drawing entries beyond the window height but continues to get the total size
		}else{
			y -= locations[i].GetSize({menuWidth-scrollbarSize, fontSize});
			locScroll.end += locations[i].GetSize({menuWidth-scrollbarSize, fontSize});
		}
	}

	// Subtracts the height from the scroll bar
	locScroll.end -= Height;
	locScroll.end += fontSize*2;

	// Sets the end and position to 0 if there is not enough entries to need a scrollbar
	if (locScroll.end < 0){
		locScroll.end = 0;
		locScroll.scroll = 0;
	}

	// Draws the scrollbar if there are more entries than the height of the window
	locScroll.Draw({menuWidth-scrollbarSize, 0}, {scrollbarSize, fHeight});
}


// Draw images and grid
void DrawMain(){
	// Draw grid
	// The % adds parallax to the background
	for (int w = -2; w < (Width/gridSize)/(*Scale) + 1; w++)
		shape.Draw({0, ((float)w*gridSize*(*Scale) - ((int)(View->y/5) % (int)gridSize*(*Scale)))*2}, {fWidth*2, 3}, grid, true);
	
	for (int h = -2; h < (Height/gridSize)/(*Scale) + 1; h++)
		shape.Draw({((float)h*gridSize*(*Scale) - ((int)(View->x/5) % (int)gridSize*(*Scale)))*2, 0}, {3, fHeight*2}, grid, true);
	

	// Draw origin if visible
	if (drawOrigin){
		shape.Draw({0,View->y/2 - Height*2/(*Scale)}, {2/(*Scale), fHeight*16/(*Scale)}, cOrigin, false);
		shape.Draw({View->x/2 - Width*2/(*Scale), 0}, {fWidth*16/(*Scale), 2/(*Scale)}, cOrigin, false);
	}

	// Draw images
	for (auto img : imgs){
		img.Draw();
		if (img.selected)
			img.DrawOutline(borderSize, imageBorder);
	}
}

// Draw tags
void DrawTags(){
	// y is set to the top of the window and is used to render down to the bottom
	float y = Height-fontSize*2+tagScroll.scroll;

	// Tag heading, button, and title
	shape.Draw({0, y}, {menuWidth-scrollbarSize, fontSize*2}, menuBackground, true);
	font.Write("Tags", {fontSize/2, y+8}, fontSize*.75f, fontColor, true, menuWidth-scrollbarSize-fontSize*2, 1);
	add.Draw({menuWidth-fontSize*2-scrollbarSize, y}, {fontSize*2, fontSize*2}, false, true);

	// Scroll bar end position
	tagScroll.end = fontSize;
	y-=fontSize;

	// Tags
	for (int i = 0; i < tags.size(); i++){

		// Draws entries if within the window height
		if (y > -fontSize){
			int del = tags[i].Draw({0, y}, {menuWidth-scrollbarSize, fontSize}, i);

			// Deletes a tag
			if (del == 1){
				vector<Tag>:: iterator t = tags.begin();
				advance(t, i);
				tags.erase(t);
				return;
			}
			y -= tags[i].listSize;
			tagScroll.end += tags[i].listSize;

		// Prevents drawing entries beyond the window height but continues to get the total size
		}else{
			y -= tags[i].GetSize({menuWidth-scrollbarSize, fontSize});
			tagScroll.end += tags[i].GetSize({menuWidth-scrollbarSize, fontSize});
		}
	}

	// Subtracts the height from the scroll bar
	tagScroll.end -= Height;
	tagScroll.end += fontSize*2;

	// Sets the end and position to 0 if there is not enough entries to need a scrollbar
	if (tagScroll.end < 0){
		tagScroll.end = 0;
		tagScroll.scroll = 0;
	}

	// Draws the scrollbar if there are more entries than the height of the window
	tagScroll.Draw({menuWidth-scrollbarSize, 0}, {scrollbarSize, fHeight});
}