void DrawImportMain();
bool LoadImport();
void MainImportInput();

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

void DrawImportMain(){
	importScroll.end = 0;
	float y = Height-fontSize*2+importScroll.scroll;

	for (int i = 0; i < folders.folders.size(); i++){
		shape.Draw(Vector2{0, y}, Vector2{fWidth-scrollbarSize, fontSize}, backing, true);
		
		// Active indicator
		if (folders.folders[i].expand)
			shape.DrawCircle(Vector2{fWidth-scrollbarSize, y} + Vector2{-fontSize/2, fontSize/2}, fontSize, 0, highlight, true);
		
		font.Write(folders.folders[i].name, Vector2{24, y}, fontSize/2, fontColor, true, fWidth-scrollbarSize-48);

		if (y > fontSize && mouse.position.Within(Vector2{0, y}, Vector2{fWidth-scrollbarSize, fontSize}) && Import.Focus()){
			shape.Draw(Vector2{0, y}, Vector2{fWidth-scrollbarSize, fontSize}, highlight, true);

			// On click
			if (mouse.Click(LM_DOWN) && y <= fHeight-fontSize*2 && y >= (float)fontSize*2){
				for (int f = 0; f < folders.folders.size(); f++)
					folders.folders[f].expand = false;
				folders.folders[i].expand = true; // Reuses the expand bool as a selection bool because I thought I was being smart
				selectedFolder = &folders.folders[i];
			}
		}
		
		y-=fontSize;
		importScroll.end += fontSize;
	}
	importScroll.end += fontSize;

	if (!folders.folders.size()){
		sFont.Use(true);
		font.Write("No image packs found.", Vector2{16, y}, fontSize/2, fontColor);
		font.Write("Ensure that your image", Vector2{16, y-fontSize}, fontSize/2, fontColor);
		font.Write("packs are saved to the", Vector2{16, y-fontSize*2}, fontSize/2, fontColor);
		font.Write("shared folder.", Vector2{16, y-fontSize*3}, fontSize/2, fontColor);
	}else{
		shape.Draw(Vector2{0, fHeight-fontSize}, Vector2{fWidth-scrollbarSize, fontSize}, importButton, true );
		font.Write("Image Packs", Vector2{24, fHeight-fontSize}, fontSize/2, fontColor, true, fWidth-scrollbarSize-24, true);
	}

	if (importScroll.end < 0){
		importScroll.end = 0;
		importScroll.scroll = 0;
	}
	importScroll.Draw(Vector2{(float)Width-scrollbarSize, 0}, Vector2{(float)scrollbarSize, (float)Height});

	createB.Draw({fWidth/3*2-scrollbarSize/3*2, 0}, {fWidth/3-scrollbarSize/3, fontSize}, false, true);
	openB.Draw({fWidth/3-scrollbarSize/3, 0}, {fWidth/3-scrollbarSize/3, fontSize}, false, true);
	importB.Draw({0, 0}, {fWidth/3-scrollbarSize/3, fontSize}, false, true);
}

void DrawFolders(float indent, float *y, Vector2 size, Table *folder){
	shape.Draw(Vector2{indent, *y}, Vector2{fWidth-scrollbarSize-indent, fontSize}, backing, true);
		
	// Active indicator
	if (!folder->expand)
		shape.DrawCircle(Vector2{fWidth-scrollbarSize, *y} + Vector2{-fontSize/2, fontSize/2}, fontSize, 0, highlight, true);
		
	font.Write(folder->name, Vector2{24+indent, *y}, fontSize/2, fontColor, true, fWidth-scrollbarSize-48);

	if (*y > fontSize && mouse.position.Within(Vector2{0, *y}, Vector2{fWidth-scrollbarSize, fontSize}) && Import.Focus()){
		shape.Draw(Vector2{indent, *y}, Vector2{fWidth-scrollbarSize-indent, fontSize}, highlight, true);

		// On click
		if (mouse.Click(LM_DOWN) && *y <= fHeight-fontSize*2 && *y >= (float)fontSize*2)
			folder->expand = !folder->expand; 
		
	}

	*y-=fontSize;
	importScroll.end += fontSize;

	if (folder->folders.size())
		for (int i = 0; i < folder->folders.size(); i++)
			DrawFolders(indent+16, y, size-Vector2{16, 0}, &(folder->folders[i]));
}

void DrawImportTags(){
	importScroll.end = 0;
	float y = Height-fontSize*2+importScroll.scroll;

	for (auto tag : importTags){
		shape.Draw(Vector2{0, y}, Vector2{fWidth-scrollbarSize, fontSize}, tag.color, true);
		font.Write(tag.name, Vector2{24, y}, fontSize/2, fontColor, true, fWidth-scrollbarSize-48);
		y-=fontSize;
		importScroll.end += fontSize;
		
		if (createB.toggled){
			for (auto subTag : tag.subTags){
				shape.Draw(Vector2{16, y}, Vector2{fWidth-scrollbarSize-16, fontSize}, subTag.color, true);
				font.Write(subTag.name, Vector2{40, y}, fontSize/2, fontColor, true, fWidth-scrollbarSize-48);
				y-=fontSize;
				importScroll.end += fontSize;
			}
		}
	}
		
	
	importScroll.end += fontSize;
	if (importScroll.end < 0){
		importScroll.end = 0;
		importScroll.scroll = 0;
	}
	importScroll.Draw(Vector2{(float)Width-scrollbarSize, 0}, Vector2{(float)scrollbarSize, (float)Height});
	importB.Draw({0, 0}, {fWidth/2, fontSize}, false, true);
	createB.Draw({fWidth/2, 0}, {fWidth/2, fontSize}, false, true);

	shape.Draw(Vector2{0, fHeight-fontSize}, Vector2{fWidth-scrollbarSize, fontSize}, importButton, true );
	font.Write("Import Tags", Vector2{24, fHeight-fontSize}, fontSize/2, fontColor, true, fWidth-scrollbarSize-24, true);
}

void DrawImportFolders(){
	importScroll.end = 0;
	float y = Height-fontSize*2+importScroll.scroll;

	for (int i = 0; i < folders.folders.size(); i++)
		DrawFolders(0, &y, Vector2{fWidth-scrollbarSize, fontSize}, &(folders.folders[i]));
	
	importScroll.end += fontSize;
	if (importScroll.end < 0){
		importScroll.end = 0;
		importScroll.scroll = 0;
	}
	importScroll.Draw(Vector2{(float)Width-scrollbarSize, 0}, Vector2{(float)scrollbarSize, (float)Height});
	importB.Draw({0, 0}, {fWidth, fontSize}, false, true);

	shape.Draw(Vector2{0, fHeight-fontSize}, Vector2{fWidth-scrollbarSize, fontSize}, importButton, true );
	font.Write("Folder Select", Vector2{24, fHeight-fontSize}, fontSize/2, fontColor, true, fWidth-scrollbarSize-24, true);
}