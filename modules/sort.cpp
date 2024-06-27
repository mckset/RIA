bool SortTag(Tag, Tag);
bool SortUint(uint, uint);

bool SortUint(uint i1, uint i2){return i1 < i2;}

string SortFormat(string s){
	string out = s;
	if (!strcmp(Lower(s.substr(0,4)).data(), "the ")){
		out = s.substr(4, s.length()-4);
		out += " ";
		out += s.substr(0,3);
	}
	
	return out;
}

bool SortFile(File f1, File f2){
	int max = f1.name.length() <= f2.name.length() ? f1.name.length() : f2.name.length();
		
	string n1 = SortFormat(Lower(f1.name));	
	string n2 = SortFormat(Lower(f2.name));

	for (int i = 0; i < max; i++){
		if (n1[i] != n2[i]){
			// Because unicode characters are negative, the sorter adds 512 to the characters as an int
			int c1 = n1[i] + 512*(n1[i]<0), 
				c2 = n2[i] + 512*(n2[i]<0);
			return (c1 < c2);
		}
	}
	return n1.length() < n2.length();
}

bool SortTag(Tag t1, Tag t2){
	int max = t1.name.length() <= t2.name.length() ? t1.name.length() : t2.name.length();

	string n1 = SortFormat(Lower(t1.name));	
	string n2 = SortFormat(Lower(t2.name));

	for (int i = 0; i < max; i++){
		if (n1[i] != n2[i]){
			// Because unicode characters are negative, the sorter adds 512 to the characters as an int
			int c1 = n1[i] + 512*(n1[i]<0), 
				c2 = n2[i] + 512*(n2[i]<0);
			return (c1 < c2);
		}
	}
	return n1.length() < n2.length();
}
