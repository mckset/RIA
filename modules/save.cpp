#include<iostream>
#include<fstream>

ifstream f;

Color GetColor(string s){
	Color out = Color{0,0,0,1};
	int start = 1;
	int end = 1;
	for (int c = 0; c < 4; c++){
		while (s.substr(end, 1) != "," && s.substr(end, 1) != "}")
			end++;
		if (c == 0)
			out.r = (float)stoi(s.substr(start, end))/255;
		else if (c == 1)
			out.g = (float)stoi(s.substr(start, end))/255;
		else if (c == 2)
			out.b = (float)stoi(s.substr(start, end))/255;
		else
			out.a = (float)stoi(s.substr(start, end))/255;
		end++;
		while (s.substr(end, 1) == " ")
			end++;
		start = end;
	}
	return out;
}

string GetLine(){
	int size = 0;
	string temp;
	char c;
	f.read(reinterpret_cast<char*>(&c), 1);
	while (c != '\n'){
		temp += c;
		f.read(reinterpret_cast<char*>(&c), 1);
	}
	return temp;
}

string GetName(string path){
	for (int i = path.length()-1; i > 0; i--)
		if (path[i] == slash[0])
			return path.substr(i+1);
	return path;
}

string GetString(){
	int size = 0;
	string temp;
	f.read(reinterpret_cast<char*>(&size), 4);
	char out[size+1];
	f.read(out, size);
	out[size] = '\0';
	temp = out;
	
	// Alter backslashes to match the OS
	for (int c = 0; c < temp.length(); c++)
		if (temp[c] == (slash[0] == '/' ? '\\' : '/'))
			temp[c] = slash[0];
	
	return temp;
}

bool Duplicate(string img, vector<File>* list){
	for (auto file : *list)
		if (file.path == img) return true;
	return false;
}

void Load(){
	f.open("save.dat", ios::in | ios::binary);
    if (!f.good()){
		if (DEBUG) printf("[Loading] No save data found\n");
		return;
	}

	struct stat st;
	int ibuffer, cbuffer;

	// Read past header
	f.read(reinterpret_cast<char*>(&ibuffer), 4);

	if (DEBUG)	printf("[Loading] Locations\n");

	// Locations
	f.read(reinterpret_cast<char*>(&ibuffer), 4);
	int locationSize = ibuffer;
	for (int l = 0; l < locationSize; l++){
		string loc = GetString();
		locations.push_back(Table{GetName(loc), loc});
		if (DEBUG) printf("%d) %s\n", l, loc.data());
	}

	if (DEBUG) printf("[Loading] Sorting locations\n");
	sort(locations.begin(), locations.end(), locations[0].SortTable);

	
	// Tags
	f.read(reinterpret_cast<char*>(&ibuffer), 4);
	int tagSize = ibuffer;
	if (DEBUG) printf("[Loading] Tags (%d)\n", tagSize);
	for (int t = 0; t < tagSize; t++){
		Tag tag;

		// Name
		tag.name = GetString();
		if (DEBUG) printf("[TAG] %s", tag.name.data());

		// Color
		f.read(reinterpret_cast<char*>(&ibuffer), 1);
		tag.color.r = (float)ibuffer/255;
		f.read(reinterpret_cast<char*>(&ibuffer), 1);
		tag.color.g = (float)ibuffer/255;
		f.read(reinterpret_cast<char*>(&ibuffer), 1);
		tag.color.b = (float)ibuffer/255;
		tag.color.a = 1;

		// Images
		f.read(reinterpret_cast<char*>(&ibuffer), 4);
		if (DEBUG) printf("\tImgs: %d", ibuffer);
		for (int i = 0; i < ibuffer; i++){
			string s = GetString();
			if (!Duplicate(s, &tag.imgs) && stat(s.c_str(), &st) == 0)
				tag.imgs.push_back(File{GetName(s), s});
			else
				if (DEBUG) printf("\nUnable to load: %s\n", s.data());
		}

		sort(tag.imgs.begin(), tag.imgs.end(), SortFile);
		
		// Sub tags
		f.read(reinterpret_cast<char*>(&ibuffer), 4);
		int subSize = ibuffer;
		if (DEBUG) printf("\tSub Tags: %d\n", subSize);

		// Repeat everything
		for (int i = 0; i < subSize; i++){
			Tag subTag;

			// Name
			subTag.name = GetString();
			if (DEBUG) printf("\t[SUB TAG] %s ", subTag.name.data());


			// Color
			f.read(reinterpret_cast<char*>(&ibuffer), 1);
			subTag.color.r = (float)ibuffer/255;
			f.read(reinterpret_cast<char*>(&ibuffer), 1);
			subTag.color.g = (float)ibuffer/255;
			f.read(reinterpret_cast<char*>(&ibuffer), 1);
			subTag.color.b = (float)ibuffer/255;
			subTag.color.a = 1;

			// Images
			f.read(reinterpret_cast<char*>(&ibuffer), 4);
			if (DEBUG) printf("Imgs: %d\n", ibuffer);

			for (int x = 0; x < ibuffer; x++){
				string s = GetString();
				if (!Duplicate(s, &subTag.imgs) && stat(s.c_str(), &st) == 0)
					subTag.imgs.push_back(File{GetName(s), s});
			}
			sort(subTag.imgs.begin(), subTag.imgs.end(), SortFile);
			subTag.subTag = true;
			tag.subTags.push_back(subTag);
		}
		sort(tag.subTags.begin(), tag.subTags.end(), SortTag);
		tags.push_back(tag);
	}
	if (DEBUG) printf("[Loading] Sorting tags\n");
	sort(tags.begin(), tags.end(), SortTag);

	// Get last used board
	board = GetString();

	f.close();
	LoadImageBoard();
	
	if (DEBUG) printf("[Loading] Finished\n");

	
}

void LoadImageBoard(){
	printf("%s\n", board.data());

	f.open("boards/"+board, ios::in | ios::binary);
    if (!f.good()){
		if (DEBUG) printf("[Loading] Unable to load image board\n");
		return;
	}
	
	// Image board and configuration
	if (DEBUG) printf("[Loading] Image board\n");

	int ibuffer = 0;
	imgs.clear();
	
	f.read(reinterpret_cast<char*>(&ibuffer), 4);
	int imgSize = ibuffer;

	for (int i = 0; i < imgSize; i++){
		Image img;
		Vector2 size;
		f.read(reinterpret_cast<char*>(&img.position.x), 4);
		f.read(reinterpret_cast<char*>(&img.position.y), 4);
		f.read(reinterpret_cast<char*>(&size.x), 4);
		f.read(reinterpret_cast<char*>(&size.y), 4);
		f.read(reinterpret_cast<char*>(&img.hFlip), sizeof(bool));
		f.read(reinterpret_cast<char*>(&img.vFlip), sizeof(bool));
		f.read(reinterpret_cast<char*>(&img.angle), sizeof(float));
		string path = GetString();
		img.LoadImage((char*)path.data());
		img.size = size;
		if (img.loaded)
			imgs.push_back(img);
	}
	
	float vX, vY, s;

	// Configuration
	f.read(reinterpret_cast<char*>(&Width), sizeof(int));
	f.read(reinterpret_cast<char*>(&Height), sizeof(int));
	f.read(reinterpret_cast<char*>(&vX), sizeof(float));
	f.read(reinterpret_cast<char*>(&vY), sizeof(float));
	f.read(reinterpret_cast<char*>(&s), sizeof(float));
	f.read(reinterpret_cast<char*>(&maximize), sizeof(float));

	Main.view = Vector2{vX, vY};
	Main.scale = s;

	f.close();
}

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
		//printf("%d : %c\n", c, c);
	}

	uint8_t* data = (uint8_t*)malloc(sizeof(uint8_t) * file.length());

	for (int x = 0; x < file.length(); x++){
		data[x] = int(file[x]);
		//printf("%d : %c\n", data[x], file[x]);
	}


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

	i.GenTexture(img);
	i.size = Vector2{(float)i.width, (float)i.height};
	if (DEBUG) printf("[Image] Loaded: %s\n", path.data());

	f.close();
	return i;
}

void Save(){
	ofstream w("save.dat", ios::out | ios::binary);
	
	if (!w.good()){
		if (DEBUG) printf("[Save] Unable to save file\n");
		return;
	}
	save = 60;

	// Data heading
	w.write((char*)"dat ", sizeof(char)*4);

	if (DEBUG) printf("[Saving] Locations\n");

	int size = locations.size();
	// Locations
	w.write(reinterpret_cast<const char*>(&size), sizeof(int)); // 4 bytes
	for (int l = 0; l < locations.size(); l++){
		int len = locations[l].path.length();
		w.write(reinterpret_cast<const char*>(&len), sizeof(int));
		w.write(locations[l].path.c_str(), sizeof(char)*len);
	}

	if (DEBUG) printf("[Saving] Tags\n");
	size = tags.size();

	// Tags
	w.write(reinterpret_cast<const char *>(&size), sizeof(int));
	for (int t = 0; t < tags.size(); t++){
		// Name
		if (DEBUG) printf("[Saving] Name %s\n", tags[t].name.data());
		int len = tags[t].name.length();
		w.write(reinterpret_cast<const char*>(&len), sizeof(int));
		w.write(tags[t].name.c_str(), sizeof(char)*len);
		
		// Color
		if (DEBUG) printf("[Saving] Color\n");
		int r = tags[t].color.r*255;
		int g = tags[t].color.g*255;
		int b = tags[t].color.b*255;
		w.write(reinterpret_cast<const char*>(&r), 1);
		w.write(reinterpret_cast<const char*>(&g), 1);
		w.write(reinterpret_cast<const char*>(&b), 1);
		
		// Images
		if (DEBUG)printf("[Saving] Images\n");
		size = tags[t].imgs.size();

		w.write(reinterpret_cast<const char*>(&size), sizeof(int));
		for (int i = 0; i < tags[t].imgs.size(); i++){
			len = tags[t].imgs[i].path.length();
			w.write(reinterpret_cast<const char*>(&len), sizeof(int));
			w.write(tags[t].imgs[i].path.c_str(), sizeof(char)*len);
		}
		
		// Sub tags
		if (DEBUG) printf("[Saving] Subtags\n");
		size = tags[t].subTags.size();
		w.write(reinterpret_cast<const char*>(&size), sizeof(int));
		for (int s = 0; s < tags[t].subTags.size(); s++){
			// Name
			int len = tags[t].subTags[s].name.length();
			w.write(reinterpret_cast<const char*>(&len), sizeof(int));
			w.write(tags[t].subTags[s].name.c_str(), sizeof(char)*len);

			int r = tags[t].subTags[s].color.r*255;
			int g = tags[t].subTags[s].color.g*255;
			int b = tags[t].subTags[s].color.b*255;
			// Color
			w.write(reinterpret_cast<const char*>(&r), 1);
			w.write(reinterpret_cast<const char*>(&g), 1);
			w.write(reinterpret_cast<const char*>(&b), 1);

			// Images
			size = tags[t].subTags[s].imgs.size();
			w.write(reinterpret_cast<const char*>(&size), sizeof(int));
			for (int i = 0; i < tags[t].subTags[s].imgs.size(); i++){
				len = tags[t].subTags[s].imgs[i].path.length();
				w.write(reinterpret_cast<const char*>(&len), sizeof(int));
				w.write(tags[t].subTags[s].imgs[i].path.c_str(), sizeof(char)*len);
			}
		}
	}
	
	if (!board.length())
		SaveBoard();

	int len = board.length();

	w.write(reinterpret_cast<const char*>(&len), sizeof(int));
	w.write(board.c_str(), sizeof(char)*len);
	w.close();

	w.open("boards/"+board, ios::out | ios::binary);
	
	if (!w.good()){
		if (DEBUG) printf("[Saving] Unable to save image board\n");
		return;
	}

	if (DEBUG) printf("[Saving] Image board\n");
	

	// Image board
	size = imgs.size();
	w.write(reinterpret_cast<const char*>(&size), sizeof(int));
	for (int i = 0; i < imgs.size(); i++){
		w.write(reinterpret_cast<const char*>(&imgs[i].position.x), sizeof(int));
		w.write(reinterpret_cast<const char*>(&imgs[i].position.y), sizeof(int));
		w.write(reinterpret_cast<const char*>(&imgs[i].size.x), sizeof(int));
		w.write(reinterpret_cast<const char*>(&imgs[i].size.y), sizeof(int));
		w.write(reinterpret_cast<const char*>(&imgs[i].hFlip), sizeof(bool));
		w.write(reinterpret_cast<const char*>(&imgs[i].vFlip), sizeof(bool));
		w.write(reinterpret_cast<const char*>(&imgs[i].angle), sizeof(float));
		int len = imgs[i].path.length();
		w.write(reinterpret_cast<const char*>(&len), sizeof(int));
		w.write(imgs[i].path.c_str(), sizeof(char)*len);
	}

	// Configuration
	float vX = View->x, vY = View->y, s = *Scale;
	w.write(reinterpret_cast<const char*>(&Width), sizeof(int));
	w.write(reinterpret_cast<const char*>(&Height), sizeof(int));
	w.write(reinterpret_cast<const char*>(&vX), sizeof(float));
	w.write(reinterpret_cast<const char*>(&vY), sizeof(float));
	w.write(reinterpret_cast<const char*>(&s), sizeof(float));
	w.write(reinterpret_cast<const char*>(&maximize), sizeof(bool));

	w.close();
	if (DEBUG) printf("\n");
}
