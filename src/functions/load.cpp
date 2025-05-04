vector<Tag> LoadTags(ifstream*, bool, bool);
bool oldSave = false;

void FixString(string *s){
	string o = "";
	for (int i = 0; i < s->length(); i++)
		if ((*s)[i] == (slash[0] == '/' ? '\\' : '/'))
			(*s)[i] = slash[0];
}

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
	while (c != '\n' && !f.eof()){
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

/*
	Another instance of windows being stupid
	For some reason, windows refuses to read file properly and wants each string to be stored in a char array before being added to a string
	If it is not, there's a 50/50 chance that the function returns random garbage some how
	Works on Linux though
	You would think a billion dollar company would avoid this issue
*/
string GetString(){
	string temp = "";
	char c[1];
	int size = 0;
	while (f.good()){// <-- This should work but windows doesn't like it
		f.read(c, 1);
		size++;
		if (c[0] == lineEnd[0])
			break;
	}
	
	// Stupid windows code
	char out[size];
	f.seekg(-size, ios::cur);
	f.read(out, size);
	for (int i = 0; i < size-1; i++)
		temp += out[i];
	FixString(&temp);
	return temp;
}

/*string GetString(){
	string temp = "";
	char c[1];
	while (f.good()){
		f.read(c, 1);
		if (c[0] == lineEnd[0])
			return temp;
		temp += c;
	}
	return temp;
}*/

string GetStringOld(){
	int size = 0;
	string temp;
	f.read(reinterpret_cast<char*>(&size), 4);
	char out[size+1];
	f.read(out, size);
	out[size] = '\0';
	temp = out;
	
	// Alter backslashes to match the OS (thanks windows)
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
	loaded = true;
	f.open("save.dat", ios::in | ios::binary);
    if (!f.good()){
		if (DEBUG) printf("[Loading] No save data found\n");
		return;
	}

	struct stat st;
	int ibuffer, cbuffer;
	char header[4];

	// Read header
	f.read(header, 4);
	oldSave = header[3] == ' ';

	if (DEBUG)	printf("[Loading] Locations\n");

	// Locations
	f.read(reinterpret_cast<char*>(&ibuffer), 4);
	int locationSize = ibuffer;
	for (int l = 0; l < locationSize; l++){
		string loc = (!oldSave ? GetString() : GetStringOld());
		locations.push_back(Table{GetName(loc), loc});
		if (DEBUG) printf("%d) %s\n", l, loc.data());
	}

	if (DEBUG) printf("[Loading] Sorting locations\n");
	sort(locations.begin(), locations.end(), locations[0].SortTable);

	
	// Tags
	tags = LoadTags(&f, false, true);
	
	if (DEBUG) printf("[Loading] Sorting tags\n");
	sort(tags.begin(), tags.end(), SortTag);

	// Get last used board
	board = (!oldSave ? GetString() : GetStringOld());

	f.close();
	LoadImageBoard();
	FindBoards();

	if (DEBUG) printf("[Loading] Finished\n");
    
    if (menuWidth == 0)
        menuWidth = Main.width/6;
}
void LoadImageBoard(){
	if (DEBUG) printf("%s\n", board.data());

	fCurrentBoard.text = board.substr(0,board.length()-4);
	f.open("boards/"+board, ios::in | ios::binary);
    if (!f.good()){
		if (DEBUG) printf("[Loading] Unable to load image board\n");
		return;
	}
	
	// Image board and configuration
	if (DEBUG) printf("[Loading] Image board\n");
	int ibuffer = 0, imgW = 0, imgH = 0;
	imgs.clear();

	// Read past screenshot data
	f.read(reinterpret_cast<char*>(&imgW), 4);
	f.read(reinterpret_cast<char*>(&imgH), 4);
	unsigned char imgData[1];
	for (int i = 0; i < imgW*imgH*3; i++)
		f.read(reinterpret_cast<char*>(imgData), 1);

	f.read(reinterpret_cast<char*>(&ibuffer), 4);
	int imgSize = ibuffer;
	for (int i = 0; i < imgSize; i++){
		Object img;
		Vector2 size;
		f.read(reinterpret_cast<char*>(&img.position.x), 4);
		f.read(reinterpret_cast<char*>(&img.position.y), 4);
		f.read(reinterpret_cast<char*>(&size.x), 4);
		f.read(reinterpret_cast<char*>(&size.y), 4);
		f.read(reinterpret_cast<char*>(&img.hFlip), sizeof(bool));
		f.read(reinterpret_cast<char*>(&img.vFlip), sizeof(bool));
		f.read(reinterpret_cast<char*>(&img.angle), sizeof(float));
		string path = (!oldSave ? GetString() : GetStringOld());
		img.path = path;
		
		if (img.path.substr(img.path.length()-4) != "webp")
			img.img.LoadImage(path);
		else
			img.img = LoadWebp(path);
			
		img.size = size;
		if (img.img.loaded)
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

vector<Tag> LoadTags(ifstream *f, bool subtag, bool checkValid){
	struct stat st;
	int buf = 0;
	vector<Tag> tags;
	
	f->read(reinterpret_cast<char*>(&buf), 4);
	int tagSize = buf;
	if (DEBUG) printf(("[TAG] Loading tags: %d\n"), tagSize);
	for (int t = 0; t < tagSize; t++){
		Tag tag;

		// Name
		tag.name = (!oldSave ? GetString() : GetStringOld());
		CheckString(tag.name);

		if (DEBUG) printf((!subtag ? "[TAG] %s\n" : "[SUBTAG] %s\n"), tag.name.data());

		// Color
		f->read(reinterpret_cast<char*>(&buf), 1);
		tag.color.r = (float)buf/255;
		f->read(reinterpret_cast<char*>(&buf), 1);
		tag.color.g = (float)buf/255;
		f->read(reinterpret_cast<char*>(&buf), 1);
		tag.color.b = (float)buf/255;
		tag.color.a = 1;

		// Colors can exceed 1 somehow
		if (tag.color.r > 1)
			tag.color.r -= 1;
		if (tag.color.g > 1)
			tag.color.g -= 1;
		if (tag.color.b > 1)
			tag.color.b -= 1;


		// Images
		f->read(reinterpret_cast<char*>(&buf), 4);
		if (DEBUG) printf("\tImgs: %d\n", buf);

		for (int i = 0; i < buf; i++){
			string s = (!oldSave ? GetString() : GetStringOld());
			if ((!Duplicate(s, &tag.imgs) && stat(s.c_str(), &st) == 0) || !checkValid)
				tag.imgs.push_back(File{GetName(s), s});
			else
				if (DEBUG) printf("\nUnable to load: %s\n", s.data());
		}

		sort(tag.imgs.begin(), tag.imgs.end(), SortFile);

		if (!subtag){
			if (DEBUG) printf("[TAG] Loading subtags: \n");
			tag.subTags = LoadTags(f, true, checkValid);
			sort(tag.subTags.begin(), tag.subTags.end(), SortTag);
		}

		tags.push_back(tag);
		if (DEBUG) printf("\n");
	}

	return tags;
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

	if (DEBUG) printf("[Image] Loaded: %s\n", path.data());

	f.close();
	return i;
}