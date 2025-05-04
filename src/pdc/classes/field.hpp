	class Field{
		public:
			string emptyText = "";
			Color color = White;
			Color activeColor = Grey;
			Color textColor = Black;
			Color border = Black;
			int borderWidth = 0;
			float fontSize = 0;
			int cursorPosition = 0;
			Vector2 position = Vector2{0,0};
			Vector2 size = Vector2{0,0};
			string text = "";
			bool active = false;
			float blink = 1;

			void Draw(Vector2 p, Vector2 s, bool fixed = true, bool centerText = false){
				position = p;
				size = s;
				Draw(fixed, centerText);
			}

			void Draw(bool fixed = true, bool centerText = false){
				shape.Draw(position, size, active ? activeColor : color, fixed);
				shape.DrawBox(position, size, border, borderWidth, fixed);
				
				if (!text.length() && !active)
					font.Write(emptyText, position.Add(4,0), fontSize == 0 ? size.y/4*3 : fontSize, textColor, fixed, size.x, centerText);
				else{
					string outText = text;
					if (active){
						outText = text.substr(0, cursorPosition);
						outText += (blink > 0.2f) ? "|" : " ";
						outText += text.substr(cursorPosition);
						blink -= deltaTime;
						if (blink < 0) blink = 1;
					}
					font.Write(outText, position.Add(4,0), fontSize == 0 ? size.y/4*3 : fontSize, textColor, fixed, size.x, centerText);
				}
			}

			void CheckClick(){
				active = false;
				if (mouse.position.Within(position, size)){
					active = true;
					cursorPosition = text.length();
				}
			}

			void Reset(){
				text = "";
				active = false;
				cursorPosition = 0;
			}

			void UpdateText(bool integers = false){
				
				// Navigation
				if (keyboard.newKey == KEY_LEFT){
					// Go to beginning of text
					if (keyboard.ctrl)
						cursorPosition = 0;

					else if (cursorPosition != 0)
						cursorPosition--;

				}else if (keyboard.newKey == KEY_RIGHT){
					// Go to end of text
					if (keyboard.ctrl)
						cursorPosition = text.length();

					else if (cursorPosition != text.length())
						cursorPosition++;

				// Deactivating field
				}else if (keyboard.newKey == KEY_ENTER || keyboard.newKey == KEY_ESCAPE){
					active = false;

				// Backspace
				}else if (keyboard.newKey == KEY_BACKSPACE){
					if (text.length() && cursorPosition != 0){
						text = text.substr(0, cursorPosition-1) + text.substr(cursorPosition);
						cursorPosition--;
					}

				// Deleting the next character
				}else if (keyboard.newKey == KEY_DELETE && cursorPosition != text.length()){
					text = text.substr(0, cursorPosition) + text.substr(cursorPosition+1);

				// Clear field
				}else if (keyboard.ctrl && keyboard.newKey == KEY_DELETE){
					cursorPosition = 0;
					text = "";

				// Typing and such
				}else{
					char c = KeyToChar(keyboard.newKey);
					if ((!integers && keyboard.newKey < 127 && keyboard.newKey > 31) || 
						(integers && c < 58 && c > 47)){
						text = text.substr(0, cursorPosition) + c +  text.substr(cursorPosition);
						cursorPosition++;
					}

				}
			}

			string ToString(){
				return "Field {\"" + emptyText + "\", " + color.ToString() + ", " + activeColor.ToString() + ", " + textColor.ToString() + "}";
			}
	};