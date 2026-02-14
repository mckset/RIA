class Dropdown{
	public:
		Color color = Color {1,1,1,1};
		Color highlight = Color {.1, .1, .1, .1};
		Color textColor = Color {0,0,0,1};
		float fontSize = 0;
		bool visible = true;
		vector<string> elements;
		bool active = false;
		int selected = 0;

		bool pressed = false;
		Vector2 position = {0, 0};
		Vector2 size = {0, 0};

		bool Hover(){return (mouse.position.Within(position, size)) && visible && CurrentWindow->focused;}


		void Draw(bool centerText = false, bool fixed = true){
			Draw(position, size.x, size.y, centerText, fixed);
		}

		void Draw(Vector2 p, float width, float elementHeight, bool centerText = false, bool fixed = true){
			if (!visible || !elements.size())
				return;

			position = p;
			size = {width, elementHeight};

			// Selected element
			fontSize == 0 ? fontSize = elementHeight/4*3 : fontSize;
			shape.Draw(position, {width, elementHeight}, color, true);
			if ((active || Hover()) && ((Window*)CurrentWindow)->focused){ 
				shape.Draw(position, {width, elementHeight}, highlight, true);
				if (mouse.Click() && Hover()) active = !active;
			}
			if (!centerText)
				font.Write(elements[selected], position + Vector2{PDC_TEXT_MARGIN, 0}, fontSize, textColor, true);
			else
				font.Write(elements[selected], position, fontSize == 0 ? elementHeight/4*3 : fontSize, textColor, true, width, centerText);

			// All the other elements
			for (int value = 0; value < elements.size() && active; value++){
				shape.Draw(position, {width, -elementHeight}, color, fixed);
				if (mouse.position.Within(position, {width, -elementHeight}) && ((Window*)CurrentWindow)->focused){
					shape.Draw(position, {width, -elementHeight}, highlight, true);
					if (mouse.Click()){
						selected = value;
						active = false;
						pressed = true;
					}
				}
					
				position.y -= elementHeight;
				if (!centerText)
					font.Write(elements[value], position + Vector2{PDC_TEXT_MARGIN, 0}, fontSize, textColor, true);
				else
					font.Write(elements[value], position, fontSize == 0 ? elementHeight/4*3 : fontSize, textColor, true, width, centerText);
			}
		}
};