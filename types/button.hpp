class Button{
	public:
		string text = "";
		Color color;
		Color hover;
		bool toggle = false;
		Vector2 position;
		Vector2 size;
		Image img;
		bool toggled = false;

		bool Hover(){return (mouse.position.Within(position, size));}

		void Init(string n, Color c, Vector2 pos = Vector2{0,0}, Vector2 s = Vector2{0,0}){
			text = n;
			position = pos;
			size = s;
			color = c;
		}

		void Init(Image i, Color c, Vector2 pos = Vector2{0,0}, Vector2 s = Vector2{0,0}){
			img = i;
			position = pos;
			size = s;
			color = c;
		}

		void Draw(bool selected = false, bool centerText = false){
			if (Hover() || toggled || selected){
				Draw(hover, centerText);
				return;
			}
			sShape.Use();
			shape.Draw(position, size, color, true);
			sImage.Use();
			if (!centerText)
				font.Write(text.data(), position, (int)size.y/4*3, White, true);
			else
				font.Write(text.data(), position, (int)size.y/4*3, White, true, size.x, centerText);
		}

		void Draw(Color c, bool centerText = false){
			sShape.Use();
			shape.Draw(position, size, c, true);
			sImage.Use();
			if (!centerText)
				font.Write(text.data(), position, (int)size.y/4*3, White, true);
			else
				font.Write(text.data(), position, (int)size.y/4*3, White, true, size.x, centerText);
		}

};