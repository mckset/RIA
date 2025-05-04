Button settingsButtons[11] = {
	Button{"Font", menuBackground, highlight, fontColor, fontSize/2},
	Button{"Background", menuBackground, highlight, fontColor, fontSize/2},
	Button{"Grid", menuBackground, highlight, fontColor, fontSize/2},
	Button{"Origin", menuBackground, highlight, fontColor, fontSize/2},
	Button{"Button Hover", menuBackground, highlight, fontColor, fontSize/2},
	Button{"Mouse Menu", menuBackground, highlight, fontColor, fontSize/2},
	Button{"Side Panels", menuBackground, highlight, fontColor, fontSize/2},
	Button{"Headers", menuBackground, highlight, fontColor, fontSize/2},
	Button{"Image Border", menuBackground, highlight, fontColor, fontSize/2},
	Button{"File Indicator", menuBackground, highlight, fontColor, fontSize/2},
	Button{"Import Menu", menuBackground, highlight, fontColor, fontSize/2}
};

Color oldColors[11];

int settingsButtonCount = 11;
ColorSelector settingsColors;
/*
void CopyColors(){
	oldColors[0] = fontColor;
	oldColors[1] = backing;
	oldColors[2] = grid;
	oldColors[3] = cOrigin;
	oldColors[4] = highlight;
	oldColors[5] = RMMenuBackground;
	oldColors[6] = menuBackground;
}*/

void DrawSettings(){
	int y = fHeight;
	shape.Draw({fWidth, y}, {-(menuWidth-scrollbarSize), -fontSize*2}, menuBackground, true);
	font.Write("Settings", {fWidth-menuWidth, y-fontSize*2}, fontSize, fontColor, true, menuWidth, 1);
}