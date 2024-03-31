//
// macro.hpp
//
/*
	Includes common variables and functions that are used throughout the game engine
	that are not specific to the engine itself. 
*/

#include <string>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

#ifndef MACRO
#define		MACRO		1
#define		PI			3.14159265358979323846
#define 	RADIANS		PI/180
#define		DEGREES		180/PI

//
// Keyboard and Mouse
//

int keySize = 74;

char lowerKeyToChar[] = {
	' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 
	' ', ' ', ',', '-', '.', '/', 
	'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
	' ', ';', ' ', '=', ' ', ' ', ' ', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i',
	'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 
	'u', 'v', 'w', 'x', 'y', 'z',
	'[', '\\', ']', ' ', ' ', '`'
};
char upperKeyToChar[] = {
	' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 
	' ', ' ', '>', '_', '<', '?', ')', '!', '@', '#', '$', 
	'%', '^', '&',  '*', '(', ' ', ':', ' ', '+', ' ', ' ', 
	' ', 'A', 'B', 'C', 'D', 'E', 'F', 'G',  'H', 'I', 
	'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R',  'S', 
	'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '{', '|', '}', 
	' ', ' ', '~'
};

float Scale = 1;

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
#define LM_UP 0
#define LM_DOWN 1
#define RM_UP 10
#define RM_DOWN 11
#define MM_UP 20
#define MM_DOWN 21 

string Lower(string);
string Upper(string);
string SortFormat(string);

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

// Removes The from the beginning of a string
string SortFormat(string s){
	string out = s;
	if (!strcmp(Lower(s.substr(0,4)).data(), "the ")){
		out = s.substr(4, s.length()-4);
		out += " ";
		out += s.substr(0,3);
	}
	
	return out;
}

#endif
