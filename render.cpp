//
// Responsible for rendering everything in the main window
//
void DrawApp();
void DrawLocations();
void DrawTags();

bool selector = false;

Button import = Button{"Image Packs", menuBackground, highlight, White, fontSize/2};
Button loadBoard = Button{"Load Board", menuBackground, highlight, White, fontSize/2};
Button newBoard = Button{"New Board", menuBackground, highlight, White, fontSize/2};
Button openTags = Button{"Tags", menuBackground, highlight, White, fontSize/2};
Button openLoc = Button{"Locations", menuBackground, highlight, White, fontSize/2};
Button closeB = Button{"Close", menuBackground, highlight, White, fontSize/2};
Button helpB = Button{"Help", menuBackground, highlight, White, fontSize/2};

string importText = "";
int importTime = 0;

vector<Field> textBoxes;

void DrawApp(){
	//
	// Draw grid
	//
	for (int w = -2; w < (Width/gridSize)/(*Scale) + 1; w++){
		shape.Draw(Vector2{0, ((float)w*gridSize*(*Scale) - ((int)(View->y/5) % (int)gridSize*(*Scale)))*2}, Vector2{fWidth*2, 3}, grid, true);
	}
	for (int h = -2; h < (Height/gridSize)/(*Scale) + 1; h++){
		shape.Draw(Vector2{((float)h*gridSize*(*Scale) - ((int)(View->x/5) % (int)gridSize*(*Scale)))*2, 0}, Vector2{3, fHeight*2}, grid, true);
	}

	// Origin
	if (drawOrigin){
		shape.Draw(Vector2{0,View->y/2 - Height*2/(*Scale)}, Vector2{2/(*Scale), fHeight*16/(*Scale)}, cOrigin, false);
		shape.Draw(Vector2{View->x/2 - Width*2/(*Scale), 0}, Vector2{fWidth*16/(*Scale), 2/(*Scale)}, cOrigin, false);
	}

	//
	// Images
	//
	for (int i = 0; i < imgs.size(); i++){
		imgs[i].Draw();
		if (imgs[i].selected)
			imgs[i].DrawOutline(borderSize/(*Scale), imageBorder);
	}

	//
	// Selector box
	//
	if (selector){
		shape.DrawBox(mouse.position, mouse.dragOff.Subtract(mouse.position), cSelector, 2, true, true);
		shape.Draw(mouse.position, mouse.dragOff.Subtract(mouse.position), cSelector, true);
	}


	// Scale text
	char buf[64];
	int perScale = (int)((*Scale)*100+1)/10;
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
	font.Write(buf, Vector2{fWidth/2,fHeight-fontSize*2}, fontSize*.75f, Black, true);
	font.Write(buf, Vector2{fWidth/2,fHeight-fontSize*2}, fontSize*.75f, White, true);


	// Saving
	if (save > 0){
		save--;;
		font.Write("Saved", Vector2{2,fontSize-2}, fontSize*.75f, Black, true, Width, 1);
		font.Write("Saved", Vector2{0,fontSize}, fontSize*.75f, White, true, Width, 1);
	
	// Import Text
	}else if (importTime > 0){
		importTime--;
		font.Write(importText, Vector2{2,fontSize-2}, fontSize*.75f, Black, true, Width, 1);
		font.Write(importText, Vector2{0,fontSize}, fontSize*.75f, White, true, Width, 1);
	}

	if (drawMouseMenu)
		rmMenu.Draw();

	//
	// Image preview
	//
	if (previewImg.loaded){
		shape.Draw(Vector2{fWidth/5*4-fontSize*4, 0}, Vector2{fWidth/5+fontSize*4, fWidth/5+fontSize*4}, menuBackground, true);

		// Draw image centered 
		previewImg.Draw(
			Vector2{fWidth/5*4+(Width/5-previewImg.size.x)/2-fontSize*2, (fWidth/5 - previewImg.size.y)/2 + fontSize/2}, 
			previewImg.size, White, true);

		font.Write("Preview", Vector2{fWidth/5*4-fontSize*4, fWidth/5+fontSize}, fontSize, White, true, Width/5+fontSize*2, 1);
		
		// Mouse 
		if (mouse.position.x > menuWidth || !menu){
			shape.Draw(
				mouse.position.Subtract(Vector2{(float)previewImg.width, (float)previewImg.height}.Multiply((*Scale)).Divide(2)), 
				Vector2{(float)previewImg.width, (float)previewImg.height}.Multiply((*Scale)), 
				menuBackground, true);
		}
	}


	// Left buttons
	if (menu){
		shape.Draw(Vector2{0}, Vector2{menuWidth, fHeight}, menuBackground, true);
		if (tagView)
			DrawTags();
		else
			DrawLocations();

		// Menu Buttons
		if (!tagView){
			openTags.Draw(Vector2{menuWidth-fontSize+scrollbarSize, fHeight-fontSize-8}, Vector2{fontSize*5, fontSize*1});
			openLoc.visible = false;
		}else{
			openLoc.Draw(Vector2{menuWidth-fontSize+scrollbarSize, fHeight-fontSize-8}, Vector2{fontSize*5, fontSize*1});
			openTags.visible = false;
		}
		closeB.Draw(Vector2{menuWidth-fontSize+scrollbarSize, fHeight-fontSize*2-8}, Vector2{fontSize*5, fontSize*1});
		
	}else{
		openTags.Draw(Vector2{8, fHeight-fontSize-8}, Vector2{fontSize*5, fontSize*1});
		openLoc.Draw(Vector2{8, fHeight-fontSize*2-8}, Vector2{fontSize*5, fontSize*1});
		helpB.Draw(Vector2{8, fHeight-fontSize*3-8}, Vector2{fontSize*5, fontSize*1});
		add.visible = false;
		closeB.visible = false;
	}

	// Right buttons
	import.Draw(Vector2{fWidth - fontSize*6-12, fHeight - fontSize-8}, Vector2{fontSize*6, fontSize});
	loadBoard.Draw(Vector2{fWidth - fontSize*6-12, fHeight - fontSize*2-8}, Vector2{fontSize*6, fontSize});
	newBoard.Draw(Vector2{fWidth - fontSize*6-12, fHeight - fontSize*3-8}, Vector2{fontSize*6, fontSize});
	
	sShape.Use();
}

//
// Locations
//
void DrawLocations(){
	float y = Height-fontSize*2+locScroll.scroll;
	locScroll.end = fontSize;

	shape.Draw(Vector2{0, y}, Vector2{menuWidth-scrollbarSize, fontSize*2}, menuBackground, true);
	font.Write("Locations", Vector2{8, y}, fontSize, fontColor, true, menuWidth-scrollbarSize);
	add.Draw(Vector2{menuWidth-fontSize*2-scrollbarSize, y}, Vector2{fontSize*2, fontSize*2}, false, true);

	y-=fontSize;

	// Locations
	for (int i = 0; i < locations.size(); i++){
		if (y > -fontSize){
			bool del = locations[i].Draw(Vector2{0, y}, Vector2{menuWidth-scrollbarSize, fontSize});
			if (del){
				locations.erase(locations.begin() + i);
				return;
			}
			y -= locations[i].listSize;
			locScroll.end += locations[i].listSize;
		}else{
			y -= locations[i].GetSize(Vector2{menuWidth-scrollbarSize, fontSize});
			locScroll.end += locations[i].GetSize(Vector2{menuWidth-scrollbarSize, fontSize});
		}
	}

	// Scroll bar
	locScroll.end -= Height;
	locScroll.end += fontSize*2;
	if (locScroll.end < 0){
		locScroll.end = 0;
		locScroll.scroll = 0;
	}
	locScroll.Draw(Vector2{menuWidth-scrollbarSize, 0}, Vector2{scrollbarSize, fHeight});
}

//
// Tags
//
void DrawTags(){
	float y = Height-fontSize*2+tagScroll.scroll;
	tagScroll.end = fontSize;

	shape.Draw(Vector2{0, y}, Vector2{menuWidth-scrollbarSize, fontSize*2}, menuBackground, true);
	font.Write("Tags", Vector2{8, y}, fontSize, fontColor, true, menuWidth-scrollbarSize);
	add.Draw(Vector2{menuWidth-fontSize*2-scrollbarSize, y}, Vector2{fontSize*2, fontSize*2}, false, true);

	y-=fontSize;

	for (int i = 0; i < tags.size(); i++){
		if (y > -fontSize){
			bool del = tags[i].Draw(Vector2{0, y}, Vector2{menuWidth-scrollbarSize, fontSize}, i);
			if (del){
				vector<Tag>:: iterator t = tags.begin();
				advance(t, i);
				tags.erase(t);
				return;
			}
			y -= tags[i].listSize;
			tagScroll.end += tags[i].listSize;
		}else{
			y -= tags[i].GetSize(Vector2{menuWidth-scrollbarSize, fontSize});
			tagScroll.end += tags[i].GetSize(Vector2{menuWidth-scrollbarSize, fontSize});
		}
	}

	// Scroll bar
	tagScroll.end -= Height;
	tagScroll.end += fontSize*2;
	if (tagScroll.end < 0){
		tagScroll.end = 0;
		tagScroll.scroll = 0;
	}

	tagScroll.Draw(Vector2{menuWidth-scrollbarSize, 0}, Vector2{scrollbarSize, fHeight});
}