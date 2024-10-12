class Font{
	public:
		// Fonts function separatly from images as they are a tilemap of letter rather than a single image per character.
		// This script contains a texture coordinate map for the font image to select the correct character needed based on the letter given
		Image fontImg;
		Vector2 tileSize;
		Vector2 spacing;

		void Load(char* path){
			// Load image
			fontImg.LoadImage(path);
			tileSize = Vector2{32,32};
			spacing = Vector2{tileSize.x/2, tileSize.y};
		}

		void Write(string text, Vector2 position, float fontSize, Color color, bool fixed = false, float limit = Width, int align = 0, bool wrap = false){
			//printf("%s\n", text);

			if (align == 1)
				position.x = position.x + limit/2 - (text.length() * fontSize)/4;
			if (limit != Width)
				limit = position.x + limit;
			float off = 0;
			for (int c = 0; c < text.length(); c++){
				if (position.x + off >= limit)
					return;
				int i = (int)text[c]-33;
				if (i >= 0 && i < 128){
					Draw(Vector2{position.x + off, position.y}, Vector2{fontSize, fontSize}, color, i, fixed);
					off += spacing.x * (fontSize/tileSize.x);
				}else if (i == -1){
					off += spacing.x * (fontSize/tileSize.x);
				}
			}
		}

		void Draw(Vector2 position, Vector2 size, Color color, int i, bool fixed = false){
			size = size.Multiply(2);
			Vector2 screenSize = size;
			
			if (!fixed)
				screenSize = Vector2{size.x*Scale, size.y*Scale};

			position.x = position.x*2/Width;
			position.y = position.y*2/Height;
			if (!fixed){
				position.x -= View.x*2/Width;
				position.y -= View.y*2/Height;
			}

			position.x-=1;
			position.y-=1;
			if (!fixed)
				position = position.Multiply(Scale);

			
			float w = position.x + screenSize.x/Width;
			float h = position.y + screenSize.y/Height;	

			// Tilemap stuff
			float tX=0, tY=0;
			float tW = 1, tH = 1;
			if (i != -1){
				tX = (i * tileSize.x)/fontImg.width;
				while (tX >= 1){
					tY += tileSize.y/fontImg.height;
					tX -= 1;
				}
				tW = tX + tileSize.x/fontImg.width;
				tH = tileSize.y/fontImg.height + tY;
			}

			float data[] = {
				// positions       			// texture coords
		     	w,  h,						tW, tH, // top right
				w, position.y,  			tW, tY, // bottom right
				position.x, position.y,  	tX, tY, // bottom left
				position.x,  h, 			tX, tH  // top left 
			};
			
			// Indices used to draw a box
			int box[] = {
				0, 1, 3,
				1, 2, 3};

			// Bind array
			glBindVertexArray(vertexArray);

			// Bind vertex buffer
			glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

			// Send vertices to buffer
			glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);

			// Bind element buffer
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);

			// Send vertices to buffer
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6*sizeof(int), box, GL_STATIC_DRAW);

			// Position attribute
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);

			// Texture coord attribute
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
			glEnableVertexAttribArray(1);
			
			// Color
			glUniform4f(glGetUniformLocation(sImage.ID, "color"), color.r, color.g, color.b, color.a);

			glBindTexture(GL_TEXTURE_2D, fontImg.ID);
			glBindVertexArray(vertexArray);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}

};
Shader sFont;
Font font;