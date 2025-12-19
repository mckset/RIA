//
// The mouse menu is hard coded for now
//

void AppendToTag(int, int);
int CheckTag(Tag);
int ExistsInTag(Tag, string);

enum Menu_Type{
	MENU_DEFAULT,
	MENU_TAGS,
	MENU_SUBTAGS
};

enum Menu_Actions{
	MENU_NONE,
	MENU_HOVERED,
	MENU_CLICKED
};

enum Tagged_Types{
	TAGGED_NONE,
	TAGGED_PARTIAL,
	TAGGED_ALL
};

int menuSubTagIndex = -1;
float subTagY = 0;

class MenuItem{
	public:
		string name;
		int action = 0;
		Color color = Transparent;

		bool Draw(Vector2 position, Vector2 size){
			shape.Draw(position, size, color, POSITION_FIXED);
			font.Write(name, Vector2{position.x + MOUSE_MENU_PADDING, position.y}, SMALL_FONT_SIZE, fontColor, POSITION_FIXED, MOUSE_MENU_SIZE-MOUSE_MENU_PADDING*2);

			// Input
			if (mouse.Within(position, size)){
				if (name != "Tags") shape.Draw(position, size, highlightColor, POSITION_FIXED);

				if (mouse.Click()){
					if (action){
						keyboard.newKey = action;
						showMouseMenu = false;
					}else if (name == "Filter" || name == "Disable Filter"){
						for (auto i : selectedImgs){
							glBindTexture(GL_TEXTURE_2D, imgs[i].img.ID);
							int size = (int)(imgs[i].img.width * imgs[i].img.height * 4);
							unsigned char* pixels = (unsigned char*) malloc(sizeof(unsigned char) * size);
							glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
							imgs[i].img.loaded = false;
							imgs[i].img.SetTexture(pixels, name == "Disable Filter");
						}
					}else if (name == "Resize"){
						scaleImages = true;
					}
					mouse.state = INPUT_NULL;
				}
				return true;
			}
			return false;
		}

		int DrawTag(Vector2 position, Vector2 size, int tagType = TAGGED_NONE){
			shape.Draw(position, size, color, POSITION_FIXED);
			font.Write(name, {position.x + MOUSE_MENU_PADDING*2, position.y}, SMALL_FONT_SIZE, fontColor, POSITION_FIXED, MOUSE_MENU_SIZE-MOUSE_MENU_PADDING*3);

			if (tagType == TAGGED_PARTIAL){
				shape.DrawCircle({position.x+MOUSE_MENU_PADDING, position.y+MOUSE_MENU_PADDING*1.5f}, FONT_SIZE, MOUSE_MENU_PADDING, White, POSITION_FIXED);
			}else if (tagType == TAGGED_ALL){
				shape.DrawCircle({position.x+MOUSE_MENU_PADDING, position.y+MOUSE_MENU_PADDING*1.5f}, FONT_SIZE, BORDER_NONE, White, POSITION_FIXED);
			}

			// Input
			if (mouse.Within(position, size)){
				if (mouse.Click() || mouse.Click(RM_DOWN))
					return MENU_CLICKED;
				return MENU_HOVERED;
			}
			return MENU_NONE;
		}
};

class Menu{
	public:
		int type = MENU_DEFAULT; // Type of menu 0 - default | 1 - tag
		Menu* tagsMenu;
		vector<MenuItem> items;
		bool expanded = false;
		Vector2 position;

		void Draw(){
			Vector2 size = Vector2{MOUSE_MENU_SIZE, (-FONT_SIZE * items.size())};

			// Background
			shape.Draw(position, size, menuBackgroundColor, POSITION_FIXED);

			float y = position.y-FONT_SIZE;
			bool shouldReset = mouse.Click() || mouse.Click(RM_DOWN) || mouse.Click(MM_DOWN);

			for (auto item : items){
				bool hovered = item.Draw({position.x, y}, {size.x, FONT_SIZE});
				if (hovered) expanded = item.name == "Tags";
				y-=FONT_SIZE;
			}

			if (expanded){
				shape.Draw({position.x, y+FONT_SIZE*2}, {size.x, -FONT_SIZE}, highlightColor, POSITION_FIXED);
				tagsMenu->position = {position.x + MOUSE_MENU_SIZE, y+FONT_SIZE*2};
				shouldReset = tagsMenu->DrawTags() || shouldReset;
			}
			

			if (shouldReset) Reset();
		}

		bool DrawTags(){
			if (!items.size()){
				for (auto tag : tags)
					items.push_back(MenuItem{tag.name, 0, tag.color});
				items.push_back(MenuItem{"+ Add Tag", 0, Transparent});
				expanded = false;
			}

			Vector2 size = Vector2{MOUSE_MENU_SIZE, (-FONT_SIZE * items.size())};

			// Background
			shape.Draw(position, size, menuBackgroundColor, POSITION_FIXED);

			float y = position.y-FONT_SIZE;
			bool shouldReset = false;

			for (int i = 0; i < items.size(); i++){
				int tagType = i < items.size()-1 ? CheckTag(tags[i]) : TAGGED_NONE;
				int action = items[i].DrawTag({position.x, y}, {size.x, FONT_SIZE}, tagType);
				
				if (action == MENU_CLICKED){

					// New tag
					if (i == items.size()-1){
						editTag = nullptr;
						parentTag = nullptr;
						newTag_ColorSelector.SetColor(Red);
						deleteTag_Button.text = "Cancel";
						newTagName_Field.text = "";
						TagWin.Show();
						return true;
					}

					// Remove images from tag
					if (tagType == TAGGED_ALL)
						for (auto img : selectedImgs)
							tags[i].DeleteFile(imgs[img].path);
					
					// Add images to tag
					else
						if (tagType == TAGGED_NONE)
							for (auto img : selectedImgs)
								tags[i].AddFile(imgs[img].path);
						else{
							for (auto img : selectedImgs){
								int index = tags[i].FileExists(imgs[img].path);
								if (index != -1) continue;
								tags[i].AddFile(imgs[img].path);
							}
						}

				}else if (action == MENU_HOVERED){
					expanded = true;
					subTagY = y+FONT_SIZE;
					menuSubTagIndex = i;
				}

				if (menuSubTagIndex == i)
					shape.Draw({position.x, y}, {size.x, FONT_SIZE}, highlightColor, POSITION_FIXED);

				y-=FONT_SIZE;
			}

			if (expanded && menuSubTagIndex < items.size()-1){
				tagsMenu->position = {position.x + MOUSE_MENU_SIZE, subTagY};
				shouldReset = tagsMenu->DrawSubTags(menuSubTagIndex);
			}

			return shouldReset;
		}

		bool DrawSubTags(int tagIndex = 0){

			items.clear();
			for (auto subTag : tags[tagIndex].subTags)
				items.push_back(MenuItem{subTag.name, 0, subTag.color});

			items.push_back(MenuItem{"+ Add Sub Tag", 0, Transparent});

			Vector2 size = Vector2{MOUSE_MENU_SIZE, (-FONT_SIZE * items.size())};

			// Background
			shape.Draw(position, size, menuBackgroundColor, POSITION_FIXED);

			float y = position.y-FONT_SIZE;
			bool shouldReset = mouse.Click() || mouse.Click(RM_DOWN) || mouse.Click(MM_DOWN);

			for (int i = 0; i < items.size(); i++){
				int tagType = i < items.size()-1 ? CheckTag(tags[tagIndex].subTags[i]) : TAGGED_NONE;
				int action = items[i].DrawTag({position.x, y}, {size.x, FONT_SIZE}, tagType);

				if (action == MENU_HOVERED)
					shape.Draw({position.x, y}, {size.x, FONT_SIZE}, highlightColor, POSITION_FIXED);

				// Input
				else if (action == MENU_CLICKED){
					
					// Add sub tag
					if (i == items.size()-1){
						parentTag = &tags[tagIndex];
						deleteTag_Button.text = "Cancel";
						TagWin.Show();
						return true;
					
					// Remove images from tag
					}else if (tagType == TAGGED_ALL){
						for (auto img: selectedImgs)
							tags[tagIndex].subTags[i].DeleteFile(imgs[img].path);
					
					}else if (tagType == TAGGED_PARTIAL){
						for (auto img: selectedImgs){
							int index = tags[tagIndex].subTags[i].FileExists(imgs[img].path);
							if (index != -1) continue;
							
							int parentIndex = tags[tagIndex].FileExists(imgs[img].path);
							if (parentIndex == -1)
								tags[tagIndex].AddFile(imgs[img].path);

							tags[tagIndex].subTags[i].AddFile(imgs[img].path);
						}
					}else{
						for (auto img: selectedImgs){
							int parentIndex = tags[tagIndex].FileExists(imgs[img].path);
							if (parentIndex == -1)
								tags[tagIndex].AddFile(imgs[img].path);

							tags[tagIndex].subTags[i].AddFile(imgs[img].path);
						}
					}
				}

				y-=FONT_SIZE;
			}

			return false;
		}

		void Reset(){
			tagsMenu->items.clear();
			showMouseMenu = false;
			expanded = 0;
			menuSubTagIndex = -1;
			mouse.state = INPUT_NULL;
		}
};

Menu mouseMenu;

void InitMenu(){
	mouseMenu.tagsMenu = new Menu{MENU_TAGS};
	mouseMenu.tagsMenu->tagsMenu = new Menu{MENU_TAGS};
	mouseMenu.items = {MenuItem{"Delete", KEY_DELETE},
		MenuItem{"Flip Horizontally", KEY_H},
		MenuItem{"Flip Vertically", KEY_V},
		MenuItem{"Resize"},
		MenuItem{"Rotate", KEY_R},
		MenuItem{"Filter"},
		MenuItem{"Disable Filter"},
		MenuItem{"Tags", KEY_TILDE}};
}

int CheckTag(Tag tag){
	int worse = TAGGED_NONE;
	int best = TAGGED_ALL;
	for (auto i : selectedImgs){
		
		bool hasImg = tag.FileExists(imgs[i].path) > -1;

		// Does not have an image
		if (!hasImg){
			if (best == TAGGED_ALL)
				best = TAGGED_PARTIAL;
		
		// Has an image
		}else if (worse == TAGGED_NONE)
			worse = TAGGED_PARTIAL;
	}

	if (best == TAGGED_ALL && worse == TAGGED_PARTIAL)
		return best;
	else if (best == TAGGED_PARTIAL && worse == TAGGED_PARTIAL)
		return best;

	return TAGGED_NONE;
}