class File{
	public:
		string name = "";
		string path = "";
};
bool SortFile(File, File);
void AppendTag(Image);

class Tag{
	public:
		string name;
		Color color;
		vector<File> imgs;
		File img;
		vector<Tag> subTags;
		Button addButton = Button{"Add", Transparent, highlight};
		int listSize=0;
		bool expand = false;
		bool subTag = false;
		bool addedImg = false;
		bool del = false;

		int Draw(Vector2 position, Vector2 size, int tag = -1, int sub = -1){
			// Tag shape
			sShape.Use();
			shape.Draw(position, size, color, true);

			// Hover
			if (mouse.position.Within(position, size))
				shape.Draw(position, size, highlight, true);

			// Name
			sImage.Use();
			font.Write(name, position, fontSize*3/4, fontColor, true, size.x-fontSize, 1);

			// Add Button
			if (sub == -1 && expand){
				add.position = position.Add(size.x-fontSize, 0);
				add.size = Vector2{fontSize, fontSize};
				add.Draw();
			}

			// Edit button
			if (expand){
				editB.position = position;
				editB.size = Vector2{fontSize, fontSize};
				editB.Draw();

				if (mouse.Click(LM_DOWN) && editB.Hover()){
					editTag = tag;
					editSub = sub;
					tagName.text = name;
					rgb[0].scroll = color.r*255;
					rgb[1].scroll = color.g*255;
					rgb[2].scroll = color.b*255;
					TagWin.Show();
				}
				
			}

			listSize = size.y;
			int subPos = 1;

			if (expand){
				// Sub tags
				for (int i = 0; i < subTags.size(); i++){
					int del = subTags[i].Draw(position.Subtract(-16.0f, size.y*subPos), size.Subtract(16, 0), tag, i);
					if (del == 1){
						vector<Tag>:: iterator t = subTags.begin();
						advance(t, i);
						subTags.erase(t);
						return 0;
					}else if (del == 2){
						imgs.push_back(File{GetName(previewPath), previewPath});
						sort(imgs.begin(), imgs.end(), SortFile);
					}

					// Expanded
					if (subTags[i].expand){
						subPos += subTags[i].imgs.size();
						listSize += size.y*subTags[i].imgs.size();
					}
					subPos++;
					listSize += size.y;
				}

				// Images
				for (int i = 0; i < imgs.size(); i++){
					// Image Tagged Indicator (ITI)
					if (previewImg.loaded && !strcmp(previewPath.data(), imgs[i].path.data())){
						sShape.Use();
						shape.Draw(position.Add(size.y+size.y/2, size.y/4), Vector2{size.y/2, size.y/2}, White, true);
						shape.Draw(position.Subtract(0.0f, size.y*subPos), size, highlight, true);
						sImage.Use();
					}
					// Prevents drawing under the window
					if (position.y - size.y*(i+1) > 0){
						font.Write(imgs[i].name, position.Subtract(-4.0f, size.y*subPos), fontSize*5/8, fontColor, true, size.x-fontSize);

						// Hover
						if (mouse.position.Within(position.Subtract(0.0f, size.y*subPos), size)){
							sShape.Use();
							shape.Draw(position.Subtract(0.0f, size.y*subPos), size, highlight, true);
							sImage.Use();

							// Preview Image
							if (mouse.Click(LM_DOWN)){
								previewImg.LoadImage((char*)imgs[i].path.data());
								previewImg.ScaleImg(Vector2{(float)Width/5, (float)Width/5});
								previewPath = imgs[i].path;
							}
						}
						subPos++;
					}

					listSize+= size.y;
				}
			}else{
				// Image Tagged Indicator (ITI)
				sShape.Use();
				for (int i = 0; i < imgs.size(); i++)
					if (previewImg.loaded && !strcmp(previewPath.data(), imgs[i].path.data()))
						shape.Draw(position.Add(size.y+size.y/2, size.y/4), Vector2{size.y/2, size.y/2}, White, true);
					
				
			}

			sImage.Use();

			if (mouse.position.Within(position, size)){
				if (mouse.Click(LM_DOWN)){
					if (!add.Hover() || subTag)
						expand = !expand;
					else{
						editTag = tag;
						editSub = -2;
						rgb[0].scroll = 0;
						rgb[1].scroll = 0;
						rgb[2].scroll = 0;
						tagName.text = "";
						TagWin.Show();
					}
					mouse.prevState = mouse.state;

				}else if (mouse.Click(RM_DOWN)){
					// Editing a tag
					if (!previewImg.loaded && !TagWin.visible){
						editTag = tag;
						editSub = sub;
						tagName.text = name;
						rgb[0].scroll = color.r*255;
						rgb[1].scroll = color.g*255;
						rgb[2].scroll = color.b*255;
						TagWin.Show();
					
					// Editing tag images
					}else if (previewImg.loaded){
						bool exists = false;
						for (int i = 0; i < imgs.size() && !exists; i++){
							if (!strcmp(imgs[i].path.data(), previewPath.data())){
								exists = true;
								vector<File>:: iterator f = imgs.begin();
								advance(f, i);
								imgs.erase(f);

								// Sub tag check
								if (!subTag)
									CheckSubTags();
								
							}
						}

						// Appending a new image
						if (!exists){
							imgs.push_back(File{GetName(previewPath), previewPath});
							sort(imgs.begin(), imgs.end(), SortFile);

							// Append to main tag also
							if (subTag)
								return 2;
						}
					}

				}else if (keyboard.newKey == KEY_DELETE){
					keyboard.newKey = -1;
					return 1;
				}
			}
			return 0;
		}

		void CheckSubTags(){
			for (int s = 0; s < subTags.size(); s++){
				for (int i = 0; i < subTags[s].imgs.size(); i++){
					if (!strcmp(subTags[s].imgs[i].path.data(), previewPath.data())){
						vector<File>:: iterator f = subTags[s].imgs.begin();
						advance(f, i);
						subTags[s].imgs.erase(f);
						break;
					}
				}
			}
		}

		int GetSize(Vector2 size){
			listSize = size.y;
			if (expand){
				// Subtags
				for (int i = 0; i < subTags.size(); i++){
					listSize += size.y;
					if (subTags[i].expand)
						listSize += size.y*subTags[i].imgs.size();
				}
				// Images
				listSize+= size.y*imgs.size();
			}
			return listSize;
		}
};
