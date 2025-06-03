/*
	Handles input for the import window as well as function for importing image packs
*/

vector<File> importFiles;

// Gets all the files in the selected folder and compares them against tags. If it's tagged, it gets saved
void CreateImport(){
	vector<File> files = selectedFolder->GetAll();
	vector<Tag> usedTags;

	// Get all the used tags
	for (auto tag : tags){
		Tag temp = FilterTag(tag, files);
		if (temp.imgs.size())
			usedTags.push_back(temp);
	}

	// Creates the import file
	ofstream w(importPath + slash + "temp.dat", ios::out | ios::binary);
	if (!w.good()){
		if (DEBUG) printf("[Import] Unable to make create the import file\n");
		importText = "Failed to make import file";
		return;
	}

	if (DEBUG) printf("Creating import file...\n");
	w.write((char*)"imp ", 4);
	SaveTags(&w, usedTags, false);
	w.close();
	save = 0;

	// Move import file over old one if it exists
	filesystem::rename(importPath + slash + "temp.dat", importFile);
}

// Checks a tag to see if it contains any given files and returns a tag with only matching files
Tag FilterTag(Tag tag, vector<File> files){
	int pathSize = importPath.length();
	Tag output = Tag{tag.name, tag.color};
	
	// Loop through all files
	for (auto file : files){
		
		// Loop through all files in a tag
		for (auto img : tag.imgs){
			if (img.path == file.path){
				output.imgs.push_back(File{file.name, file.path.substr(pathSize)});

				// Filter subtags
				for (auto subTag : tag.subTags){
					vector<File> f;
					f.push_back(file);
					Tag temp = FilterTag(subTag, f);

					if (temp.imgs.size()){
						bool add = true;
						for (int i = 0; i < output.subTags.size(); i++){
							if (output.subTags[i].name == subTag.name){
								add = false;
								output.subTags[i].imgs.push_back(File{file.name, file.path.substr(pathSize)});
								break;
							}
						}

						if (add)
							output.subTags.push_back(temp);
					}
				}
				break;
			}
		}
	}
	return output;
}

// Get tags to import
void FoldersImportInput(){
	if (mouse.Click() && importB.Hover()){
		importFiles = folders.GetHidden();
		importScroll.scroll = 0;

		// Loop through all the tags imported from the file
		for (auto tag : completeTags){
			bool newTag = true;

			// Loop through all the images in each tag
			for (auto img : tag.imgs){

				// Check if a file in the folder matches the image
				for (auto file : importFiles){
					if (importPath + img.path == file.path){
						if (newTag){
							newTag = false;
							importTags.push_back(Tag{tag.name, tag.color});
						}
						importTags.back().imgs.push_back(file);

						// Sub tags (same idea)
						for (auto subTag : tag.subTags){
							for (auto subImg : subTag.imgs){
								if (subImg.path == img.path){
									bool newSub = true;

									for (int i = 0; i < importTags.back().subTags.size(); i++){
										if (importTags.back().subTags[i].name == subTag.name){
											newSub = false;
											importTags.back().subTags[i].imgs.push_back(file);
											break;
										}
									}
									if (newSub){
										importTags.back().subTags.push_back(Tag{subTag.name, subTag.color});
										importTags.back().subTags.back().imgs.push_back(file);
									}
									break;
								}
							}
						}
						break;
					}
				}
			}
		}
		sort(importTags.begin(), importTags.end(), SortTag);
		for (int i = 0; i < importTags.size(); i++)
			sort(importTags[i].subTags.begin(), importTags[i].subTags.end(), SortTag);

		importB.text = "Import";
		createB.text = "Subtags";
		createB.toggled = true;
		Import.Render = &DrawImportTags;
		Import.Input = &TagsImportInput;
	}
}

// Returns the first folder in the path
string GetFolderName(string path){
	for (int i = 1; i < path.length(); i++)
		if (path[i] == slash[0])
			return path.substr(1, i-1);
	return path;
}

// Checks if an image is already in the given tag to prevent double tagging
void ImportTag(Tag imported, Tag *base){
	importScroll.scroll = 0;
	for (auto img : imported.imgs){
		bool newImg = true;
		for (int i = 0; i < base->imgs.size(); i++){
			if (img.path == base->imgs[i].path){
				newImg = false;
				break;
			}
		}
		if (newImg)
			base->imgs.push_back(img);
	}
}

// Get saved image pack information
bool LoadImport(){

	// Reset variables
	importScroll.scroll = 0;
	oldSave = false;
	importTags.clear();
    completeTags.clear();
	folders.folders.clear();

	// Open import.data
	f.open(importFile, ios::in | ios::binary);
    if (!f.good()){
		if (DEBUG) printf("[Image Pack] import.dat not found at: %s\n", importFile.data());
		return false;
	}

	int buf = 0;
	f.read(reinterpret_cast<char*>(&buf), 4);
	completeTags = LoadTags(&f, false, false);
	f.close();
	return true;
}

// Handles the tag import menu input
void TagsImportInput(){
	if (mouse.Click()){
		
		// Toggle sub tag import
		if (createB.Hover())
			createB.toggled = !createB.toggled;
		
		// Import tags
		else if (importB.Hover()){
			for (auto tag : importTags){
				bool newTag = true;
				for (int t = 0; t < tags.size(); t++){

					// If the tag exists, append to it
					if (tags[t].name == tag.name){
						newTag = false;
						ImportTag(tag, &tags[t]);

						// Append sub tags if enabled
						if (createB.toggled){
							for (auto subTag : tag.subTags){
								bool newSub = true;
								for (int s = 0; s < tags[t].subTags.size(); s++){
									if (subTag.name == tags[t].subTags[s].name){
										newSub = false;
										ImportTag(subTag, &tags[t].subTags[s]);
										break;
									}
								}
								if (newSub)
									tags[t].subTags.push_back(subTag);
							}
						}
						break;
					}

				}

				// Tag doesn't exist
				if (newTag){

					// Append sub tags if enabled
					if (createB.toggled)
						tags.push_back(tag);
					else{
						Tag t = Tag{tag.name, tag.color};
						t.imgs = tag.imgs;
						tags.push_back(t);
					}
				}
			}
			sort(tags.begin(), tags.end(), SortTag);
			for (int i = 0; i < tags.size(); i++){
				sort(tags[i].imgs.begin(), tags[i].imgs.end(), SortFile);
				sort(tags[i].subTags.begin(), tags[i].subTags.end(), SortTag);
				for (int s = 0; s < tags[i].subTags.size(); s++)
					sort(tags[i].subTags[s].imgs.begin(), tags[i].subTags[s].imgs.end(), SortFile);
			}
			Import.Hide();
		}
	}
}

// Default input for the import window
void MainImportInput(){
	if (mouse.Click()){

		// Create import file
		if (createB.Hover()){
			importPath = selectedFolder->path;
			importFile = importPath + slash + "import.dat";
			CreateImport();
			importTime = 400;
			Import.Hide();
			if (!importText.length())
				importText = "Created import file";

		// Import tags
		}else if (importB.Hover() && selectedFolder){
			importPath = selectedFolder->path;
			importFile = importPath+  slash + "import.dat";
			
			if (!LoadImport()){
				Import.Hide();
				importTime = 400;
				importText = "Missing/corrupt import data";
			}
			
			folders.path = importPath;
			importFiles = folders.GetAll();
			sort(folders.folders.begin(), folders.folders.end(), folders.SortTable);
			
			importB.text = "Import Folders";
			Import.Render = &DrawImportFolders;
			Import.Input = &FoldersImportInput;

		// Open folder
		}else if (openB.Hover()){
			OpenShared();
		}
	}
}