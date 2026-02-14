char jpgHeader[] = {(char)0xFF, (char)0xD8, (char)0xFF, (char)0xE0};
char pngHeader[] = {(char)0x89, (char)0x50, (char)0x4E, (char)0x47};
char webpHeader[] = {(char)0x52, (char)0x49, (char)0x46, (char)0x46};

void DrawDownloader(){
	downloadedImageName_Field.Draw({PADDING, fHeight-PADDING-FONT_SIZE*2}, {fWidth-PADDING*2, FONT_SIZE*2}, ALIGN_CENTER);

	downloadImport_Button.Draw({PADDING, PADDING}, {fWidth/2-PADDING*2, FONT_SIZE*2}, ALIGN_CENTER);
	downloadCancel_Button.Draw({Width/2+PADDING, PADDING}, {fWidth/2-PADDING*2, FONT_SIZE*2}, ALIGN_CENTER);
}

void GetDownloadedFileType(){
	ifstream f;
	f.open(path + "downloads" + slash[0] + downloadFile, ios::in | ios::binary);
	char header[4];
	f.read(header, 4);
	f.close();

	downloadedImageType = INVALID;

	// JPG
	if (header[0] == jpgHeader[0] && 
		header[1] == jpgHeader[1] &&
		header[2] == jpgHeader[2] &&
		header[3] == jpgHeader[3])
		downloadedImageType = JPG;

	// PNG
	if (header[0] == pngHeader[0] && 
		header[1] == pngHeader[1] &&
		header[2] == pngHeader[2] &&
		header[3] == pngHeader[3])
		downloadedImageType = PNG;

	// WEBP
	if (header[0] == webpHeader[0] && 
		header[1] == webpHeader[1] &&
		header[2] == webpHeader[2] &&
		header[3] == webpHeader[3])
		downloadedImageType = WEBP;
			
}