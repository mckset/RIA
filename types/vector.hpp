class Vector2{
	public:
		float x, y;

		Vector2 ABS(){return Vector2{abs(x), abs(y)};}

		Vector2 Add(Vector2 v2){return Vector2{x + v2.x, y + v2.y};}
		Vector2 Add(float x2, float y2){return Vector2{x + x2, y + y2};}
		Vector2 Add(float x1){return Vector2{x + x1, y + x1};}
		
		Vector2 Subtract(Vector2 v2){return Vector2{x - v2.x, y - v2.y};}
		Vector2 Subtract(float x2){return Vector2{x - x2, y - x2};}
		Vector2 Subtract(float x2, float y2){return Vector2{x - x2, y - y2};}
		Vector2 Subtract(int x2, int y2){return Vector2{x - (float)x2, y - (float)y2};}

		Vector2 Divide(float d){return Vector2{x/d, y/d};}
		Vector2 Divide(float d1, float d2){return Vector2{x/d1, y/d2};}
		Vector2 Divide(Vector2 v){return Vector2{x/v.x, y/v.y};}

		Vector2 Mod(int mod){return Vector2{(float)((int)x % mod), (float)((int)y % mod)};}

		Vector2 Multiply(float m){return Vector2{x*m, y*m};}
		Vector2 Multiply(float m1 ,float m2){return Vector2{x*m1, y*m2};}
		Vector2 Multiply(Vector2 m){return Vector2{x*m.x, y*m.y};}

		float Distance(Vector2 v2){return (abs(x-v2.x) + abs(y-v2.y));}

		bool Within(Vector2 origin, Vector2 size){
			if (size.x < 0){
				origin.x += size.x;
				size.x = -size.x;
			}
			if (size.y < 0){
				origin.y += size.y;
				size.y = -size.y;
			}

			return (x > origin.x && x < origin.x + size.x && y > origin.y && y < origin.y + size.y);
		}
		
		bool Equal(Vector2 v){return (v.x == x && v.y == y);}

};

class Vector4 {
	public:
		float x, y, z, w;
};



