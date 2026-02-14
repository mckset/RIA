void DownloaderInput(){

	if (downloadImport_Button.pressed || downloadedImageName_Field.submitted){
		if (!downloadedImageName_Field.text.length()) return;

		string ext = ".jpg";
		if (downloadedImageType == PNG)
			ext = ".png";
		else if (downloadedImageType == WEBP)
			ext = ".webp";

		filesystem::rename(path + "downloads" + slash[0] + downloadFile, path + "downloads" + slash[0] + downloadedImageName_Field.text + ext);
		DownloadWin.Hide();
		for (auto& location : locations){
			if (location.path == path + "downloads"){
				location.initialized = false;
				break;
			}
		}

		ImageContainer img;
		img.Load(path + "downloads" + slash[0] + downloadedImageName_Field.text + ext);
		img.position = Main.view - img.size/2;
		imgs.push_back(img);

		downloadedImageName_Field.Reset();

	}else if (downloadCancel_Button.pressed){
		DownloadWin.Hide();
	}
}