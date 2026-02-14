const float boardNamePlate = FONT_SIZE*2;
bool newBoard = false;

Field* boardName_Field = nullptr;

class Board{
	public:
		string name = "Default";
		string path = "";
		Image img;
		Vector2 position = {0,0};
		Vector2 size = {0,0};
		bool loadFailed = false;

		Button delete_Button{"x", Transparent, highlightColor, fontColor, FONT_SIZE};
		Field name_Field{"Default", Transparent, highlightColor, fontColor, FONT_SIZE};

		void Draw(Vector2 p, Vector2 s){
			if (!img.loaded && !loadFailed)
				GetBoardScreenshot();

			position = p;
			size = s;
			shape.Draw(position, size, locationHeadingColor, POSITION_FIXED);
			name_Field.Draw(position, {size.x, FONT_SIZE*2}, ALIGN_CENTER);

			if (name_Field.active) boardName_Field = &name_Field;

			if (name_Field.submitted && name != name_Field.text){
				if (stat((::path+"boards/"+name_Field.text+".brd").c_str(), &st) == 0){
					statusText = name_Field.text + " already exists";
					statusTextTimer = STATUS_TIME;
					name_Field.text = "";
				}else{
					fs::rename(::path+"boards/"+name+".brd", ::path+"boards/"+name_Field.text+".brd");
					name = name_Field.text;
					FindBoards();
				}
			}
			//font.Write(name, position+Vector2{BOARD_PADDING, 0}, FONT_SIZE, fontColor, POSITION_FIXED, Main.sideMenuWidth-PADDING, ALIGN_CENTER);
			s.y -= boardNamePlate;

			float ratio = (img.width > img.height) ? s.x/img.width : s.y/img.height;
			img.Draw(p+s/2-Vector2{(float)img.width, (float)img.height}*ratio/2+Vector2{BOARD_PADDING, BOARD_PADDING+boardNamePlate}, Vector2{(float)img.width, (float)img.height}*ratio-BOARD_PADDING*2, White, 0, POSITION_FIXED);

			delete_Button.Draw(position + size - FONT_SIZE*2 - BOARD_BUTTON_PADDING, {FONT_SIZE*2, FONT_SIZE*2}, ALIGN_CENTER);
			if (delete_Button.pressed && !showTutorial){
				Warn(WARNING_BOARD, (void*)this, nullptr);
			}

			if (mouse.Within(position, size) && CurrentWindow->focused && !delete_Button.hovered && !name_Field.active && !name_Field.hovered){
				shape.Draw(position, size, highlightColor, true);

				// Load board
                if (mouse.Click() && !showTutorial){
                    board = name + ".brd";
					if (loadThread || imageBoardThread){
						closeThread = true;

						if (loadThread){
							loadThread->join();
							free(loadThread);
							loadThread = nullptr;
							for (int i = 0; i < textCache.size(); i++)
								CheckString(textCache[i]);
							textCache.clear();

						}else{
							imageBoardThread->join();
							free(imageBoardThread);
						}
						closeThread = false;
					}
					loadedImages.clear();
                    imageBoardThread = new thread(LoadImageBoard);
					
                }else if (keyboard.newKey == KEY_DELETE){
					Warn(WARNING_BOARD, (void*)this, nullptr);
				}

			// Selected board
            }else if (board == name+".brd"){
				shape.Draw(position, {size.x, FONT_SIZE*2}, highlightColor, true);
			}


		}

		// Reads screenshot data from an image board save
		void GetBoardScreenshot(){

			// Screenshot gets corrupted when this flag is not on
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

			ifstream f;

			// Make sure the board exists
			f.open(path, ios::in | ios::binary);
			if (!f.good()){
				if (DEBUG) printf("[Loading] Unable to load image board\n");
				loadFailed = true;
			}

			// 4 bytes width, 4 bytes height, width*height*3 image data
			f.read(reinterpret_cast<char*>(&img.width), 4);
			f.read(reinterpret_cast<char*>(&img.height), 4);
			unsigned char *imgData = (unsigned char*)malloc(img.width*img.height*3);
			f.read(reinterpret_cast<char*>(imgData), img.width*img.height*3);
			SetTextureRBG(imgData, &img);
			img.loaded = true;
		}
};
vector<Board> imgBoards;