void OpenShared();
void SaveBoard();

void OpenShared(){
	popen("xdg-open shared/", "r");
}


void OpenHelp(){
	popen("xdg-open help.html", "r");
}
