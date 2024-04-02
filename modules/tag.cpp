void DrawTag();
void TagInput();
void ResetTagEdit();


Button addTag = Button{"Add Tag", menuBackground, highlight};

void DrawTag(){
	sShape.Use();
	for (int i = 0; i < 3; i++)
		rgb[i].Draw(Vector2{32, (float)Height/5*i + Height/5}, Vector2{(float)Width-64, (float)scrollbarSize/2});
	tagName.position = Vector2{32, (float)Height/5*4};
	tagName.size = Vector2{(float)Width-64, (float)scrollbarSize};
	tagName.background = Color{(float)rgb[0].scroll/255, (float)rgb[1].scroll/255, (float)rgb[2].scroll/255, 1};
	tagName.Draw();
	addTag.position = Vector2{8, 0};
	addTag.size = Vector2{fontSize*8, fontSize*2};
	addTag.Draw();
}

void TagInput(){
	if (mouse.Click(LM_DOWN)){
		tagName.CheckClick();
		if (addTag.Hover()){
			// New Tag
			if (editTag == -1)
				tags.push_back(Tag{tagName.text, tagName.background});
			
			// Edit Tag
			else if (editSub == -1){
				tags[editTag].name = tagName.text;
				tags[editTag].color = tagName.background;

			// New Sub Tag
			}else if (editSub == -2)
				tags[editTag].subTags.push_back(Tag{tagName.text, tagName.background});
			
			// Edit Sub Tag
			else{
				tags[editTag].subTags[editSub].name = tagName.text;
				tags[editTag].subTags[editSub].color = tagName.background;
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
		if (keyboard.newKey > 31 && keyboard.newKey < 127 && keyboard.newKey != KEY_BACKSPACE)
			tagName.text += KeyToChar(keyboard.newKey);
		else if (keyboard.newKey == KEY_BACKSPACE && tagName.text.length())
			tagName.text = tagName.text.substr(0, tagName.text.length()-1);
		else if (keyboard.newKey == KEY_ENTER)
			tagName.active = false;
	
	keyboard.newKey = -1;
	mouse.prevState = mouse.state;
}

void ResetTagEdit(){
	editTag = -1;
	editSub = -1;
	tagName.text = "";
	tagName.background = Black;
	rgb[0].scroll = 0;
	rgb[1].scroll = 0;
	rgb[2].scroll = 0;
}