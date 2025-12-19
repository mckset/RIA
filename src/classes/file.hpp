enum Image_Types{
	INVALID,
	JPG,
	PNG,
	WEBP
};

class File{
	public:
		string name = "";
		string path = "";
};

bool IsImage(string path){
	string ext = Lower(path.substr(path.length()-4));
	return ext == ".png" || ext == "jpeg" || ext == ".jpg" || ext == "webp";
}


