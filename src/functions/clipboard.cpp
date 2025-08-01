#ifndef _WIN32
void Copy(){
	if (!selImgs.size())
		return;

	string ext = "";
	if (imgs[selImgs[0]].path[imgs[selImgs[0]].path.length()-4] == '.')
		ext = imgs[selImgs[0]].path.substr(imgs[selImgs[0]].path.length()-3);
	else
		ext = imgs[selImgs[0]].path.substr(imgs[selImgs[0]].path.length()-4);
	
	string cmd = "";
	if (!WAYLAND) cmd = "xclip -selection clipboard -target image/" + ext + " -i \"" + imgs[selImgs[0]].path + "\"";
	else cmd = "wl-copy < \"" + imgs[selImgs[0]].path + "\"";

	for (auto i : selImgs)
		internalClipboard.push_back(imgs[i].path);
	system(cmd.c_str());
}

bool cbReady = false;

void ReadClipboard(const char *out){
	out = glfwGetClipboardString(Main.w);
	cbReady = true;
}

void Paste(){
	cbReady = false;
	pasted = true;
	const char* c = "";
	thread cbThread(ReadClipboard, c);
	for (int i = 0; i < 50 && !cbReady; i++)
		SleepFor(100);
	if (cbReady){
		cbThread.join();
		if (DEBUG) printf("Paste: %s\n", c);
	}else
		cbThread.detach();
	string cb = c;
	if (DEBUG) printf("Paste: %s\n", cb.data());
	if (!cb.length()){
		if (DEBUG) printf("Failed to read clipbord\n");
		if (internalClipboard.size()){
			for (auto p : internalClipboard){
				Object img = Object{{0,0}, {1,1}, 0, 0, p};
				if (img.path.substr(img.path.length()-4) == "webp")
					img.img = LoadWebp(img.path);
				else
					img.img.LoadImage(img.path);
				img.size = Vector2{(float)img.img.width, (float)img.img.height};
				img.position = mouse.ToScreenSpace() - img.size/2;
				imgs.push_back(img);
			}
		}
		internalClipboard.clear();
		return;
	}
	int s = 0;
	string path = "";
	vector<string> images;

	// Split clipboard string by path
	for (int i = 0; i < cb.length(); i++){
		if (cb[i] == '\n'){
			images.push_back(path);
			s=i+1;
			path = "";
		}else 
			path += cb[i];
	}
	if (path.length())
		images.push_back(path);

	// Parse each path
	bool paths = false;
	for (int i = 0; i < images.size(); i++){
		string path = images[i];
		if (images[i].length() > 7 && images[i].substr(0, 7) == "file://")
			path = images[i].substr(7);

		Object img;
		if (IsImage(path)){
				if (path.substr(path.length()-4) == "webp")
					img.img = LoadWebp(path);
				else
					img.img.LoadImage(path.data());
				img.size = Vector2{(float)img.img.width, (float)img.img.height};
				img.position = mouse.ToScreenSpace() - img.size/2;
				imgs.push_back(img);
				paths = true;
		}
	}
	if (!paths && internalClipboard.size()){
		Object img;
		img.img.LoadImage(internalClipboard[0]);
		img.size = Vector2{(float)img.img.width, (float)img.img.height};
		img.position = mouse.ToScreenSpace() - img.size/2;
		imgs.push_back(img);
	}
	internalClipboard.clear();
}
#endif

#ifdef _WIN32
typedef struct _DROPFILES {
  DWORD pFiles;
  POINT pt;
  BOOL  fNC;
  BOOL  fWide;
} DROPFILES, *LPDROPFILES;

void Copy(){
	if (!selImgs.size()){
		if (DEBUG) printf("No images to copy\n");
		return;
	}
	size_t size = sizeof(DROPFILES) + sizeof(wchar_t);
	for (auto img : selImgs)
		size += (imgs[img].path.length() + 1) * sizeof(wchar_t);
	auto cHandle{unique_ptr<void, decltype(&GlobalFree)>(GlobalAlloc(GHND, size), GlobalFree)};
	if (!cHandle){
		if (DEBUG) printf("Unable to allocate clipboard memory\n");	
		return;
	}

	DROPFILES *df = static_cast<DROPFILES*>(GlobalLock(cHandle.get()));
	df->pFiles = sizeof(DROPFILES);
	df->fWide = true;
	wchar_t *clipboard = reinterpret_cast<wchar_t*>(df+1);

	for (auto img : selImgs){
		copy(imgs[img].path.begin(), imgs[img].path.end(), clipboard);
		clipboard += imgs[img].path.size();
		*clipboard++ = L'\0';
	}
	*clipboard = L'\0';
	GlobalUnlock(cHandle.get());

	if (OpenClipboard(0)){
		EmptyClipboard();
		SetClipboardData(CF_HDROP, cHandle.release());
		CloseClipboard();
	}else if (DEBUG) printf("Unable to open clipboard\n");
}

void Paste(){
	if (!OpenClipboard(0)){
		if (DEBUG) printf("Failed to open clipboard\n");
		return;
	}
	
	HANDLE cHandle;
	cHandle = GetClipboardData(CF_HDROP);
	bool pasted = false;

	if (cHandle){
		HDROP hd = static_cast<HDROP>(cHandle);
		uint fileCount = DragQueryFileW(hd, 0xFFFFFFFF, nullptr, 0);
		vector<string> images;

		for (int i = 0; i < fileCount; i++){
			vector<wchar_t> file(MAX_PATH);
			if (DragQueryFileW(hd, i, file.data(), MAX_PATH)){
				wstring sfile(file.data());
				string path(sfile.begin(), sfile.end());
				if (!IsImage(path)) continue;
				
				Object img;
				if (path.substr(path.length()-4) == "webp")
					img.img = LoadWebp(path);
				else
					img.img.LoadImage(path.data());
				img.size = Vector2{(float)img.img.width, (float)img.img.height};
				img.position = mouse.ToScreenSpace() - img.size/2;
				imgs.push_back(img);
				pasted = true;
			}
		}
	}
	if (!pasted && internalClipboard.size()){
		Object img;
		img.img.LoadImage(internalClipboard[0]);
		img.size = Vector2{(float)img.img.width, (float)img.img.height};
		img.position = mouse.ToScreenSpace() - img.size/2;
		imgs.push_back(img);
	}
	GlobalUnlock(cHandle);
	CloseClipboard();
}
#endif