/*
	Handles all the functions needed to save RIA
*/

#include<iostream>
#include<fstream>

ifstream f;
bool saving = false;
bool saveError = false;
string saveText = "";
const char lineEnd[1] = {'\0'};

// Main save function
void Save(){
	saveError = true;
	saving  = true;

	// Sets the board name if left empty
	if (!board.length())
		board = "Default.brd";

	// Saves tags and locations
	saveText = "Opening save.dat";
	ofstream w(path + "temp.dat", ios::out | ios::binary);
	
	if (!w.good()){
		if (DEBUG) printf("[Save] Unable to save file\n");
		saving = false;
		return;
	}
	save = 600;

	// Data heading
	w.write((char*)"dat\1", sizeof(char)*4);

	if (DEBUG) printf("[Saving] Locations\n");

	int size = locations.size();
	
	// Locations
	w.write(reinterpret_cast<const char*>(&size), sizeof(int)); // 4 bytes
	for (int l = 0; l < locations.size(); l++){
		saveText = "Saving locations (" + to_string(l+1) + "/" + to_string(locations.size()) + ")";
		save = 600;
		w.write(locations[l].path.c_str(), locations[l].path.length());
		w.write(lineEnd, 1);
	}

	// Tags
	if (DEBUG) printf("[Saving] Tags\n");
	size = tags.size();

	SaveTags(&w, tags, false);

	// Current board name
	w.write(board.c_str(), board.length());
	w.write(lineEnd, 1);
	w.close();

	// Overwrite save file
	filesystem::rename(path+"temp.dat", path+"save.dat");

	// Save image board
	SaveImageBoard();
	
	saving = false;
	saveError = false;
	saveText = "Saved";
	save = 60;
	if (DEBUG) printf("\n");
}

// Saves the current image board
void SaveImageBoard(){
	// Board
	if (DEBUG) printf("Board: %s\n", board.data());
	ofstream w;
	w.open(path + "temp.brd", ios::out | ios::binary);

	save = 600;
	saveText = "Saving image board";
	
	if (!w.good()){
		if (DEBUG) printf("[Saving] Unable to save image board\n");
		saving = false;
		return;
	}

	if (DEBUG) printf("[Saving] Image board\n");
	
	// Image board

	// Save screenshot
	int width = Main.width, height = Main.height;
	if (width > height)
		width = height;
	else
		height = width;
	w.write(reinterpret_cast<const char*>(&width), 4);
	w.write(reinterpret_cast<const char*>(&height), 4);
	w.write(reinterpret_cast<const char*>(boardScreenshot), width*height*3);

	// Images
	int size = imgs.size();
	w.write(reinterpret_cast<const char*>(&size), 4);
	for (int i = 0; i < imgs.size(); i++){
		save = 600;
		w.write(reinterpret_cast<const char*>(&imgs[i].position.x), sizeof(int));
		w.write(reinterpret_cast<const char*>(&imgs[i].position.y), sizeof(int));
		w.write(reinterpret_cast<const char*>(&imgs[i].size.x), sizeof(int));
		w.write(reinterpret_cast<const char*>(&imgs[i].size.y), sizeof(int));
		w.write(reinterpret_cast<const char*>(&imgs[i].hFlip), sizeof(bool));
		w.write(reinterpret_cast<const char*>(&imgs[i].vFlip), sizeof(bool));
		w.write(reinterpret_cast<const char*>(&imgs[i].angle), sizeof(float));
		w.write(imgs[i].path.c_str(), imgs[i].path.length());
		w.write(lineEnd, 1);
		if (DEBUG) printf("%s\n", imgs[i].path.data());
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

	// Overwrite board
	filesystem::rename(path + "temp.brd", path+slash[0]+"boards"+slash[0]+board);
}

// Saves tag information
void SaveTags(ofstream *w, vector<Tag> tags, bool subtag = false){
	int buf = tags.size();
	w->write(reinterpret_cast<const char*>(&buf), 4);
	string count = to_string(buf);
	int subTagCount = 0;
	
	// Loop through tags
	for (auto tag : tags){
		save = 600;

		// Name
		if (DEBUG) printf("[Saving] Name %s\n", tag.name.data());\
		buf = tag.name.length();
		w->write(tag.name.c_str(), tag.name.length());
		w->write(lineEnd, 1);

		if (!subtag){
			subTagCount++;
			saveText = "Saving tag " + tag.name + " (" + to_string(subTagCount) + "/" + count + ")";
		}

		// Color
		if (DEBUG) printf("[Saving] Color\n");
		int r = tag.color.r*255;
		int g = tag.color.g*255;
		int b = tag.color.b*255;
		w->write(reinterpret_cast<const char*>(&r), 1);
		w->write(reinterpret_cast<const char*>(&g), 1);
		w->write(reinterpret_cast<const char*>(&b), 1);

		// Images
		if (DEBUG)printf("[Saving] Images\n");
		buf = tag.imgs.size();

		w->write(reinterpret_cast<const char*>(&buf), 4);
		for (auto img : tag.imgs){
			w->write(img.path.c_str(), img.path.length());
			w->write(lineEnd,1);
		}
		
		// Sub tags
		buf = 0;
		if (!subtag)
			if (tag.subTags.size())
				SaveTags(w, tag.subTags, true);
			else
				w->write(reinterpret_cast<const char*>(&buf), 4);
	}
}