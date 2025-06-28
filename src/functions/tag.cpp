/*
	Draws the tag editor and handles the input
*/

Button addTag = Button{"Confirm", menuBackground, highlight, White, fontSize/2};
Button delTag = Button{"Delete", menuBackground, highlight, White, fontSize/2};

Color newColor;
Vector2 csPos = Vector2{8,6};
bool csLock = false;

float huePosition;
Color hue;

// Draws the tag editor
void DrawTag(){

	// Gets the color from the color selector
	Color HSV = hue.ToHSV();
	HSV.g = ((csPicker.x) / csSize.x)*100;
	HSV.b = ((csPicker.y) / csSize.y)*100;
	newColor = ToRGB(HSV.r, HSV.g, HSV.b);
	
	// Color selector math
	float c = 1-((float)sHue.scroll/(float)sHue.end);
	if (c <= .33) hue = Color{1-(c/.33f), c/.33f, 0, 1};
	else if (c <=.67) hue = Color{0, 1-((c-.33f)/.33f), (c-.33f)/.33f, 1};
	else hue = Color{((c-.67f)/.33f), 0, 1-(c-.67f)/.33f, 1};

	// Draw the color selector
	shape.DrawHueSelector(csPos+Vector2{csSize.x+16,0}, {64, csSize.y});
	sHue.Draw(csPos+Vector2{csSize.x+16,0}, {64, csSize.y});
	shape.DrawColorSelector(csPos, csSize, hue);
	shape.DrawCircle(csPicker + csPos, 24, 24, newColor);
	shape.DrawCircle(csPicker + csPos, 24, 4, Black);

	// Draws the tag name and options
	tagName.color = newColor;
	tagName.Draw(Vector2{0, fHeight - fontSize*1.5f}, Vector2{fWidth, fontSize*1.5f}, true, true);
	addTag.Draw(Vector2{0, fHeight-fontSize*2.5f}, Vector2{fWidth/2, fontSize}, false, true, TagWin.Focus());
	delTag.Draw(Vector2{fWidth/2, fHeight-fontSize*2.5f}, Vector2{fWidth/2, fontSize}, false, true, TagWin.Focus());
}

// Tag editor input
void TagInput(){
	// Color selector input
	if (mouse.position.Within(csPos, csSize) && mouse.state == LM_DOWN && !sHue.scrollLock || csLock){
		csPicker = mouse.position - csPos;

		if (csPicker.x < 0)
			csPicker.x = 0;
		else if (csPicker.x > csSize.x)
			csPicker.x = csSize.x;

		if (csPicker.y < 0)
			csPicker.y = 0;
		else if (csPicker.y > csSize.y)
			csPicker.y = csSize.y;

		csLock = true;

		if (mouse.state != LM_DOWN)
			csLock = false;
	}

	if (mouse.Click()){
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
			return;
		}
	}
	
	// Editing the text field
	if (tagName.active){
		//keyboard.Type(&tagName.text);
		tagName.UpdateText();
		CheckString(tagName.text);
	}
}

// Resets tag editor variables
void ResetTagEdit(){
	editTag = -1;
	editSub = -1;
	tagName.text = "";
	tagName.color = Black;
	rgb[0].scroll = 0;
	rgb[1].scroll = 0;
	rgb[2].scroll = 0;
}