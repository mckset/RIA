class Checkbox{
	public:
		Color color = White;
		Color activeColor = Black;
		float margin = CHECKBOX_MARGIN;
		Color border = Black;
		int borderWidth = 0;
		bool active = false;
		bool box = true;
		Vector2 position = {0, 0};
		float size = 0;
		bool pressed = false;

		bool Hover(){return (mouse.position.Within(position, {size, size})) && CurrentWindow->focused;}

		void Draw(Vector2 p, float s){
			Draw(p, s, active);
		}

		void Draw(Vector2 p, float s, bool a, bool fixed = POSITION_FIXED){
			size = s;
			position = p;
			shape.Draw(position, {size, size}, color, fixed);
			active = a;

			if (pressed) pressed = false;

			if (Hover() && mouse.Click()){
				active = !a;
				pressed = true;
			}

			if (active) shape.Draw(position+margin, {size-margin*2, size-margin*2}, activeColor, fixed);
		}
};