//Sets delimiter between status commands. NULL character ('\0') means no delimiter.
static char *delim = " · ";

//Modify this file to change what commands output to your statusbar, and recompile using the make command.
static const Block blocks[] = {
	/*Icon*/ /*Command*/	/*Interval*/	/*Update Signal*/
	{"",    "sb-music",	    5,	            11},
	{"",	"sb-volume",	0,              10},
	{"",	"sb-news",		5,              7},
	{"",	"sb-mail",		5,              6},
	{"",	"sb-internet",	5,              4},
	{"",	"sb-disk",  	5,              2},
	{"",	"sb-battery",	5,              3},
	{"",	"sb-clock",	    1,	            1},
};

