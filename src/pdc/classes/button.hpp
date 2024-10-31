class Button{
	public:
		string text = "";
		Color color = Black;
		Color hover = White;
		Color textColor = White;
		float fontSize = 0;
		bool toggled = false;
		Vector2 position;
		Vector2 size;
		Image img;
		bool visible = true;

		bool Hover(){return (mouse.position.Within(position, size)) && visible;}

		void Draw(Vector2 p, Vector2 s, bool selected = false, bool centerText = false, bool focused = true){
			position = p;
			size = s;
			Draw(selected, centerText, focused);
		}

		void Draw(bool selected = false, bool centerText = false, bool focused = true){
			if (!visible) visible = true;

			if ((Hover() || toggled || selected) && focused){
				Draw(hover, centerText);
				return;
			}
			shape.Draw(position, size, color, true);

			if (!centerText)
				font.Write(text.data(), position + Vector2{4, 0}, fontSize == 0 ? (int)size.y/4*3 : fontSize, textColor, true);
			else
				font.Write(text.data(), position, fontSize == 0 ? size.y/4*3 : fontSize, textColor, true, size.x, centerText);
		}

		void Draw(Color c, bool centerText = false){
			shape.Draw(position, size, c, true);
			
			if (!centerText)
				font.Write(text.data(), position + Vector2{4, 0}, fontSize == 0 ? (int)size.y/4*3 : fontSize, textColor, true);
			else
				font.Write(text.data(), position, fontSize == 0 ? (int)size.y/4*3 : fontSize, textColor, true, size.x, centerText);
		}

		string ToString(){
			return "Button {\"" + text + "\", " + color.ToString() + ", " + hover.ToString() + ", " + textColor.ToString() + "}";
		}
};