Image LoadWebp(string);

class ImageContainer{
	public:
		Vector2 position = Vector2{0,0};
		Vector2 size = Vector2{1,1};
		float angle = 0, prevAngle = 0;
		string path = "";
		bool vFlip = false, hFlip = false, selected = false, loaded = false;
		Image img;
		int width = 0, height = 0;
		unsigned char* imgData = nullptr;

		void Draw(){
			img.vFlip = vFlip;
			img.hFlip = hFlip;
			img.Draw(position, size, White, angle, POSITION_RELATIVE, size/2);
			if (selected)
				shape.DrawBox(position, size, imageSelectedColor, IMAGE_BORDER_SIZE/(*Scale), POSITION_RELATIVE, angle);
		}

		void Draw(Vector2 pos, Vector2 s, Color color = White, bool fixed = POSITION_RELATIVE){
			img.vFlip = vFlip;
			img.hFlip = hFlip;
			img.Draw(pos, s, color, angle, fixed, size/2);

			if (selected)
				shape.DrawBox(pos, s, imageSelectedColor, IMAGE_BORDER_SIZE, fixed, angle);
			
		}

		void Load(string p){
			path = p;
			loaded = false;
			if (path.substr(path.length()-4) != "webp")
				img.LoadImage(path);
			else
				img = LoadWebp(path);

			if (!img.loaded)
				return;
			
			loaded = true;
			size = {(float)img.width, (float)img.height};
		}

		void LoadPreview(string p){
			Load(p);
			ScaleImg({PREVIEW_IMAGE_SIZE, PREVIEW_IMAGE_SIZE});
		}

		void LoadImageData(string path){
			loaded = false;
			int channels;
			if (imgData) stbi_image_free(imgData);

			stbi_set_flip_vertically_on_load(true);
			imgData = stbi_load(path.c_str(), &width, &height, &channels, STBI_rgb_alpha);
		
			if (imgData){
				printf("[Image] Loaded %s: %dx%d\n", path.c_str(), width, height);
				loaded = true;
			}else{
				printf("[Image] Failed to load image: %s\n", path.c_str());
			}
		}

		// Loads a webp image
		Image LoadWebp(string path){
			Image i;
			ifstream f(path.c_str(), ios::in | ios::binary);

			if (!f.good()){
				if (DEBUG) printf("[Image] Invalid path: %s\n", path.data());
				return i;
			}
			string file = "";
			char c = 0;
			while (!f.eof()){
				f.read(&c, sizeof(char));
				file += c;
			}

			uint8_t* data = (uint8_t*)malloc(sizeof(uint8_t) * file.length());

			for (int x = 0; x < file.length(); x++)
				data[x] = int(file[x]);

			WebPBitstreamFeatures wf;
			WebPGetFeatures(data, file.length(), &wf);

			i.width = wf.width;
			i.height = wf.height;

			// Invert data
			unsigned char* temp = WebPDecodeRGBA(data, file.length(), NULL, NULL);
			unsigned char* img = (unsigned char*)malloc(sizeof(unsigned char)*i.width*i.height*4);
			int imgO = 0;
			for (int y = i.height-1; y >= 0; y--)
				for (int x = 0; x < i.width*4; x++){
					img[imgO] = temp[y*i.width*4+x];
					imgO++;
				}

			i.SetTexture(img, false);

			if (DEBUG) printf("[Image] loadedSave: %s\n", path.data());

			f.close();
			return i;
		}

		// Loads a webp image data
		void LoadWebpData(string path){
			loaded = false;
			if (imgData) stbi_image_free(imgData);

			ifstream f(path.c_str(), ios::in | ios::binary);

			if (!f.good()){
				if (DEBUG) printf("[Image] Invalid path: %s\n", path.data());
				return;
			}
			string file = "";
			char c = 0;
			while (!f.eof()){
				f.read(&c, sizeof(char));
				file += c;
			}

			uint8_t* data = (uint8_t*)malloc(sizeof(uint8_t) * file.length());

			for (int x = 0; x < file.length(); x++)
				data[x] = int(file[x]);

			WebPBitstreamFeatures wf;
			WebPGetFeatures(data, file.length(), &wf);

			width = wf.width;
			height = wf.height;

			// Invert data
			unsigned char* temp = WebPDecodeRGBA(data, file.length(), NULL, NULL);
			imgData = (unsigned char*)malloc(sizeof(unsigned char)*width*height*4);
			int imgIndex = 0;
			for (int y = height-1; y >= 0; y--)
				for (int x = 0; x < width*4; x++){
					imgData[imgIndex] = temp[y*width*4+x];
					imgIndex++;
				}

			if (DEBUG) printf("[Image] loaded: %s\n", path.data());
			f.close();

			loaded = true;
		}

		void SetTexture(){
			if (!loaded || !imgData) return;
			img.width = width;
			img.height = height;

			img.SetTexture(imgData);
			stbi_image_free(imgData);
		}

		void DrawOutline(float w, Color color){
			//img.DrawOutline(position, size, angle, w, color);
		}

		bool WithinWindow(Vector2 start, Vector2 end){
			// Move starting position to lower left corner
			if (start.x > end.x){
				int x = start.x;
				start.x = end.x;
				end.x = x;
			}
			if (start.y > end.y){
				int y = start.y;
				start.y = end.y;
				end.y = y;
			}

			start = (start - Vector2{fWidth/2, fHeight/2}) / (*Scale) + Vector2{fWidth/2, fHeight/2} + (*View);
			end = (end - Vector2{fWidth/2, fHeight/2}) / (*Scale) + Vector2{fWidth/2, fHeight/2} + (*View);

			Vector2 p = position;

			// Bounds check on corners
			if (p.Within(start, end-start) ||
				(p+Vector2{size.x, 0}).Within(start, end-start) ||
				(p+Vector2{0, size.y}).Within(start, end-start) ||
				(p+size).Within(start, end-start))
					return true;

			// Bounds check on edges
			if (((start.x <= p.x && end.x >= p.x || // Left edge
				start.x <= p.x+size.x && end.x >= p.x+size.x) && // Right edge
				start.y > p.y  && end.y < p.y + size.y) || // Within height

				(start.y <= p.y && end.y >= p.y || // Bottom edge
				start.y <= p.y+size.y && end.y >= p.y+size.y) &&
				(start.x > p.x && end.x < p.x+size.x)) // Top edge
				return true;
			return false;
		}

		void ScaleImg(Vector2 bounds, bool ratio = true){
			if (bounds.x <= 0)
				bounds.x = 1;
			if (bounds.y <= 0)
				bounds.y = 1;

			if (ratio){
				if (bounds.x/(float)img.width < bounds.y/(float)img.height){
					size.y = (float)img.height * (bounds.x/(float)img.width);
					size.x = bounds.x;
				}else{
					size.x = (float)img.width * (bounds.y/(float)img.height);
					size.y = bounds.y;
				}
			}else
				size = bounds;
		}
};

void SetTextureRBG(unsigned char* data, Image* img){
	glGenTextures(1, &(img->ID));
	glBindTexture(GL_TEXTURE_2D, img->ID);

	// Set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// Set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Load texture into memory
	glBindTexture(GL_TEXTURE_2D, img->ID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img->width, img->height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	img->loaded = true;
}