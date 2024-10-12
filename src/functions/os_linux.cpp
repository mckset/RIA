void GetBoard();
void OpenShared();
void SaveBoard();

void GetBoard(){
	string file = GetFile("*.brd");
	struct stat st;
	if (stat(file.c_str(), &st) == 0){
		board = file.substr(path.length()+8);
		LoadImageBoard();
	}
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
		if (DEBUG) printf("%s\n", board.data());
		
	}
	
}