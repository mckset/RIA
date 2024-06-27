bool shift = false;

class Keyboard{
	public:
		int event;
		int newKey;
		int prevKey;
		vector<int> keys;
		bool GetKey(int key){
			for (auto k : keys)
				if (k == key)
					return true;
			return false;
		}

		void AddKey(int key){
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
			bool exists = false;
			for (int i = 0; i < keys.size(); i++)
				if (keys[i] == key){
					exists = true;
					vector<int>:: iterator k = keys.begin();
					advance(k,i);
					keys.erase(k);
					return;
				}
		}

		void Type(string *str, bool integers = false, int pos = 0){
			if (newKey != KEY_BACKSPACE && newKey != KEY_DELETE){
				char c = ToChar(newKey);
				if (!integers && newKey < 127 && newKey > 31)
					*str = str->substr(0, str->length() - pos) + c + str->substr(str->length() - pos);
				else if (integers && c < 58 && c > 47)
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

char KeyToChar(int key){
	if (key == -1)
		return 0;
	if (keyboard.GetKey(KEY_LEFT_SHIFT))
		return UpperMap[key];
	else
		return LowerMap[key];
}