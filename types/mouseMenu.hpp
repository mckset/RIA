//
// The mouse menu is hard coded for now
//

int CheckTags(Tag);

class MenuItem{
	public:
		string name;
		int action = 0;
		Color color = Transparent;
};

class Menu{
	public:
		int t = 0; // Type of menu 0 - 
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

				if (t == 1){
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
					
					if (expand == tagIndex + 1){
						sShape.Use();
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
							
							if (mouse.position.Within(Vector2{position.x+size*8, y2}, Vector2{(float)size*8, (float)size})){
								sShape.Use();
								shape.Draw(Vector2{position.x+size*8, y2}, Vector2{(float)size*8, (float)size}, highlight, true);
							}
							
							y2-=size;
							subIndex++;
						}
					}


				//
				// Menu options
				//
				
				}else if (mouse.position.Within(Vector2{position.x, y}, Vector2{(float)size*8, (float)size})){

					// List tags
					if (t == 0){
						if (!strcmp("Tag", item.name.data()) && !expand){
							expand = 1;
							tagsMenu[0].items.clear();
							for (auto tag : tags){
								tagsMenu[0].items.push_back(MenuItem{tag.name, 0, tag.color});
							}
							tagsMenu[0].position = Vector2 {position.x + fontSize*8, y+size};
						}else if (strcmp("Tag", item.name.data()))
							expand = 0;

					}

					sShape.Use();
					shape.Draw(Vector2{position.x, y}, Vector2{(float)size*8, (float)size}, highlight, true);


					// Main functions
					if (mouse.Click(LM_DOWN) || mouse.Click(RM_DOWN)){

						// Basic functions
						if (t == 0){
							if (!strcmp("Tags", item.name.data())){
								keyboard.newKey = item.action;
								mouse.state = -1;
							}else if (selImgs.size()){
								if (!strcmp("Resize", item.name.data())){
									imgScale = true;
									mouse.dragOff = mouse.position;
									mouse.drag = true;
									mouse.state = RM_UP;
									mouse.prevState = RM_DOWN;
								}else{
									keyboard.newKey = item.action;
									mouse.state = -1;
								}
							}

						}
						
						Reset();
						return;
					}
				}
				tagIndex++;
				y-=size;
			}
			if (!mouse.position.Within(position, Vector2{(float)size*8, (float)size}) && (mouse.Click(LM_DOWN) || mouse.Click(RM_DOWN))){
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