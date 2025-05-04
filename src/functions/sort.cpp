bool SortTag(Tag, Tag);
bool SortUint(uint, uint);
bool SortString(string, string);
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

bool SortBoards(Board b1, Board b2){return SortString(b1.name, b2.name);}

bool SortFile(File f1, File f2){
	string s1 = f1.name;
	string s2 = f2.name;
	
	// Remove file extension
	if (s1.length() > 4)
		s1 = s1.substr(0, s1.length() - (s1[s1.length()-4] == '.' ? 4 : 5));
	if (s2.length() > 4)
		s2 = s2.substr(0, s2.length() - (s2[s2.length()-4] == '.' ? 4 : 5));

	return SortString(s1, s2);
}

bool SortString(string s1, string s2){
	string n1 = SortFormat(Lower(s1));	
	string n2 = SortFormat(Lower(s2));

	int max = n1.length() <= n2.length() ? n1.length() : n2.length();

	for (int i = 0; i < max-1; i++){
		if (n1[i] != n2[i]){
			// Because unicode characters are negative, the sorter adds 512 to the characters as an int
			int c1 = n1[i] + 512*(n1[i]<0), 
				c2 = n2[i] + 512*(n2[i]<0);
			return (c1 < c2);
		}
	}

	if (n1.length() == n2.length()){
		int c1 = n1[max-1] + 512*(n1[max-1]<0), 
			c2 = n2[max-1] + 512*(n2[max-1]<0);
		return (c1 < c2);
	}
	
	return n1.length() < n2.length();
}

bool SortTag(Tag t1, Tag t2){return SortString(t1.name, t2.name);}
