Vector2 ScreenSpace(Vector2);

class Image{
	public:
		unsigned int ID;
		int width, height, channels; // Image variables
		int trueWidth, trueHeight; // The original value of an image
		float angle=0, prevAngle=0; // Angle to draw image at
		Vector2 bounds; // Boundary when rotating
		Vector2 size; // Width and height
		Vector2 position; // Location
		Vector2 tileSize; // Size of a tile in a tile sheet
		Vector2 screenPos; // Position on the screen
		Vector2 tR, tL, bR, bL;
		Vector2 pTL, pTR, pBL, pBR;
		bool selected = false; // If the image is selected
		bool loaded = false;
		bool hFlip = false;
		bool vFlip = false;
		string path = "";

		void Draw(){
			Draw(position, size, White);
		}

		void Draw(Vector2 position, Vector2 size, Color color = White, bool fixed = false){
			Draw(position, size, color, -1, fixed);
		}

		// Draws to pixel size
		void Draw(Vector2 position, Vector2 size, Color color, int i, bool fixed = false){
			size = size.Multiply(2); // Compensates for -1:1 grid size
			tR = Vector2{!hFlip ? position.x+size.x/2 : position.x, !vFlip ? position.y+size.y/2 : position.y};
			tL = Vector2{!hFlip ? position.x : position.x+size.x/2, !vFlip ? position.y+size.y/2 : position.y};
			bR = Vector2{!hFlip ? position.x+size.x/2 : position.x, !vFlip ? position.y : position.y+size.y/2};
			bL = Vector2{!hFlip ? position.x : position.x+size.x/2, !vFlip ? position.y : position.y+size.y/2};
			pTL = tL;
			pTR = tR;
			pBR = bR;
			pBL = bL;

			if (angle != 0){
				if (angle < 0)
					angle += 360*(float)RADIANS;
				else if (angle > 360*RADIANS)
					angle -= 360*(float)RADIANS;
				Vector2 o = Vector2{tL.x+(tR.x-tL.x)/2, bL.y+(tL.y-bL.y)/2};
				tR = Rotate(tR, o);
				tL = Rotate(tL, o);
				bR = Rotate(bR, o);
				bL = Rotate(bL, o);
				pTL = tL;
				pTR = tR;
				pBR = bR;
				pBL = bL;
			}

			// Converts pixels to screen space
			if (!fixed){
				tR = Vector2{tR.x*2/Width-1,tR.y*2/Height-1}.Subtract(View.Multiply(2).Divide(Width, Height)).Multiply(Scale);
				tL = Vector2{tL.x*2/Width-1,tL.y*2/Height-1}.Subtract(View.Multiply(2).Divide(Width, Height)).Multiply(Scale);
				bR = Vector2{bR.x*2/Width-1,bR.y*2/Height-1}.Subtract(View.Multiply(2).Divide(Width, Height)).Multiply(Scale);
				bL = Vector2{bL.x*2/Width-1,bL.y*2/Height-1}.Subtract(View.Multiply(2).Divide(Width, Height)).Multiply(Scale);
			}else{
				// Multiplied by 2 to offset properly otherwise it will only move half
				tR = Vector2{tR.x*2/Width-1,tR.y*2/Height-1};
				tL = Vector2{tL.x*2/Width-1,tL.y*2/Height-1};
				bR = Vector2{bR.x*2/Width-1,bR.y*2/Height-1};
				bL = Vector2{bL.x*2/Width-1,bL.y*2/Height-1};
			}


			// Tilemap stuff
			float tX=0, tY=0;
			float tW = 1, tH = 1;
			if (i != -1){
				tX = (i * tileSize.x)/width;
				while (tX >= 1){
					tY += tileSize.y/height;
					tX -= 1;
				}
				tW = tX + tileSize.x/width;
				tH = tileSize.y/height + tY;
			}

			float data[] = {
				// positions       	// texture coords
		     	tR.x, tR.y,			tW, tH, // top right
				bR.x, bR.y,  		tW, tY, // bottom right
				bL.x, bL.y,  		tX, tY, // bottom left
				tL.x, tL.y, 		tX, tH  // top left 
			};
			float min = tR.x, max = tR.x;
			if (tL.x > max)
				max = tL.x;
			if (bR.x > max)
				max = bR.x;
			if (bL.x > max)
				max = bL.x;
			if (tL.x < min)
				min = tL.x;
			if (bR.x < min)
				min = bR.x;
			if (bL.x < min)
				min = bL.x;
			bounds.x = (max-min)*size.x;
			max = tR.y;
			min = tR.y;
			if (tL.y > max)
				max = tL.y;
			if (bR.y > max)
				max = bR.y;
			if (bL.y > max)
				max = bL.y;
			if (tL.y < min)
				min = tL.y;
			if (bR.y < min)
				min = bR.y;
			if (bL.y < min)
				min = bL.y;
			bounds.y = (max-min)*size.y;

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

			glBindTexture(GL_TEXTURE_2D, ID);
			glBindVertexArray(vertexArray);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}

		void DrawOutline(int w, Color c){
			// Top Left to Top Right
			shape.DrawLine(pTL, pTR, c, w, false);

			// Bottom Left to Top Left
			shape.DrawLine(pBL, pTL, c, w, false);

			// Bottom Left to Bottom Right
			shape.DrawLine(pBL, pBR, c, w, false);
			
			// Bottom Right to Top Right
			shape.DrawLine(pBR, pTR, c, w, false);
		}

		void ScaleImg(Vector2 bounds, bool ratio = true){
			if (bounds.x <= 0)
				bounds.x = 1;
			if (bounds.y <= 0)
				bounds.y = 1;

			if (ratio){
				if (bounds.x/(float)trueWidth < bounds.y/(float)trueHeight){
					size.y = (float)trueHeight * (bounds.x/(float)trueWidth);
					size.x = bounds.x;
				}else{
					size.x = (float)trueWidth * (bounds.y/(float)trueHeight);
					size.y = bounds.y;
				}
			}else
				size = bounds;
		}

		void SetTexture(unsigned char* data){
			// Loads texture into memory
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			trueWidth = width;
			trueHeight = height;
			glGenerateMipmap(GL_TEXTURE_2D);
		}

		bool LoadImage(char* p, bool nearest = false){
			loaded = false;
			stbi_set_flip_vertically_on_load(true);
			glGenTextures(1, &ID);
			glBindTexture(GL_TEXTURE_2D, ID);

			// set the texture wrapping parameters
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

			// set texture filtering parameters
			if (nearest){
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			}else{
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			}

			unsigned char* data = stbi_load(p, &width, &height, &channels, STBI_rgb_alpha);
		
			if (data){
				printf("[Image] Loaded %s: %dx%d\n", p, width, height);
				size = Vector2{(float)width, (float)height};
				SetTexture(data);
				loaded  = true;
				path = p;
			}else{
				printf("[Image] Failed to load image: %s\n", p);
			}
			stbi_image_free(data);
			return loaded;
		}

		void GenTexture(unsigned char* data){
			glGenTextures(1, &ID);
			glBindTexture(GL_TEXTURE_2D, ID);

			// set the texture wrapping parameters
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

			// set texture filtering parameters
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

			SetTexture(data);
			loaded = true;
		}

		unsigned char* Tile(int start, int newWidth, int newHeight, int imgWidth){
			//printf("%d:%d\n", w, h);
			glBindTexture(GL_TEXTURE_2D, ID);
			unsigned char *output = (unsigned char*) malloc(sizeof(unsigned char) * newWidth * newHeight * 4);
			unsigned char *pixels = (unsigned char*) malloc(sizeof(unsigned char) * trueWidth * trueHeight * 4);
			glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
			int i = 0;
			for (int y = 0; y < newHeight; y++)
				for (int x = 0; x < newWidth*4; x++){
					output[i] = pixels[start*4+x+(y*imgWidth*4)];
					i++;
				}
			return output;
		}

		bool Empty(){
			glBindTexture(GL_TEXTURE_2D, ID);
			unsigned char *pixels = (unsigned char*) malloc(sizeof(unsigned char) * (trueWidth * trueHeight) * 4);
			glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
			for (int i = 3; i < trueWidth * trueHeight * 4; i+=4)
				if (pixels[i] != 0)
					return false;
			return true; 
		}

	private:
		int CheckImage(char* path){
			char ext[5];
			ext[0] = path[strlen(path)-4];
			ext[1] = path[strlen(path)-3];
			ext[2] = path[strlen(path)-2];
			ext[3] = path[strlen(path)-1];
			ext[4] = '\0';
			return !strcmp(ext, ".png")*1 + !strcmp(ext, "jpeg")*2 + !strcmp(ext, ".jpg")*2;
		}

		Vector2 Rotate(Vector2 position, Vector2 origin){
			Vector2 output;

			output.x = ((position.x - origin.x) * cos(angle))
				-((position.y - origin.y) * sin(angle));
			output.y = ((position.x - origin.x) * sin(angle))
				+ ((position.y - origin.y) * cos(angle));
			output = output.Add(origin);
			float newA = atan(output.x/output.y)*(float)DEGREES;	
			
			return output;
		}
};

Vector2 ScreenSpace(Vector2 position){
	return position.Multiply(Scale)
					.Add(
						Vector2{(float)Width/2, (float)Height/2}
						.Subtract(Vector2{(float)Width/2, (float)Height/2}
						.Multiply(Scale)))
					.Subtract(View.Multiply(Scale));
}