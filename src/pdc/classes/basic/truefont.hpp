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
		uint ID = 0;
		uint advance;
		Vector2 size;
		Vector2 origin;
};

map<int, Unichar> characters;
Shader sFont;
int fontResolution = 64;
float fontSpacing = 1;


class Font{
	public:
		bool loaded = false;

		Vector2 Write(string text, Vector2 position, float size, Color color, bool fixed = false, float limit = Width, int align = ALIGN_LEFT, bool wrap = false){
			if (!loaded) return {0,0};
			
			// Gets the length of the text
			float length = 0;
			float fontSize = size;
			float offset = 0;
			fontSize /= 32; // Magic numbers
			fontSize *= 48.0f/(float)fontResolution;
			for (int i = 0; i < text.length(); i++){
				int ch = text[i];
				if (text[i] < 0){
					string c = "";
					c+= text[i];
					i++;
					while (text[i] <= -65){
						c += text[i];
						i++;
					}
					i--;
					ch = UTF16(c);
				}
				Unichar c = characters[ch];
				length += (c.advance >> 6) * fontSize*1.145;
			}

			
			if (!wrap){
				// Crop the text if text bleeds past the width
				if (text.length()*size > limit && limit != Width){
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
				}else if (align == ALIGN_CENTER)
					offset = (float)(limit - length)/2;
				else if (align == ALIGN_RIGHT)
					offset = (float)(limit - length - size);
			}
					
			
			length = 0;

			// Normal text
			if (!wrap){
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
						length += Draw(UTF16(c), position.Add(length + offset, size/2), size, color, fixed);
					}else
						length += Draw(text[i], position.Add(length + offset, size/2), size, color, fixed);
				}
				return {length, fontSize*LINE_SPACING};
			}

			// Wrapped text
			int lines=0;
			for (int i = 0; i < text.length(); i++){
				string word = "";
				
				// Get next word
				for (i; i < text.length() && text[i] != ' ' && text[i] != '\n' && text[i] != '\t'; i++)
					word += text[i];
				if (i < text.length()) word += text[i];

				// Get length of word
				float wordSize = 0;
				for (int x = 0; x < word.length(); x++){
					if (word[x] < 0){
						string c = "";
						c+= word[x];
						x++;
						while (word[x] <= -65){
							c += word[x];
							x++;
						}
						x--;
						wordSize += GlyphSize(UTF16(c), size);
					}else								
						wordSize += GlyphSize(word[x], size);
				}

				// Warp if it bleeds pass limit
				if (wordSize + length >= limit){

					lines++;
					// Word is too big for the screen at the starting position
					if (length == 0){
						for (int x = 0; x < word.length(); x++){
							if (word[x] < 0){
								string c = "";

								c+= word[x];
								x++;
								while (word[x] <= -65){
									c += word[x];
									x++;
								}
								x--;
								int charSize = GlyphSize(UTF16(c), size);
								if (position.x + charSize > limit){
									position.y -= size*LINE_SPACING;
								}
								length += Draw(UTF16(c), position.Add(length + offset, size/2), size, color, fixed);
							}else{
								int charSize = GlyphSize(word[x], size);
								if (position.x + charSize > limit){
									position.y -= size*LINE_SPACING;
								}
								length += Draw(word[x], position.Add(length + offset, size/2), size, color, fixed);
							}
						}

					// Move line down
					}else{
						length=0;
						position.y -= size*LINE_SPACING;
						for (int x = 0; x < word.length(); x++){
							if (word[x] < 0){
								string c = "";
								c+= word[x];
								x++;
								while (word[x] <= -65){
									c += word[x];
									x++;
								}
								x--;
								length += Draw(UTF16(c), position.Add(length + offset, size/2), size, color, fixed);
							}else
								length += Draw(word[x], position.Add(length + offset, size/2), size, color, fixed);
						}
					}
					
				// Normal
				}else{
					for (int x = 0; x < word.length(); x++){
						if (word[x] < 0){
							string c = "";
							c+= word[x];
							x++;
							while (word[x] <= -65){
								c += word[x];
								x++;
							}
							x--;
							length += Draw(UTF16(c), position.Add(length + offset, size/2), size, color, fixed);
						}else
							length += Draw(word[x], position.Add(length + offset, size/2), size, color, fixed);
					}
				}
				// Handle escape characters
				if (word[word.length()-1] == '\n'){
					position.y -= size*LINE_SPACING;
					length = 0;
					lines++;
				}

				if (word[word.length()-1] == '\t')
					for (int s = 0; s < TAB_SIZE; s++)
						length += Draw(' ', position.Add(length + offset, size/2), size, color, fixed);
			}
			return Vector2{length, lines*LINE_SPACING*size};
		}

		float GlyphSize(int ch, float fontSize){
			Unichar c = characters[ch];
			if (!c.ID) return 0;
			fontSize /= 32;
			fontSize *= 48.0f/(float)fontResolution;
			return (c.advance >> 6) * fontSize*1.145;
		}

		float Draw(int ch, Vector2 position, float fontSize, Color color, bool fixed = false){
			if (ch == '\n' || ch == '\t') return 0;

			sFont.Use();
			Unichar c = characters[ch];
			if (!c.ID) return 0;

			fontSize /= 32;
			fontSize *= 48.0f/(float)fontResolution;

			position.x += c.origin.x * fontSize;
			position.y -= (c.size.y - c.origin.y)*fontSize;
			position.y += fontSize;


			position.x = position.x*2/fWidth;
			position.y = position.y*2/fHeight;

			position.x-=1;
			position.y-=1;
			
			if (!fixed){
				position = position - ((*View) * 2 / Vector2{fWidth, fHeight}) * (*Scale);
			}
				
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
Font font;


void AddChar(int c){
	if (!font.loaded) return;
	
	// load character glyph 
	if (FT_Load_Char(face, c, FT_LOAD_RENDER)){
		printf("Failed to load character %c\n", c);
		return;
	}
			
	// generate texture
	Unichar ch;
	ch.advance = face->glyph->advance.x;
	ch.size = Vector2{(float)face->glyph->bitmap.width, (float)face->glyph->bitmap.rows};
	ch.origin = Vector2{(float)face->glyph->bitmap_left, (float)face->glyph->bitmap_top};

	glGenTextures(1, &ch.ID);
	glBindTexture(GL_TEXTURE_2D, ch.ID);
	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		GL_RED,
		(int)ch.size.x,
		(int)ch.size.y,
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

	characters[c] = ch;
}


int UTF16(string s){
	wstring_convert<codecvt_utf8_utf16<char16_t>,char16_t> convert;
	u16string dest = convert.from_bytes(s);
	uint utf16 = dest[0];
	return utf16;
}


void CheckString(string s){
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); 
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
			if (!characters[ch].ID){
				printf("Added character: %s : %u\n", char16.data(), ch);
				AddChar(ch);
			}
		}
	}
}

void InitFont(){
	FT_Set_Pixel_Sizes(face, 0, fontResolution);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction
		
	// Cache english alphabet
	for (int c = 0; c < 127; c++)
		AddChar(c);

}