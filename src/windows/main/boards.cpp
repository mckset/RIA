unsigned char *boardScreenshot;

void DrawBoards(){
	float y = fHeight + boardScroll.scroll;
	shape.Draw({fWidth, y}, {-(menuWidth), -fontSize*2}, menuBackground, true);
	font.Write("Boards", {fWidth-menuWidth, y-fontSize*2}, fontSize, fontColor, true, menuWidth, 1);
	
	addBoard.Draw({fWidth-fontSize*2, y-fontSize*2}, {fontSize*2, fontSize*2}, false, true);

	// Add new board
	if (addBoard.Hover() && mouse.Click()){
		newBoard = true;
		fCurrentBoard.Reset();
		fCurrentBoard.active = true;
		mouse.prevState = LM_DOWN;
	}
	y -= fontSize;
	shape.Draw({fWidth-menuWidth, y-fontSize*2}, {menuWidth, fontSize}, locationHeading, true);
	font.Write("Current: ", {fWidth-menuWidth+8, y-fontSize*2}, fontSize/2, fontColor, true, menuWidth, 0);
	fCurrentBoard.Draw({fWidth-menuWidth+fontSize*4+8, y-fontSize*2}, {menuWidth, fontSize});

	if (mouse.position.Within({fWidth-menuWidth+4, y-fontSize*2}, {menuWidth, fontSize}) || fCurrentBoard.active){
		shape.Draw({fWidth-menuWidth, y-fontSize*2}, {menuWidth, fontSize}, highlight, true);
		if (mouse.Click()){
			fCurrentBoard.CheckClick();
			if (!fCurrentBoard.active)
				fCurrentBoard.text = board.substr(0,board.length()-4);
		}
	}

	if (fCurrentBoard.active){
		fCurrentBoard.UpdateText();
		if (keyboard.newKey == KEY_ESCAPE){
			fCurrentBoard.text = board.substr(0,board.length()-4);
			newBoard = false;
		}else if (keyboard.newKey == KEY_ENTER){
			if (!newBoard){
				ifstream f;
				f.open("boards/" + board, ios::in | ios::binary);
				if (f.good()){
					for (int i = 0; i < imgBoards.size(); i++)
						if (imgBoards[i].name == board.substr(0,board.length()-4)){
							imgBoards[i].name = fCurrentBoard.text;
							sort(imgBoards.begin(), imgBoards.end(), SortBoards);
							break;
						}
					f.close();
					filesystem::rename("boards/" + board, "boards/"+fCurrentBoard.text + ".brd");
				}else{
					FindBoards();
				}
				board = fCurrentBoard.text + ".brd";
			}else{
				board = fCurrentBoard.text + ".brd";
				TakeBoardScreenshot();
				SaveImageBoard();
				FindBoards();
				newBoard = false;
			}
		}
	}

	boardScroll.end = 32;

	y -=  menuWidth+fontSize*4+8;

	for (auto board : imgBoards){
		if (!board.Draw({fWidth-menuWidth, y}, {menuWidth, menuWidth+boardNamePlate})){
			FindBoards();
			return;
		}
		y -= menuWidth+boardNamePlate+8;
		boardScroll.end += menuWidth+boardNamePlate+8;
	}
	boardScroll.end -= Height;
	boardScroll.end += fontSize*2;
	if (boardScroll.end < 0){
		boardScroll.end = 0;
		boardScroll.scroll = 0;
	}
	boardScroll.Draw({fWidth-menuWidth-scrollbarSize, 0}, {scrollbarSize, fHeight});
}

void FindBoards(){
	imgBoards.clear();
	string path = "boards";
	path += slash[0];
	// Empty directory
	if ((stat(path.c_str(), &s) == 0) == 0)
		return;

	for (const auto& entry : fs::directory_iterator((string)path)) {
		fs::path ePath = entry.path();
		string sPath = ePath.string();
		const char* p = sPath.c_str();

		if (stat(p, &s) == 0) // Is valid
			if (s.st_mode & S_IFREG){
				Board b = Board{GetName(p).substr(0,GetName(p).length()-4), GetBoardScreenshot(p)};
				CheckString(b.name);
				imgBoards.push_back(b);
			}
	}
	sort(imgBoards.begin(), imgBoards.end(), SortBoards);
}

Image GetBoardScreenshot(string board){
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	Image img;
	ifstream f;

	f.open(board, ios::in | ios::binary);
    if (!f.good()){
		if (DEBUG) printf("[Loading] Unable to load image board\n");
		return img;
	}

	f.read(reinterpret_cast<char*>(&img.width), 4);
	f.read(reinterpret_cast<char*>(&img.height), 4);
	unsigned char *imgData = (unsigned char*)malloc(img.width*img.height*3);
	f.read(reinterpret_cast<char*>(imgData), img.width*img.height*3);
	img.SetTexture(imgData, false, GL_RGB);
	return img;
}


void TakeBoardScreenshot(){
	glPixelStorei(GL_PACK_ALIGNMENT, 1);
	if (boardScreenshot) free(boardScreenshot);
	boardScreenshot = (unsigned char*)malloc(Main.width*Main.height*3);
	Main.Input = nullptr;
	Main.Render = &DrawMain;
	Main.Draw(backing);

	// Get screenshot of image board
	int w = Main.width, h = Main.height;
	int offsetX = 0, offsetY = 0;
	if (w > h){
		offsetX = (w-h)/2;
		w = h;
	}else if (h > w){
		offsetY = (h-w)/2;
		h = w;
	}

	glReadPixels(offsetX, offsetY, w, h, GL_RGB, GL_UNSIGNED_BYTE, boardScreenshot);
	Main.Render = &DrawApp;
	Main.Input = &MainInput;
}