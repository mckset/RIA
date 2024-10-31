void CreateImport();
Tag CleanTag(Tag, bool);

void DrawImport();
string DrawPacks();
string DrawFolders();
string DrawImportTags();

void FilterTags();
void ImportTags();
bool IsImportTag(Tag);
bool LoadImport();

int stage = 0; // Stage of importing files



//
// Handles drawing the import window
//
void DrawImport(){
	float y = Height-fontSize*2+importScroll.scroll;
	importScroll.end = 0;

	string output = "";

	// Display image packs
	if (stage == 0){
		output = DrawPacks();
			

	// Display image pack folders
	}else if (stage == 1){
		output = DrawFolders();
		

	// List tags and subtags
	}else if (stage == 2){
		output = DrawImportTags();
	

	// Overwrite check
	}else if (stage == -1){
		shape.Draw(Vector2{0, (float)Height-fontSize}, Vector2{(float)Width, fontSize}, importButton, true);
		font.Write("Overwrite?", Vector2{24, (float)Height-fontSize}, fontSize/2, fontColor, true, (float)Width-scrollbarSize-24, true);

		font.Write("Import.dat already exists.", Vector2{16, y}, fontSize/2, fontColor, true);
		font.Write("Do you want to overwrite", Vector2{16, y-fontSize}, fontSize/2, fontColor, true);
		font.Write("the existing data?", Vector2{16, y-fontSize*2}, fontSize/2, fontColor, true);
		font.Write("(There's no undo)", Vector2{16, y-fontSize*3}, fontSize/2, fontColor, true);

		createB.position = Vector2{0, 0};
		importB.position = Vector2{(float)Width/2, 0};

		createB.size = Vector2{(float)Width/2, fontSize};
		importB.size = Vector2{(float)Width/2, fontSize};

		importB.Draw(false, true);
		createB.Draw(false, true);

		if (mouse.Click(LM_DOWN)){
			if (createB.Hover()){
				folders.GetFiles();
				CreateImport();
				importTime = 400;
				Import.Hide();
				importText = "Updated import file";
			}
			ResetImport();
			Import.Hide();
		}

	}
	if (stage > -1){
		// Scroll bar
		importScroll.end -= Height;
		if (importScroll.end < 0){
			importScroll.end = 0;
			importScroll.scroll = 0;
		}

		importScroll.Draw(Vector2{(float)Width-scrollbarSize, 0}, Vector2{(float)scrollbarSize, (float)Height});
	}
	importText = output;
}

//
// Display image packs and return any errors
//
string DrawPacks(){
	float y = Height-fontSize*2+importScroll.scroll;

	for (int i = 0; i < folders.folders.size(); i++){
		shape.Draw(Vector2{0, y}, Vector2{(float)Width-scrollbarSize, fontSize}, backing, true);
		
		// Active indicator
		if (folders.folders[i].expand)
			shape.Draw(Vector2{0, y}, Vector2{(float)Width-scrollbarSize, fontSize}, highlight, true);
		
		font.Write(folders.folders[i].name, Vector2{24, y}, fontSize/2, fontColor, true, (float)Width-scrollbarSize-48);

		if (y > fontSize && mouse.position.Within(Vector2{0, y}, Vector2{(float)Width-scrollbarSize, fontSize}) && Import.Focus()){
			shape.Draw(Vector2{0, y}, Vector2{(float)Width-scrollbarSize, fontSize}, highlight, true);

			// On click
			if (mouse.Click(LM_DOWN) && y <= (float)Height-fontSize*2 && y >= (float)fontSize*2){
				for (int f = 0; f < folders.folders.size(); f++)
					folders.folders[f].expand = false;
				folders.folders[i].expand = true;
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
		shape.Draw(Vector2{0, (float)Height-fontSize}, Vector2{(float)Width-scrollbarSize, fontSize}, importButton, true );
		font.Write("Image Packs", Vector2{24, (float)Height-fontSize}, fontSize/2, fontColor, true, (float)Width-scrollbarSize-24, true);
	}
	importB.position = Vector2{0, 0};
	importB.size = Vector2{(float)Width/3-scrollbarSize/3, fontSize};
	openB.position = Vector2{(float)Width/3-scrollbarSize/3, 0};
	openB.size = Vector2{(float)Width/3-scrollbarSize/3, fontSize};
	createB.position = Vector2{(float)Width/3*2-scrollbarSize/3*2, 0};
	createB.size = Vector2{(float)Width/3-scrollbarSize/3, fontSize};

	createB.Draw(false, true);
	openB.Draw(false, true);
	importB.Draw(false, true);

	if (mouse.Click(LM_DOWN)){

		// Create import file
		if (createB.Hover()){
			for (int i = 0; i < folders.folders.size(); i++){
				if (folders.folders[i].expand){
					struct stat st;

					importPath = folders.folders[i].path;
					importFile = importPath + slash + "import.dat";
					folders.path = folders.folders[i].path;

					if (stat(importFile.c_str(), &st) == 0){
						stage = -1;
						importB.text = "Cancel";
						createB.text = "Overwrite";
						importText = "";
					}else{
						folders.GetFiles();
						CreateImport();
						importTime = 400;
						Import.Hide();
						importText = "Created import file";
					}
				}
			}

		// Import tags
		}else if (importB.Hover()){
			for (int i = 0; i < folders.folders.size(); i++){
				if (folders.folders[i].expand){
					importPath = folders.folders[i].path;
					importFile = importPath+  slash + "import.dat";
					if (!LoadImport()){
						Import.Hide();
						importTime = 400;
						importText = "Missing/corrupt import data";
					}
					folders.path = folders.folders[i].path;
					folders.GetFiles();
					sort(folders.folders.begin(), folders.folders.end(), folders.SortTable);
					for (int f = 0; f < folders.folders.size(); f++){
						folders.folders[f].expand = true;
						folders.folders[f].GetFiles();
					}
					stage++;
					importB.text = "Import Folders";
					break;
				}
			}

		// Open folder
		}else if (openB.Hover()){
			OpenShared();
		}
	}
	return "";
}


//
// Display image pack folders
//
string DrawFolders(){
	float y = Height-fontSize*2+importScroll.scroll;

	// List folders
	for (int i = 0; i < folders.folders.size(); i++){
		shape.Draw(Vector2{0, y}, Vector2{(float)Width-scrollbarSize, fontSize}, backing, true);

		// Import indicator
		if (folders.folders[i].expand)
			shape.Draw(Vector2{fontSize/4, y+fontSize/4}, Vector2{fontSize/2, fontSize/2}, White, true);
			
		if (y > fontSize && mouse.position.Within(Vector2{0, y}, Vector2{(float)Width-scrollbarSize, fontSize}) && Import.Focus()){
			shape.Draw(Vector2{0, y}, Vector2{(float)Width-scrollbarSize, fontSize}, highlight, true);

			// On click
			if (mouse.Click(LM_DOWN) && y <= (float)Height-fontSize*2 && y >= (float)fontSize*2)
				folders.folders[i].expand = !folders.folders[i].expand;
		}
		font.Write(folders.folders[i].name + "[" + to_string(folders.folders[i].files.size()) + "]", 
			Vector2{24, y}, fontSize/2, fontColor, true, (float)Width-scrollbarSize-48);

		y-=fontSize;
		importScroll.end += fontSize;
	}
	importScroll.end += fontSize*2;

	shape.Draw(Vector2{0, (float)Height-fontSize}, Vector2{(float)Width-scrollbarSize, fontSize}, importButton, true );
	font.Write("Folders", Vector2{24, (float)Height-fontSize}, fontSize/2, fontColor, true, (float)Width-scrollbarSize-24, true);

	importB.position = Vector2{0, 0};
	importB.size = Vector2{(float)Width-scrollbarSize, fontSize};
	importB.Draw();
	if (mouse.Click(LM_DOWN) && importB.Hover()){
		FilterTags();
		stage++;
		createB.text = "Include Subtags";
		createB.toggled = true;
		importB.text = "Import Tags";
	}
	return "";
}

//
// Display tags to import
//
string DrawImportTags(){
	float y = Height-fontSize*2+importScroll.scroll;

	if (!importTags.size()){
		ImportTags();
		Import.Hide();
		importTime = 400;
		return "Removed imported images";
	}

	for (auto t : importTags){
		shape.Draw(Vector2{0, y}, Vector2{(float)Width-scrollbarSize, fontSize}, t.color, true);

		font.Write(t.name, Vector2{8, y}, fontSize/2, fontColor, true, (float)Width-scrollbarSize-48);
		y-=fontSize;
		importScroll.end += fontSize;

		// Subtags
		if (createB.toggled){
			for (auto s : t.subTags){
				shape.Draw(Vector2{16, y}, Vector2{(float)Width-scrollbarSize-16, fontSize}, s.color, true);
				font.Write(s.name, Vector2{24, y}, fontSize/2, fontColor, true, (float)Width-scrollbarSize-48);

				y-=fontSize;
				importScroll.end += fontSize;
			}
		}			
	}
	importScroll.end += fontSize;

	shape.Draw(Vector2{0, (float)Height-fontSize}, Vector2{(float)Width-scrollbarSize, fontSize}, importButton, true);
	font.Write("Tags to Import", Vector2{24, (float)Height-fontSize}, fontSize/2, fontColor, true, (float)Width-scrollbarSize-24, true);

	importScroll.end += fontSize;
	importB.position = Vector2{0, 0};
	importB.size = Vector2{(float)Width/2-scrollbarSize/2, fontSize};
	createB.position = Vector2{(float)Width/2-scrollbarSize/2, 0};
	createB.size = Vector2{(float)Width/2-scrollbarSize/2, fontSize};
		
	importB.Draw();
	createB.Draw();
			
	// Toggle Subtags
	if ((mouse.Click(LM_DOWN) && createB.Hover()) || keyboard.newKey == KEY_TAB)
		createB.toggled = !createB.toggled;
			
	// Import Tags
	else if ((mouse.Click(LM_DOWN) && importB.Hover()) || keyboard.newKey == KEY_ENTER){
		ImportTags();
		Import.Hide();
		importTime = 400;
		return "Successfully imported tags";
	}
	return "";
}

//
// Loop through all the available tags and grab only the ones in the selected folders
//
void FilterTags(){
	completeTags.clear();
	for (int i = 0; i < importTags.size(); i++)
		completeTags.push_back(importTags[i]);
	importTags.clear();

	for (auto t : completeTags){
		Tag tag;
		tag.name = t.name;
		tag.color = t.color;

		// Get used images per tag
		for (auto i : t.imgs){
			string checkPath = i.path.substr(0, i.path.length() - i.name.length() - 1);
			for (auto f : folders.folders)
				if (!strcmp(checkPath.data(), f.path.data())){
					if (f.expand){
						tag.imgs.push_back(i);
					}
					break;
				}
			
		}

		// Get sub tags
		if (tag.imgs.size()){
			for (auto s : t.subTags){
				Tag subTag;
				subTag.name = s.name;
				subTag.color = s.color;
				for (auto i : s.imgs){
					for (auto ti : tag.imgs){
						if (!strcmp(i.path.data(), ti.path.data())){
							subTag.imgs.push_back(i);
						}
					}
				}
				if (subTag.imgs.size()){

					// Prevents overwritting user defined colors
					for (auto mainTag : tags){
						if (!strcmp(mainTag.name.data(), tag.name.data())){
							for (auto mainSub : mainTag.subTags){
								if (!strcmp(mainSub.name.data(), subTag.name.data())){
									subTag.color = mainSub.color;
									break;
								}
							}
							break;
						}
					}
					tag.subTags.push_back(subTag);
				}
			}
			// Prevents overwritting user defined colors
			for (auto mainTag : tags){
				if (!strcmp(mainTag.name.data(), tag.name.data())){
					tag.color = mainTag.color;
					break;
				}
			}
			importTags.push_back(tag);
		}
	}
	for (auto t : importTags){
		printf("%s : %ld\n", t.name.data(), t.imgs.size());
		for (auto s : t.subTags)
			printf("\t%s : %ld\n", s.name.data(), s.imgs.size());
	}
}


//
// Checks if a tag belongs to the image pack
//
bool IsImportTag(Tag tag){
	for (auto t : completeTags)
		if (!strcmp(tag.name.data(), t.name.data()))
			return true;
	return false;
}

//
// Imports tags from an image pack
//
void ImportTags(){
	vector<Tag> tempTags;

	folders.path = importPath;
	folders.GetFiles();

	// Remove all images that share the same path as the import directory from the main tag variable
	for (auto t : tags){
		Tag tag;
		tag.name = t.name;
		tag.color = t.color;

		// Only remove images from tags that belong to the image pack
		if (IsImportTag(tag)){
			// Images
			for (auto i : t.imgs){
				bool imported = false;
				for (auto f : folders.folders){
					if (!strcmp(i.path.substr(0, i.path.length()-i.name.length()-1).data(), f.path.data())){
						imported = true;
						break;
					}
				}

				if (!imported)
					tag.imgs.push_back(i);
			}

			// Subtags
			for (auto s : t.subTags){
				Tag subTag;
				subTag.name = s.name;
				subTag.color = s.color;

				// Only remove images from subtags that belong to the image pack
				if (IsImportTag(subTag)){

					// Images
					for (auto i : s.imgs){
						for (auto img : tag.imgs){
							if (!strcmp(i.path.data(), img.path.data())){
								subTag.imgs.push_back(i);
								break;
							}
						}
					}
				}else
					subTag = s;

				if (subTag.imgs.size())
						tag.subTags.push_back(subTag);
			}
		}else
			tag = t;
		
		if (tag.imgs.size())
			tempTags.push_back(tag);
	}

	// Append tags
	for (auto tag : importTags){
		bool newTag = true;

		for (int t = 0; t < tempTags.size(); t++){

			// Append to tag
			if (!strcmp(tag.name.data(), tempTags[t].name.data())){
				newTag = false;

				// Append images into existing tag
				for (auto img : tag.imgs)
					tempTags[t].imgs.push_back(img);
				
				// Only check sub tags if the button is pressed
				if (createB.toggled){
					
					// Sub tags
					for (auto subTag : tag.subTags){
						bool newSub = true;

						for (int s = 0; s < tempTags[t].subTags.size(); s++){

							// Append images into existing sub tag
							if (!strcmp(subTag.name.data(), tempTags[t].subTags[s].name.data())){
								newSub = false;
								for (auto img : subTag.imgs)
									tempTags[t].subTags[s].imgs.push_back(img);
								break;
							}
						}
						if (newSub)
							tempTags[t].subTags.push_back(subTag);
					}
				}
				break;
			}
		}
		if (newTag){
			tempTags.push_back(tag);
			if (!createB.toggled)
				tempTags[tempTags.size()-1].subTags.clear();
		}
	}
	tags = tempTags;
	
	// Sort
	sort(tags.begin(), tags.end(), SortTag);
	for (int t = 0; t < tags.size(); t++){
		sort(tags[t].imgs.begin(), tags[t].imgs.end(), SortFile);
		sort(tags[t].subTags.begin(), tags[t].subTags.end(), SortTag);
		for (int s = 0; s < tags[t].subTags.size(); s++)
			sort(tags[t].subTags[s].imgs.begin(), tags[t].subTags[s].imgs.end(), SortFile);
	}
}

//
// Get saved image pack information
//
bool LoadImport(){
	importTags.clear();

	f.open(importFile, ios::in | ios::binary);
    if (!f.good()){
		printf("[Image Pack] import.dat not found at: %s\n", importFile.data());
		return false;
	}

	int ibuffer;

	// Read past header
	f.read(reinterpret_cast<char*>(&ibuffer), 4);

	printf("[Image Pack] Importing Tags\n");
	// Tags
	f.read(reinterpret_cast<char*>(&ibuffer), 4);
	int tagCount = ibuffer;
	for (int t = 0; t < tagCount; t++){
		Tag tag;

		// Name
		tag.name = GetString();

		// Color
		f.read(reinterpret_cast<char*>(&ibuffer), 1);
		tag.color.r = (float)ibuffer/255;
		f.read(reinterpret_cast<char*>(&ibuffer), 1);
		tag.color.g = (float)ibuffer/255;
		f.read(reinterpret_cast<char*>(&ibuffer), 1);
		tag.color.b = (float)ibuffer/255;
		tag.color.a = 1;

		// Images
		f.read(reinterpret_cast<char*>(&ibuffer), 4);
		for (int i = 0; i < ibuffer; i++){
			string p = GetString();
			tag.imgs.push_back(File{GetName(p), importPath + slash + p});
		}

		sort(tag.imgs.begin(), tag.imgs.end(), SortFile);

		// Sub tags
		f.read(reinterpret_cast<char*>(&ibuffer), 4);
		int subSize = ibuffer;

		// Repeat everything
		for (int i = 0; i < subSize; i++){
			Tag subTag;

			// Name
			subTag.name = GetString();

			// Color
			f.read(reinterpret_cast<char*>(&ibuffer), 1);
			subTag.color.r = (float)ibuffer/255;
			f.read(reinterpret_cast<char*>(&ibuffer), 1);
			subTag.color.g = (float)ibuffer/255;
			f.read(reinterpret_cast<char*>(&ibuffer), 1);
			subTag.color.b = (float)ibuffer/255;
			subTag.color.a = 1;

			// Images
			f.read(reinterpret_cast<char*>(&ibuffer), 4);
			for (int x = 0; x < ibuffer; x++){
				string p = GetString();
				subTag.imgs.push_back(File{GetName(p), importPath + slash + p});
			}
			sort(subTag.imgs.begin(), subTag.imgs.end(), SortFile);
			tag.subTags.push_back(subTag);
		}
		sort(tag.subTags.begin(), tag.subTags.end(), SortTag);
		importTags.push_back(tag);
	}
	f.close();
	return true;
}

Tag CleanTag(Tag t, bool condition){
		Tag tag;
		tag.name = t.name;
		tag.color = t.color;

		// Get used images
		for (auto i : t.imgs){
			for (auto f : folders.folders){
				if (!strcmp(i.path.substr(0, i.path.length()-i.name.length()-1).data(), f.path.data()) && condition){
					tag.imgs.push_back(i);
					break;
				}else if (strcmp(i.path.substr(0, i.path.length()-i.name.length()-1).data(), f.path.data()) && !condition){
					tag.imgs.push_back(i);
					break;
				}
			}
		}
		return tag;
}