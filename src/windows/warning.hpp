/*
enum Warnings{
	WARNING_LOCATION,
	WARNING_TAG,
	WARNING_SUBTAG,
	WARNING_BOARD
};
*/

class WarningWindow : public Window{
	public:
		int type;
		void* warningObject = nullptr;
		void* warningParent = nullptr;

		void DrawWarning(){
			
			string text = "Delete ";
			string name = "";
			if (type == WARNING_LOCATION){
				text += "folder";
				name = ((Location*)warningObject)->name + "?";
			}else if (type == WARNING_TAG){
				text += "tag";
				name = ((Tag*)warningObject)->name + "?";
			}else if (type == WARNING_SUBTAG){
				text += "sub tag";
				name = ((Tag*)warningObject)->name + "?";
			}else if (type == WARNING_BOARD){
				text += "board";
				name = ((Board*)warningObject)->name + "?";
			}

			font.Write(text, {PADDING, PADDING+FONT_SIZE*5}, FONT_SIZE, fontColor, POSITION_FIXED, fWidth-PADDING*2, ALIGN_CENTER);
			font.Write(name, {PADDING, PADDING+FONT_SIZE*3}, FONT_SIZE, fontColor, POSITION_FIXED, fWidth-PADDING*2, ALIGN_CENTER);
			accept_Button.Draw({PADDING, PADDING}, {fWidth/2-PADDING*2, FONT_SIZE*2}, ALIGN_CENTER);
			cancel_Button.Draw({Width/2+PADDING, PADDING}, {fWidth/2-PADDING*2, FONT_SIZE*2}, ALIGN_CENTER);
		}

		void WarningInput(){
			if (keyboard.newKey == KEY_ESCAPE)
				Hide();
				
			if (accept_Button.pressed){
				Hide();

				if (type == WARNING_LOCATION){
					for (int i = 0; i < locations.size(); i++){
						if ((void*)&locations[i] == warningObject){
							locations.erase(locations.begin()+i);
							warningObject = nullptr;
							return;
						}
					}
				}else if (type == WARNING_TAG){
					for (int i = 0; i < tags.size(); i++){
						if ((void*)&tags[i] == warningObject){
							tags.erase(tags.begin()+i);
							warningObject = nullptr;
							return;
						}
					}
				}else if (type == WARNING_SUBTAG){
					Tag* tag = (Tag*)warningParent;
					for (int i = 0; i < tag->subTags.size(); i++){
						if ((void*)&tag->subTags[i] == warningObject){
							tag->subTags.erase(tag->subTags.begin()+i);
							warningObject = nullptr;
							warningParent = nullptr;
							return;
						}
					}
				}else if (type == WARNING_BOARD){
					string s = "boards";
					s += slash[0];
					s += ((Board*)warningObject)->name + ".brd";
					filesystem::remove(s.c_str());
					FindBoards();
				}

			}else if (cancel_Button.pressed){
				Hide();
			}
		}

	private:
		Button accept_Button = Button{"Delete", menuBackgroundColor, highlightColor, fontColor, FONT_SIZE};
		Button cancel_Button = Button{"Cancel", menuBackgroundColor, highlightColor, fontColor, FONT_SIZE};
};

WarningWindow WarningWin;


void DrawWarning(){
	WarningWin.DrawWarning();

}

void Warn(int t, void* object, void* parentObject = nullptr){
	WarningWin.type = t;
	WarningWin.warningObject = object;
	WarningWin.warningParent = parentObject;
	WarningWin.Show();
}

void WarningInput(){
	WarningWin.WarningInput();
}

