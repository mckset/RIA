using namespace std;
#if UBUNTU
	#include <experimental/filesystem>
	namespace fs = std::experimental::filesystem;
#else
	#include <filesystem>
	namespace fs = std::filesystem;
#endif


void SortFolders(int, int);
bool IsImage(string);

class Table{
	public:
		string name = "";
		string path = "";
		bool expand = false;
		bool inited = false;
		int listSize = 0;
		vector<Table> folders;
		vector<File> files;

		bool Draw(Vector2 position, Vector2 size, int m = -1, int sub = -1){
			if (!inited)
				GetFiles(m, sub);

			listSize = 0;

			// Heading
			sShape.Use();
			shape.Draw(position, size, locationHeading, true);
			
			// Text
			sImage.Use();
			font.Write(name, position.Add(0, size.y/8), fontSize*3/4, fontColor, true, size.x);

			//
			// Mouse
			//
			if (mouse.position.Within(position, size)){

				// Deleting
				if (sub == -1 && keyboard.newKey == KEY_DELETE){
					keyboard.newKey = -1;
					return true;
				}

				// Expand folder
				if (mouse.Click(LM_DOWN)){
					expand = !expand;
					for (int i = 0; i < folders.size(); i++)
						folders[i].inited = false;
					mouse.state = mouse.prevState;
				}else{
					sShape.Use();
					shape.Draw(position, size, highlight, true);
				}
			}			

			listSize = size.y;

			// Folders and Files
			if (expand){
				// Folders
				for (int i = 0; i < folders.size(); i++){
					if (position.y - listSize > -fontSize)
						bool update = folders[i].Draw(position.Add(16, -listSize), Vector2{size.x - 16, size.y}, m, i);
					listSize += folders[i].listSize;
				}
				// Files
				sImage.Use();
				for (int i = 0; i < files.size(); i++){
					if (position.y - listSize > -fontSize){
						sImage.Use();
						font.Write(files[i].name, position.Add(16, -listSize+size.y/8), fontSize*5/8, fontColor, true, size.x-8);
						
						sShape.Use();
						// Tagged indicator
						bool tagged = false;
						for (auto tag : tags){
							for (auto img : tag.imgs){
								if (!strcmp(files[i].path.data(), img.path.data())){
									shape.Draw(position.Add(0, -listSize), Vector2{16, size.y}, White, true);
									tagged = true;
									break;
								}
							}
							if (tagged)
								break;
						}
						if (previewImg.loaded && !strcmp(previewPath.data(), files[i].path.data())){
							shape.Draw(position.Add(16, -listSize), size.Subtract(16, 0), highlight, true);
						}

						// Mouse
						if (mouse.position.Within(position.Add(16, -listSize), size.Subtract(16, 0))){
						
							// Preview Image
							if (mouse.Click(LM_DOWN)){
								previewImg.LoadImage((char*)files[i].path.data());
								previewImg.ScaleImg(Vector2{(float)Width/5, (float)Width/5});
								mouse.prevState= mouse.state;
								previewPath = files[i].path;
							}
							shape.Draw(position.Add(16, -listSize), size.Subtract(16, 0), highlight, true);
						}
					}
					listSize += size.y;
				}
			}
			return false;
		}

		void GetFiles(int i = -1, int sub = -1){
			folders.clear();
			files.clear();
			struct stat s;

			// Empty directory
			if ((stat(path.c_str(), &s) == 0) == 0)
				return;

			for (const auto& entry : fs::directory_iterator((string)path)) {
				fs::path ePath = entry.path();

				string sPath = ePath.string();
				const char* p = sPath.c_str();

				if (stat(p, &s) == 0) // Is valid

					if (s.st_mode & S_IFDIR){ // Folders
						folders.push_back(Table{GetName(p), p});
						//printf("Folder: %s\n", GetName(p).data());
					}

					else if (s.st_mode & S_IFREG && IsImage(p)){ // Files
							files.push_back(File{GetName(p), p});
							//printf("Image: %s\n", GetName(p).data());
					}
			}
			sort(files.begin(), files.end(), SortFile);
			SortFolders(i, sub);
			inited = true;
		}

		int GetSize(Vector2 size){
			listSize = size.y;
			if (expand){
				for (int i = 0; i < folders.size(); i++){
					listSize += size.y;
					listSize += folders[i].GetSize(size);
				}
				
				listSize += size.y*files.size();
			}
			return listSize;
		}

};


bool SortTable(Table, Table);

bool IsImage(string path){
	char ext[5];
	ext[0] = path[path.length()-4];
	ext[1] = path[path.length()-3];
	ext[2] = path[path.length()-2];
	ext[3] = path[path.length()-1];
	ext[4] = '\0';
	return !strcmp(ext, ".png") || !strcmp(ext, "jpeg") || !strcmp(ext, ".jpg");
}