//
// Draws a list of boards as well as finding them and taking screenshots
//

// Pointer to the current board screenshot
unsigned char *boardScreenshot;

bool SortBoards(Board, Board);

void MainWindow::CreateBoard(string boardName){
	string temp = boardName + ".brd";

	if (stat((path+"boards/"+temp).c_str(), &st) == 0){
		statusText = temp + " already exists";
		statusTextTimer = STATUS_TIME;
		if (DEBUG) printf("[Saving] Image board already exists\n");
		return;
	}

	board = temp;
	imgs.clear();
	if (boardScreenshot) free(boardScreenshot);
	boardScreenshot = (unsigned char*)malloc(Main.width*Main.height*3);

	for (int i = 0; i < Main.width*Main.height*3; i++){
		boardScreenshot[i] = (unsigned char)51;
	}

	SaveImageBoard();
	FindBoards();
}

void MainWindow::CopyBoard(string boardName){
	string temp = boardName + ".brd";

	if (stat((path+"boards/"+temp).c_str(), &st) == 0){
		statusText = temp + " already exists";
		statusTextTimer = STATUS_TIME;
		if (DEBUG) printf("[Saving] Image board already exists\n");
		return;
	}

	fs::copy(path+"boards/"+board, path+"boards/"+temp);
	board = temp;
	FindBoards();
}

// Draws saved image boards in the boards folder
void MainWindow::DrawBoards(){
	// y is set to the top of the window and is used to render down to the bottom
	float y = fHeight + board_Scrollbar.scroll;

	// Draw board heading, background, and button
	shape.Draw({fWidth, y}, {-(sideMenuWidth+SCROLLBAR_SIZE), -FONT_SIZE*2}, menuBackgroundColor, POSITION_FIXED);
	font.Write("Boards", {fWidth-sideMenuWidth+FONT_SIZE*2, y-FONT_SIZE*2}, FONT_SIZE, fontColor, POSITION_FIXED, sideMenuWidth-FONT_SIZE*4, ALIGN_CENTER);
	addBoard_Button.Draw({fWidth-FONT_SIZE*2, y-FONT_SIZE*2}, {FONT_SIZE*2, FONT_SIZE*2}, ALIGN_CENTER);

	// Add new board
	if (addBoard_Button.pressed){
		BoardWin.Show();
	}
	y -= FONT_SIZE;

	// Scroll bar end position
	board_Scrollbar.end = 32;
	y -=  sideMenuWidth+FONT_SIZE*3+BOARD_PADDING;

	// Draws each board
	for (auto& board : imgBoards){
		board.Draw({fWidth-sideMenuWidth, y}, {sideMenuWidth, sideMenuWidth+boardNamePlate});
		y -= sideMenuWidth+boardNamePlate+BOARD_PADDING;
		board_Scrollbar.end += sideMenuWidth+boardNamePlate+BOARD_PADDING;
	}

	// Subtracts the height from the scroll bar
	board_Scrollbar.end -= Height;
	board_Scrollbar.end += FONT_SIZE*2;

	// Sets the end and position to 0 if there is not enough entries to need a scrollbar
	if (board_Scrollbar.end < 0){
		board_Scrollbar.end = 0;
		board_Scrollbar.scroll = 0;
	}

	// Draws the scrollbar if there are more entries than the height of the window
	board_Scrollbar.Draw({fWidth-sideMenuWidth-SCROLLBAR_SIZE, 0}, {SCROLLBAR_SIZE, fHeight-FONT_SIZE*2});
}



// Finds boards in the boards folder
void FindBoards(){
	
	Field* boardName_Field = nullptr;

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
				Board b = Board{GetName(pa).substr(0,GetName(pa).length()-4), pa};
				CheckString(b.name);
				b.GetBoardScreenshot();
				b.name_Field.emptyText = b.name;
				imgBoards.push_back(b);
			}
	}
	sort(imgBoards.begin(), imgBoards.end(), SortBoards);
}

bool SortBoards(Board b1, Board b2){return SortString(b1.name, b2.name);}

// Create image board screenshot
void MainWindow::TakeBoardScreenshot(){
	// Screenshot gets corrupted when this flag is not on
	glPixelStorei(GL_PACK_ALIGNMENT, 1);

	// Clear buffer if it is being used
	if (boardScreenshot) free(boardScreenshot);
	boardScreenshot = (unsigned char*)malloc(Main.width*Main.height*3);
	
	/* 
		Render board without menus showing
		This does not work under Wayland anymore and must be skipped
	*/
	if (!(LINUX && WAYLAND)){
		Main.display = MAIN_SCREENSHOT;
		Main.Draw(backgroundColor);
	}

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
}