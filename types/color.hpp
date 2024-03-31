class Color{
	public:
		float r, g, b, a;
		
		bool Equal(Color c){
			return (c.r == r && c.b == b && c.g == g && c.a == a);
		}
};
