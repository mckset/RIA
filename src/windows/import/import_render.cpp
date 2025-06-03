/*
	Handles rendering for all stages of the import window
*/

Scrollbar importScroll = Scrollbar{scrollbarBacking, scrollbarNotch, highlight, Transparent};

Button createB = Button{"Create", importButton, highlight, White, fontSize/2};
Button openB = Button{"Open", importButton, highlight, White, fontSize/2};
Button importB = Button{"Import", importButton, highlight, White, fontSize/2};

Table folders;
Table *selectedFolder;

string importPath = "";
string importFile = "";

vector<Tag> importTags; // A list of only tags and images that will be imported 
vector<Tag> completeTags; // A list of all the tags and images in an image pack

// Display a given folder and its sub folders
void DrawFolders(float indent, float *y, Vector2 size, Table *folder){
	// Entry background
	shape.Draw(Vector2{indent, *y}, Vector2{fWidth-scrollbarSize-indent, fontSize}, scrollbarNotch, true);
	
	// Active indicator
	if (!folder->expand)
		shape.DrawCircle(Vector2{0, *y} + Vector2{fontSize/2, fontSize/2}, fontSize, 0, highlight, true);
		
	// Write name
	font.Write(folder->name, Vector2{24+indent, *y}, fontSize/2, fontColor, true, fWidth-scrollbarSize-48);

	// Handle mouse input for the current entry
	if (*y > fontSize && mouse.position.Within(Vector2{0, *y}, Vector2{fWidth-scrollbarSize, fontSize}) && Import.Focus()){
		// Highlight
		shape.Draw(Vector2{indent, *y}, Vector2{fWidth-scrollbarSize-indent, fontSize}, highlight, true);

		// On click
		if (mouse.Click() && *y <= fHeight-fontSize*2 && *y >= (float)fontSize*2)
			folder->expand = !folder->expand; 
		
	}

	*y-=fontSize;
	importScroll.end += fontSize;

	// If there are subfolders, run the function on each folder
	if (folder->folders.size())
		for (int i = 0; i < folder->folders.size(); i++)
			DrawFolders(indent+16, y, size-Vector2{16, 0}, &(folder->folders[i]));
}

// Draw all the folders associated with an image pack
void DrawImportFolders(){
	// y is set to the top of the window and is used to render down to the bottom
	float y = Height-fontSize*2+importScroll.scroll;

	// Scroll bar end position
	importScroll.end = 0;

	// Loop through each folder
	for (int i = 0; i < folders.folders.size(); i++)
		DrawFolders(0, &y, Vector2{fWidth-scrollbarSize, fontSize}, &(folders.folders[i]));
	
	// Subtracts the height from the scroll bar
	importScroll.end += fontSize*2 - fHeight;

	// Sets the end and position to 0 if there is not enough entries to need a scrollbar
	if (importScroll.end < 0){
		importScroll.end = 0;
		importScroll.scroll = 0;
	}

	// Draws the scrollbar if there are more entries than the height of the window
	importScroll.Draw(Vector2{(float)Width-scrollbarSize, 0}, Vector2{(float)scrollbarSize, (float)Height});

	// Accept button
	importB.Draw({0, 0}, {fWidth, fontSize}, false, true);

	// Draw header over folders that might be higher from scrolling
	shape.Draw(Vector2{0, fHeight-fontSize}, Vector2{fWidth-scrollbarSize, fontSize}, importButton, true );
	font.Write("Folder Select", Vector2{24, fHeight-fontSize}, fontSize/2, fontColor, true, fWidth-scrollbarSize-24, true);
}

// Draws the found image packs (default)
void DrawImportMain(){

	// y is set to the top of the window and is used to render down to the bottom
	float y = Height-fontSize*2+importScroll.scroll;

	// Scroll bar end position
	importScroll.end = 0;

	// List all the image packs found
	for (int i = 0; i < folders.folders.size(); i++){
		// Entry background
		shape.Draw(Vector2{0, y}, Vector2{fWidth-scrollbarSize, fontSize}, scrollbarNotch, true);
		
		// Active indicator
		if (folders.folders[i].expand)
			shape.DrawCircle(Vector2{fontSize, y} + Vector2{-fontSize/2, fontSize/2}, fontSize, 0, highlight, true);
		
		// Write name
		font.Write(folders.folders[i].name, Vector2{24, y}, fontSize/2, fontColor, true, fWidth-scrollbarSize-48);

		// Handle mouse input for the current entry
		if (mouse.position.Within(Vector2{0, y}, Vector2{fWidth-scrollbarSize, fontSize}) && Import.Focus()){
			// Highlight
			shape.Draw(Vector2{0, y}, Vector2{fWidth-scrollbarSize, fontSize}, highlight, true);

			// On click
			if (mouse.Click() && y <= fHeight-fontSize*2 && y >= (float)fontSize){
				for (int f = 0; f < folders.folders.size(); f++)
					folders.folders[f].expand = false;
				folders.folders[i].expand = true; // Reuses the expand bool as a selection bool because I thought I was being smart
				selectedFolder = &folders.folders[i];
			}
		}
		y-=fontSize;
		importScroll.end += fontSize;
	}

	// Subtracts the height from the scroll bar
	importScroll.end += fontSize*2 - fHeight;

	// Display if there isn't any image packs available
	if (!folders.folders.size()){
		sFont.Use(true);
		font.Write("No image packs found.", Vector2{16, y}, fontSize/2, fontColor);
		font.Write("Check that your image", Vector2{16, y-fontSize}, fontSize/2, fontColor);
		font.Write("packs are saved to the", Vector2{16, y-fontSize*2}, fontSize/2, fontColor);
		font.Write("shared folder.", Vector2{16, y-fontSize*3}, fontSize/2, fontColor);

	// If there are image packs
	}else{
		shape.Draw(Vector2{0, fHeight-fontSize}, Vector2{fWidth-scrollbarSize, fontSize}, importButton, true );
		font.Write("Image Packs", Vector2{24, fHeight-fontSize}, fontSize/2, fontColor, true, fWidth-scrollbarSize-24, true);
	}

	// Sets the end and position to 0 if there is not enough entries to need a scrollbar
	if (importScroll.end < 0){
		importScroll.end = 0;
		importScroll.scroll = 0;
	}

	// Draws the scrollbar if there are more entries than the height of the window
	importScroll.Draw(Vector2{(float)Width-scrollbarSize, 0}, Vector2{(float)scrollbarSize, (float)Height});

	// Draw buttons
	createB.Draw({fWidth/3*2-scrollbarSize/3*2, 0}, {fWidth/3-scrollbarSize/3, fontSize}, false, true);
	openB.Draw({fWidth/3-scrollbarSize/3, 0}, {fWidth/3-scrollbarSize/3, fontSize}, false, true);
	importB.Draw({0, 0}, {fWidth/3-scrollbarSize/3, fontSize}, false, true);
}

// Draw all the tags that will be imported
void DrawImportTags(){
	
	// y is set to the top of the window and is used to render down to the bottom
	float y = Height-fontSize*2+importScroll.scroll;
	
	// Scroll bar end position
	importScroll.end = 0;

	// Loop through each tag
	for (auto tag : importTags){
		// Draw tag with color
		shape.Draw(Vector2{0, y}, Vector2{fWidth-scrollbarSize, fontSize}, tag.color, true);
		font.Write(tag.name, Vector2{24, y}, fontSize/2, fontColor, true, fWidth-scrollbarSize-48);

		y-=fontSize;
		importScroll.end += fontSize;
		
		// Show sub tags if the toggle is on (default)
		if (createB.toggled){
			for (auto subTag : tag.subTags){
				shape.Draw(Vector2{16, y}, Vector2{fWidth-scrollbarSize-16, fontSize}, subTag.color, true);
				font.Write(subTag.name, Vector2{40, y}, fontSize/2, fontColor, true, fWidth-scrollbarSize-48);
				y-=fontSize;
				importScroll.end += fontSize;
			}
		}
	}
		
	// Subtracts the height from the scroll bar
	importScroll.end += fontSize*2 - fHeight;

	// Sets the end and position to 0 if there is not enough entries to need a scrollbar
	if (importScroll.end < 0){
		importScroll.end = 0;
		importScroll.scroll = 0;
	}

	// Draws the scrollbar if there are more entries than the height of the window
	importScroll.Draw(Vector2{(float)Width-scrollbarSize, 0}, Vector2{(float)scrollbarSize, (float)Height});

	// Draw buttons
	importB.Draw({0, 0}, {fWidth/2, fontSize}, false, true); // Accept
	createB.Draw({fWidth/2, 0}, {fWidth/2, fontSize}, false, true); // Toggle tags

	// Draw header over tags that might be higher from scrolling
	shape.Draw(Vector2{0, fHeight-fontSize}, Vector2{fWidth-scrollbarSize, fontSize}, importButton, true );
	font.Write("Import Tags", Vector2{24, fHeight-fontSize}, fontSize/2, fontColor, true, fWidth-scrollbarSize-24, true);
}

// Resets all the variables used in the import window
void ResetImport(){
	importScroll.scroll = 0;
	importScroll.end = 0;

	createB = Button{"Create", importButton, highlight, White, fontSize/2};
	createB.toggled = false;
	importB = Button{"Import", importButton, highlight, White, fontSize/2};

	folders.path = path + slash + "shared";
	folders.GetFiles();
	completeTags.clear();
	Import.Render = &DrawImportMain;
	Import.Input = &MainImportInput;
}
