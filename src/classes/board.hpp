const float boardNamePlate = FONT_SIZE*2;
bool newBoard = false;

class Board{
	public:
		string name = "Default";
		string path = "";
		Image img;
		Vector2 position = {0,0};
		Vector2 size = {0,0};
		bool loadFailed = false;

		int Draw(Vector2 p, Vector2 s){
			if (!img.loaded && !loadFailed)
				GetBoardScreenshot();

			position = p;
			size = s;
			shape.Draw(position, size, locationHeadingColor, POSITION_FIXED);
			int l = font.Write(name, position+BOARD_PADDING, FONT_SIZE, fontColor, POSITION_FIXED, sideMenuWidth-PADDING, ALIGN_CENTER).x;
			s.y -= boardNamePlate;

			float ratio = (img.width > img.height) ? s.x/img.width : s.y/img.height;
			img.Draw(p+s/2-Vector2{(float)img.width, (float)img.height}*ratio/2+Vector2{BOARD_PADDING, BOARD_PADDING+boardNamePlate}, Vector2{(float)img.width, (float)img.height}*ratio-BOARD_PADDING*2, White, 0, POSITION_FIXED);

			if (mouse.Within(position, size) && CurrentWindow->focused){
				shape.Draw(position, size, highlightColor, true);

                if (mouse.Click()){
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
					string s = "boards";
					s += slash[0];
					s += name + ".brd";
					filesystem::remove(s.c_str());
					return 0;
				}
            }
			return 1;
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