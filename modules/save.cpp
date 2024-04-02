#include<iostream>
#include<fstream>

string GetName(string);

ifstream f;

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
		if (temp[c] == opSlash[0])
			temp[c] = slash[0];
	
	return temp;
}

void Load(){
	f.open("save.dat", ios::in | ios::binary);
    if (!f.good()){
		printf("[Loading] No save data found\n");
		return;
	}

	int ibuffer, cbuffer;

	// Read past header
	f.read(reinterpret_cast<char*>(&ibuffer), 4);

	printf("[Loading] Locations\n");

	// Locations
	f.read(reinterpret_cast<char*>(&ibuffer), 4);
	int locationSize = ibuffer;
	for (int l = 0; l < locationSize; l++){
		string loc = GetString();
		locations.push_back(Table{GetName(loc), loc});
		printf("%d) %s\n", l, loc.data());
	}

	printf("[Loading] Sorting locations\n");
	sort(locations.begin(), locations.end(), locations[0].SortTable);

	
	// Tags
	f.read(reinterpret_cast<char*>(&ibuffer), 4);
	int tagSize = ibuffer;
	printf("[Loading] Tags (%d)\n", tagSize);
	for (int t = 0; t < tagSize; t++){
		Tag tag;

		// Name
		tag.name = GetString();
		printf("[TAG] %s", tag.name.data());

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
		printf("\tImgs: %d", ibuffer);
		for (int i = 0; i < ibuffer; i++){
			string s = GetString();
			tag.imgs.push_back(File{GetName(s), s});
		}

		sort(tag.imgs.begin(), tag.imgs.end(), SortFile);
		
		// Sub tags
		f.read(reinterpret_cast<char*>(&ibuffer), 4);
		int subSize = ibuffer;
		printf("\tSub Tags: %d\n", subSize);

		// Repeat everything
		for (int i = 0; i < subSize; i++){
			Tag subTag;

			// Name
			subTag.name = GetString();
			printf("\t[SUB TAG] %s", subTag.name.data());


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
			printf("Imgs: %d\n", ibuffer);

			for (int x = 0; x < ibuffer; x++){
				string s = GetString();
				subTag.imgs.push_back(File{GetName(s), s,});
			}
			sort(subTag.imgs.begin(), subTag.imgs.end(), SortFile);
			subTag.subTag = true;
			tag.subTags.push_back(subTag);
		}
		sort(tag.subTags.begin(), tag.subTags.end(), SortTag);
		tags.push_back(tag);
	}
	printf("[Loading] Sorting tags\n");
	sort(tags.begin(), tags.end(), SortTag);

	
	// Image board and configuration
	printf("[Loading] Image board\n");
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
	// Configuration
	f.read(reinterpret_cast<char*>(&Width), sizeof(int));
	f.read(reinterpret_cast<char*>(&Height), sizeof(int));
	f.read(reinterpret_cast<char*>(&View.x), sizeof(float));
	f.read(reinterpret_cast<char*>(&View.y), sizeof(float));
	f.read(reinterpret_cast<char*>(&Scale), sizeof(float));
	f.read(reinterpret_cast<char*>(&maximize), sizeof(float));

	printf("[Loading] Finished\n");
	f.close();
}

void Save(){
	ofstream w("save.dat", ios::out | ios::binary);
	
	if (!w.good()){
		printf("[Save] Unable to save file");
		return;
	}
	save = 60;

	// Data heading
	w.write((char*)"dat ", sizeof(char)*4);

	printf("[Saving] Locations\n");

	int size = locations.size();
	// Locations
	w.write(reinterpret_cast<const char*>(&size), sizeof(int)); // 4 bytes
	for (int l = 0; l < locations.size(); l++){
		int len = locations[l].path.length();
		w.write(reinterpret_cast<const char*>(&len), sizeof(int));
		w.write(locations[l].path.c_str(), sizeof(char)*len);
	}

	printf("[Saving] Tags\n");
	size = tags.size();

	// Tags
	w.write(reinterpret_cast<const char *>(&size), sizeof(int));
	for (int t = 0; t < tags.size(); t++){
		// Name
		printf("[Saving] Name %s\n", tags[t].name.data());
		int len = tags[t].name.length();
		w.write(reinterpret_cast<const char*>(&len), sizeof(int));
		w.write(tags[t].name.c_str(), sizeof(char)*len);
		
		// Color
		printf("[Saving] Color\n");
		int r = tags[t].color.r*255;
		int g = tags[t].color.g*255;
		int b = tags[t].color.b*255;
		w.write(reinterpret_cast<const char*>(&r), 1);
		w.write(reinterpret_cast<const char*>(&g), 1);
		w.write(reinterpret_cast<const char*>(&b), 1);
		
		// Images
		printf("[Saving] Images\n");
		size = tags[t].imgs.size();

		w.write(reinterpret_cast<const char*>(&size), sizeof(int));
		for (int i = 0; i < tags[t].imgs.size(); i++){
			len = tags[t].imgs[i].path.length();
			w.write(reinterpret_cast<const char*>(&len), sizeof(int));
			w.write(tags[t].imgs[i].path.c_str(), sizeof(char)*len);
		}
		
		// Sub tags
		printf("[Saving] Subtags\n");
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

	printf("[Saving] Image board\n");
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
	w.write(reinterpret_cast<const char*>(&Width), sizeof(int));
	w.write(reinterpret_cast<const char*>(&Height), sizeof(int));
	w.write(reinterpret_cast<const char*>(&View.x), sizeof(float));
	w.write(reinterpret_cast<const char*>(&View.y), sizeof(float));
	w.write(reinterpret_cast<const char*>(&Scale), sizeof(float));
	w.write(reinterpret_cast<const char*>(&maximize), sizeof(bool));

	w.close();
	printf("\n");
}
