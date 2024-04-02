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
		
};
Keyboard keyboard;
