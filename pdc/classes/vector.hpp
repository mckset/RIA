/*
	VECTOR.HPP
	
	A vector2 is simply as it sounds.
	It contains both an x and y value, usually coordinates.
	Most of the functions are declared to easily alter the values of a vector2 without needing to constantly declare new ones.
	The engine declares a view pointer to point to the current windows view space.

*/

class Vector2{
	public:
		float x, y;

		// Operation overrides

		// Addition
		Vector2 operator+(double d){return Vector2{x+(float)d, y+(float)d};}
		Vector2 operator+(float f){return Vector2{x+f, y+f};}
		Vector2 operator+(int i){return Vector2{x+(float)i, y+(float)i};}
		Vector2 operator+(Vector2 v){return Vector2{x+v.x, y+v.y};}

		// Division
		Vector2 operator/(double d){return Vector2{x/(float)d, y/(float)d};}
		Vector2 operator/(float f){return Vector2{x/f, y/f};}
		Vector2 operator/(int i){return Vector2{x/(float)i, y/(float)i};}
		Vector2 operator/(Vector2 v){return Vector2{x/v.x, y/v.y};}

		// Multiply
		Vector2 operator*(double d){return Vector2{x*(float)d, y*(float)d};}
		Vector2 operator*(float f){return Vector2{x*f, y*f};}
		Vector2 operator*(int i){return Vector2{x*(float)i, y*(float)i};}
		Vector2 operator*(Vector2 v){return Vector2{x*v.x, y*v.y};}

		// Subtraction
		Vector2 operator-(double d){return Vector2{x-(float)d, y-(float)d};}
		Vector2 operator-(float f){return Vector2{x-f, y-f};}
		Vector2 operator-(int i){return Vector2{x-(float)i, y-(float)i};}
		Vector2 operator-(Vector2 v){return Vector2{x-v.x, y-v.y};}

		// Mod
		Vector2 operator%(int i){return Vector2{(float)((int)x % i), (float)((int)y % i)};}

		// Add equal
		void operator+=(double d){x+=d; y+=d;}
		void operator+=(float f){x+=f; y+=f;}
		void operator+=(int i){x+=i; y+=i;}
		void operator+=(Vector2 v){x+=v.x; y+=v.y;}

		// Divide equal
		void operator/=(double d){x/=d; y/=d;}
		void operator/=(float f){x/=f; y/=f;}
		void operator/=(int i){x/=i; y/=i;}
		void operator/=(Vector2 v){x/=v.x; y/=v.y;}

		// Multiply equal
		void operator*=(double d){x*=d; y*=d;}
		void operator*=(float f){x*=f; y*=f;}
		void operator*=(int i){x*=i; y*=i;}
		void operator*=(Vector2 v){x*=v.x; y*=v.y;}

		// Subtract equal
		void operator-=(double d){x-=d; y-=d;}
		void operator-=(float f){x-=f; y-=f;}
		void operator-=(int i){x-=i; y-=i;}
		void operator-=(Vector2 v){x-=v.x; y-=v.y;}

		// Logic
		bool operator==(Vector2 v){return (x == v.x && y == v.y);}
		bool operator!=(Vector2 v){return (x != v.x && y != v.y);}
		bool operator<=(Vector2 v){return (Sum() <= v.Sum());}
		bool operator<(Vector2 v){return (Sum() < v.Sum());}
		bool operator>=(Vector2 v){return (Sum() >= v.Sum());}
		bool operator>(Vector2 v){return (Sum() > v.Sum());}

		// Math shortcuts
		void operator++(){x++;y++;}
		void operator--(){x--;y--;}
		Vector2 Floor(Vector2 v){return Vector2 {abs(x) > v.x ? v.x*(1+-2*(x<0)) : x, abs(y) > v.y ? v.y*(1+-2*(y<0)) : y};}

		Vector2 ABS(){return Vector2{std::abs(x), std::abs(y)};}
		Vector2 Round(){return {std::roundf(x), std::roundf(y)};}
		Vector2 Square(){return Vector2{x*x, y*y};}
		float Sum(){return x+y;}
		

		// Basic math
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

		void Clear(){x=0;y=0;};
		float ScalarMultiply(Vector2 m){return x*m.x + y*m.y;}
		float VectorMultiply(Vector2 m){return x*m.y-y*m.x;}

		string ToString(){
			return "Vector2 {" + to_string(x) + ", " + to_string(y) + "}";
		}

		// Special functions

		// Vector related functions
		float ShortAngle(Vector2 mid, Vector2 end, bool d = true){return acos((*this-mid).Normalize().ScalarMultiply((end-mid).Normalize()))*(DEGREES*d);}
		
		// Gets the angle assuming that the end line is the base line of the axis
		float Angle(Vector2 mid, Vector2 end, bool d = true){
			float a = acos((*this-mid).Normalize().ScalarMultiply((end-mid).Normalize()))*(DEGREES*d);
			
			return a;
		}
		float Distance(Vector2 v2){return (std::abs(x-v2.x) + std::abs(y-v2.y));}
		bool Equal(Vector2 v){return (v.x == x && v.y == y);}
		float Magnitude(){return sqrt(Square().Sum());} // Oh Yeah!!!
		Vector2 Normalize(){return *this/Magnitude();} // Direction

		void Rotate(Vector2 center, float a){
			float rX = cos(a) * (x - center.x) - sin(a) * (y-center.y) + center.x;
			float rY = sin(a) * (x - center.x) + cos(a) * (y - center.y) + center.y;
			x = rX;
			y = rY;
		}

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

};

Vector2 *View;