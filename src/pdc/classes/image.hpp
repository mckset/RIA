Vector2 ScreenSpace(Vector2);

class Image{
	public:
		unsigned int ID;
		int width=0, height=0, channels=0; // Image variables
		bool loaded = false;
		bool hFlip = false;
		bool vFlip = false;
		bool filter = true;
		string path = "";

		// Draws to pixel size
		void Draw(Vector2 position, Vector2 size, Color color = White, float angle = 0, bool fixed = false){
			if (!loaded)
				return;
					
			sImage.Use();

			Vector2 tR = Vector2{!hFlip ? position.x + size.x : position.x, !vFlip ? position.y + size.y : position.y};
			Vector2 tL = Vector2{!hFlip ? position.x : position.x + size.x, !vFlip ? position.y + size.y : position.y};
			Vector2 bR = Vector2{!hFlip ? position.x + size.x : position.x, !vFlip ? position.y : position.y + size.y};
			Vector2 bL = Vector2{!hFlip ? position.x : position.x + size.x, !vFlip ? position.y : position.y + size.y};

			if (angle != 0){
				if (angle < 0)
					angle += 360;
				else if (angle > 360*RADIANS)
					angle -= 360;
				float a = angle*RADIANS;
				Vector2 center = position + size/2;
				tR.Rotate(center, a);
				tL.Rotate(center, a);
				bR.Rotate(center, a);
				bL.Rotate(center, a);
			}

			// Converts pixels to screen space
			if (!fixed){
				tR = Vector2{tR.x*2/Width-1,tR.y*2/Height-1}.Subtract(View->Multiply(2).Divide(Width, Height)).Multiply(*Scale);
				tL = Vector2{tL.x*2/Width-1,tL.y*2/Height-1}.Subtract(View->Multiply(2).Divide(Width, Height)).Multiply(*Scale);
				bR = Vector2{bR.x*2/Width-1,bR.y*2/Height-1}.Subtract(View->Multiply(2).Divide(Width, Height)).Multiply(*Scale);
				bL = Vector2{bL.x*2/Width-1,bL.y*2/Height-1}.Subtract(View->Multiply(2).Divide(Width, Height)).Multiply(*Scale);
					
			}else{
				// Multiplied by 2 to offset properly otherwise it will only move half
				tR = Vector2{tR.x*2/Width-1,tR.y*2/Height-1};
				tL = Vector2{tL.x*2/Width-1,tL.y*2/Height-1};
				bR = Vector2{bR.x*2/Width-1,bR.y*2/Height-1};
				bL = Vector2{bL.x*2/Width-1,bL.y*2/Height-1};
			}

			float data[] = {
				// positions       	// texture coords
				tR.x, tR.y,			1, 1, // top right
				bR.x, bR.y,  		1, 0, // bottom right
				bL.x, bL.y,  		0, 0, // bottom left
				tL.x, tL.y, 		0, 1 // top left 
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
			glUniform4f(glGetUniformLocation(ShaderID, "color"), color.r, color.g, color.b, color.a);
			
			glBindTexture(GL_TEXTURE_2D, ID);
			glBindVertexArray(vertexArray);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}

		void DrawOutline(Vector2 position, Vector2 size, float angle, int w, Color c){
			Vector2 tR = Vector2{!hFlip ? position.x + size.x : position.x, !vFlip ? position.y + size.y : position.y};
			Vector2 tL = Vector2{!hFlip ? position.x : position.x + size.x, !vFlip ? position.y + size.y : position.y};
			Vector2 bR = Vector2{!hFlip ? position.x + size.x : position.x, !vFlip ? position.y : position.y + size.y};
			Vector2 bL = Vector2{!hFlip ? position.x : position.x + size.x, !vFlip ? position.y : position.y + size.y};

			if (angle != 0){
				if (angle < 0)
					angle += 360;
				else if (angle > 360*RADIANS)
					angle -= 360;
				float a = angle*RADIANS;
				Vector2 center = position + size/2;
				tR.Rotate(center, a);
				tL.Rotate(center, a);
				bR.Rotate(center, a);
				bL.Rotate(center, a);
			}

			// Top Left to Top Right
			shape.DrawLine(tL, tR, c, w, false);

			// Bottom Left to Top Left
			shape.DrawLine(bL, tL, c, w, false);

			// Bottom Left to Bottom Right
			shape.DrawLine(bL, bR, c, w, false);
				
			// Bottom Right to Top Right
			shape.DrawLine(bR, tR, c, w, false);
		}

		bool LoadImage(string p, bool nearest = false){
			filter = !nearest;
			loaded = false;
			stbi_set_flip_vertically_on_load(true);
			unsigned char* data = stbi_load(p.c_str(), &width, &height, &channels, STBI_rgb_alpha);
			
			if (data){
				SetTexture(data, nearest);
				if (PDC_DEBUG) printf("[Image] Loaded %s: %dx%d\n", p.c_str(), width, height);
				//path = p;
			}else{
				if (PDC_DEBUG) printf("[Image] Failed to load image: %s\n", p.c_str());
			}
			stbi_image_free(data);
			return loaded;
		}

		void SetTexture(unsigned char* data, bool nearest = false, int mode = GL_RGBA){
			glGenTextures(1, &ID);
			glBindTexture(GL_TEXTURE_2D, ID);

			// Set the texture wrapping parameters
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

			// Set texture filtering parameters
			if (nearest){
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			}else{
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			}

			// Load texture into memory
			glBindTexture(GL_TEXTURE_2D, ID);
			glTexImage2D(GL_TEXTURE_2D, 0, mode, width, height, 0, mode, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
			loaded = true;
		}

		unsigned char* Tile(int start, int newWidth, int newHeight, int imgWidth){
			glBindTexture(GL_TEXTURE_2D, ID);
			unsigned char *output = (unsigned char*) malloc(sizeof(unsigned char) * newWidth * newHeight * 4);
			unsigned char *pixels = (unsigned char*) malloc(sizeof(unsigned char) * width * height * 4);
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
			unsigned char *pixels = (unsigned char*) malloc(sizeof(unsigned char) * (width * height) * 4);
			glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
			for (int i = 3; i < width * height * 4; i+=4)
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

		Vector2 Rotate(Vector2 position, Vector2 center, float angle){
			float rX = cos(angle) * (position.x - center.x) - sin(angle) * (position.y-center.y) + center.x;
			float rY = sin(angle) * (position.x - center.x) + cos(angle) * (position.y - center.y) + center.y;
			return Vector2{rX,rY};
		}
};

Vector2 ScreenSpace(Vector2 position){
	return position*(*Scale)
			+ ((Vector2{fWidth, fHeight}/2) - (Vector2{fWidth, fHeight}/2) * (*Scale))
			- View->Multiply((*Scale));
}