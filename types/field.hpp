class Field{
	public:
		string emptyText = "";
		Color background = White;
		Color activeC = Grey;
		Color textC = Black;
		Color border = Black;
		int borderWidth = 0;
		Vector2 position = Vector2{0,0};
		Vector2 size = Vector2{0,0};
		string text = "";
		bool active = false;

		void Draw(bool fixed = true, bool centerText = false){
			sShape.Use();
			shape.Draw(position, size, active ? activeC : background, fixed);
			shape.DrawBox(position, size, border, borderWidth, fixed);
			
			sImage.Use();
			if (!text.length() && !active)
				font.Write(emptyText.data(), position.Add(4,0), size.y/4*3, textC, fixed, size.x, centerText);
			else
				font.Write(text.data(), position.Add(4,0), size.y/4*3, textC, fixed, size.x, centerText);
		}

		void CheckClick(){
			active = false;
			if (mouse.position.Within(position, size))
				active = true;
		}
};
