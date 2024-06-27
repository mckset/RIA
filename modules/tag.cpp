void DrawTag();
void TagInput();
void ResetTagEdit();

Button addTag = Button{"Confirm", menuBackground, highlight, White, fontSize/2};
Button delTag = Button{"Delete", menuBackground, highlight, White, fontSize/2};

Color newColor;
Color newColorHue;
Vector2 csPicker = Vector2{256,256};
Vector2 csSize = Vector2{256, 256};
Vector2 csPos = Vector2{8,8};
bool csLock = false;

float csSaturation = 0;
float csDarkness = 0;

void DrawTag(){
	newColorHue = Color{(float)rgb[0].scroll/255, (float)rgb[1].scroll/255, (float)rgb[2].scroll/255, 1};
	newColor = newColorHue.Brighten(csSaturation).Darken(csDarkness);

	tagName.color = newColor;
	tagName.Draw(Vector2{0, fHeight - fontSize*2}, Vector2{fWidth, fontSize*2}, true, true);
	addTag.Draw(Vector2{0, fHeight-fontSize*3}, Vector2{fWidth/2, fontSize}, false, true);
	delTag.Draw(Vector2{fWidth/2, fHeight-fontSize*3}, Vector2{fWidth/2, fontSize}, false, true);
	
	// Color selector
	rgb[0].Draw(Vector2{csPos.x + csSize.x + 8, 80}, Vector2{fWidth-(csPos.x + csSize.x + 8)-8, 16}, true, rgb[1].scrollLock || rgb[2].scrollLock);
	rgb[1].Draw(Vector2{csPos.x + csSize.x + 8, 128}, Vector2{fWidth-(csPos.x + csSize.x + 8)-8, 16}, true, rgb[0].scrollLock || rgb[2].scrollLock);
	rgb[2].Draw(Vector2{csPos.x + csSize.x + 8, 176}, Vector2{fWidth-(csPos.x + csSize.x + 8)-8, 16}, true, rgb[0].scrollLock || rgb[1].scrollLock);
	

	shape.DrawColorSelector(csPos, csSize, newColorHue);
	shape.DrawCircle(csPicker + csPos, 24, 24, newColor);
	shape.DrawCircle(csPicker + csPos, 24, 4, Black);
}

void TagInput(){
	if (mouse.position.Within(csPos, csSize) && mouse.state == LM_DOWN && !rgb[0].scrollLock && !rgb[1].scrollLock && !rgb[2].scrollLock || csLock){
		csPicker = mouse.position - csPos;

		if (csPicker.x < 0)
			csPicker.x = 0;
		else if (csPicker.x > csSize.x)
			csPicker.x = csSize.x;

		if (csPicker.y < 0)
			csPicker.y = 0;
		else if (csPicker.y > csSize.y)
			csPicker.y = csSize.y;

		csSaturation = 1 - csPicker.x/csSize.x;
		csDarkness = 1 - csPicker.y/csSize.y;
		csLock = true;

		if (mouse.state != LM_DOWN)
			csLock = false;
	}

	if (mouse.Click(LM_DOWN)){
		tagName.CheckClick();
		if (addTag.Hover()){
			// New Tag
			if (editTag == -1)
				tags.push_back(Tag{tagName.text, tagName.color});
			
			// Edit Tag
			else if (editSub == -1){
				tags[editTag].name = tagName.text;
				tags[editTag].color = tagName.color;

			// New Sub Tag
			}else if (editSub == -2)
				tags[editTag].subTags.push_back(Tag{tagName.text, tagName.color});
			
			// Edit Sub Tag
			else{
				tags[editTag].subTags[editSub].name = tagName.text;
				tags[editTag].subTags[editSub].color = tagName.color;
			}
			TagWin.Hide();
			if (editSub == -2 || editSub >= 0)
				sort(tags[editTag].subTags.begin(), tags[editTag].subTags.end(), SortTag);
			else
				sort(tags.begin(), tags.end(), SortTag);
			
			ResetTagEdit();
			keyboard.newKey = -1;
			mouse.prevState = mouse.state;
			return;
		}else if (delTag.Hover()){
			// New Tag
			if (editTag == -1)
				return;
			
			// Edit Tag
			else if (editSub == -1){
				vector<Tag>:: iterator t = tags.begin();
				advance(t, editTag);
				tags.erase(t);

			// New Sub Tag
			}else if (editSub == -2)
				return;
			
			// Edit Sub Tag
			else{
				vector<Tag>:: iterator t = tags[editTag].subTags.begin();
				advance(t, editSub);
				tags[editTag].subTags.erase(t);
			}

			TagWin.Hide();
			if (editSub == -2 || editSub >= 0)
				sort(tags[editTag].subTags.begin(), tags[editTag].subTags.end(), SortTag);
			else
				sort(tags.begin(), tags.end(), SortTag);
			
			ResetTagEdit();
			keyboard.newKey = -1;
			mouse.prevState = mouse.state;
			return;
		}
	}
	
	// Editing the text field
	if (tagName.active)
		keyboard.Type(&tagName.text);
	
	keyboard.newKey = -1;
	mouse.prevState = mouse.state;
}

void ResetTagEdit(){
	editTag = -1;
	editSub = -1;
	tagName.text = "";
	tagName.color = Black;
	rgb[0].scroll = 0;
	rgb[1].scroll = 0;
	rgb[2].scroll = 0;
}