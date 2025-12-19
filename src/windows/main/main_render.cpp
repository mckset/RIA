//
// Responsible for rendering everything in the main window
//


void DrawApp(){
	DrawBoard();

	// Left menu
	if (showLeftMenu){
		shape.Draw({0}, {sideMenuWidth, fHeight}, menuBackgroundColor, POSITION_FIXED);

		if (showTags){
			DrawTags();
			openLocations_Button.Draw({sideMenuWidth-FONT_SIZE+SCROLLBAR_SIZE, fHeight-FONT_SIZE-BOARD_BUTTON_PADDING}, {BOARD_BUTTON_SIZE, FONT_SIZE*1});
		}else{
			DrawLocations();
			openTags_Button.Draw({sideMenuWidth-FONT_SIZE+SCROLLBAR_SIZE, fHeight-FONT_SIZE-BOARD_BUTTON_PADDING}, {BOARD_BUTTON_SIZE, FONT_SIZE*1});
		}
		leftClose_Button.Draw({sideMenuWidth-FONT_SIZE+SCROLLBAR_SIZE, fHeight-FONT_SIZE*2-BOARD_BUTTON_PADDING}, {BOARD_BUTTON_SIZE, FONT_SIZE*1});

	}else{
		openTags_Button.Draw({BOARD_BUTTON_PADDING, fHeight-FONT_SIZE-BOARD_BUTTON_PADDING}, {BOARD_BUTTON_SIZE, FONT_SIZE*1});
		openLocations_Button.Draw({BOARD_BUTTON_PADDING, fHeight-FONT_SIZE*2-BOARD_BUTTON_PADDING}, {BOARD_BUTTON_SIZE, FONT_SIZE*1});
		help_Button.Draw({BOARD_BUTTON_PADDING, fHeight-FONT_SIZE*3-BOARD_BUTTON_PADDING}, {BOARD_BUTTON_SIZE, FONT_SIZE*1});
	}

	// Right Menu
	if (showRightMenu){
		shape.Draw({fWidth, 0}, {-sideMenuWidth-SCROLLBAR_SIZE, fHeight}, menuBackgroundColor, POSITION_FIXED);
		DrawBoards();

		imagePack_Button.Draw({fWidth - BOARD_BUTTON_SIZE-BOARD_BUTTON_PADDING-sideMenuWidth-SCROLLBAR_SIZE, fHeight - FONT_SIZE-BOARD_BUTTON_PADDING}, {BOARD_BUTTON_SIZE, FONT_SIZE});
		imageBoards_Button.Draw({fWidth - BOARD_BUTTON_SIZE-BOARD_BUTTON_PADDING-sideMenuWidth-SCROLLBAR_SIZE, fHeight - FONT_SIZE*2-BOARD_BUTTON_PADDING}, {BOARD_BUTTON_SIZE, FONT_SIZE});
		save_Button.Draw({fWidth - BOARD_BUTTON_SIZE-BOARD_BUTTON_PADDING-sideMenuWidth-SCROLLBAR_SIZE, fHeight - FONT_SIZE*3-BOARD_BUTTON_PADDING}, {BOARD_BUTTON_SIZE, FONT_SIZE});
	}else{
		imagePack_Button.Draw({fWidth - BOARD_BUTTON_SIZE-BOARD_BUTTON_PADDING, fHeight - FONT_SIZE-BOARD_BUTTON_PADDING}, {BOARD_BUTTON_SIZE, FONT_SIZE});
		imageBoards_Button.Draw({fWidth - BOARD_BUTTON_SIZE-BOARD_BUTTON_PADDING, fHeight - FONT_SIZE*2-BOARD_BUTTON_PADDING}, {BOARD_BUTTON_SIZE, FONT_SIZE});
		save_Button.Draw({fWidth - BOARD_BUTTON_SIZE-BOARD_BUTTON_PADDING, fHeight - FONT_SIZE*3-BOARD_BUTTON_PADDING}, {BOARD_BUTTON_SIZE, FONT_SIZE});
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
		if ((mouse.position.x > sideMenuWidth || !showLeftMenu) && (mouse.position.x < fWidth - sideMenuWidth || !showRightMenu) &&
			!((openTags_Button.hovered && !showTags) || (openLocations_Button.hovered && showTags) || (leftClose_Button.hovered && showLeftMenu) || (help_Button.hovered && !showLeftMenu) || save_Button.hovered || imageBoards_Button.hovered || imagePack_Button.hovered)){
			shape.Draw(
				mouse.position - (Vector2{(float)previewImg.img.width, (float)previewImg.img.height} * (*Scale)/2), 
				Vector2{(float)previewImg.img.width, (float)previewImg.img.height} * (*Scale), 
				menuBackgroundColor, POSITION_FIXED);
		}
	}

	if (showMouseMenu) mouseMenu.Draw();
}

// Draw images and grid
void DrawBoard(){
	DrawImages();
	
	// Selector box
	if (showImageSelector){
		shape.DrawBox(mouse.position, mouse.dragOffset - mouse.position, imageSelectorColor, IMAGE_SELECTOR_BORDER_SIZE, POSITION_FIXED);
		shape.Draw(mouse.position, mouse.dragOffset - mouse.position, imageSelectorColor, POSITION_FIXED);
	}

	// Verbose text
	if (statusText.length()){
		font.Write(statusText, {SHADOW_OFFSET, FONT_SIZE-SHADOW_OFFSET}, SMALL_FONT_SIZE, Black, POSITION_FIXED, Width, ALIGN_CENTER);
		font.Write(statusText, {0,FONT_SIZE}, SMALL_FONT_SIZE, White, POSITION_FIXED, Width, ALIGN_CENTER);

		if (statusTextTimer != -1){
			statusTextTimer -= deltaTime;
			if (statusTextTimer <=0)
				statusText = "";
		}
		
	}

	// Zoom scale
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
}

void DrawImages(){
	// The % adds parallax to the background
	for (int w = -2; w < (Height/GRID_SIZE)/(*Scale) + 1; w++)
		shape.Draw({0, ((float)w*GRID_SIZE*(*Scale) - ((int)(View->y/5) % (int)GRID_SIZE*(*Scale)))*2}, {fWidth*2, 3}, gridColor, POSITION_FIXED);
	
	for (int h = -2; h < (Width/GRID_SIZE)/(*Scale) + 1; h++)
		shape.Draw({((float)h*GRID_SIZE*(*Scale) - ((int)(View->x/5) % (int)GRID_SIZE*(*Scale)))*2, 0}, {3, fHeight*2}, gridColor, POSITION_FIXED);
	

	// Draw origin if visible
	if (showOrigin){
		shape.Draw({0,View->y/2 - Height*8/(*Scale)}, {2/(*Scale), fHeight*16/(*Scale)}, gridOriginColor);
		shape.Draw({View->x/2 - Width*8/(*Scale), 0}, {fWidth*16/(*Scale), 2/(*Scale)}, gridOriginColor);
	}

	// Images
	for (auto img : imgs)
		img.Draw();
}

// Draw locations
void DrawLocations(){
	float y = Height - FONT_SIZE*2 + locations_Scrollbar.scroll;

	shape.Draw({0, y}, {sideMenuWidth-SCROLLBAR_SIZE, FONT_SIZE*2}, menuBackgroundColor, POSITION_FIXED);
	font.Write("Locations", {FONT_SIZE/2, y}, FONT_SIZE, fontColor, POSITION_FIXED, sideMenuWidth-SCROLLBAR_SIZE-FONT_SIZE*2, ALIGN_CENTER);
	add_Button.Draw({sideMenuWidth-FONT_SIZE*2-SCROLLBAR_SIZE, y}, {FONT_SIZE*2, FONT_SIZE*2}, ALIGN_CENTER);

	y -= FONT_SIZE;

	for (int i = 0; i < locations.size(); i++){
		if (locations[i].Draw({0, y}, {sideMenuWidth - SCROLLBAR_SIZE, FONT_SIZE}) == LOCATION_DELETE){
			// Delete tag
			locations.erase(locations.begin() + i);
			i--;
			continue;
		}
		y -= locations[i].listSize;
	}

	y-=locations_Scrollbar.scroll;

	if (y >= 0){
		locations_Scrollbar.end = 0;
		locations_Scrollbar.scroll = 0;
	}else
		locations_Scrollbar.end = -y;

	locations_Scrollbar.Draw({sideMenuWidth-SCROLLBAR_SIZE}, {SCROLLBAR_SIZE, fHeight});
}

// Draw tags
void DrawTags(){
	float y = Height - FONT_SIZE*2 + tags_Scrollbar.scroll;

	shape.Draw({0, y}, {sideMenuWidth-SCROLLBAR_SIZE, FONT_SIZE*2}, menuBackgroundColor, POSITION_FIXED);
	font.Write("Tags", {FONT_SIZE/2, y}, FONT_SIZE, fontColor, POSITION_FIXED, sideMenuWidth-SCROLLBAR_SIZE-FONT_SIZE*2, ALIGN_CENTER);
	add_Button.Draw({sideMenuWidth-FONT_SIZE*2-SCROLLBAR_SIZE, y}, {FONT_SIZE*2, FONT_SIZE*2}, ALIGN_CENTER);

	y -= FONT_SIZE;

	for (int i = 0; i < tags.size(); i++){
		if (tags[i].Draw({0, y}, {sideMenuWidth - SCROLLBAR_SIZE, FONT_SIZE}) == TAG_DELETE){
			// Delete tag
			tags.erase(tags.begin() + i);
			i--;
			keyboard.newKey = INPUT_NULL;
			continue;
		}
		y -= tags[i].listSize;
	}

	y-=tags_Scrollbar.scroll;

	if (y >= 0){
		tags_Scrollbar.end = 0;
		tags_Scrollbar.scroll = 0;
	}else
		tags_Scrollbar.end = -y;

	tags_Scrollbar.Draw({sideMenuWidth-SCROLLBAR_SIZE}, {SCROLLBAR_SIZE, fHeight});
}