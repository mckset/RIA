void LoadConfig(){
	string settings[] = {"font_path:", "font_color:", "font_size:", 
		"board_color:", "grid_color:", "grid_size:", "origin_color:", "mouse_highlight:", "image_border:", "border_size:",
		"sidebar_color:", "sidebar_heading:", "sidebar_file:", "right_mouse_menu:", "import_background:"};

	int setCount = 15;

	f.open("config.cfg", ios::binary);

    if (!f.good()){
		if (DEBUG) printf("[Config] Missing config.cfg. Using default values.\n");
		return;
	}

	string var = "";

	while (f){
		if (!var.length()){
			var = Lower(GetLine());

			bool valid = false;
			for (string s : settings)
				if (var == s){
					valid = true;
					break;
				}
			if (var == "end")
				break;
			if (!valid)
				var = "";
		}else{
			string newLine;

			do{
				newLine = GetLine();
			}while (!newLine.length());

			bool valid = true;
			for (string s : settings)
				if (newLine == s){
					var = newLine;
					valid = false;
					break;
				}
			if (valid){
				
				if (var == settings[0] && stat(newLine.c_str(), &s) == 0)	fontPath = newLine;
				else if (var == settings[0])	continue;
				else if (var == settings[1]) 	fontColor = GetColor(newLine);
				else if (var == settings[2]) 	fontSize = (float)stoi(newLine);
				else if (var == settings[3]) 	backing = GetColor(newLine);
				else if (var == settings[4])	grid = GetColor(newLine);
				else if (var == settings[5])	gridSize = stoi(newLine);
				else if (var == settings[6])	cOrigin = GetColor(newLine);
				else if (var == settings[7])	highlight = GetColor(newLine);
				else if (var == settings[8])	imageBorder = GetColor(newLine);
				else if (var == settings[9])	borderSize = stoi(newLine);
				else if (var == settings[10])	menuBackground = GetColor(newLine);
				else if (var == settings[11])	locationHeading = GetColor(newLine);
				else if (var == settings[12])	locationFile = GetColor(newLine);
				else if (var == settings[13])	RMMenuBacking = GetColor(newLine);
				else if (var == settings[14])	importBacking = GetColor(newLine);

				var = "";
			}
		}
	}

	f.close();
	if (DEBUG) printf("Loaded config\n");
	highlight = Color {.7, .7, .7, .5};
}