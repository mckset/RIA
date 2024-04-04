//
// Responsible for rendering everything except the import window
//


void DrawApp();
void DrawLocations();
void DrawTags();

bool selector = false;

Button import = Button{"Image Packs", menuBackground, highlight};
Button openTags = Button{"Tags", menuBackground, highlight};
Button openLoc = Button{"Locations", menuBackground, highlight};
Button closeB = Button{"Close", menuBackground, highlight};
Button helpB = Button{"Help", menuBackground, highlight};

string importText = "";
int importTime = 0;

void DrawApp(){
	//
	// Draw grid
	//
	sShape.Use();
	for (int w = -2; w < (Width/gridSize)/Scale + 1; w++){
		shape.Draw(Vector2{0, ((float)w*gridSize*Scale - ((int)(View.y/5) % (int)gridSize*Scale))*2}, Vector2{(float)Width*2, 3}, grid, true);
	}
	for (int h = -2; h < (Height/gridSize)/Scale + 1; h++){
		shape.Draw(Vector2{((float)h*gridSize*Scale - ((int)(View.x/5) % (int)gridSize*Scale))*2, 0}, Vector2{3, (float)Height*2}, grid, true);
	}

	// Origin
	if (drawOrigin){
		shape.Draw(Vector2{0,View.y/2 - Height*2/Scale}, Vector2{2/Scale, (float)Height*16/Scale}, cOrigin, false);
		shape.Draw(Vector2{View.x/2 - Width*2/Scale, 0}, Vector2{(float)Width*16/Scale, 2/Scale}, cOrigin, false);
	}

	//
	// Images
	//
	sImage.Use();
	for (int i = 0; i < imgs.size(); i++){
		imgs[i].Draw();
		if (imgs[i].selected){
			sShape.Use();
			imgs[i].DrawOutline(borderSize/Scale, imageBorder);
			sImage.Use();
		}
	}

	//
	// Selector box
	//
	if (selector){
		sShape.Use();
		shape.DrawBox(mouse.position, mouse.dragOff.Subtract(mouse.position), cSelector, 2, true, true);
		shape.Draw(mouse.position, mouse.dragOff.Subtract(mouse.position), cSelector, true);
	}

	sImage.Use();

	// Scale text
	char buf[64];
	int perScale = (int)(Scale*100+1)/10;
	snprintf(buf, sizeof(buf), "%d", perScale);
	if (perScale >= 10){
		buf[2] = '0';
		buf[3] = '%';
		buf[4] = '\0';
	}else{
		buf[1] = '0';
		buf[2] = '%';
		buf[3] = '\0';
	}
	font.Write(buf, Vector2{(float)Width/2,(float)Height-33}, 24, Black, true);
	font.Write(buf, Vector2{(float)Width/2,(float)Height-32}, 24, White, true);


	// Saving
	if (save > 0){
		save--;;
		font.Write("Saved", Vector2{0,46}, fontSize, Black, true, Width, 1);
		font.Write("Saved", Vector2{0,48}, fontSize, White, true, Width, 1);
	
	// Import Text
	}else if (importTime > 0){
		importTime--;
		font.Write(importText, Vector2{0,46}, fontSize, Black, true, Width, 1);
		font.Write(importText, Vector2{0,48}, fontSize, White, true, Width, 1);
	}

	if (drawMouseMenu)
		rmMenu.Draw(fontSize);

	//
	// Image preview
	//
	if (previewImg.loaded){
		sShape.Use();
		shape.Draw(Vector2{(float)Width/5*4-fontSize*4, 0}, Vector2{(float)Width/5+fontSize*4, (float)Width/5+fontSize*4}, menuBackground, true);

		sImage.Use();

		// Draw image centered 
		previewImg.Draw(
			Vector2{(float)Width/5*4+(Width/5-previewImg.size.x)/2-fontSize*2, ((float)Width/5 - previewImg.size.y)/2 + fontSize/2}, 
			previewImg.size, White, true);

		font.Write("Preview", Vector2{(float)Width/5*4-fontSize*4, (float)Width/5+fontSize}, fontSize*1.5, White, true, Width/5+fontSize*2, 1);
		
		// Mouse 
		if (mouse.position.x > Width/3 || !menu){
			sShape.Use();
			shape.Draw(
				mouse.position.Subtract(Vector2{(float)previewImg.trueWidth, (float)previewImg.trueHeight}.Multiply(Scale).Divide(2)), 
				Vector2{(float)previewImg.trueWidth, (float)previewImg.trueHeight}.Multiply(Scale), 
				menuBackground, true);
		}
	}


	openTags.size = Vector2{fontSize*4, fontSize*1};
	openLoc.size = Vector2{fontSize*4, fontSize*1};
	helpB.size = Vector2{fontSize*4, fontSize*1};
	if (menu){
		sShape.Use();
		shape.Draw(Vector2{0}, Vector2{(float)Width/3, (float)Height}, menuBackground, true);
		if (tagView)
			DrawTags();
		else
			DrawLocations();

		// Menu Buttons
		if (!tagView){
			openTags.position = Vector2{(float)Width/3-fontSize+scrollbarSize, (float)Height-fontSize};
			openLoc.position = Vector2{-(float)Width/3-fontSize+scrollbarSize, -(float)Height-fontSize};
			add.position = Vector2{(float)Width/3-fontSize*2-scrollbarSize, (float)Height-fontSize*2+locScroll.scroll};
			openTags.Draw();
		}else{
			openTags.position = Vector2{-(float)Width/3-fontSize+scrollbarSize, -(float)Height-fontSize};
			openLoc.position = Vector2{(float)Width/3-fontSize+scrollbarSize, (float)Height-fontSize};
			add.position = Vector2{(float)Width/3-fontSize*2-scrollbarSize, (float)Height-fontSize*2+tagScroll.scroll};
			openLoc.Draw();
		}
		add.size = Vector2{fontSize*2, fontSize*2};
		closeB.position = Vector2{(float)Width/3-fontSize+scrollbarSize, (float)Height-fontSize*2};
		closeB.size = Vector2{fontSize*4, fontSize*1};

		add.Draw(false, true);
		closeB.Draw();
		
	}else{
		openTags.position = Vector2{8, (float)Height-fontSize-8};
		openLoc.position = Vector2{8, (float)Height-fontSize*2-8};
		helpB.position = Vector2{8, (float)Height-fontSize*3-8};
		openTags.Draw();
		openLoc.Draw();
		helpB.Draw();
	}


	// Import button
	import.position = Vector2{(float)Width - fontSize*5-12, (float)Height - fontSize-8};
	import.size = Vector2{fontSize*5, fontSize};
	import.Draw();
	
}

//
// Locations
//
void DrawLocations(){
	float y = Height-fontSize*2+locScroll.scroll;
	locScroll.end = 0;

	sShape.Use();
	shape.Draw(Vector2{0, y}, Vector2{(float)Width/3-scrollbarSize, fontSize*2}, menuBackground, true);

	sImage.Use();
	font.Write("Locations", Vector2{8, y}, fontSize*1.5, fontColor, true, (float)Width/3-scrollbarSize);

	y-=fontSize;

	// Locations
	for (int i = 0; i < locations.size(); i++){
		if (y > -fontSize){
			bool del = locations[i].Draw(Vector2{0, y}, Vector2{(float)Width/3-scrollbarSize, fontSize});
			if (del){
				vector<Table>:: iterator t = locations.begin();
				advance(t, i);
				locations.erase(t);
				return;
			}
			y -= locations[i].listSize;
		}else
			y -= locations[i].GetSize(Vector2{(float)Width/3-scrollbarSize, fontSize});
		
		locScroll.end += locations[i].listSize;
	}

	// Scroll bar
	locScroll.end -= Height;
	if (locScroll.end < 0){
		locScroll.end = 0;
		locScroll.scroll = 0;
	}

	sShape.Use();
	locScroll.Draw(Vector2{(float)Width/3-scrollbarSize, 0}, Vector2{scrollbarSize, (float)Height});

}

//
// Tags
//
void DrawTags(){
	float y = Height-fontSize*2+tagScroll.scroll;
	tagScroll.end = 0;

	sShape.Use();
	shape.Draw(Vector2{0, y}, Vector2{(float)Width/3-scrollbarSize, fontSize*2}, menuBackground, true);

	sImage.Use();
	font.Write("Tags", Vector2{8, y}, fontSize*1.5, fontColor, true, (float)Width/3-scrollbarSize);

	y-=fontSize;

	for (int i = 0; i < tags.size(); i++){
		if (y > -fontSize){
			bool del = tags[i].Draw(Vector2{0, y}, Vector2{(float)Width/3-scrollbarSize, fontSize}, i);
			if (del){
				vector<Tag>:: iterator t = tags.begin();
				advance(t, i);
				tags.erase(t);
				return;
			}
			y -= tags[i].listSize;
		}else
			y -= tags[i].GetSize(Vector2{(float)Width/3-scrollbarSize, fontSize});
		
		tagScroll.end += tags[i].listSize;
	}

	// Scroll bar
	tagScroll.end -= Height;
	tagScroll.end += fontSize*2;
	if (tagScroll.end < 0){
		tagScroll.end = 0;
		tagScroll.scroll = 0;
	}

	sShape.Use();
	tagScroll.Draw(Vector2{(float)Width/3-scrollbarSize, 8}, Vector2{scrollbarSize, (float)Height-16});
}