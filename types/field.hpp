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

		void Draw(bool fixed = true){
			sShape.Use();
			shape.Draw(position, size, active ? activeC : background, fixed);
			shape.DrawBox(position, size, border, borderWidth, fixed);
			
			sImage.Use();
			if (!text.length() && !active)
				font.Write(emptyText.data(), position.Add(4,0), size.y/4*3, textC, fixed);
			else
				font.Write(text.data(), position.Add(4,0), size.y/4*3, textC, fixed);

			/*
			sFont.Use();
			if (text.length() == 0 && !active)
				font.Write(emptyText, position.Add(4, size.y/2-4), 1.0f, textC);
			else
				font.Write(text, position.Add(4, size.y/2-4), 1.0f, textC);
			*/
		}

		void CheckClick(){
			active = false;
			if (mouse.position.Within(position, size))
				active = true;
		}
};
