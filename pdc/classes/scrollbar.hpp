class Scrollbar{
	public: 
		Color bar = White;
		Color notch = Grey;
		Color hightlight = DarkGrey;
		Color border = Black;
		int borderWidth = 0;
		bool horizontal = false;
		int scroll = 0;
		int notchSize = 24;
		int end = 0;
		bool scrollLock = false;

		void Draw(Vector2 position, Vector2 size, bool fixed = true, bool disableScroll = false){
			if (end == 0)
				return;
			if (!horizontal){
					
				// Backing
				shape.Draw(position, size, bar, fixed);

				// Notch
				shape.Draw(position + Vector2{0, (size.y * (1 - ((float)scroll/(float)end))) + (notchSize * ((float)scroll/(float)end))}, 
					Vector2{size.x, (float)-notchSize}, notch, true);
			}else{
				// Backing
				shape.Draw(position, size, bar, fixed);

				// Notch
				shape.Draw(position.Add(size.x * (float)scroll/((float)end) - (notchSize * ((float)scroll/(float)end)), 0), 
					Vector2{(float)notchSize, size.y}, notch, true);

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