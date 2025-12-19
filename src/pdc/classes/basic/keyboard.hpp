class Keyboard{
	public:
		int event;
		int newKey;
		int prevKey;
		bool enter = false;
		bool ctrl = false;
		bool shift = false;

		vector<int> keys;
		
		bool GetKey(int key){
			for (auto k : keys)
				if (k == key)
					return true;
			return false;
		}

		void AddKey(int key){
			enter = (key == KEY_ENTER || key == KEY_KP_ENTER || enter);
			shift = (key == KEY_LEFT_SHIFT || key == KEY_RIGHT_SHIFT || shift);
			ctrl = (key == KEY_LEFT_CONTROL || key == KEY_RIGHT_CONTROL || ctrl);

			newKey = key;
			bool exists = false;
			for (auto k : keys)
				if (k == key){
					exists = true;
					return;
				}
			if (!exists)
				keys.push_back(key);
		}

		void DelKey(int key){
			if (key == KEY_ENTER && !GetKey(KEY_KP_ENTER) || key == KEY_KP_ENTER && !GetKey(KEY_ENTER))
				enter = false;
			else if (key == KEY_LEFT_SHIFT && !GetKey(KEY_RIGHT_SHIFT) || key == KEY_RIGHT_SHIFT && !GetKey(KEY_LEFT_SHIFT))
				shift = false;
			else if (key == KEY_LEFT_CONTROL && !GetKey(KEY_RIGHT_CONTROL) || key == KEY_RIGHT_SHIFT && !GetKey(KEY_LEFT_CONTROL))
				ctrl = false;

			bool exists = false;
			for (int i = 0; i < keys.size(); i++)
				if (keys[i] == key){
					exists = true;
					keys.erase(keys.begin() + i);
					return;
				}
		}

		void Type(string *str, bool integers = false, int pos = 0){
			if (newKey != KEY_BACKSPACE && newKey != KEY_DELETE){
				char c = ToChar(newKey);
				if (!integers && newKey < 127 && newKey > 31)
					*str = str->substr(0, str->length() - pos) + c + str->substr(str->length() - pos);
				else if (integers && ((c < 58 && c > 47) || c == 56))
					*str = str->substr(0, str->length() - pos) + c + str->substr(str->length() - pos);
			}else if (str->length()){
				if (newKey == KEY_BACKSPACE)
					*str = str->substr(0, str->length() - pos - 1) + str->substr(str->length() - pos);

				else if (newKey == KEY_DELETE && pos != 0)
					*str = str->substr(0, str->length() - pos) + str->substr(str->length() - pos + 1);
			}
		}

		char ToChar(int key){
			if (key == -1)
				return 0;

			if (GetKey(KEY_LEFT_SHIFT))
				return UpperMap[key];
			else
				return LowerMap[key];
		}

};
Keyboard keyboard;

void SetKeyboardState(GLFWwindow* w, int key, int code, int action, int mod){
	if (action == 1)
		keyboard.AddKey(key);
	else if (action == 0)
		keyboard.DelKey(key);
}

char KeyToChar(int key){
	if (key == -1)
		return 0;
	if (keyboard.GetKey(KEY_LEFT_SHIFT))
		return UpperMap[key];
	else
		return LowerMap[key];
}