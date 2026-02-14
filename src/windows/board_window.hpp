enum BoardType{
	BOARD_NEW,
	BOARD_EDIT
};

class BoardWindow : public Window{
	public:
		void DrawWindow(){
			empty_Button.Draw({PADDING/2, PADDING}, {fWidth/3-PADDING, MEDIUM_FONT_SIZE*2}, ALIGN_CENTER);
			copy_Button.Draw({fWidth/3+PADDING/2, PADDING}, {fWidth/3-PADDING, MEDIUM_FONT_SIZE*2}, ALIGN_CENTER);
			cancel_Button.Draw({fWidth/3*2+PADDING/2, PADDING}, {fWidth/3-PADDING, MEDIUM_FONT_SIZE*2}, ALIGN_CENTER);

			name_Field.Draw({PADDING/2, MEDIUM_FONT_SIZE*5}, {fWidth-PADDING, FONT_SIZE*2}, ALIGN_CENTER);
		}

		void WindowInput(){
			if (cancel_Button.pressed)
				Hide();
			else if (empty_Button.pressed){
				Hide();
				Main.CreateBoard(name_Field.text);
			}else if (copy_Button.pressed){
				Hide();
				Main.CopyBoard(name_Field.text);
			}
		}

	private:
		Button cancel_Button{"Cancel", menuBackgroundColor, highlightColor, fontColor, MEDIUM_FONT_SIZE};
		Button copy_Button{"Copy", menuBackgroundColor, highlightColor, fontColor, MEDIUM_FONT_SIZE};
		Button empty_Button{"New", menuBackgroundColor, highlightColor, fontColor, MEDIUM_FONT_SIZE};

		Field name_Field{"Board Name", fieldBackgroundColor, highlightColor, fontColor, FONT_SIZE};
};
BoardWindow BoardWin;


void DrawBoardWindow(){
	BoardWin.DrawWindow();
}

void BoardWindowInput(){
	BoardWin.WindowInput();
}