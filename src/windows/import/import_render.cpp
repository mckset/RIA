/*
	Handles rendering for all stages of the import window
*/

Scrollbar import_Scrollbar = Scrollbar{scrollbarBackgroundColor, scrollbarNotchColor};

Button imagePackCreate_Button = Button{"Create", importButtonColor, importHighlightColor, White, SMALL_FONT_SIZE};
Button imagePackOpen_Button = Button{"Open", importButtonColor, importHighlightColor, White, SMALL_FONT_SIZE};
Button imagePackImport_Button = Button{"Import", importButtonColor, importHighlightColor, White, SMALL_FONT_SIZE};

Location folders;
Location *selectedFolder;

string importPath = "";
string importFile = "";

vector<Tag> importTags; // A list of only tags and images that will be imported 
vector<Tag> completeTags; // A list of all the tags and images in an image pack

// Display a given folder and its sub folders
void DrawFolders(float indent, float *y, Vector2 size, Location *folder){
	// Entry background
	shape.Draw(Vector2{indent, *y}, Vector2{fWidth-SCROLLBAR_SIZE-indent, FONT_SIZE}, scrollbarNotchColor, POSITION_FIXED);
	
	// Active indicator
	if (!folder->expanded)
		shape.DrawCircle(Vector2{indent, *y} + Vector2{SMALL_FONT_SIZE, SMALL_FONT_SIZE}, FONT_SIZE, BORDER_NONE, highlightColor, POSITION_FIXED);
		
	// Write name
	font.Write(folder->name, Vector2{PADDING+indent, *y}, SMALL_FONT_SIZE, fontColor, POSITION_FIXED, fWidth-SCROLLBAR_SIZE-PADDING*2);

	// Handle mouse input for the current entry
	if (*y > FONT_SIZE && mouse.position.Within(Vector2{0, *y}, Vector2{fWidth-SCROLLBAR_SIZE, FONT_SIZE}) && CurrentWindow->focused){
		// highlightColor
		shape.Draw(Vector2{indent, *y}, Vector2{fWidth-SCROLLBAR_SIZE-indent, FONT_SIZE}, highlightColor, POSITION_FIXED);

		// On click
		if (mouse.Click() && *y <= fHeight-FONT_SIZE*2 && *y >= (float)FONT_SIZE*2)
			folder->expanded = !folder->expanded; 
		
	}

	*y-=FONT_SIZE;
	import_Scrollbar.end += FONT_SIZE;

	// If there are subfolders, run the function on each folder
	if (folder->folders.size())
		for (int i = 0; i < folder->folders.size(); i++)
			DrawFolders(indent+FILE_PADDING*2, y, size-Vector2{FILE_PADDING*2, 0}, &(folder->folders[i]));
}

// Draw all the folders associated with an image pack
void DrawImportFolders(){
	// y is set to the top of the window and is used to render down to the bottom
	float y = Height-FONT_SIZE*2+import_Scrollbar.scroll;

	// Scroll bar end position
	import_Scrollbar.end = 0;

	// Loop through each folder
	for (int i = 0; i < folders.folders.size(); i++)
		DrawFolders(0, &y, Vector2{fWidth-SCROLLBAR_SIZE, FONT_SIZE}, &(folders.folders[i]));
	
	// Subtracts the height from the scroll bar
	import_Scrollbar.end += FONT_SIZE*2 - fHeight;

	// Sets the end and position to 0 if there is not enough entries to need a scrollbar
	if (import_Scrollbar.end < 0){
		import_Scrollbar.end = 0;
		import_Scrollbar.scroll = 0;
	}

	// Draws the scrollbar if there are more entries than the height of the window
	import_Scrollbar.Draw(Vector2{(float)Width-SCROLLBAR_SIZE, 0}, Vector2{(float)SCROLLBAR_SIZE, (float)Height});

	// Accept button
	imagePackImport_Button.Draw({0, 0}, {fWidth, FONT_SIZE}, ALIGN_CENTER);

	// Draw header over folders that might be higher from scrolling
	shape.Draw(Vector2{0, fHeight-FONT_SIZE}, Vector2{fWidth-SCROLLBAR_SIZE, FONT_SIZE}, importButtonColor, POSITION_FIXED );
	font.Write("Folder Select", Vector2{PADDING, fHeight-FONT_SIZE}, SMALL_FONT_SIZE, fontColor, POSITION_FIXED, fWidth-SCROLLBAR_SIZE-PADDING, ALIGN_CENTER);
}

// Draws the found image packs (default)
void DrawImportMain(){

	// y is set to the top of the window and is used to render down to the bottom
	float y = Height-FONT_SIZE*2+import_Scrollbar.scroll;

	// Scroll bar end position
	import_Scrollbar.end = 0;

	// List all the image packs found
	for (int i = 0; i < folders.folders.size(); i++){
		// Entry background
		shape.Draw(Vector2{0, y}, Vector2{fWidth-SCROLLBAR_SIZE, FONT_SIZE}, scrollbarNotchColor, POSITION_FIXED);
		
		// Active indicator
		if (folders.folders[i].expanded)
			shape.DrawCircle(Vector2{FONT_SIZE, y} + Vector2{-SMALL_FONT_SIZE, SMALL_FONT_SIZE}, FONT_SIZE, 0, highlightColor, POSITION_FIXED);
		
		// Write name
		font.Write(folders.folders[i].name, Vector2{PADDING, y}, SMALL_FONT_SIZE, fontColor, POSITION_FIXED, fWidth-SCROLLBAR_SIZE-48);

		// Handle mouse input for the current entry
		if (mouse.position.Within(Vector2{0, y}, Vector2{fWidth-SCROLLBAR_SIZE, FONT_SIZE}) && CurrentWindow->focused){
			// highlightColor
			shape.Draw(Vector2{0, y}, Vector2{fWidth-SCROLLBAR_SIZE, FONT_SIZE}, highlightColor, POSITION_FIXED);

			// On click
			if (mouse.Click() && y <= fHeight-FONT_SIZE*2 && y >= (float)FONT_SIZE){
				for (int f = 0; f < folders.folders.size(); f++)
					folders.folders[f].expanded = false;
				folders.folders[i].expanded = true; // Reuses the expanded bool as a selection bool because I thought I was being smart
				selectedFolder = &folders.folders[i];
			}
		}
		y-=FONT_SIZE;
		import_Scrollbar.end += FONT_SIZE;
	}

	// Subtracts the height from the scroll bar
	import_Scrollbar.end += FONT_SIZE*2 - fHeight;

	// Display if there isn't any image packs available
	if (!folders.folders.size()){
		font.Write("No image packs found.", Vector2{16, y}, SMALL_FONT_SIZE, fontColor);
		font.Write("Check that your image", Vector2{16, y-FONT_SIZE}, SMALL_FONT_SIZE, fontColor);
		font.Write("packs are saved to the", Vector2{16, y-FONT_SIZE*2}, SMALL_FONT_SIZE, fontColor);
		font.Write("shared folder.", Vector2{16, y-FONT_SIZE*3}, SMALL_FONT_SIZE, fontColor);

	// If there are image packs
	}else{
		shape.Draw(Vector2{0, fHeight-FONT_SIZE}, Vector2{fWidth-SCROLLBAR_SIZE, FONT_SIZE}, importButtonColor, POSITION_FIXED );
		font.Write("Image Packs", Vector2{PADDING, fHeight-FONT_SIZE}, SMALL_FONT_SIZE, fontColor, POSITION_FIXED, fWidth-SCROLLBAR_SIZE-PADDING, ALIGN_CENTER);
	}

	// Sets the end and position to 0 if there is not enough entries to need a scrollbar
	if (import_Scrollbar.end < 0){
		import_Scrollbar.end = 0;
		import_Scrollbar.scroll = 0;
	}

	// Draws the scrollbar if there are more entries than the height of the window
	import_Scrollbar.Draw(Vector2{(float)Width-SCROLLBAR_SIZE, 0}, Vector2{(float)SCROLLBAR_SIZE, (float)Height});

	// Draw buttons
	imagePackCreate_Button.Draw({fWidth/3*2-SCROLLBAR_SIZE/3*2, 0}, {fWidth/3-SCROLLBAR_SIZE/3, FONT_SIZE}, ALIGN_CENTER);
	imagePackOpen_Button.Draw({fWidth/3-SCROLLBAR_SIZE/3, 0}, {fWidth/3-SCROLLBAR_SIZE/3, FONT_SIZE}, ALIGN_CENTER);
	imagePackImport_Button.Draw({0, 0}, {fWidth/3-SCROLLBAR_SIZE/3, FONT_SIZE}, ALIGN_CENTER);
}

// Draw all the tags that will be imported
void DrawImportTags(){
	
	// y is set to the top of the window and is used to render down to the bottom
	float y = Height-FONT_SIZE*2+import_Scrollbar.scroll;
	
	// Scroll bar end position
	import_Scrollbar.end = 0;

	// Loop through each tag
	for (auto tag : importTags){
		// Draw tag with color
		shape.Draw(Vector2{0, y}, Vector2{fWidth-SCROLLBAR_SIZE, FONT_SIZE}, tag.color, POSITION_FIXED);
		font.Write(tag.name, Vector2{PADDING, y}, SMALL_FONT_SIZE, fontColor, POSITION_FIXED, fWidth-SCROLLBAR_SIZE-PADDING*2);

		y-=FONT_SIZE;
		import_Scrollbar.end += FONT_SIZE;
		
		// Show sub tags if the toggle is on (default)
		if (imagePackCreate_Button.toggled){
			for (auto subTag : tag.subTags){
				shape.Draw(Vector2{PADDING, y}, Vector2{fWidth-SCROLLBAR_SIZE-PADDING, FONT_SIZE}, subTag.color, POSITION_FIXED);
				font.Write(subTag.name, Vector2{40, y}, SMALL_FONT_SIZE, fontColor, POSITION_FIXED, fWidth-SCROLLBAR_SIZE-PADDING*2);
				y-=FONT_SIZE;
				import_Scrollbar.end += FONT_SIZE;
			}
		}
	}
		
	// Subtracts the height from the scroll bar
	import_Scrollbar.end += FONT_SIZE*2 - fHeight;

	// Sets the end and position to 0 if there is not enough entries to need a scrollbar
	if (import_Scrollbar.end < 0){
		import_Scrollbar.end = 0;
		import_Scrollbar.scroll = 0;
	}

	// Draws the scrollbar if there are more entries than the height of the window
	import_Scrollbar.Draw(Vector2{(float)Width-SCROLLBAR_SIZE, 0}, Vector2{(float)SCROLLBAR_SIZE, (float)Height});

	// Draw buttons
	imagePackImport_Button.Draw({0, 0}, {fWidth/2, FONT_SIZE}, ALIGN_CENTER); // Accept
	imagePackCreate_Button.Draw({fWidth/2, 0}, {fWidth/2, FONT_SIZE}, ALIGN_CENTER); // Toggle tags

	// Draw header over tags that might be higher from scrolling
	shape.Draw(Vector2{0, fHeight-FONT_SIZE}, Vector2{fWidth-SCROLLBAR_SIZE, FONT_SIZE}, importButtonColor, POSITION_FIXED );
	font.Write("Import Tags", Vector2{PADDING, fHeight-FONT_SIZE}, SMALL_FONT_SIZE, fontColor, POSITION_FIXED, fWidth-SCROLLBAR_SIZE-PADDING, ALIGN_CENTER);
}

// Resets all the variables used in the import window
void ResetImport(){
	import_Scrollbar.scroll = 0;
	import_Scrollbar.end = 0;

	imagePackCreate_Button = Button{"Create", importButtonColor, importHighlightColor, White, SMALL_FONT_SIZE};
	imagePackCreate_Button.toggled = false;
	imagePackImport_Button = Button{"Import", importButtonColor, importHighlightColor, White, SMALL_FONT_SIZE};

	folders.path = path + "shared";
	folders.GetFiles(false);
	completeTags.clear();
	Import.Render = &DrawImportMain;
	Import.Input = &MainImportInput;
}
