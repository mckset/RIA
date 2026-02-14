enum Main_Displays{
	MAIN_APP,
	MAIN_SCREENSHOT,
	MAIN_TUTORIAL
};

class MainWindow : public Window{
	public:
		bool rotateImages = false;
		bool scaleImages = false; // If the program should scale images
		bool showImageSelector = false;
		bool showLeftMenu = false;
		bool showOrigin = true;
		bool showMouseMenu = false; // Draws the right click menu when true
		bool showRightMenu = false;
		bool showTags = false;

		int display = MAIN_APP;

		float sideMenuWidth = 0;
		float sideMenuRatio =	0.20f; // Ratio of the menu width to the window width

		void AppInput();

		void BoardInput();

		void CreateBoard(string);
		void CopyBoard(string);

		void DrawApp();
		void DrawBoard();
		void DrawBoards();
		void DrawEmpty();
		void DrawImages();
		void DrawLocations();
		void DrawStage();
		void DrawTags();
		void DrawTutorial();

		void ImageInput();

		void LeftMenuInput();

		void OpenTutorial();


		void RightMenuInput();
		void ReorderImages();
		void ResetImages();

		int SelectImage();

		void TakeBoardScreenshot();
		void TutorialInput();


	private:
		int zoomTextTimer = 0;

		Button add_Button = Button{"+", Transparent, highlightColor, White, FONT_SIZE};
		Button addBoard_Button = Button{"+", Transparent, highlightColor, White, FONT_SIZE};
		Button help_Button = Button{"Help", menuBackgroundColor, highlightColor, White, SMALL_FONT_SIZE};
		Button leftClose_Button = Button{"Close", menuBackgroundColor, highlightColor, White, SMALL_FONT_SIZE};
		Button openTags_Button = Button{"Tags", menuBackgroundColor, highlightColor, White, SMALL_FONT_SIZE};
		Button openLocations_Button = Button{"Folders", menuBackgroundColor, highlightColor, White, SMALL_FONT_SIZE};
		Button imagePack_Button = Button{"Image Packs", menuBackgroundColor, highlightColor, White, SMALL_FONT_SIZE};
		Button imageBoards_Button = Button{"Boards", menuBackgroundColor, highlightColor, White, SMALL_FONT_SIZE};
		Button refreshLocations_Button = Button{"", Transparent, highlightColor, fontColor, FONT_SIZE};
		Button save_Button = Button{"Save", menuBackgroundColor, highlightColor, White, SMALL_FONT_SIZE};

		Scrollbar board_Scrollbar = Scrollbar{scrollbarBackgroundColor, scrollbarNotchColor};
		Scrollbar tags_Scrollbar = Scrollbar{scrollbarBackgroundColor, scrollbarNotchColor};
		Scrollbar locations_Scrollbar = Scrollbar{scrollbarBackgroundColor, scrollbarNotchColor};
};

MainWindow Main;


void DrawMain(){
	Main.sideMenuWidth = Main.sideMenuRatio*fWidth;
	if (Main.display == MAIN_APP){
		Main.DrawApp();
	}else if (Main.display == MAIN_TUTORIAL){
		Main.DrawTutorial();
	}else if (Main.display == MAIN_SCREENSHOT){
		Main.DrawImages();
		Main.display = MAIN_APP;
	}
}

void MainInput(){
	if (Main.display == MAIN_APP)
		Main.AppInput();
	else if (Main.display == MAIN_TUTORIAL){
		Main.TutorialInput();
	}else if (Main.display == MAIN_SCREENSHOT){
		
	}
}
