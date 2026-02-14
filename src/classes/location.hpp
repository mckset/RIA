enum Location_Options{
	LOCATION_NONE,
	LOCATION_DELETE
};


class Location{
	public:
		string name = "";
		string path = "";
		bool expanded = false;
		bool initialized = false;
		int listSize = 0;

		vector<Location> folders;
		vector<File> files;

		Button delete_Button{"x", Transparent, highlightColor, fontColor, SMALL_FONT_SIZE};

		int Draw(Vector2 position, Vector2 size, bool isSubFolder = false){
			bool deleteLocation = false;

			if (!initialized)
				GetFiles();

			// Prevent drawing under the window and return the list size
			if (position.y - size.y < -size.y*3){
				GetListSize(size.y);
				return LOCATION_NONE;
			}

			listSize = size.y;
			bool draw = position.y - size.y*2 <= fHeight; // Prevent drawing above the window

			if (draw){
				// Heading
				shape.Draw(position, size, locationHeadingColor, POSITION_FIXED);
			
				// Text
				font.Write(name, position, SMALL_FONT_SIZE, fontColor, POSITION_FIXED, size.x-FONT_SIZE);

				// Delete button
				if (!isSubFolder){
					delete_Button.Draw({size.x-FONT_SIZE, position.y}, {FONT_SIZE, FONT_SIZE}, ALIGN_CENTER);
					if (delete_Button.pressed){
						deleteLocation = true;
					}
				}

				// Input
				if (mouse.Within(position, size) && CurrentWindow->focused){
					// Deleting
					if (!isSubFolder && keyboard.newKey == KEY_DELETE){
						keyboard.newKey = INPUT_NULL;
						deleteLocation = true;
					}

					// Expand folder
					if (mouse.Click() && !delete_Button.hovered)
						expanded = !expanded;

					if (!isSubFolder && !delete_Button.hovered)
						shape.Draw(position, size, highlightColor, POSITION_FIXED);
					
				}
			}

			if (!expanded)
				return !deleteLocation ? LOCATION_NONE : LOCATION_DELETE;

			// Folders
			for (auto& folder : folders){
				folder.Draw(position + Vector2{FILE_PADDING, (float)-listSize}, Vector2{size.x - FILE_PADDING, size.y}, true);
				listSize += folder.listSize;
			}
			
			draw = position.y - size.y - listSize <= fHeight;

			// Files
			for (auto file : files){
				
				if (draw){
					shape.Draw(position + Vector2{FILE_PADDING, (float)-listSize}, size - Vector2{FILE_PADDING, 0}, locationFileColor, POSITION_FIXED);
					font.Write(file.name, position + Vector2{FILE_PADDING, (float)-listSize}, SMALL_FONT_SIZE, fontColor, POSITION_FIXED, size.x-FILE_PADDING);
						
					// Tagged indicator
					if (fileTagMap[file.path])
						shape.Draw(position + Vector2{0, (float)-listSize}, Vector2{FILE_PADDING, size.y}, White, POSITION_FIXED);
						
					if (previewImg.img.loaded && previewImg.path == file.path)
						shape.Draw(position + Vector2{FILE_PADDING, (float)-listSize}, size - Vector2{FILE_PADDING, 0}, highlightColor, POSITION_FIXED);
						

					// Mouse
					if (mouse.Within(position + Vector2{0, (float)-listSize}, size - Vector2{FILE_PADDING, 0}) && CurrentWindow->focused){
						
						// Preview Image
						if (mouse.Click() && !showTutorial)
							previewImg.LoadPreview(file.path);
						
						shape.Draw(position + Vector2{FILE_PADDING, (float)-listSize}, size + Vector2{-FILE_PADDING, 0}, highlightColor, POSITION_FIXED);
					}
				}else
					draw = position.y - size.y*2 - listSize <= fHeight;
				listSize += size.y;
			}
			return !deleteLocation ? LOCATION_NONE : LOCATION_DELETE;
		}

		void GetFiles(bool checkText = true){
			folders.clear();
			files.clear();

			// Empty directory
			if ((stat(path.c_str(), &st) == 0) == 0)
				return;

			for (const auto& entry : fs::directory_iterator((string)path)) {
				fs::path ePath = entry.path();
				string sPath = ePath.string();
				const char* p = sPath.c_str();

				// Windows doesn't know how to handle UTF-16 characters in paths for some reason
				#if LINUX
					if (stat(p, &st) == 0) // Is valid

						if (st.st_mode & S_IFDIR){ // Folders
							folders.push_back(Location{GetName(p), p});
							if (checkText) CheckString(folders[folders.size()-1].name);
							else textCache.push_back(folders[folders.size()-1].name);
						
						}else if (st.st_mode & S_IFREG && IsImage(p)){ // Files
							files.push_back(File{GetName(p), p});
							if (checkText) CheckString(files[files.size()-1].name);
							else textCache.push_back(files[files.size()-1].name);
							if (fileTagMap.find(p) == fileTagMap.end())
								fileTagMap.insert({p, false});
						}
				#else
					if (fs::is_directory(p)){ // Folder
						folders.push_back(Location{GetName(p), p});
						if (checkText) CheckString(folders[folders.size()-1].name);
						else textCache.push_back(folders[folders.size()-1].name);
					
					}else if (IsImage(p)){ // File
						files.push_back(File{GetName(p), p});
						if (checkText) CheckString(files[files.size()-1].name);
						else textCache.push_back(files[files.size()-1].name);
						if (fileTagMap.find(p) == fileTagMap.end())
								fileTagMap.insert(p, false);
					}
				#endif
			}
			sort(folders.begin(), folders.end(), SortLocations);
			sort(files.begin(), files.end(), SortFile);

			initialized = true;
		}

		vector<File> GetHidden(){
			vector<File> output;

			if (!expanded)
				output = files;
			
			for (int i = 0; i < folders.size(); i++){
				vector<File> temp = folders[i].GetHidden();
				for (auto file : temp)
					output.push_back(file);
			}
			
			return output;
		}

		vector<File> GetAll(){
			vector<File> output;
			GetFiles(false);
			output = files;
			
			for (int i = 0; i < folders.size(); i++){
				vector<File> files = folders[i].GetAll();
				for (auto file : files)
					output.push_back(file);
			}

			sort(output.begin(), output.end(), SortFile);
			return output;
		}

		void GetListSize(float size){
			listSize = size;
			if (!expanded) return;

			for (auto folder : folders){
				listSize += size;
				if (!folder.expanded) continue;

				listSize += folder.files.size()*size;
			}
			listSize += files.size()*size;
		}

		static bool SortLocations(Location l1, Location l2){
			int max = l1.name.length() <= l2.name.length() ? l1.name.length() : l2.name.length();

			string n1 = SortFormat(Lower(l1.name));	
			string n2 = SortFormat(Lower(l2.name));

			for (int i = 0; i < max; i++){
				if (n1[i] != n2[i]){
					// Because unicode characters are negative, the sorter adds 512 to the characters as an int
					int c1 = n1[i] + 512*(n1[i]<0), 
						c2 = n2[i] + 512*(n2[i]<0);
					return (c1 < c2);
				}
			}
			return n1.length() < n2.length();
		}

		void RefreshTable(){
			GetFiles();

			for (auto& folder : folders)
				if (folder.expanded || folder.initialized)
					folder.RefreshTable();
		}
};

vector<Location> locations;

void UpdateLocationTags(vector<Location> *locations){
	for (auto &location : *locations)
		location.GetFiles(false);
}