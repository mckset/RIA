const float boardNamePlate = fontSize*2;
bool newBoard = false;

class Board{
	public:
		string name = "Default";
		Image img;
		Vector2 position = {0,0};
		Vector2 size = {0,0};

		bool Hover(){return mouse.position.Within(position, size);}

		int Draw(Vector2 p, Vector2 s, bool focused = true){
			position = p;
			size = s;
			shape.Draw(position, size, locationHeading, true);
			int l = font.Write(name, position+8, fontSize, fontColor, true, menuWidth-16, 1);
			s.y -= boardNamePlate;

			float ratio = (img.width > img.height) ? s.x/img.width : s.y/img.height;
			img.Draw(p+s/2-Vector2{(float)img.width, (float)img.height}*ratio/2+Vector2{8, 8+boardNamePlate}, Vector2{(float)img.width, (float)img.height}*ratio-16, White, 0, true);

			if (Hover() && focused){
				shape.Draw(position, size, highlight, true);
                if (mouse.Click()){
                    board = name + ".brd";
                    LoadImageBoard();
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
};
vector<Board> imgBoards;