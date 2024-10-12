class Color{
	public:
		float r, g, b, a;
			
		bool Equal(Color c){
			return (c.r == r && c.b == b && c.g == g && c.a == a);
		}

		Color Brighten(float f){
			return Color{clamp(r + f, 0.0f, 1.0f), clamp(g + f, 0.0f, 1.0f), clamp(b + f, 0.0f, 1.0f), a};
		}

		Color Darken(float f){
			return Color{clamp(r - f, 0.0f, 1.0f), clamp(g - f, 0.0f, 1.0f), clamp(b - f, 0.0f, 1.0f), a};
		}

		Color Saturate(float f){
			return Color{r * f, g * f, b * f, a};
		}

		Color Subtract(float sR, float sG, float sB, float sA, float min = 0){
			return Color{
				r-sR >= min ? r-sR : min, 
				g-sG >= min ? g-sG : min,
				b-sB >= min ? b-sB : min,
				a-sA >= min ? a-sA : min};}

		string ToString(){
			return "Color {" + to_string(r) + ", " + to_string(g) + ", " + to_string(b) + "}";
		}
};

string ColorToHex(Color c){
	int r1 = (int)(c.r*255) >> 4;
	int r2 = ((int)(c.r*255) - r1*16);
	int g1 = (int)(c.g*255) >> 4;
	int g2 = ((int)(c.g*255) - g1*16);
	int b1 = (int)(c.b*255) >> 4;
	int b2 = ((int)(c.b*255) - b1*16);

	//printf("%f %f %f) ", c.r, c.g, c.b);
	//printf("%s ", IntToHex[r1] + IntToHex[r2] + IntToHex[b1] +IntToHex[b2] + IntToHex[g1] + IntToHex[g2])

	return IntToHex[r1] + IntToHex[r2] + IntToHex[g1] +IntToHex[g2] + IntToHex[b1] + IntToHex[b2];
}

Color HexToColor(string hex){
	Color c = {0,0,0,1};

	if (hex.length() == 3){
		c.r = (float)(HexToInt[hex[0]]*16 + HexToInt[hex[0]])/255;
		c.g = (float)(HexToInt[hex[1]]*16 + HexToInt[hex[1]])/255;
		c.b = (float)(HexToInt[hex[2]]*16 + HexToInt[hex[2]])/255;
	}else if (hex.length() == 6){
		c.r = (float)(HexToInt[hex[0]]*16 + HexToInt[hex[1]])/255;
		c.g = (float)(HexToInt[hex[2]]*16 + HexToInt[hex[3]])/255;
		c.b = (float)(HexToInt[hex[4]]*16 + HexToInt[hex[5]])/255;
	}

	return c;
}

Color RBG(float r, float g, float b){
	return Color {r/255, g/255, b/255, 1};
}

Color RGBA(float r, float g, float b, float a){
	return Color {r/255, g/255, b/255, a/255};
}

// Basic colors
Color White = Color{1,1,1,1};
Color Grey = Color{.5,.5,.5,1};
Color DarkGrey = Color{.1,.1,.1,1};
Color Black = Color{0,0,0,1};
Color Red = Color{1,0,0,1};
Color Green = Color{0,1,0,1};
Color Blue = Color{0,0,1,1};
Color Cyan = Color{0,1,1,1};
Color Purple = Color{1,0,1,1};
Color Yellow = Color{1,1,0,1};
Color Transparent = Color{0,0,0,0};