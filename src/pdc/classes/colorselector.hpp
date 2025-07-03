class ColorSelector{
	public:
		float saturation = 1;
		float value = 1;
		Color color;

		Vector2 position;
		Vector2 size;
		Vector2 pickerPos = {0, 0};

		bool Hover(){return mouse.position.Within(position, size);}
		void Reset(){value = 1; saturation = 1; pickerPos = {0,0};}

		void Draw(Vector2 pos, Vector2 s, Color baseColor, bool fixed = true){
			color = baseColor.Brighten(saturation).Darken(value);

			position = pos;
			size = s;

			shape.DrawColorSelector(position, size, baseColor, fixed);
			shape.DrawCircle(position + pickerPos, 24, 24, color, fixed);
			shape.DrawCircle(position + pickerPos, 24, 4, Black, fixed);
		}

		void MovePicker(Vector2 pos){
			pickerPos = pos;
			saturation = 1 - pickerPos.x/size.x;
			value = 1 - pickerPos.y/size.y;
		}
};