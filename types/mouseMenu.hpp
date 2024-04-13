//
// The mouse menu is hard coded for now
//

void AppendToTag(int, int);
int CheckTags(Tag);
int ExistsInTag(Tag, string);


class MenuItem{
	public:
		string name;
		int action = 0;
		Color color = Transparent;
};

class Menu{
	public:
		int t = 0; // Type of menu 0 - default | 1 - tag
		vector<Menu> tagsMenu; // Use vector so it can use the same code without needing to copy the whole class
		vector<MenuItem> items;
		int expand = 0;
		Vector2 position;

		void Draw(int size){
			sShape.Use();
			shape.Draw(position, Vector2{(float)size*8, -1*((float)size * items.size())}, RMMenuBacking, true);

			float y = position.y-size;
			int tagIndex = 0;
			for (auto item : items){
				sShape.Use();
				shape.Draw(Vector2{position.x, y}, Vector2{(float)size*8, (float)size}, item.color, true);

				if (t == 1 && tagIndex != items.size()-1){
					int tagged = CheckTags(tags[tagIndex]);
					if (tagged == 1)
						shape.DrawBox(Vector2{position.x + size/4, y+size/4}, Vector2{(float)size/2, (float)size/2}, White, 2, true);
					else if (tagged == 2)
						shape.Draw(Vector2{position.x + size/4, y+size/4}, Vector2{(float)size/2, (float)size/2}, White, true);
				}

				sImage.Use();
				font.Write(item.name, Vector2{position.x+24, y}, size*3/4, fontColor, true, size*8-48);

				
				// Draw tags
				if (!strcmp("Tag", item.name.data()) && expand){
					sShape.Use();
					shape.Draw(Vector2{position.x, y}, Vector2{(float)size*8, (float)size}, highlight, true);
					tagsMenu[t].Draw(size);
				}
				
				
				//
				// Sub tags
				//

				if (t == 1){
					if (mouse.position.x < position.x+size*8 && mouse.position.x > position.x)
						if (mouse.position.Within(Vector2{position.x, y}, Vector2{(float)size*8, (float)size}))
							expand = tagIndex+1;

					
					
					if (expand == tagIndex+1 && tagIndex != items.size()-1){
						sShape.Use();
						
						// Backing
						shape.Draw(Vector2{position.x+size*8, y+size}, Vector2{(float)size*8, -1*((float)size * (tags[tagIndex].subTags.size()+1))}, RMMenuBacking, true);
						// Highlight
						shape.Draw(Vector2{position.x, y}, Vector2{(float)size*8, (float)size}, highlight, true);

						float y2 = y;
						int subIndex = 0;
						for (auto tag : tags[tagIndex].subTags){
							sShape.Use();
							shape.Draw(Vector2{position.x+size*8, y2}, Vector2{(float)size*8, (float)size}, tag.color, true);

							int tagged = CheckTags(tags[tagIndex].subTags[subIndex]);
							if (tagged == 1)
								shape.DrawBox(Vector2{position.x+size*8 + size/4, y2+size/4}, Vector2{(float)size/2, (float)size/2}, White, 2, true);
							else if (tagged == 2)
								shape.Draw(Vector2{position.x+size*8 + size/4, y2+size/4}, Vector2{(float)size/2, (float)size/2}, White, true);

							sImage.Use();
							font.Write(tag.name, Vector2{position.x+size*8+24, y2}, size*3/4, fontColor, true, size*8-48);
							
							// Sub tag clicked
							if (mouse.position.Within(Vector2{position.x+size*8, y2}, Vector2{(float)size*8, (float)size})){
								sShape.Use();
								shape.Draw(Vector2{position.x+size*8, y2}, Vector2{(float)size*8, (float)size}, highlight, true);

								if (mouse.Click(LM_DOWN) || mouse.Click(RM_DOWN)){
									AppendToTag(tagIndex, subIndex);
								}
							}
							y2-=size;
							subIndex++;
						}

						// 
						// Add new sub tag
						// 

						sImage.Use();
						font.Write("+ Add Sub Tag", Vector2{position.x+size*8+24, y2}, size*3/4, fontColor, true, size*8-48);
							
						// Sub tag clicked
						if (mouse.position.Within(Vector2{position.x+size*8, y2}, Vector2{(float)size*8, (float)size})){
							sShape.Use();
							shape.Draw(Vector2{position.x+size*8, y2}, Vector2{(float)size*8, (float)size}, highlight, true);

							if (mouse.Click(LM_DOWN) || mouse.Click(RM_DOWN)){
								editTag = tagIndex;
								editSub = -2;
								TagWin.Show();
							}
						}
					}
				}

				//
				// Menu options
				//
				
				if (mouse.position.Within(Vector2{position.x, y}, Vector2{(float)size*8, (float)size})){

					// List tags
					if (t == 0){
						if (!strcmp("Tag", item.name.data())){
							if (!expand){
								expand = 1;
								tagsMenu[0].items.clear();
								for (auto tag : tags){
									tagsMenu[0].items.push_back(MenuItem{tag.name, 0, tag.color});
								}
								tagsMenu[0].items.push_back(MenuItem{"+ Add Tag", 0, Transparent});
								tagsMenu[0].position = Vector2 {position.x + fontSize*8, y+size};
							}
							if (mouse.Click(LM_DOWN) || mouse.Click(RM_DOWN)){
								menu = true;
								tagView = true;
							}

						}else if (strcmp("Tag", item.name.data()) && expand)
							expand = 0;
					}

					if (strcmp("Tag", item.name.data()) || t == 1){
						sShape.Use();
						shape.Draw(Vector2{position.x, y}, Vector2{(float)size*8, (float)size}, highlight, true);
					}

					// Main functions
					if (mouse.Click(LM_DOWN) || mouse.Click(RM_DOWN)){

						// Basic functions
						if (t == 0){
							if (selImgs.size()){
								if (!strcmp("Resize", item.name.data())){
									imgScale = true;
									mouse.dragOff = mouse.position;
									mouse.drag = true;
									mouse.state = RM_UP;
									mouse.prevState = RM_DOWN;
								}else if (strcmp("Tag", item.name.data())){
									keyboard.newKey = item.action;
									mouse.state = -1;
								}
							}

						// Tags	
						}else if (t == 1){
							if (tagIndex != items.size()-1)
								AppendToTag(tagIndex, -1);
							else{
								editTag = -1;
								TagWin.Show();
							}
						}
						
						Reset();
						return;
					}
				}
				tagIndex++;
				y-=size;
			}
			if (!mouse.position.Within(position, Vector2{(float)size*8, (float)size}) && (mouse.Click(LM_DOWN) || mouse.Click(RM_DOWN)) && t == 0){
				mouse.state = -1;
				Reset();
			}
		}

		void Reset(){
			drawMouseMenu = false;
			expand = 0;
		}
};

Menu rmMenu; // Right mouse menu

void InitMenu(){
	rmMenu.tagsMenu.push_back(Menu{1});
	rmMenu.items.push_back(MenuItem{"Delete", KEY_DELETE});
	rmMenu.items.push_back(MenuItem{"Flip Horizontally", KEY_H});
	rmMenu.items.push_back(MenuItem{"Flip Vertically", KEY_V});
	rmMenu.items.push_back(MenuItem{"Resize"});
	rmMenu.items.push_back(MenuItem{"Rotate", KEY_R});
	rmMenu.items.push_back(MenuItem{"Tag", KEY_TILDE});
}

void AppendToTag(int tag, int sub){
	
	// Gets wheter to append or remove from the tag
	bool append = false;

	// Regular tag
	if (sub == -1){
		for (auto i : selImgs)
			if (!ExistsInTag(tags[tag], imgs[i].path)){
				append = true;
				break;
			}
	}else
		for (auto i : selImgs)
			if (!ExistsInTag(tags[tag].subTags[sub], imgs[i].path)){
				append = true;
				break;
			}

	if (append){

		// Regular tag images
		for (auto i : selImgs)
			if (!ExistsInTag(tags[tag], imgs[i].path))
				tags[tag].imgs.push_back(File{GetName(imgs[i].path), imgs[i].path});
		
		// Sub tag
		if (sub != -1){
			for (auto i : selImgs)
				if (!ExistsInTag(tags[tag].subTags[sub], imgs[i].path))
					tags[tag].subTags[sub].imgs.push_back(File{GetName(imgs[i].path), imgs[i].path});
		}

	// Removing an image
	}else{
		
		// Regular tag images
		if (sub == -1){
			for (auto i : selImgs){
				int imgIndex = ExistsInTag(tags[tag], imgs[i].path);
				if (imgIndex){
					vector<File>:: iterator img = tags[tag].imgs.begin();
					advance(img, imgIndex-1);
					tags[tag].imgs.erase(img);
				}
			}

		// Sub tag
		}else{
			for (auto i : selImgs){
				int imgIndex = ExistsInTag(tags[tag].subTags[sub], imgs[i].path);
				if (imgIndex){
					vector<File>:: iterator img = tags[tag].subTags[sub].imgs.begin();
					advance(img, imgIndex-1);
					tags[tag].subTags[sub].imgs.erase(img);
				}
			}
		}
	}
	for (int t = 0; t < tags.size(); t++){
		sort(tags[t].imgs.begin(), tags[t].imgs.end(), SortFile);
		if (sub != -1){
			for (int i = 0; i < tags[t].subTags.size(); i++){
				sort(tags[t].subTags[i].imgs.begin(), tags[t].subTags[i].imgs.end(), SortFile);
			}
		}
	}
}

int CheckTags(Tag tag){
	int state = 0;
	bool valid = true;
	for (auto i : selImgs){
		
		bool exists = false;
		for (auto img : tag.imgs){
			if (!strcmp(imgs[i].path.data(), img.path.data())){
				exists = true;
				if (valid)
					state = 2;
				else
					return 1;
				break;
			}
		}

		if (!exists && valid){
			valid = false;
			if (state == 2)
				return 1;
		}
	}
	return state;
}

int ExistsInTag(Tag tag, string s){
	for (int i = 0; i < tag.imgs.size(); i++){
		if (!strcmp(tag.imgs[i].path.data(), s.data()))
			return i+1;
	}
	return 0;
}