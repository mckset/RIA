	class Field{
		public:
			string emptyText = "";
			Color color = White;
			Color activeColor = Grey;
			Color textColor = Black;
			Color border = Black;
			int borderWidth = 0;
			float fontSize = 0;
			Vector2 position = Vector2{0,0};
			Vector2 size = Vector2{0,0};
			string text = "";
			bool active = false;

			void Draw(Vector2 p, Vector2 s, bool fixed = true, bool centerText = false){
				position = p;
				size = s;
				Draw(fixed, centerText);
			}

			void Draw(bool fixed = true, bool centerText = false){
				shape.Draw(position, size, active ? activeColor : color, fixed);
				shape.DrawBox(position, size, border, borderWidth, fixed);
				
				if (!text.length() && !active)
					font.Write(emptyText.data(), position.Add(4,0), fontSize == 0 ? size.y/4*3 : fontSize, textColor, fixed, size.x, centerText);
				else
					font.Write(text.data(), position.Add(4,0), fontSize == 0 ? size.y/4*3 : fontSize, textColor, fixed, size.x, centerText);
			}

			void CheckClick(){
				active = false;
				if (mouse.position.Within(position, size))
					active = true;
			}

			void UpdateText(bool integers = false){
				if (keyboard.newKey != KEY_BACKSPACE){
					char c = KeyToChar(keyboard.newKey);
					if (!integers && keyboard.newKey < 127 && keyboard.newKey > 31){
						text += c;
					}else if (integers && c < 58 && c > 47){
						text += c;
					}
				}else if (keyboard.newKey == KEY_BACKSPACE){
					if (text.length()){
						text = text.substr(0, text.length()-1);
					}
				}
			}

			string ToString(){
				return "Field {\"" + emptyText + "\", " + color.ToString() + ", " + activeColor.ToString() + ", " + textColor.ToString() + "}";
			}
	};