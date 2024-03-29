/* See LICENSE file for copyright and license details. */

/* Constants */
#define TERMINAL "st"
#define TERMCLASS "st"
#define BROWSER "brave"

/* appearance */
static unsigned int borderpx  = 3;        /* border pixel of windows */
static unsigned int snap      = 32;       /* snap pixel */
static unsigned int gappih    = 20;       /* horiz inner gap between windows */
static unsigned int gappiv    = 10;       /* vert inner gap between windows */
static unsigned int gappoh    = 10;       /* horiz outer gap between windows and screen edge */
static unsigned int gappov    = 30;       /* vert outer gap between windows and screen edge */
static int swallowfloating    = 0;        /* 1 means swallow floating windows by default */
static int smartgaps          = 0;        /* 1 means no outer gap when there is only one window */
static int showbar            = 1;        /* 0 means no bar */
static int topbar             = 1;        /* 0 means bottom bar */
// static const int usealtbar    = 0;
// static const char *altbarclass = "dwmblocks";
static char *fonts[]          = { 
    "CommitMono:size=16", 
    "NotoColorEmoji:pixelsize=12:antialias=true:autohint=true"
};
static char normbgcolor[]     = "#1a1b26";
static char normbordercolor[] = "#1a1b26";
static char normfgcolor[]     = "#C0CAF5";
static char selfgcolor[]      = "#222222";
static char selbordercolor[]  = "#C0CAF5";
static char selbgcolor[]      = "#A9B1D6";
static char *colors[][3] = {
    /*               fg           bg           border   */
    [SchemeNorm] = { normfgcolor, normbgcolor, normbordercolor },
    [SchemeSel]  = { selfgcolor,  selbgcolor,  selbordercolor  },
};

typedef struct {
	const char *name;
	const void *cmd;
} Sp;

static const char scratchpadname[] = "scratchpad";
static const char *scratchpadcmd[] = { "st", "-t", scratchpadname, "-g", "120x34", NULL };

const char *spcmd1[] = {TERMINAL, "-n", "spterm", "-g", "120x34", NULL };
const char *spcmd2[] = {TERMINAL, "-n", "spcalc", "-f", "monospace:size=16", "-g", "50x20", "-e", "bc", "-lq", NULL };
static Sp scratchpads[] = {
	/* name         cmd  */
	{"spterm",      spcmd1},
	{"spcalc",      spcmd2},
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	*/
	/* class        instance    title       	    tags mask    isfloating   isterminal  noswallow  monitor */
	{ "Gimp",       NULL,       NULL,       	    1 << 8,       0,           0,         0,        -1 },
	{ TERMCLASS,    NULL,       NULL,       	    0,            0,           1,         0,        -1 },
	{ TERMCLASS,    NULL,       "lf",       	    0,            0,           1,         0,        -1 },
	{ NULL,         NULL,       "Event Tester",   0,            0,           0,         1,        -1 },
	{ TERMCLASS,    "bg",       NULL,       	    1 << 7,       0,           1,         0,        -1 },
	{ TERMCLASS,    "spterm",   NULL,       	    SPTAG(0),     1,           1,         0,        -1 },
	{ TERMCLASS,    "spcalc",   NULL,       	    SPTAG(1),     1,           1,         0,        -1 },
};

/* layout(s) */
static float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static int nmaster     = 1;    /* number of clients in master area */
static int resizehints = 0;    /* 1 means respect size hints in tiled resizals */
#define FORCE_VSPLIT 1  /* nrowgrid layout: force two clients to always split vertically */
#include "vanitygaps.c"
static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",	tile },			/* Default: Master on left, slaves on right */
	{ "TTT",	bstack },		/* Master on top, slaves on bottom */

	{ "[@]",	spiral },		/* Fibonacci spiral */
	{ "[\\]",	dwindle },		/* Decreasing in size right and leftward */

	{ "[D]",	deck },			/* Master on left, slaves in monocle-like mode on right */
	{ "[M]",	monocle },		/* All windows on top of eachother */

	{ "|M|",	centeredmaster },		/* Master in middle, slaves on sides */
	{ ">M>",	centeredfloatingmaster },	/* Same but master floats */

	{ "><>",	NULL },			/* no layout function means floating behavior */
	{ NULL,		NULL },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },
#define STACKKEYS(MOD,ACTION) \
	{ MOD,	XK_j,	ACTION##stack,	{.i = INC(+1) } }, \
	{ MOD,	XK_k,	ACTION##stack,	{.i = INC(-1) } }, \
	{ MOD,  XK_v,   ACTION##stack,  {.i = 0 } }, \
	/* { MOD, XK_grave, ACTION##stack, {.i = PREVSEL } }, \ */
	/* { MOD, XK_a,     ACTION##stack, {.i = 1 } }, \ */
	/* { MOD, XK_z,     ACTION##stack, {.i = 2 } }, \ */
	/* { MOD, XK_x,     ACTION##stack, {.i = -1 } }, */

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }
#define TERMCMD(cmd) { .v = (const char*[]){ TERMINAL, cmd, NULL } }

/* commands */
static const char *termcmd[]  = { TERMINAL, NULL };

/*
 * Xresources preferences to load at startup
 */
ResourcePref resources[] = {
    { "borderpx",		      INTEGER,    &borderpx },
    { "color0",	          STRING,     &normbgcolor },
    { "color0",	          STRING,     &normbordercolor },
    { "color0",	          STRING,     &selfgcolor },
    { "color4",	          STRING,     &normfgcolor },
    { "color4",	          STRING,     &selbgcolor },
    { "color8",	          STRING,     &selbordercolor },
    { "gappih",	          INTEGER,    &gappih },
    { "gappiv",	          INTEGER,    &gappiv },
    { "gappoh",	          INTEGER,    &gappoh },
    { "gappov",		        INTEGER,    &gappov },
    { "mfact",	          FLOAT,      &mfact },
    { "nmaster",	        INTEGER,    &nmaster },
    { "resizehints",      INTEGER,    &resizehints },
    { "showbar",          INTEGER,    &showbar },
    { "smartgaps",		    INTEGER,    &smartgaps },
    { "snap",	            INTEGER,    &snap },
    { "swallowfloating",	INTEGER,    &swallowfloating },
    { "topbar",	          INTEGER,    &topbar },
};

#include <X11/XF86keysym.h>
#include "shiftview.c"

static Key keys[] = {
	/* V is automatically bound above in STACKKEYS */
	/* J and K are automatically bound above in STACKEYS */
	STACKKEYS(MODKEY,           focus)
	STACKKEYS(MODKEY|ShiftMask, push)
	TAGKEYS(XK_1,		0)
	TAGKEYS(XK_2,		1)
	TAGKEYS(XK_3,		2)
	TAGKEYS(XK_4,		3)
	TAGKEYS(XK_5,		4)
	TAGKEYS(XK_6,		5)
	TAGKEYS(XK_7,		6)
	TAGKEYS(XK_8,		7)
	TAGKEYS(XK_9,		8)
	/* modifier             key         function    argument */
    // Windows
	{ MODKEY,			      XK_q,	    killclient,	    {0} },
	{ MODKEY,			      XK_f,	    togglefullscr,	{0} },
	// { MODKEY|ShiftMask,	XK_f,		  togglescratch,	{.v = scratchpadcmd} },
	{ MODKEY,			      XK_s,	    togglesticky,	  {0} },
	{ MODKEY,			      XK_o,	    incnmaster,     {.i = +1 } },
	{ MODKEY|ShiftMask,	XK_o,	    incnmaster,     {.i = -1 } },
	{ MODKEY,			      XK_space,	zoom,		        {0} },
	{ MODKEY|ShiftMask, XK_space,	togglefloating,	{0} },

    // Workspace
	{ MODKEY,			      XK_0,	        view,	          {.ui = ~0 } },
	{ MODKEY|ShiftMask,	XK_0,	        tag,	          {.ui = ~0 } },
	{ MODKEY,		        XK_h,	        shiftview,      { .i = -1 } },
	{ MODKEY,		        XK_l,	        shiftview,      { .i = 1 } },
	{ MODKEY,			      XK_a,	        togglegaps,	    {0} },
	{ MODKEY|ShiftMask,	XK_a,	        defaultgaps,    {0} },
	{ MODKEY,		        XK_z,	        incrgaps,       {.i = +3 } },
	{ MODKEY,		        XK_x,	        incrgaps,       {.i = -3 } },
	{ MODKEY,			      XK_g,		      setmfact,       {.f = -0.05} },
	{ MODKEY,			      XK_semicolon,	setmfact,       {.f = +0.05} },
	{ MODKEY,			      XK_d,	        spawn,          SHCMD("dmenu_run -l 14") },
	{ MODKEY|ShiftMask,	XK_d,	        spawn,	        TERMCMD("fp") },

    // Layouts
	{ MODKEY,			        XK_t,		setlayout,	{.v = &layouts[0]} }, /* tile */
	{ MODKEY|ShiftMask,		XK_t,		setlayout,	{.v = &layouts[1]} }, /* bstack */
	{ MODKEY,			        XK_y,		setlayout,	{.v = &layouts[2]} }, /* spiral */
	{ MODKEY|ShiftMask,		XK_y,		setlayout,	{.v = &layouts[3]} }, /* dwindle */
	{ MODKEY,			        XK_u,		setlayout,	{.v = &layouts[4]} }, /* deck */
	{ MODKEY|ShiftMask,		XK_u,		setlayout,	{.v = &layouts[5]} }, /* monocle */
	{ MODKEY,			        XK_i,		setlayout,	{.v = &layouts[6]} }, /* centeredmaster */
	{ MODKEY|ShiftMask,		XK_i,		setlayout,	{.v = &layouts[7]} }, /* centeredfloatingmaster */
	{ MODKEY,			        XK_b,		togglebar,	{0} },

    // App Bindings
	{ MODKEY,			        XK_Return,	spawn,		{.v = termcmd } },
	{ MODKEY|ShiftMask,		XK_Return,	spawn,		SHCMD("sd") },
	{ MODKEY,			        XK_w,	      spawn,	  SHCMD(BROWSER) },
	{ MODKEY|ShiftMask,		XK_w,	      spawn,		SHCMD("getbookmark") },
	{ MODKEY,			        XK_e,	      spawn,		TERMCMD("neomutt") },
	{ MODKEY|ShiftMask,		XK_e,	      spawn,		{ .v = (const char*[]){ TERMINAL, "abook", "-C", "~/.config/abook/abookrc", "--datafile", "~/.config/abook/addressbook", NULL }}},
	{ MODKEY,			        XK_r,	      spawn,	  TERMCMD("lf") },
	{ MODKEY|ShiftMask,		XK_r,	      spawn,	  TERMCMD("htop") },
	{ MODKEY,	        	  XK_n,	      spawn,		TERMCMD("zellij -l ~/.config/zellij/notes.kdl")},
	{ MODKEY|ShiftMask,		XK_n,	      spawn,		{ .v = (const char*[]){ TERMINAL, "newsboat", NULL} }},
	{ MODKEY|ShiftMask,   XK_b,		    spawn,	  TERMCMD("fb") },
	{ MODKEY,		          XK_Escape,	spawn,	  SHCMD("sysact") },
	{ MODKEY,		          XK_grave,   spawn,	  SHCMD("dmenuunicode") },
	{ MODKEY|ShiftMask,		XK_grave,   spawn,	  SHCMD("passmenu -l 10") },
	{ MODKEY,			        XK_minus,   spawn,		SHCMD("wpctl set-volume @DEFAULT_AUDIO_SINK@ 5%-; kill -44 $(pidof dwmblocks)") },
	{ MODKEY|ShiftMask,		XK_minus,   spawn,		SHCMD("wpctl set-volume @DEFAULT_AUDIO_SINK@ 15%-; kill -44 $(pidof dwmblocks)") },
	{ MODKEY,			        XK_equal,   spawn,		SHCMD("wpctl set-volume @DEFAULT_AUDIO_SINK@ 5%+; kill -44 $(pidof dwmblocks)") },
	{ MODKEY|ShiftMask,		XK_equal,   spawn,		SHCMD("wpctl set-volume @DEFAULT_AUDIO_SINK@ 15%+; kill -44 $(pidof dwmblocks)") },

    // Media Controls
	{ MODKEY,			        XK_p,		          spawn,		SHCMD("mpc toggle") },
	{ MODKEY|ShiftMask,		XK_p,		          spawn,		SHCMD("mpc pause; pauseallmpv") },
	{ MODKEY,			        XK_bracketleft,		spawn,		SHCMD("mpc seek -10") },
	{ MODKEY|ShiftMask,		XK_bracketleft,		spawn,		SHCMD("mpc seek -60") },
	{ MODKEY,			        XK_bracketright,	spawn,		SHCMD("mpc seek +10") },
	{ MODKEY|ShiftMask,		XK_bracketright,	spawn,		SHCMD("mpc seek +60") },
	{ MODKEY,			        XK_comma,	        spawn,		SHCMD("mpc prev") },
	{ MODKEY|ShiftMask,		XK_comma,         spawn,		SHCMD("mpc seek 0%") },
	{ MODKEY,			        XK_period,        spawn,		SHCMD("mpc next") },
	{ MODKEY|ShiftMask,		XK_period,        spawn,		SHCMD("mpc repeat") },
	{ MODKEY,			        XK_m,		          spawn,		TERMCMD("ncmpcpp") },
	{ MODKEY|ShiftMask,		XK_m,		          spawn,		SHCMD("wpctl set-mute @DEFAULT_AUDIO_SINK@ toggle; kill -44 $(pidof dwmblocks)") },


    // Function keys
	{ MODKEY,		        XK_F1,	        spawn,		SHCMD("groff -mom /usr/local/share/dwm/larbs.mom -Tpdf | zathura -") },
	{ MODKEY,		        XK_F2,	        spawn,		SHCMD("tutorialvids") },
	{ MODKEY,		        XK_F3,	        spawn,		SHCMD("displayselect") },
	{ MODKEY,		        XK_F4,	        spawn,		TERMCMD("pulsemixer; kill -44 $(pidof dwmblocks)") },
	{ MODKEY,		        XK_F5,	        xrdb,		  {.v = NULL } },
	{ MODKEY,		        XK_F6,	        spawn,		{.v = spcmd2} },
	{ MODKEY,		        XK_F7,	        spawn,		SHCMD("td-toggle") },
	{ MODKEY,		        XK_F8,	        spawn,		SHCMD("mw -Y") },
	{ MODKEY,		        XK_F9,	        spawn,		SHCMD("dmenumount") },
	{ MODKEY,		        XK_F10,	        spawn,		SHCMD("dmenuumount") },
	{ MODKEY,		        XK_F11,	        spawn,		SHCMD("openwebcam") },
	{ MODKEY,		        XK_F12,	        spawn,		SHCMD("remaps") },
	{ 0,				        XK_Print,       spawn,		SHCMD("maim ~/picx/screenshot/pic-full-$(date '+%y%m%d-%H%M-%S').png") },
	{ ShiftMask,        XK_Print,       spawn,		SHCMD("maimpick") },
	{ MODKEY,		        XK_Print,       spawn,		SHCMD("dmenurecord") },
	{ MODKEY|ShiftMask,	XK_Print,       spawn,		SHCMD("dmenurecord kill") },
	{ MODKEY,		        XK_Delete,  	  spawn,		SHCMD("dmenurecord kill") },
	{ MODKEY,		        XK_Scroll_Lock,	spawn,		SHCMD("killall screenkey || screenkey &") },

    // Custom Key Inputs
	{ 0, XF86XK_AudioMute,		      spawn,		SHCMD("wpctl set-mute @DEFAULT_AUDIO_SINK@ toggle; kill -44 $(pidof dwmblocks)") },
	{ 0, XF86XK_AudioRaiseVolume,   spawn,		SHCMD("wpctl set-volume @DEFAULT_AUDIO_SINK@ 3%+; kill -44 $(pidof dwmblocks)") },
	{ 0, XF86XK_AudioLowerVolume,   spawn,		SHCMD("wpctl set-volume @DEFAULT_AUDIO_SINK@ 3%-; kill -44 $(pidof dwmblocks)") },
	{ 0, XF86XK_AudioPrev,	        spawn,		SHCMD("mpc prev") },
	{ 0, XF86XK_AudioNext,	        spawn,		SHCMD("mpc next") },
	{ 0, XF86XK_AudioPause,	        spawn,		SHCMD("mpc pause") },
	{ 0, XF86XK_AudioPlay,	        spawn,		SHCMD("mpc play") },
	{ 0, XF86XK_AudioStop,	        spawn,		SHCMD("mpc stop") },
	{ 0, XF86XK_AudioRewind,        spawn,		SHCMD("mpc seek -10") },
	{ 0, XF86XK_AudioForward,       spawn,		SHCMD("mpc seek +10") },
	{ 0, XF86XK_AudioMedia,	        spawn,		TERMCMD("ncmpcpp") },
	{ 0, XF86XK_AudioMicMute,       spawn,		SHCMD("pactl set-source-mute @DEFAULT_SOURCE@ toggle") },
	{ 0, XF86XK_Display,	          spawn,		SHCMD("displayselect") },
	{ 0, XF86XK_Bluetooth,	        spawn,		{.v = (const char*[]){ "blueman-manager", NULL } } },
	{ 0, XF86XK_Calculator,	        spawn,		TERMCMD("bc -l") },
	{ 0, XF86XK_Sleep,		          spawn,		{.v = (const char*[]){ "sudo", "-A", "zzz", NULL } } },
	{ 0, XF86XK_WWW,		            spawn,		{.v = (const char*[]){ BROWSER, NULL } } },
	{ 0, XF86XK_DOS,		            spawn,		{.v = termcmd } },
	{ 0, XF86XK_ScreenSaver,        spawn,		SHCMD("slock & xset dpms force off; mpc pause; pauseallmpv") },
	{ 0, XF86XK_TaskPane,	          spawn,		TERMCMD("htop") },
	{ 0, XF86XK_Mail,		            spawn,		TERMCMD("neomutt") },
	{ 0, XF86XK_MyComputer,		      spawn,		TERMCMD("lf /") },
	{ 0, XF86XK_Launch1,		        spawn,		SHCMD("xset dpms force off") },
	{ 0, XF86XK_TouchpadToggle,	    spawn,		SHCMD("(synclient | grep 'TouchpadOff.*1' && synclient TouchpadOff=0) || synclient TouchpadOff=1") },
	{ 0, XF86XK_TouchpadOff,	      spawn,		SHCMD("synclient TouchpadOff=1") },
	{ 0, XF86XK_TouchpadOn,		      spawn,		SHCMD("synclient TouchpadOff=0") },
	{ 0, XF86XK_MonBrightnessUp,    spawn,		SHCMD("brightnessctl set +15%") },
	{ 0, XF86XK_MonBrightnessDown,	spawn,		SHCMD("brightnessctl set 15%-") },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
#ifndef __OpenBSD__
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button1,        sigdwmblocks,   {.i = 1} },
	{ ClkStatusText,        0,              Button2,        sigdwmblocks,   {.i = 2} },
	{ ClkStatusText,        0,              Button3,        sigdwmblocks,   {.i = 3} },
	{ ClkStatusText,        0,              Button4,        sigdwmblocks,   {.i = 4} },
	{ ClkStatusText,        0,              Button5,        sigdwmblocks,   {.i = 5} },
	{ ClkStatusText,        ShiftMask,      Button1,        sigdwmblocks,   {.i = 6} },
#endif
	{ ClkStatusText,        ShiftMask,      Button3,        spawn,          TERMCMD("nvim ~/.local/src/dwmblocks/config.h") },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        defaultgaps,    {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkClientWin,		      MODKEY,		      Button4,	      incrgaps,	      {.i = +1} },
	{ ClkClientWin,		      MODKEY,		      Button5,	      incrgaps,	      {.i = -1} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
	{ ClkTagBar,		        0,	            Button4,        shiftview,      {.i = -1} },
	{ ClkTagBar,	          0,	            Button5,        shiftview,      {.i = 1} },
	{ ClkRootWin,	          0,	            Button2,        togglebar,      {0} },
};

