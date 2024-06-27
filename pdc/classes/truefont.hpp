#ifdef _WIN32
	#include "../../outsourced/freetype/ft2build.h"
	#include FT_FREETYPE_H
	
#else
	#include <ft2build.h>
	#include FT_FREETYPE_H
#endif

FT_Library  ft;
FT_Face     face;

void AddChar(int);
int UTF16(string);
void CheckString(string);
void InitFont();

// Unicode Character
class Unichar{
	public:
		uint ID;
		uint advance;
		Vector2 size;
		Vector2 origin;
};

map<int, Unichar> characters;
Shader sFont;

int fontResolution = 48;
float fontSpacing = 1;

class Font{
	public:

		void Write(int* text, int text_s, Vector2 position, float size, Color color, bool fixed = false, float limit = Width, int align = 0, bool wrap = false){
			int length = 0;
			char temp[text_s];
			for (int i = 0; i < text_s && length < limit; i++){
				temp[i] = text[i];
				length += Draw(text[i], position.Add(length, 0), size, color);
			}
		}

		void Write(string text, Vector2 position, float size, Color color, bool fixed = false, float limit = Width, int align = 0, bool wrap = false){
			int length = 0;
			if (limit != Width && text.length()*size > limit)
				text = text.substr(0, (int)(limit/size));
			for (int i = 0; i < text.length(); i++){
				if (text[i] == '\n' || text[i] == '\t')
					text[i] = ' ';
			}
			float offset = 0;
			if (align == 1)
				offset = (float)(limit/2 - text.length()*size/2);
			
			for (int i = 0; i < text.length() && i*size + offset < limit; i++){
				length += Draw(text[i], position.Add(length + offset, size/2), size, color);
			}
		}

		int Draw(int ch, Vector2 position, float fontSize, Color color){
			sFont.Use();
			Unichar c = characters[ch];

			fontSize /= 32;


			position.x += c.origin.x * fontSize;
			position.y -= (c.size.y - c.origin.y)*fontSize;
			position.y += fontSize;

			position.x = position.x*2/fWidth;
			position.y = position.y*2/fHeight;

			position.x-=1;
			position.y-=1;
				
			float w = c.size.x*fontSize;
			float h = c.size.y*fontSize;
			w *= 2/fWidth;
			h *= 2/fHeight;

			float vertices[6][4] = {
				{position.x,		position.y + h,   0.0f, 0.0f },            
				{position.x,   		position.y,       0.0f, 1.0f },
				{position.x + w,	position.y,       1.0f, 1.0f },
				{position.x,     	position.y + h,   0.0f, 0.0f },
				{position.x + w, 	position.y,       1.0f, 1.0f },
				{position.x + w, 	position.y + h,   1.0f, 0.0f }           
			};

			// Bind array
			glBindVertexArray(vertexArray);
			glBindTexture(GL_TEXTURE_2D, c.ID);
			// Bind vertex buffer
			glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); 
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			// Color
			glUniform3f(glGetUniformLocation(sFont.ID, "textColor"), color.r, color.g, color.b);
			// render quad

			glDrawArrays(GL_TRIANGLES, 0, 6);
			
			return (c.advance >> 6) * fontSize*1.145;
		}

		private:
			const float defWidth = 640;
			const float defHeight = 480;
};

void AddChar(int c){
	// load character glyph 
	if (FT_Load_Char(face, c, FT_LOAD_RENDER)){
		printf("Failed to load character %c %d\n", c, c);
		return;
	}
			
	// generate texture
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		GL_RED,
		face->glyph->bitmap.width,
		face->glyph->bitmap.rows,
		0,
		GL_RED,
		GL_UNSIGNED_BYTE,
		face->glyph->bitmap.buffer
	);
	// set texture options
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	if (texture == 0)
		return;
		
	Unichar ch;
	ch.ID = texture;
	ch.advance = face->glyph->advance.x;
	ch.size = Vector2{(float)face->glyph->bitmap.width, (float)face->glyph->bitmap.rows};
	ch.origin = Vector2{(float)face->glyph->bitmap_left, (float)face->glyph->bitmap_top};
	characters.insert(pair<int, Unichar>(c, ch));
	//printf("%c) %c %f\n", c, ch.origin.y);
}

int UTF16(string s){
	return stoi(s);
}

void CheckString(string s){
	char data[s.length()];
	for (int c = 0; c < s.length(); c++)
		data[c] = s[c];
	for (int i = 0; i < s.length(); i++){
		if (s[i] < 0 || s[i] > 127){
			bool checked = false;
			for (int e = i+1; e < s.length(); e++){
				if ((s[e] >= '\300' && s[e] <= '\3FF') || (s[e] <= 127 && s[e] >= 32)){
					string sub = s.substr(i, e-i);
					int ch = UTF16(sub.data());
					if (characters.find(ch) == characters.end()){
						printf("Added character: %s : %d\n", sub.data(), ch);
						AddChar(ch);
					}
					i=e-1;
					checked = true;
					break;
				}
			}
			if (!checked){
				int ch = UTF16(s.substr(i, s.length()-i));
				if (characters.find(ch) == characters.end()){
					printf("Added character: %s : %d\n", s.substr(i, s.length()-i).data(), ch);
					AddChar(ch);
				}
				break;
			}
		}
	}
}

void InitFont(){
	FT_Set_Pixel_Sizes(face, 0, fontResolution);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction
		
	ifstream f;
	f.open("font.cache", ios::in | ios::binary);
	if (!f.good()){
		printf("[Loading] No cache data found\n");
		// Add english alphabet
		for (int c = 0; c < 127; c++)
			AddChar(c);
	}else{
		char header[4];
		f.read(header, 4);
		if (strcmp(header, "font"))
			return;
		
		int size = 0;
		int buf = 0;
		f.read(reinterpret_cast<char*>(&size), 4);
		for (int i = 0; i < size; i++){
			f.read(reinterpret_cast<char*>(&buf), 4);
			AddChar(buf);
		}
	}
}
Font font;