class File{
	public:
		string name = "";
		string path = "";
};
bool SortFile(File, File);
void AppendTag(Image);

Button editB = Button{"Edit", Transparent, highlight, White, fontSize/3};
Scrollbar tagScroll = Scrollbar{scrollbarBacking, scrollbarNotch, highlight, Transparent};
Field tagName = Field{"Tag Name", fieldBack, highlight, fontColor, Black, 0, fontSize*.75f}; // Edit tag name

// Edit tag colors
Scrollbar rgb[3] = { 
	Scrollbar{Red, White, highlight, Black, 0, true, 0, 8, 255},
	Scrollbar{Green, White, highlight, Black, 0, true, 0, 8, 255},
	Scrollbar{Blue, White, highlight, Black, 0, true, 0, 8, 255}};

Scrollbar sHue = Scrollbar{Transparent, White, highlight, Transparent, 0, false, 0, 8, 360};

class Tag{
	public:
		string name;
		Color color;
		vector<File> imgs;
		File img;
		vector<Tag> subTags;
		Button addButton = Button{"+", Transparent, highlight, White, fontSize/2};
		int listSize=0;
		bool expand = false;
		bool subTag = false;
		bool addedImg = false;
		bool del = false;

		int Draw(Vector2 position, Vector2 size, int tag = -1, int sub = -1){
			// Tag shape
			shape.Draw(position, size, color, true);

			// Hover
			if (mouse.position.Within(position, size))
				shape.Draw(position, size, highlight, true);

			// Name
			font.Write(name, position, fontSize/2, fontColor, true, size.x-fontSize/2, 1);

			// Add Button
			if (sub == -1 && expand)
				addButton.Draw(position.Add(size.x-fontSize, 0), Vector2{fontSize, fontSize}, false, true);

			// Edit button
			if (expand){
				editB.Draw(position+Vector2{16, 0}, Vector2{fontSize*2, fontSize}, false, true);

				if (mouse.Click() && editB.Hover() && Main.Focus()){
					editTag = tag;
					editSub = sub;
					tagName.Reset();
					tagName.text = name;
					Color HSV = color.ToHSV();
					sHue.scroll = 360-HSV.r;
					csPicker.x = HSV.g/100.0 * csSize.x;
					csPicker.y = HSV.b/100.0 * csSize.y;
					TagWin.Show();
					mouse.prevState = LM_DOWN;
				}
				
			}

			listSize = size.y;
			int subPos = 1;

			if (expand){
				// Sub tags
				for (int i = 0; i < subTags.size(); i++){
					int del = subTags[i].Draw(position.Subtract(-16.0f, size.y*subPos), size.Subtract(16, 0), tag, i);
					if (del == 1){
						subTags.erase(subTags.begin() + i);
						return 0;
					}else if (del == 2){
						bool exists = false;
						for (auto img : imgs){
							if (img.path == previewImg.path){
								exists = true;
								break;
							}
						}
						if (!exists){
							imgs.push_back(File{GetName(previewImg.path), previewImg.path});
							sort(imgs.begin(), imgs.end(), SortFile);
						}
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
					// Image Tagged Indicator
					if (previewImg.img.loaded && previewImg.path == imgs[i].path){
						shape.DrawCircle(position + Vector2{8, size.y/2}, fontSize-4, 0, White, true);
						shape.Draw(position.Subtract(0.0f, size.y*subPos), size, highlight, true);
					}
					// Prevents drawing under the window
					if (position.y - size.y*(i+1) > 0){
						font.Write(imgs[i].name, position.Subtract(-4.0f, size.y*subPos), fontSize/2, fontColor, true, size.x-fontSize);

						// Hover
						if (mouse.position.Within(position.Subtract(0.0f, size.y*subPos), size) && Main.Focus()){
							shape.Draw(position.Subtract(0.0f, size.y*subPos), size, highlight, true);

							// Preview Image
							if (mouse.Click()){
								if (imgs[i].path.substr(imgs[i].path.length()-4) != "webp")
									previewImg.img.LoadImage(imgs[i].path);
								else
									previewImg.img = LoadWebp(imgs[i].path);
								previewImg.path = imgs[i].path;
								previewImg.GetImgSize();
								previewImg.ScaleImg(Vector2{(float)Width/5, (float)Width/5});
								previewImg.path = imgs[i].path;
							}
						}
						subPos++;
					}

					listSize+= size.y;
				}
			}else{
				// Image Tagged Indicator
				for (int i = 0; i < imgs.size(); i++)
					if (previewImg.img.loaded && previewImg.path == imgs[i].path.data())
						shape.DrawCircle(position + Vector2{8, size.y/2}, fontSize-4, 0, White, true);
					
				
			}

			if (!Main.Focus())
				return 0;

			if (mouse.position.Within(position, size)){
				if (mouse.Click()){
					if (!addButton.Hover() || sub > -1)
						expand = !expand;
					else{
						editTag = tag;
						editSub = -2;
						Color HSV = color.ToHSV();
						sHue.scroll = 360-HSV.r;
						csPicker.x = HSV.g/100.0 * csSize.x;
						csPicker.y = HSV.b/100.0 * csSize.y;
						tagName.Reset();
						TagWin.Show();
					}
					mouse.prevState = mouse.state;

				}else if (mouse.Click(RM_DOWN)){
					// Editing a tag
					if (!previewImg.img.loaded){
						editTag = tag;
						editSub = sub;
						tagName.Reset();
						tagName.text = name;
						Color HSV = color.ToHSV();
						sHue.scroll = 360-HSV.r;
						csPicker.x = HSV.g/100.0 * csSize.x;
						csPicker.y = HSV.b/100.0 * csSize.y;
						TagWin.Show();
					
					// Editing tag images
					}else if (previewImg.img.loaded){
						bool exists = false;
						for (int i = 0; i < imgs.size() && !exists; i++){
							if (imgs[i].path == previewImg.path){
								exists = true;
								imgs.erase(imgs.begin() + i);

								// Sub tag check
								if (sub < 0){
									printf("1\n");
									CheckSubTags();
								}
								
							}
						}

						// Appending a new image
						if (!exists){
							imgs.push_back(File{GetName(previewImg.path), previewImg.path});
							sort(imgs.begin(), imgs.end(), SortFile);

							// Append to main tag also
							if (sub > -1)
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
					if (subTags[s].imgs[i].path == previewImg.path){
						subTags[s].imgs.erase(subTags[s].imgs.begin() + i);
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
vector<Tag> tags;