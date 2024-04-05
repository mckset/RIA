string CondensePath(char*, int);
string GetFolder();
char* GetFile();
void OpenShared();
void OpenHelp();

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
