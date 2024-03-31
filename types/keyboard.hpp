class Keyboard{
	public:
		int event;
		int newKey;
		int prevKey;
		int* keys;
		int keySize;

		bool GetKey(int key){
			for (int i = 0; i < keySize; i++)
				if (keys[i] == key)
					return true;
			return false;
		}

		void AddKey(int key){
			newKey = key;
			int* temp = keys;
			keys = (int*)malloc(sizeof(int) * (keySize+1));
			for (int i = 0; i < keySize; i++)
				keys[i] = temp[i];
			keys[keySize] = key;
			keySize++;
		}

		void DelKey(int key){
			int* temp = keys;
			keys = (int*)malloc(sizeof(int) * (keySize-1));
			bool removed = false;
			for (int i = 0; i < keySize; i++){
				if (!removed && keys[i] != key)
					keys[i] = temp[i];
				else if (keys[i] == key)
					removed = true;
				else
					keys[i-1] = temp[i];
			}
			keySize--;
		}
		
};
Keyboard keyboard;
