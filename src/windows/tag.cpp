/*
	Draws the tag editor and handles the input
*/

Button addTag_Button = Button{"Confirm", menuBackgroundColor, highlightColor, White, SMALL_FONT_SIZE};
Button deleteTag_Button = Button{"Delete", menuBackgroundColor, highlightColor, White, SMALL_FONT_SIZE};

Color newTagColor;
ColorSelector newTag_ColorSelector = ColorSelector{Red, {0,0}, {TAG_COLOR_SELECTOR_SIZE, TAG_COLOR_SELECTOR_SIZE}};

// Draws the tag editor
void DrawTag(){

	newTagColor = newTag_ColorSelector.Draw({PADDING, PADDING}, {TAG_COLOR_SELECTOR_SIZE, TAG_COLOR_SELECTOR_SIZE}, SCROLLBAR_SIZE, newTagColor);

	newTagName_Field.color = newTagColor;
	newTagName_Field.Draw(Vector2{0, fHeight - FONT_SIZE*2}, Vector2{fWidth, FONT_SIZE*2}, ALIGN_CENTER);

	addTag_Button.Draw(Vector2{0, fHeight-FONT_SIZE*3}, Vector2{fWidth/2, FONT_SIZE}, ALIGN_CENTER);
	deleteTag_Button.Draw(Vector2{fWidth/2, fHeight-FONT_SIZE*3}, Vector2{fWidth/2, FONT_SIZE}, ALIGN_CENTER);
}


// Resets tag editor variables
void ResetTagEditor(){
	newTagName_Field.text = "";
	newTagColor = Red;
	editTag = nullptr;
	parentTag = nullptr;
	TagWin.Hide();
}

// Tag editor input
void TagInput(){

	if (addTag_Button.pressed){

		// New tag
		if (!editTag && !parentTag){
			tags.push_back(Tag{newTagName_Field.text, newTagColor});
			sort(tags.begin(), tags.end(), SortTag);

		// New sub tag
		}else if (parentTag){
			parentTag->subTags.push_back(Tag{newTagName_Field.text, newTagColor, true});
			sort(parentTag->subTags.begin(), parentTag->subTags.end(), SortTag);

		// Edit tag/sub tag
		}else {
			editTag->name = newTagName_Field.text;
			editTag->color = newTagColor;
		}

		ResetTagEditor();
		return;
	}else if (deleteTag_Button.pressed){

		// Cancel new tag
		if (!editTag){
			ResetTagEditor();
			return;
		}

		// Delete tag or sub tag
		for (int i = 0; i < tags.size(); i++){

			// Sub tag
			if (editTag->isSubTag){
				for (int s = 0; s < tags[i].subTags.size(); s++){
					if (&tags[i].subTags[s] == editTag){
						tags[i].subTags.erase(tags[i].subTags.begin() + s);
						ResetTagEditor();
						return;
					}
				}
				continue;
			}

			// Tag
			if (&tags[i] == editTag){
				tags.erase(tags.begin() + i);
				ResetTagEditor();
				return;
			}
		}
	}
}
