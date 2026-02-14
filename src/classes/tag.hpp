extern Button deleteTag_Button;
extern Color newTagColor;
extern ColorSelector newTag_ColorSelector;
extern Field newTagName_Field;

void UpdateFileTagMap(string);

enum Tag_Options{
	TAG_NONE,
	TAG_DELETE,
	TAG_FILE_ADDED
};

class Tag;

Tag* editTag = nullptr;
Tag* parentTag = nullptr;

Button tagAdd_Button = Button{"+", Transparent, highlightColor, White, SMALL_FONT_SIZE};
Button tagEdit_Button = Button{"Edit", Transparent, highlightColor, White, TINY_FONT_SIZE};

class Tag{
	public:
		string name;
		Color color;
		bool isSubTag = false;
		vector<File> files;
		vector<Tag> subTags;
		int listSize = 0;
		bool expanded = false;

		int Draw(Vector2 position, Vector2 size){
			listSize = size.y;

			// Prevent drawing under the window and return the list size
			if (position.y - size.y < -size.y*3){
				GetListSize(size.y);
				return TAG_NONE;
			}

			bool clicked = false;
			bool hovered = mouse.Within(position, size);
			bool draw = position.y - size.y*2 <= fHeight; // Prevent drawing above the window
			int tagOption = TAG_NONE;

			if (draw){
				// Tag background color
				shape.Draw(position, size, color, POSITION_FIXED);

				// Tag is hovered
				if (CurrentWindow->focused && hovered && !tagAdd_Button.hovered && !tagEdit_Button.hovered){
					if (keyboard.newKey == KEY_DELETE)
						return TAG_DELETE;
					clicked = mouse.Click();
					shape.Draw(position, size, highlightColor, POSITION_FIXED);
				}
				// Name
				font.Write(name, position+Vector2{PADDING*2, 0}, SMALL_FONT_SIZE, fontColor, POSITION_FIXED, size.x-PADDING*3.5, ALIGN_CENTER);

				if (!expanded && clicked){
					expanded = true;
					clicked = false;
				}

				// Add/remove image from tag
				if (mouse.Click(RM_DOWN) && hovered){
					if (!previewImg.loaded){
						EditTag();

					// Delete image
					}else if (!DeleteFile(previewImg.path)){

						// Add image
						AddFile(previewImg.path);
						if (isSubTag) tagOption = TAG_FILE_ADDED;
					}
				}
			}

			

			// Draw indicator that the preview image has this tag
			if (!expanded){

				listSize = size.y;

				// No preview image or above the window
				if (!previewImg.loaded || !draw)
					return tagOption;

				for (int i = 0; i < files.size(); i++)
					if (previewImg.path == files[i].path.data())
						shape.DrawCircle(position + Vector2{size.y/2, size.y/2}, size.y-size.y/8, BORDER_NONE, White, POSITION_FIXED);
				
				return tagOption;
			}

			if (draw){
				// Draw buttons
				if (!isSubTag)
					tagAdd_Button.Draw(position, {PADDING, size.y}, ALIGN_CENTER);

				// Add sub tag
				if (tagAdd_Button.pressed && !editTag && clicked){
					editTag = nullptr;
					parentTag = this;
					newTag_ColorSelector.SetColor(Red);
					deleteTag_Button.text = "Cancel";
					newTagName_Field.text = "";
					TagWin.Show();
				}

				tagEdit_Button.Draw(position+Vector2{size.x-PADDING*2, 0}, Vector2{PADDING*2, size.y}, ALIGN_CENTER);

				// Edit tag
				if (clicked && tagEdit_Button.pressed  && !showTutorial)
					EditTag();

				// Show files
				if (clicked && !tagEdit_Button.pressed && !tagAdd_Button.pressed)
					expanded = false;
			}

			// Draw subtags
			for (int i = 0; i < subTags.size(); i++){
				int tagOption = subTags[i].Draw(position - Vector2{-PADDING, (float)listSize}, size - Vector2{PADDING, 0});
				if (tagOption == TAG_DELETE){
					Warn(WARNING_SUBTAG, (void*)&subTags[i], (void*)this);

				}else if (tagOption == TAG_FILE_ADDED && FileExists(previewImg.path) == -1  && !showTutorial){
					files.push_back(File{GetName(previewImg.path), previewImg.path});
					sort(files.begin(), files.end(), SortFile);
				}
				listSize += subTags[i].listSize;
			}

			draw = position.y - size.y - listSize <= fHeight;

			// Draw files
			for (int i = 0; i < files.size(); i++){
				if (!draw){
					listSize += size.y;
					draw = position.y - size.y - listSize <= fHeight;
					continue;
				}else if (position.y - listSize < -size.y){
					listSize += size.y * (files.size()-i);
					break;
				}

				// Selected file is tagged
				if (previewImg.path == files[i].path){
					shape.Draw(position - Vector2{0, (float)listSize}, size, highlightColor, POSITION_FIXED);
					if (isSubTag)
						shape.DrawCircle(position + Vector2{size.y/2, size.y/2}, size.y-size.y/8, BORDER_NONE, White, POSITION_FIXED);
				}
					
				

				// Name
				font.Write(files[i].name, position - Vector2{-PADDING, (float)listSize}, SMALL_FONT_SIZE, fontColor, POSITION_FIXED, size.x-PADDING);

				// Mouse events
				if (mouse.Within(position - Vector2{0, (float)listSize}, size) && CurrentWindow->focused){
					shape.Draw(position - Vector2{0, (float)listSize}, size, highlightColor, POSITION_FIXED);
					
					// Set as preview image
					if (mouse.Click()  && !showTutorial)
						previewImg.LoadPreview(files[i].path);
				}
				listSize += size.y;
			}
			
			return tagOption;
		}

		void AddFile(string path){
			files.push_back(File{GetName(path), path});
			
			if (fileTagMap.find(path) == fileTagMap.end())
				fileTagMap.insert({path, true});
			else
				fileTagMap[path] = true;

			sort(files.begin(), files.end(), SortFile);
		}

		bool DeleteFile(string path){
			int index = FileExists(path);
			if (index == -1) return false;

			files.erase(files.begin() + index);

			if (isSubTag)
				return true;

			for (auto& subTag : subTags){
				for (int f = 0; f < subTag.files.size(); f++){
					if (subTag.files[f].path == path){
						subTag.files.erase(subTag.files.begin() + f);
						break;
					}
				}
			}

			UpdateFileTagMap(path);

			return true;
		}

		void EditTag(){
			editTag = this;
			deleteTag_Button.text = "Delete";
			newTag_ColorSelector.SetColor(color);
			newTagColor = color;
			newTagName_Field.text = name;
			TagWin.Show();
		}

		int GetListSize(int size){
			if (!expanded){
				listSize = size;
				return listSize;
			}

			listSize = 0;
			for (auto subTag : subTags)
				listSize += (subTag.expanded ? subTag.files.size() : 0) + 1;
			
			listSize += files.size();
			listSize *= size;
			return listSize;
		}

		int FileExists(string path){
			for (int i = 0; i < files.size(); i++)
				if (files[i].path == path)
					return i;

			return -1;
		}
};

vector<Tag> tags;


void UpdateFileTagMap(string path){
	for (auto tag : tags)
		for (auto file : tag.files)
			if (file.path == path)
				return;
	
	fileTagMap[path] = false;
}