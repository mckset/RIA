//
// Draws a list of boards as well as finding them and taking screenshots
//

// Pointer to the current board screenshot
unsigned char *boardScreenshot;

// Draws saved image boards in the boards folder
void DrawBoards(){
	// y is set to the top of the window and is used to render down to the bottom
	float y = fHeight + boardScroll.scroll;

	// Draw board heading, background, and button
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

	// Draws the current board information
	shape.Draw({fWidth-menuWidth, y-fontSize*2}, {menuWidth, fontSize}, locationHeading, true);
	font.Write("Current: ", {fWidth-menuWidth+8, y-fontSize*2}, fontSize/2, fontColor, true, menuWidth, 0);
	fCurrentBoard.Draw({fWidth-menuWidth+fontSize*4+8, y-fontSize*2}, {menuWidth, fontSize});

	// Handles mouse events for the current board field
	if (mouse.position.Within({fWidth-menuWidth+4, y-fontSize*2}, {menuWidth, fontSize}) || fCurrentBoard.active){
		shape.Draw({fWidth-menuWidth, y-fontSize*2}, {menuWidth, fontSize}, highlight, true);
		if (mouse.Click()){
			fCurrentBoard.CheckClick();
			if (!fCurrentBoard.active)
				fCurrentBoard.text = board.substr(0,board.length()-4);
		}
	}

	// Updates text for the current board field
	if (fCurrentBoard.active){
		fCurrentBoard.UpdateText();

		// Cancel input
		if (keyboard.newKey == KEY_ESCAPE){
			fCurrentBoard.text = board.substr(0,board.length()-4);
			newBoard = false;

		// Rename board
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
					filesystem::rename(path + "boards" + slash[0] + board, path + "boards" + slash[0] + fCurrentBoard.text + ".brd");
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

	// Scroll bar end position
	boardScroll.end = 32;
	y -=  menuWidth+fontSize*4+8;

	// Draws each board
	for (auto board : imgBoards){

		// Deletes a board if returned with 0
		if (!board.Draw({fWidth-menuWidth, y}, {menuWidth, menuWidth+boardNamePlate})){
			FindBoards();
			return;
		}
		y -= menuWidth+boardNamePlate+8;
		boardScroll.end += menuWidth+boardNamePlate+8;
	}

	// Subtracts the height from the scroll bar
	boardScroll.end -= Height;
	boardScroll.end += fontSize*2;

	// Sets the end and position to 0 if there is not enough entries to need a scrollbar
	if (boardScroll.end < 0){
		boardScroll.end = 0;
		boardScroll.scroll = 0;
	}

	// Draws the scrollbar if there are more entries than the height of the window
	boardScroll.Draw({fWidth-menuWidth-scrollbarSize, 0}, {scrollbarSize, fHeight});
}

// Finds boards in the boards folder
void FindBoards(){
	
	// Setup
	imgBoards.clear();
	string p = path+"boards";
	p += slash[0];

	// Empty directory
	if ((stat(p.c_str(), &st) == 0) == 0)
		return;

	// Loop through all files and folders
	for (const auto& entry : fs::directory_iterator((string)p)) {
		fs::path ePath = entry.path();
		string sPath = ePath.string();
		const char* pa = sPath.c_str();

		if (stat(pa, &st) == 0) // Is valid
			if (st.st_mode & S_IFREG && sPath.substr(sPath.length()-4) == ".brd"){ // Is a file
				Board b = Board{GetName(pa).substr(0,GetName(pa).length()-4), GetBoardScreenshot(pa)};
				CheckString(b.name);
				imgBoards.push_back(b);
			}
	}
	sort(imgBoards.begin(), imgBoards.end(), SortBoards);
}

// Reads screenshot data from an image board save
Image GetBoardScreenshot(string board){

	// Screenshot gets corrupted when this flag is not on
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);


	Image img;
	ifstream f;

	// Make sure the board exists
	f.open(board, ios::in | ios::binary);
    if (!f.good()){
		if (DEBUG) printf("[Loading] Unable to load image board\n");
		return img;
	}

	// 4 bytes width, 4 bytes height, width*height*3 image data
	f.read(reinterpret_cast<char*>(&img.width), 4);
	f.read(reinterpret_cast<char*>(&img.height), 4);
	unsigned char *imgData = (unsigned char*)malloc(img.width*img.height*3);
	f.read(reinterpret_cast<char*>(imgData), img.width*img.height*3);
	img.SetTexture(imgData, false, GL_RGB);
	return img;
}

// Create image board screenshot
void TakeBoardScreenshot(){
	// Screenshot gets corrupted when this flag is not on
	glPixelStorei(GL_PACK_ALIGNMENT, 1);

	// Clear buffer if it is being used
	if (boardScreenshot) free(boardScreenshot);
	boardScreenshot = (unsigned char*)malloc(Main.width*Main.height*3);
	
	// Render board without menus showing
	Main.Input = nullptr;
	Main.Render = &DrawMain; //<-- DrawMain and NOT DrawApp
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

	// Store screenshot in buffer
	glReadPixels(offsetX, offsetY, w, h, GL_RGB, GL_UNSIGNED_BYTE, boardScreenshot);
	
	// Reset main window
	Main.Render = &DrawApp;
	Main.Input = &MainInput;
}