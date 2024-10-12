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