char* CondensePath(char*, int);
char* GetFile();

char* GetFile(){
	char path[4097];
	FILE *f;
	f = popen("zenity --file-selection --file-filter=\'*.png *.jpg *.jpeg\'", "r");
	fgets(path, 4097, f);
	return CondensePath(path, 4097);
}

char* GetFolder(){
	char path[4097];
	FILE *f;
	f = popen("zenity --file-selection --directory", "r");
	fgets(path, 4097, f);
	return CondensePath(path, 4097);
}

char* CondensePath(char* path, int size){
	char *output;
	for (int i = 0; i < size; i++){
		//printf("%d) %c\n", i, path[i]);
		if (path[i] == '\0'){
			output = (char*)malloc(sizeof(char)*i);
			for (int x = 0; x < i-1; x++)
				output[x] = path[x];
			output[i-1] = '\0';
			return output;
		}
	}
	return path;
}
