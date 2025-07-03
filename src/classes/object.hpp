class Object{
	public:
		Vector2 position = Vector2{0,0};
		Vector2 size = Vector2{1,1};
		float angle = 0, prevAngle = 0;
		string path = "";
		bool vFlip = false, hFlip = false, selected = false;
		Image img;

		void Draw(){
			img.vFlip = vFlip;
			img.hFlip = hFlip;
			img.Draw(position, size, White, angle);
		}

		void Draw(Vector2 pos, Vector2 s, Color color = White, bool fixed = false){
			img.vFlip = vFlip;
			img.hFlip = hFlip;
			img.Draw(pos, s, color, angle, fixed);
		}


		void DrawOutline(float w, Color color){
			img.DrawOutline(position, size, angle, w, color);
		}

		void GetImgSize(){
			size = Vector2{(float)img.width, (float)img.height};
		}

		bool WithinWindow(Vector2 start, Vector2 end){
			// Move starting position to lower left corner
			if (start.x > end.x){
				int x = start.x;
				start.x = end.x;
				end.x = x;
			}
			if (start.y > end.y){
				int y = start.y;
				start.y = end.y;
				end.y = y;
			}
			Vector2 p = ScreenSpace(position);

			// Bounds check on corners
			if (p.Within(start, end-start) ||
				(p+Vector2{size.x, 0}).Within(start, end-start) ||
				(p+Vector2{0, size.y}).Within(start, end-start) ||
				(p+size).Within(start, end-start))
					return true;

			// Bounds check on edges
			if (((start.x <= p.x && end.x >= p.x || // Left edge
				start.x <= p.x+size.x && end.x >= p.x+size.x) && // Right edge
				start.y > p.y  && end.y < p.y + size.y) || // Within height

				(start.y <= p.y && end.y >= p.y || // Bottom edge
				start.y <= p.y+size.y && end.y >= p.y+size.y) &&
				(start.x > p.x && end.x < p.x+size.x)) // Top edge
				return true;
			return false;
		}

		void ScaleImg(Vector2 bounds, bool ratio = true){
			if (bounds.x <= 0)
				bounds.x = 1;
			if (bounds.y <= 0)
				bounds.y = 1;

			if (ratio){
				if (bounds.x/(float)img.width < bounds.y/(float)img.height){
					size.y = (float)img.height * (bounds.x/(float)img.width);
					size.x = bounds.x;
				}else{
					size.x = (float)img.width * (bounds.y/(float)img.height);
					size.y = bounds.y;
				}
			}else
				size = bounds;
		}
};