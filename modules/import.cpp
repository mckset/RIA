string DrawImport();
void CreateImport();
void ImportTags();
bool LoadImport();
void FilterTags();

int stage = 0; // Stage of importing files

Button createB = Button{"Create", importButton, highlight};
Button importB = Button{"Import", importButton, highlight};

Table folders;

string importPath = "";
string importFile = "";
vector<Tag> importTags;

void ResetImport(){
	stage = 0;

	importScroll.scroll = 0;
	importScroll.end = 0;

	createB = Button{"Create", importButton, highlight, false};
	createB.toggled = false;
	importB = Button{"Import", importButton, highlight, false};

	folders.path = path + slash + "shared";
	folders.GetFiles();
}

string DrawImport(){
	float y = Height-fontSize+importScroll.scroll;
	importScroll.end = 0;

	//
	// Main menu
	//
	if (stage == 0){
		for (int i = 0; i < folders.folders.size(); i++){
			sShape.Use();
			shape.Draw(Vector2{0, y}, Vector2{(float)Width-scrollbarSize, fontSize}, backing, true);
			// Active indicator
			if (folders.folders[i].expand)
				shape.Draw(Vector2{fontSize/4, y+fontSize/4}, Vector2{fontSize/2, fontSize/2}, White, true);
			
			sImage.Use();
			font.Write(folders.folders[i].name, Vector2{16, y}, fontSize, fontColor, true);

			if (y > fontSize && mouse.position.Within(Vector2{0, y}, Vector2{(float)Width-scrollbarSize, fontSize})){
				sShape.Use();
				shape.Draw(Vector2{0, y}, Vector2{(float)Width-scrollbarSize, fontSize}, highlight, true);

				// On click
				if (mouse.Click(LM_DOWN)){
					for (int f = 0; f < folders.folders.size(); f++)
						folders.folders[f].expand = false;
					folders.folders[i].expand = true;
				}
			}

			y-=fontSize;
			importScroll.end += fontSize;
		}
		importB.position = Vector2{0, 0};
		importB.size = Vector2{(float)Width/2-scrollbarSize/2, fontSize};
		createB.position = Vector2{(float)Width/2-scrollbarSize/2, 0};
		createB.size = Vector2{(float)Width/2-scrollbarSize/2, fontSize};
		y-=fontSize;

		createB.Draw();
		importB.Draw();

		if (mouse.Click(LM_DOWN)){

			// Create import file
			if (createB.Hover()){
				for (int i = 0; i < folders.folders.size(); i++){
					if (folders.folders[i].expand){
						importPath = folders.folders[i].path;
						importFile = importPath + slash + "import.dat";
						folders.path = folders.folders[i].path;
						folders.GetFiles();
						CreateImport();
						importTime = 120;
						Import.Hide();
						return "Created import file";
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
							importTime = 120;
							return "Missing/corrupt import data";
						}
						folders.path = folders.folders[i].path;
						folders.GetFiles();
						sort(folders.folders.begin(), folders.folders.end(), SortTable);
						for (int f = 0; f < folders.folders.size(); f++){
							folders.folders[f].expand = true;
							folders.folders[f].GetFiles();
						}
						stage++;
						importB.text = "Import Folders";
						break;
					}
				}
			}
		}

	//
	// List Folder info
	//
	}else if (stage == 1){

		// List folders
		for (int i = 0; i < folders.folders.size(); i++){
			sShape.Use();
			shape.Draw(Vector2{0, y}, Vector2{(float)Width-scrollbarSize, fontSize}, backing, true);

			// Import indicator
			if (folders.folders[i].expand)
				shape.Draw(Vector2{fontSize/4, y+fontSize/4}, Vector2{fontSize/2, fontSize/2}, White, true);
			
			if (y > fontSize && mouse.position.Within(Vector2{0, y}, Vector2{(float)Width-scrollbarSize, fontSize})){
				shape.Draw(Vector2{0, y}, Vector2{(float)Width-scrollbarSize, fontSize}, highlight, true);

				// On click
				if (mouse.Click(LM_DOWN))
					folders.folders[i].expand = !folders.folders[i].expand;
			}
			sImage.Use();
			font.Write(folders.folders[i].name, Vector2{24, y}, fontSize, fontColor, true);
			string imgCount = "[";
			imgCount += to_string(folders.folders[i].files.size());
			imgCount += "]";
			font.Write(imgCount, Vector2{(float)24 + fontSize/2*(folders.folders[i].name.length()+1), y}, fontSize, fontColor, true);
			y-=fontSize;
			importScroll.end += fontSize;
		}
		importB.position = Vector2{0, 0};
		importB.size = Vector2{(float)Width-scrollbarSize, fontSize};
		importB.Draw();
		if (mouse.Click(LM_DOWN) && importB.Hover()){
			FilterTags();
			stage++;
			createB.text = "Include Subtags";
			createB.toggle = true;
			createB.toggled = true;
			importB.text = "Import Tags";
		}

	//
	// List tags and subtags
	//
	}else if (stage == 2){
		if (!importTags.size()){
			ImportTags();
			Import.Hide();
			importTime = 120;
			return "Removed imported images";
		}

		for (auto t : importTags){
			sShape.Use();
			shape.Draw(Vector2{0, y}, Vector2{(float)Width-scrollbarSize, fontSize}, t.color, true);

			sImage.Use();
			font.Write(t.name, Vector2{24, y}, fontSize, fontColor, true);
			y-=fontSize;
			importScroll.end += fontSize;

			// Subtags
			if (createB.toggled){
				for (auto s : t.subTags){
					sShape.Use();
					shape.Draw(Vector2{8, y}, Vector2{(float)Width-scrollbarSize-8, fontSize}, s.color, true);

					sImage.Use();
					font.Write(s.name, Vector2{32, y}, fontSize, fontColor, true);

					y-=fontSize;
					importScroll.end += fontSize;
				}
			}			
		}
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
			importTime = 120;
			return "Successfully imported tags";
		}
	}

	// Scroll bar
	importScroll.end -= Height;
	if (importScroll.end < 0){
		importScroll.end = 0;
		importScroll.scroll = 0;
	}

	sShape.Use();
	importScroll.Draw(Vector2{(float)Width-scrollbarSize, 0}, Vector2{(float)scrollbarSize, (float)Height});

	return "";
}

void CreateImport(){
	ofstream w(importFile, ios::out | ios::binary);

	if (!w.good()){
		printf("[Creation] Unable to make an import file\n");
		return;
	}

	printf("Creating import file...\n");
	w.write((char*)"imp ", sizeof(char)*4);

	vector<Tag> usedTags;

	// Get used tags
	for (auto t : tags){
		Tag tag;
		tag.name = t.name;
		tag.color = t.color;

		// Get used images
		for (auto i : t.imgs){
			for (auto f : folders.folders){
				if (!strcmp(i.path.substr(0, i.path.length()-i.name.length()-1).data(), f.path.data())){
					tag.imgs.push_back(i);
					break;
				}
			}
		}

		// Get used sub tags
		for (auto s : t.subTags){
			Tag subTag;
			subTag.name = s.name;
			subTag.color = s.color;

			// Get used images
			for (auto i : s.imgs){
				for (auto f : folders.folders){
					if (!strcmp(i.path.substr(0, i.path.length()-i.name.length()-1).data(), f.path.data())){
						subTag.imgs.push_back(i);
						break;
					}
				}
			}
			if (subTag.imgs.size())
				tag.subTags.push_back(subTag);
		}

		if (tag.imgs.size())
			usedTags.push_back(tag);
	}

	printf("Tags: %ld\n", usedTags.size());

	int size = usedTags.size();
	w.write(reinterpret_cast<const char*>(&size), sizeof(int));

	// Get images in a used tag
	for (auto tag : usedTags){
		printf("[TAG]: %s", tag.name.data());

		int len = tag.name.length();

		w.write(reinterpret_cast<const char*>(&len), sizeof(int));
		w.write(tag.name.c_str(), sizeof(char)*len);

		// Color
		int r = tag.color.r*255;
		int g = tag.color.g*255;
		int b = tag.color.b*255;
		w.write(reinterpret_cast<const char*>(&r), 1);
		w.write(reinterpret_cast<const char*>(&g), 1);
		w.write(reinterpret_cast<const char*>(&b), 1);
			
		printf("\tImgs: %ld", tag.imgs.size());

		// Write img count
		size = tag.imgs.size();
		w.write(reinterpret_cast<const char*>(&size), sizeof(int));

		// Loop through tag imgs to write the images path
		for (auto img : tag.imgs){
			len = img.path.length() - importPath.length();
			w.write(reinterpret_cast<const char*>(&len), sizeof(int));
			w.write((img.path.substr(importPath.length()+1)).c_str(), sizeof(char)*len);
		}

		printf("\tSub Tags: %ld\n", tag.subTags.size());

		size = tag.subTags.size();
		w.write(reinterpret_cast<const char*>(&size), sizeof(int));

		// Write sub tag info
		for (auto subTag : tag.subTags){
			printf("\t[SUB TAG]: %s", subTag.name.data());

			len = subTag.name.length();
			w.write(reinterpret_cast<const char*>(&len), sizeof(int));
			w.write(subTag.name.c_str(), sizeof(char)*len);

			// Color
			int r = subTag.color.r*255;
			int g = subTag.color.g*255;
			int b = subTag.color.b*255;
			w.write(reinterpret_cast<const char*>(&r), 1);
			w.write(reinterpret_cast<const char*>(&g), 1);
			w.write(reinterpret_cast<const char*>(&b), 1);
				
			printf("\tImgs: %ld\n", subTag.imgs.size());
			
			// Write img count
			size = subTag.imgs.size();
			w.write(reinterpret_cast<const char*>(&size), sizeof(int));

			// Loop through tag imgs to write the images path
			for (auto img : subTag.imgs){
				len = img.path.length() - importPath.length();
				w.write(reinterpret_cast<const char*>(&len), sizeof(int));
				w.write((img.path.substr(importPath.length()+1)).c_str(), sizeof(char)*len);
			}
		}
	}
	w.close();
}

void FilterTags(){
	vector<Tag> tempTags;
	for (int i = 0; i < importTags.size(); i++)
		tempTags.push_back(importTags[i]);
	importTags.clear();
	for (auto t : tempTags){
		Tag tag;
		tag.name = t.name;
		tag.color = t.color;

		// Get used images per tag
		for (auto i : t.imgs){
			string checkPath = i.path.substr(0, i.path.length() - i.name.length() - 1);
			for (auto f : folders.folders)
				if (!strcmp(checkPath.data(), f.path.data())){
					if (f.expand)
						tag.imgs.push_back(i);
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
				if (subTag.imgs.size())
					tag.subTags.push_back(subTag);
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

void ImportTags(){
	vector<Tag> tempTags;

	folders.path = importPath;
	folders.GetFiles();

	// Remove all images that share the same path as the import directory from the main tag variable
	for (auto t : tags){
		Tag tag;
		tag.name = t.name;
		tag.color = t.color;

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

			// Images
			for (auto i : s.imgs){
				for (auto img : tag.imgs){
					if (!strcmp(i.path.data(), img.path.data())){
						subTag.imgs.push_back(i);
						break;
					}
				}

			}
			if (subTag.imgs.size())
				tag.subTags.push_back(subTag);
		}

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

bool LoadImport(){
	importTags.clear();

	f.open(importFile, ios::in | ios::binary);
    if (!f.good()){
		printf("[Import] import.dat not found at: %s\n", importFile.data());
		return false;
	}

	int ibuffer;

	// Read past header
	f.read(reinterpret_cast<char*>(&ibuffer), 4);

	printf("[Import] Importing Tags\n");
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
