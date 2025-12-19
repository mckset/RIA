/*
	Handles loading as well as functions to assist in loading elements such as webp
*/

enum Loading_Progress{
	LOAD_NONE,
	LOAD_STARTED,
	LOAD_LOCATIONS,
	LOAD_TAGS,
	LOAD_BOARDS,
	LOAD_FINISHED
};

ifstream readFile;

bool oldSave = false; // True if an old save file

// Converts path slashes to match system
void FixString(string *s){
	string o = "";
	for (int i = 0; i < s->length(); i++)
		if ((*s)[i] == (slash[0] == '/' ? '\\' : '/'))
			(*s)[i] = slash[0];
}

// Gets a color from a string
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

string GetName(string path){
	for (int i = path.length()-1; i > 0; i--)
		if (path[i] == slash[0])
			return path.substr(i+1);
	return path;
}

/*
	Another instance of windows being stupid
	For some reason, windows refuses to read files properly and wants each string to be stored in a char array before being added to a string
	If it is not, there's a 50/50 chance that the function returns random garbage
	Works on Linux though
	You would think a billion dollar company would avoid this issue
*/
string GetString(ifstream *f){
	string temp = "";
	char c[1];
	int size = 0;
	while (f->good()){// <-- This should work but windows doesn't like it
		f->read(c, 1);
		size++;
		if (c[0] == lineEnd[0])
			break;
	}
	
	// Stupid windows code
	char out[size];
	f->seekg(-size, ios::cur);
	f->read(out, size);
	for (int i = 0; i < size-1; i++)
		temp += out[i];
	FixString(&temp);
	return temp;
}

// Backwards compatible name loading for old saves
string GetStringOld(){
	int size = 0;
	string temp;
	readFile.read(reinterpret_cast<char*>(&size), 4);
	char out[size+1];
	readFile.read(out, size);
	out[size] = '\0';
	temp = out;
	
	// Alter backslashes to match the OS (thanks windows)
	for (int c = 0; c < temp.length(); c++)
		if (temp[c] == (slash[0] == '/' ? '\\' : '/'))
			temp[c] = slash[0];
	
	return temp;
}

// Checks if an image already exists in a list
bool Duplicate(string img, vector<File>* list){
	for (auto file : *list)
		if (file.path == img) return true;
	return false;
}

// Main loading function
void Load(){
	statusTextTimer = -1;
	loadedSave = LOAD_STARTED;
	readFile.open(path + "save.dat", ios::in | ios::binary);
    if (!readFile.good()){
		if (DEBUG) printf("[Loading] No save data found\n");
		locations.push_back(Location{"downloads", path+"downloads"});
		return;
	}

	struct stat st;
	int ibuffer, cbuffer;
	char header[4];

	// Read header
	readFile.read(header, 4);
	oldSave = header[3] == ' ';

	if (DEBUG)	printf("[Loading] Locations\n");

	statusText = "Loading Locations";

	// Locations
	loadedSave = LOAD_LOCATIONS;
	vector<Location> unsortedLocations;
	readFile.read(reinterpret_cast<char*>(&ibuffer), 4);
	int locationSize = ibuffer;
	bool downloadsLoaded = false;
	for (int l = 0; l < locationSize; l++){
		string loc = (!oldSave ? GetString(&readFile) : GetStringOld());
		unsortedLocations.push_back(Location{GetName(loc), loc});
		if (loc == path+"downloads") downloadsLoaded = true;
		if (DEBUG) printf("%d) %s\n", l, loc.data());
	}

	if (!downloadsLoaded) unsortedLocations.push_back(Location{"downloads", path+"downloads"});
	if (DEBUG) printf("[Loading] Sorting locations\n");
	sort(unsortedLocations.begin(), unsortedLocations.end(), unsortedLocations[0].SortLocations);

	UpdateLocationTags(&unsortedLocations);

	locations = unsortedLocations;
	
	// Tags
	statusText = "Loading Tags";
	loadedSave = LOAD_TAGS;
	vector<Tag> unsortedTags = LoadTags(&readFile, false, true);
	
	if (DEBUG) printf("[Loading] Sorting tags\n");
	sort(unsortedTags.begin(), unsortedTags.end(), SortTag);
	tags = unsortedTags;

	// Get last used board
	board = (!oldSave ? GetString(&readFile) : GetStringOld());

	readFile.close();


	FindBoards();
	LoadImageBoard();

	if (DEBUG) printf("[Loading] Finished\n");
    
    if (sideMenuWidth == 0)
        sideMenuWidth = Main.width*sideMenuRatio;
	
	loadedSave = LOAD_FINISHED;
}

// Loads an image board
void LoadImageBoard(){
	loadedSave = LOAD_BOARDS;
	statusTextTimer = -1;

	if (DEBUG) printf("%s\n", board.data());

	loadedImages.clear();

	currentBoard_Field.text = board.substr(0,board.length()-4);
	readFile.open("boards/"+board, ios::in | ios::binary);
    if (!readFile.good()){
		if (DEBUG) printf("[Loading] Unable to load image board\n");
		return;
	}
	if (DEBUG) printf("[Loading] Image board\n");

	float vX, vY, s;

	// Image board and configuration
	int ibuffer = 0, imgW = 0, imgH = 0;
	imgs.clear();

	// Read past screenshot data
	readFile.read(reinterpret_cast<char*>(&imgW), 4);
	readFile.read(reinterpret_cast<char*>(&imgH), 4);
	unsigned char imgData[1];
	for (int i = 0; i < imgW*imgH*3; i++)
		readFile.read(reinterpret_cast<char*>(imgData), 1);

	readFile.read(reinterpret_cast<char*>(&ibuffer), 4);
	int imgSize = ibuffer;

	vector<ImageContainer> boardImages;
	for (int i = 0; i < imgSize; i++){
		if (closeThread){
			readFile.close();
			return;
		}
		ImageContainer img;
		Vector2 size;
		readFile.read(reinterpret_cast<char*>(&img.position.x), 4);
		readFile.read(reinterpret_cast<char*>(&img.position.y), 4);
		readFile.read(reinterpret_cast<char*>(&size.x), 4);
		readFile.read(reinterpret_cast<char*>(&size.y), 4);
		readFile.read(reinterpret_cast<char*>(&img.hFlip), sizeof(bool));
		readFile.read(reinterpret_cast<char*>(&img.vFlip), sizeof(bool));
		readFile.read(reinterpret_cast<char*>(&img.angle), sizeof(float));
		string path = (!oldSave ? GetString(&readFile) : GetStringOld());
		img.path = path;
		img.size = size;
		boardImages.push_back(img);
	}
	
	// Configuration
	readFile.read(reinterpret_cast<char*>(&Width), sizeof(int));
	readFile.read(reinterpret_cast<char*>(&Height), sizeof(int));
	readFile.read(reinterpret_cast<char*>(&vX), sizeof(float));
	readFile.read(reinterpret_cast<char*>(&vY), sizeof(float));
	readFile.read(reinterpret_cast<char*>(&s), sizeof(float));
	readFile.read(reinterpret_cast<char*>(&maximize), sizeof(bool));
	Main.view = Vector2{vX, vY};
	if (s > 0)
		Main.scale = s;

	readFile.close();

	// Load images
	for (auto &img : boardImages){
		statusText = "Loading " + img.path;
		if (img.path.substr(img.path.length()-4) != "webp")
			img.LoadImageData(img.path);
		else
			img.LoadWebpData(img.path);
		
		if (img.loaded)
			loadedImages.push_back(img);
	}
			

	statusText = "Loaded " + board;
	statusTextTimer = STATUS_TIME;
}

// Loads tags
vector<Tag> LoadTags(ifstream *f, bool subTag, bool checkValid){
	struct stat st;
	int buf = 0;
	vector<Tag> tags;
	
	f->read(reinterpret_cast<char*>(&buf), 4);
	int tagSize = buf;
	if (DEBUG) printf(("[TAG] Loading tags: %d\n"), tagSize);
	for (int t = 0; t < tagSize; t++){
		Tag tag;

		tag.isSubTag = subTag;
		
		// Name
		tag.name = (!oldSave ? GetString(f) : GetStringOld());
		textCache.push_back(tag.name);

		if (DEBUG) printf((!subTag ? "[TAG] %s\n" : "[Sub Tag] %s\n"), tag.name.data());

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
			string s = (!oldSave ? GetString(f) : GetStringOld());
			if ((!Duplicate(s, &tag.files) && stat(s.c_str(), &st) == 0) || !checkValid){
				tag.files.push_back(File{GetName(s), s});
				if (fileTagMap.find(s) == fileTagMap.end())
					fileTagMap.insert({s, true});
			}else
				if (DEBUG) printf("\nUnable to load: %s\n", s.data());
		}

		sort(tag.files.begin(), tag.files.end(), SortFile);

		if (!subTag){
			if (DEBUG) printf("[TAG] Loading subtags: \n");
			tag.subTags = LoadTags(f, true, checkValid);
			sort(tag.subTags.begin(), tag.subTags.end(), SortTag);
		}

		tags.push_back(tag);
		if (DEBUG) printf("\n");
	}

	return tags;
}

// Removes the extension from a file
string RemoveExt(string file){
	for (int i = file.length()-1; i > 0; i--)
		if (file[i] == '.')
			return file.substr(0,i);
	return file;
}