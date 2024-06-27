string CondensePath(char*, int);
string GetFolder();
void GetBoard();
char* GetFile();
void OpenShared();
void SaveBoard();

string CondensePath(char* path, int size){
	string output = "";
	for (int i = 0; i < size && path[i] != '\0' && path[i] != '\n'; i++){
		output += path[i];
	}
	return output;
}

char* GetFile(){
	char path[4097];
	FILE *f;
	f = popen("zenity --file-selection --file-filter=\'*.png *.jpg *.jpeg\'", "r");
	fgets(path, 4097, f);
	return (char*)CondensePath(path, 4097).data();
}

void GetBoard(){
	char p[4097];
	FILE *f;
	f = popen("zenity --file-selection --filename=\"boards/ \" --file-filter=\'*.brd\'", "r");
	fgets(p, 4097, f);
	string file = CondensePath(p, 4097);
	struct stat st;
	if (stat(file.c_str(), &st) == 0){
		board = file.substr(path.length()+8);
		LoadImageBoard();
	}
}

string GetFolder(){
	struct stat st;
	char p[4097];
	FILE *f;
	
	f = popen("zenity --file-selection --directory", "r");
	fgets(p, 4097, f);

	string path = CondensePath(p, 4097);

	if (stat(path.c_str(), &st) == 0)
		return path;
	return (char*)"";
}

void OpenShared(){
	popen("xdg-open shared/", "r");
}

void OpenHelp(){
	popen("xdg-open help.html", "r");
}

void SaveBoard(){
	char p[4097];
	FILE *f;
	f = popen("zenity --file-selection --filename=\"boards/ \" --file-filter=\'*.brd\' --save", "r");
	fgets(p, 4097, f);
	string file = CondensePath(p, 4097);

	struct stat st;
	if (stat(file.substr(0,path.length()+8).c_str(), &st) == 0){
		board = file.substr(path.length()+8);
		if (board.length() < 4)
			board += ".brd";
		else if (strcmp(board.substr(board.length()-4).data(), ".brd"))
			board += ".brd";
		printf("%s\n", board.data());
		
	}
	
}