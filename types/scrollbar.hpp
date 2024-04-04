class Scrollbar{
	public: 
		Color bar = White;
		Color notch = Grey;
		Color hightlight = DarkGrey;
		Color border = Black;
		int borderWidth = 0;
		bool horizontal = false;
		int scroll = 0;
		int notchSize = 4;
		int end = 24;
		bool scrollLock = false;

		void Draw(Vector2 position, Vector2 size, bool fixed = true, bool disableScroll = false){
			if (!horizontal){
				
				// Backing
				shape.Draw(position, size, bar, fixed);

				// Notch
				shape.Draw(position.Add((float)notchSize, size.y - (size.y)*((float)(scroll)/(float)(end+notchSize*4))), Vector2{size.x-notchSize*2, -16}, notch, true);
			}else{
				// Backing
				shape.Draw(position, size, bar, fixed);

				// Notch
				shape.Draw(position.Add(size.x * (float)scroll/((float)end+notchSize), 0), Vector2{32, (float)notchSize*2}, notch, true);

			}

			// Generic input functions
			if ((mouse.position.Within(position, size) || scrollLock)){
				if (!disableScroll){
					if (mouse.Click(LM_DOWN) || scrollLock){
						if (!horizontal){
							scroll = end - end * ((mouse.position.y - position.y)/size.y);
						}else{
							scroll = end * ((mouse.position.x - position.x)/size.x);
						}
						if (scroll < 0)
							scroll = 0;
						if (scroll > end)
							scroll = end;
						scrollLock = true;
						mouse.drag = false;
					}
				}
				if (mouse.state != LM_DOWN)
					scrollLock = false;
				
			}
		}
};