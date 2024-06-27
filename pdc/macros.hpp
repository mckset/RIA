//
// macro.hpp
//

#include <string>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef MACRO
#define		MACRO		1
#define		PI			3.14159265358979323846
#define 	RADIANS		PI/180
#define		DEGREES		180/PI

//
// Keyboard and Mouse
//

int keySize = 74;

std::map<int, char> LowerMap{
	// Lower case
	{32, ' '}, {39, '\''},
	{44, ','}, {45, '-'}, {46, '.'}, {47, '/'}, {48, '0'}, {49, '1'},
	{50, '2'}, {51, '3'}, {52, '4'}, {53, '5'}, {54, '6'}, {55, '7'}, {56, '8'}, {57, '9'}, {59, ';'},
	{60, '='},  {65, 'a'}, {66, 'b'}, {67, 'c'}, {68, 'd'}, {69, 'e'},
	{70, 'f'}, {71, 'g'}, {72, 'h'}, {73, 'i'}, {74, 'j'}, {75, 'k'}, {76, 'l'}, {77, 'm'}, {78, 'n'}, {79, 'o'},
	{80, 'p'}, {81, 'q'}, {82, 'r'}, {83, 's'}, {84, 't'}, {85, 'u'}, {86, 'v'}, {87, 'w'}, {88, 'x'}, {89, 'y'},
	{90, 'z'}, {91, '['}, {92, '\\'}, {93, ']'}, {96, '`'},
	{320, '0'}, {321, '1'}, {322, '2'}, {323, '3'}, {324, '4'}, {325, '5'}, {326, '6'}, {327, '7'}, {328, '8'}, {329, '9'},
	{330, '.'}, {331, '/'}, {322, '*'}, {323, '-'}, {324, '+'}
};

std::map<int, char> UpperMap{
	// Upper case
	{32, ' '}, {39, '\"'},
	{44, '<'}, {45, '_'}, {46, '>'}, {47, '?'}, {48, ')'}, {49, '!'},
	{50, '@'}, {51, '#'}, {52, '$'}, {53, '%'}, {54, '^'}, {55, '&'}, {56, '*'}, {57, '('}, {59, ':'},
	{60, '+'},  {65, 'A'}, {66, 'B'}, {67, 'C'}, {68, 'D'}, {69, 'E'},
	{70, 'F'}, {71, 'G'}, {72, 'H'}, {73, 'I'}, {74, 'J'}, {75, 'K'}, {76, 'L'}, {77, 'M'}, {78, 'N'}, {79, 'O'},
	{80, 'P'}, {81, 'Q'}, {82, 'R'}, {83, 'S'}, {84, 'T'}, {85, 'U'}, {86, 'V'}, {87, 'W'}, {88, 'X'}, {89, 'Y'},
	{90, 'Z'}, {91, '{'}, {92, '|'}, {93, '}'}, {96, '~'},
	{320, '0'}, {321, '1'}, {322, '2'}, {323, '3'}, {324, '4'}, {325, '5'}, {326, '6'}, {327, '7'}, {328, '8'}, {329, '9'},
	{330, '.'}, {331, '/'}, {322, '*'}, {323, '-'}, {324, '+'}
};

std::map<char, int> HexToInt{
	{'0', 0}, {'1', 1}, {'2', 2}, {'3', 3}, {'4', 4}, {'5', 5},
	{'6', 6}, {'7', 7}, {'8', 8}, {'9', 9}, {'A', 10}, {'B', 11},
	{'C', 12}, {'D', 13}, {'E', 14}, {'F', 15}, {'a', 10}, {'b', 11},
	{'c', 12}, {'d', 13}, {'e', 14}, {'f', 15},
};

std::map<int, string> IntToHex{
	{0, "0"}, {1, "1"}, {2, "2"}, {3, "3"}, {4, "4"}, {5, "5"},
	{6, "6"}, {7, "7"}, {8, "8"}, {9, "9"}, {10, "A"}, {11, "B"},
	{12, "C"}, {13, "D"}, {14, "E"}, {15, "F"},{10, "a"}, {11, "b"},
	{12, "c"}, {13, "d"}, {14, "e"}, {15, "f"}
};

// Keys 
#define 	KEY_SPACE   		32
#define 	KEY_APOSTROPHE   	39
#define 	KEY_COMMA   		44 
#define 	KEY_MINUS   		45 
#define 	KEY_PERIOD   		46 
#define 	KEY_SLASH   		47
#define 	KEY_0   			48
#define 	KEY_1   			49
#define 	KEY_2   			50
#define 	KEY_3   			51
#define 	KEY_4   			52
#define 	KEY_5   			53
#define 	KEY_6   			54
#define 	KEY_7   			55
#define 	KEY_8   			56
#define 	KEY_9  				57
#define 	KEY_SEMICOLON   	59
#define 	KEY_EQUAL   		61 
#define 	KEY_A   			65
#define 	KEY_B   			66
#define 	KEY_C   			67
#define 	KEY_D   			68
#define 	KEY_E   			69
#define 	KEY_F   			70
#define 	KEY_G   			71
#define 	KEY_H   			72
#define 	KEY_I   			73
#define 	KEY_J   			74
#define 	KEY_K   			75
#define 	KEY_L   			76
#define 	KEY_M   			77
#define 	KEY_N   			78
#define 	KEY_O   			79
#define 	KEY_P   			80
#define 	KEY_Q   			81
#define 	KEY_R   			82
#define 	KEY_S   			83
#define 	KEY_T   			84
#define 	KEY_U   			85
#define 	KEY_V   			86
#define 	KEY_W   			87
#define 	KEY_X   			88
#define 	KEY_Y   			89
#define 	KEY_Z   			90
#define 	KEY_LEFT_BRACKET	91
#define 	KEY_BACKSLASH		92
#define 	KEY_RIGHT_BRACKET	93
#define 	KEY_TILDE			96
#define 	KEY_WORLD_1			161 
#define 	KEY_WORLD_2			162
#define 	KEY_ESCAPE			256
#define 	KEY_ENTER			257
#define 	KEY_TAB				258
#define 	KEY_BACKSPACE		259
#define 	KEY_INSERT			260
#define 	KEY_DELETE			261
#define 	KEY_RIGHT			262
#define 	KEY_LEFT 			263
#define 	KEY_DOWN			264
#define 	KEY_UP				265
#define 	KEY_PAGE_UP			266
#define 	KEY_PAGE_DOWN		267
#define 	KEY_HOME			268
#define 	KEY_END				269
#define 	KEY_CAPS_LOCK		280
#define 	KEY_SCROLL_LOCK		281
#define 	KEY_NUM_LOCK		282
#define 	KEY_PRINT_SCREEN	283
#define 	KEY_PAUSE			284
#define 	KEY_F1				290
#define 	KEY_F2				291
#define 	KEY_F3				292
#define 	KEY_F4				293
#define 	KEY_F5				294
#define 	KEY_F6				295
#define 	KEY_F7				296
#define 	KEY_F8				297
#define 	KEY_F9				298
#define 	KEY_F10				299
#define 	KEY_F11				300
#define 	KEY_F12				301
#define 	KEY_F13				302
#define 	KEY_F14				303
#define 	KEY_F15				304
#define 	KEY_F16				305
#define 	KEY_F17				306
#define 	KEY_F18				307
#define 	KEY_F19				308
#define 	KEY_F20				309
#define 	KEY_F21				310
#define 	KEY_F22				311
#define 	KEY_F23				312
#define 	KEY_F24				313
#define 	KEY_F25				314
#define 	KEY_KP_0			320
#define 	KEY_KP_1			321
#define 	KEY_KP_2			322
#define 	KEY_KP_3			323
#define 	KEY_KP_4			324
#define 	KEY_KP_5			325
#define 	KEY_KP_6			326
#define 	KEY_KP_7			327
#define 	KEY_KP_8			328
#define 	KEY_KP_9			329
#define 	KEY_KP_DECIMAL		330
#define 	KEY_KP_DIVIDE		331
#define 	KEY_KP_MULTIPLY		332
#define 	KEY_KP_SUBTRACT		333
#define 	KEY_KP_ADD			334
#define 	KEY_KP_ENTER		335
#define 	KEY_KP_EQUAL		336
#define 	KEY_LEFT_SHIFT		340
#define 	KEY_LEFT_CONTROL	341
#define 	KEY_LEFT_ALT		342
#define 	KEY_LEFT_SUPER		343
#define 	KEY_RIGHT_SHIFT		344
#define 	KEY_RIGHT_CONTROL	345
#define 	KEY_RIGHT_ALT		346
#define 	KEY_RIGHT_SUPER		347
#define 	KEY_MENU			348

// Mouse buttons
#define 	LM_UP 				0
#define 	LM_DOWN 			1
#define 	RM_UP 				10
#define 	RM_DOWN 			11
#define 	MM_UP 				20
#define 	MM_DOWN 			21
#define		MBACK_UP			30
#define		MBACK_DOWN			31
#define		MFRONT_UP			40
#define		MFRONT_DOWN			41

string Lower(string s){
	for (int i = 0; i < s.length(); i++)
		if (s[i] >= 65 && s[i] <= 90)
			s[i] += 32;
	return s;
}

string Upper(string s){
	for (int i = 0; i < s.length(); i++)
		if (s[i] >= 97 && s[i] <= 122)
			s[i] -= 32;
	return s;
}

bool IsOdd(int i){return ((i/2)*2 != i);}

int FindIn(string str, string find, int start = 0){
	for (start; start < str.length(); start++){
		if (Lower(str.substr(start, find.length())) == Lower(find))
			return start;
	}
	return -1;
}

bool IsNumber(char c){
	return (c >= 48 && c <= 57);
}

bool operator==(string s1, string s2){return !strcmp(s1.data(), s2.data());}
#endif
