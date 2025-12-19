//
// Draws a list of boards as well as finding them and taking screenshots
//

// Pointer to the current board screenshot
unsigned char *boardScreenshot;

bool SortBoards(Board, Board);

void CreateBoard(){
	string temp = currentBoard_Field.text + ".brd";

	if (stat((path+"boards/"+temp).c_str(), &st) == 0){
		if (DEBUG) printf("[Saving] Image board already exists\n");
		currentBoard_Field.active = true;
		currentBoard_Field.submitted = false;
		mouse.state = INPUT_NULL;
		return;
	}

	board = temp;
	TakeBoardScreenshot();
	SaveImageBoard();
	FindBoards();
	newBoard = false;
	currentBoard_Field.active = false;
}

// Draws saved image boards in the boards folder
void DrawBoards(){
	// y is set to the top of the window and is used to render down to the bottom
	float y = fHeight + board_Scrollbar.scroll;

	// Draw board heading, background, and button
	shape.Draw({fWidth, y}, {-(sideMenuWidth), -FONT_SIZE*2}, menuBackgroundColor, POSITION_FIXED);
	font.Write("Boards", {fWidth-sideMenuWidth, y-FONT_SIZE*2}, FONT_SIZE, fontColor, POSITION_FIXED, sideMenuWidth-FONT_SIZE*2, ALIGN_CENTER);
	addBoard_Button.Draw({fWidth-FONT_SIZE*2, y-FONT_SIZE*2}, {FONT_SIZE*2, FONT_SIZE*2}, ALIGN_CENTER);

	// Add new board
	if (addBoard_Button.pressed){
		if (!currentBoard_Field.text.length()){
			currentBoard_Field.active = true;
			newBoard = true;
			mouse.state = INPUT_NULL;
		}else
			CreateBoard();
	}
	y -= FONT_SIZE;

	// Draws the current board information
	shape.Draw({fWidth-sideMenuWidth, y-FONT_SIZE*2}, {sideMenuWidth, FONT_SIZE}, locationHeadingColor, POSITION_FIXED);
	font.Write("Current: ", {fWidth-sideMenuWidth+BOARD_PADDING, y-FONT_SIZE*2}, FONT_SIZE/2, fontColor, POSITION_FIXED, sideMenuWidth);
	
	currentBoard_Field.Draw({fWidth-sideMenuWidth+FONT_SIZE*4+BOARD_PADDING, y-FONT_SIZE*2}, {sideMenuWidth, FONT_SIZE});

	// Handles mouse events for the current board field
	if ((mouse.position.Within({fWidth-sideMenuWidth+4, y-FONT_SIZE*2}, {sideMenuWidth, FONT_SIZE}) && CurrentWindow->focused) || currentBoard_Field.active){
		shape.Draw({fWidth-sideMenuWidth, y-FONT_SIZE*2}, {sideMenuWidth, FONT_SIZE}, highlightColor, POSITION_FIXED);
		if (mouse.Click()){
			currentBoard_Field.CheckClick();
			if (!currentBoard_Field.active)
				currentBoard_Field.text = board.substr(0,board.length()-4);
		}
	}

	if (currentBoard_Field.submitted){
		if (!newBoard){
			ifstream f;
			f.open("boards/" + board, ios::in | ios::binary);
			if (f.good()){
				for (int i = 0; i < imgBoards.size(); i++)
					if (imgBoards[i].name == board.substr(0,board.length()-4)){
						imgBoards[i].name = currentBoard_Field.text;
						sort(imgBoards.begin(), imgBoards.end(), SortBoards);
						break;
					}
				f.close();
				filesystem::rename(path + "boards" + slash[0] + board, path + "boards" + slash[0] + currentBoard_Field.text + ".brd");
			}else{
				FindBoards();
			}
			board = currentBoard_Field.text + ".brd";

		}else
			CreateBoard();
	}

	// Scroll bar end position
	board_Scrollbar.end = 32;
	y -=  sideMenuWidth+FONT_SIZE*4+BOARD_PADDING;

	// Draws each board
	for (auto& board : imgBoards){

		// Deletes a board if returned with 0
		if (!board.Draw({fWidth-sideMenuWidth, y}, {sideMenuWidth, sideMenuWidth+boardNamePlate})){
			FindBoards();
			return;
		}
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
	board_Scrollbar.Draw({fWidth-sideMenuWidth-SCROLLBAR_SIZE, 0}, {SCROLLBAR_SIZE, fHeight});
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
				Board b = Board{GetName(pa).substr(0,GetName(pa).length()-4), pa};
				CheckString(b.name);
				imgBoards.push_back(b);
			}
	}
	sort(imgBoards.begin(), imgBoards.end(), SortBoards);
}

bool SortBoards(Board b1, Board b2){return SortString(b1.name, b2.name);}

// Create image board screenshot
void TakeBoardScreenshot(){
	// Screenshot gets corrupted when this flag is not on
	glPixelStorei(GL_PACK_ALIGNMENT, 1);

	// Clear buffer if it is being used
	if (boardScreenshot) free(boardScreenshot);
	boardScreenshot = (unsigned char*)malloc(Main.width*Main.height*3);
	
	// Render board without menus showing
	Main.Input = nullptr;
	Main.Render = &DrawImages; //<-- DrawMain and NOT DrawApp
	Main.Draw(backgroundColor);

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