#include <ft2build.h>
#include FT_FREETYPE_H
FT_Library  ft;
FT_Face     face;

void AddChar(int);
int UTF16(string);
void CheckString(string);
void InitFont();
int GetUTF16Length(string);

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

int fontResolution = 128;
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

		float Write(string text, Vector2 position, float size, Color color, bool fixed = false, float limit = Width, int align = 0, bool wrap = false){
			int length = 0;

			// Crop the text if center aligned in case the text bleeds past the width
			if (align && text.length()*size > limit && limit != Width){
				int textSize = (int)(limit/size);
				string cropped = "";
				for (int i = 0; i < text.length() && textSize > 0; i++){
					if (text[i] < 0){
						cropped += text[i];
						i++;
						while (text[i] <= -65){
							cropped += text[i];
							i++;
						}
						i--;
					}else
						cropped += text[i];
					textSize--;
				}
				text = cropped;
			}

			char data[text.length()];
			for (int i = 0; i < text.length(); i++){
				if (text[i] == '\n' || text[i] == '\t')
					text[i] = ' ';
			}
			float offset = 0;
			if (align == 1)
				offset = (float)(limit/2 - text.length()*size/2);
			
			for (int i = 0; i < text.length() && length + offset + size < limit; i++){
				if (text[i] < 0){
					string c = "";
					c+= text[i];
					i++;
					while (text[i] <= -65){
						c += text[i];
						i++;
					}
					i--;
					length += Draw(UTF16(c), position.Add(length + offset, size/2), size, color);
				}else
					length += Draw(text[i], position.Add(length + offset, size/2), size, color);
			}
			return length;
		}

		int Draw(int ch, Vector2 position, float fontSize, Color color){
			sFont.Use();
			Unichar c = characters[ch];

			fontSize /= 32;
			fontSize *= 48.0f/(float)fontResolution;

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
};

void AddChar(int c){
		
	// load character glyph 
	if (FT_Load_Char(face, c, FT_LOAD_RENDER)){
		printf("Failed to load character %c\n", c);
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
}

int UTF16(string s){
	std::wstring_convert<std::codecvt_utf8_utf16<char16_t>,char16_t> convert;
	std::u16string dest = convert.from_bytes(s);
 
	uint utf16 = 0;
	utf16 = dest[0];

	return utf16;
}


void CheckString(string s){
	char data[s.length()];
	for (int c = 0; c < s.length(); c++)
		data[c] = s[c];

	

	for (int i = 0; i < s.length(); i++){
		if (s[i] < 0){
			string char16 = "";
			char16 += s[i];
			i++;
			while (s[i] <= -65){
				char16 += s[i];
				i++;
			}
			i--;
			int ch = UTF16(char16);
			if (characters.find(ch) == characters.end()){
				if (PDC_DEBUG) printf("Added character: %s : %u\n", char16.data(), ch);
				AddChar(ch);
			}
		}else if (s[i] > 127){
			printf("\n");
		}
	}
}

void InitFont(){
	FT_Set_Pixel_Sizes(face, 0, fontResolution);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction
		
	// Add english alphabet
	for (int c = 0; c < 127; c++)
		AddChar(c);
}
Font font;