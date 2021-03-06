/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 2;        /* border pixel of windows */
static const unsigned int gappx     = 10;        /* gaps between windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const unsigned int systraypinning = 0;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayonleft = 0;   	/* 0: systray in the right corner, >0: systray on left of status text */
static const unsigned int systrayspacing = 2;   /* systray spacing */
static const int systraypinningfailfirst = 1;   /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray        = 1;     /* 0 means no systray */
static const int showbar            = 1;     /* 0 means no bar */
static const int topbar             = 1;     /* 0 means bottom bar */
static const char *fonts[]          = {"JetBrainsMono Nerd Font-Regular:size=10", "Apple Color Emoji:pixelsize=16:antialias=true:autohint=true"};
static const char dmenufont[]       = "JetBrainsMono Nerd Font-Regular:size=10"; 
static const char col_gray1[]       = "#282a36";
static const char col_gray2[]       = "#bd93f9";
static const char col_gray3[]       = "#ff79c6";
static const char col_gray4[]       = "#bd93f9";
static const char col_cyan[]        = "#282a36";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_cyan  },
	[SchemeSel]  = { col_gray4, col_cyan,  col_gray2 },
};

typedef struct {
	const char *name;
	const void *cmd;
} Sp;
const char *spcmd1[] = {"st", "-n", "scratchpad1", "-g", "120x34", NULL };
const char *spcmd2[] = {"st", "-n", "scratchpad2", "-g", "120x34", NULL };
const char *spcmd3[] = {"st", "-n", "scratchpad3", "-g", "120x34", NULL };
static Sp scratchpads[] = {
	/* name          cmd  */
	{"scratchpad1",    spcmd1},
	{"scratchpad2",    spcmd2},
	{"scratchpad3",    spcmd3},
};


/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class                  instance    title       tags mask     isfloating   monitor */
	{ NULL,		  "scratchpad1",		NULL,		SPTAG(0),		1,			 -1,			 0 },
	{ NULL,		  "scratchpad2",		NULL,		SPTAG(1),		1,			 -1,			 0 },
	{ NULL,		  "scratchpad3",	    NULL,		SPTAG(2),		1,			 -1,			 0 },
};

/* layout(s) */
static const float mfact     = 0.50; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "???",      tile },    /* first entry is default */
	{ "???",      NULL }, /* no layout function means floating behavior */
	{ "???",      monocle },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|Mod1Mask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *termcmd[]  = { "st", NULL };


static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY|ShiftMask,		        XK_Return, spawn,         {.v = dmenucmd } },
	{ MODKEY, 	                    XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_BackSpace, togglebar,   {0} },
	{ MODKEY,                       XK_t,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_a,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_d,      setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_space,  zoom,           {0} },
/*	{ MODKEY,                       XK_Tab,    view,           {0} },*/
	{ MODKEY,             			XK_c,      killclient,     {0} },
	{ MODKEY|ShiftMask,             XK_f,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY|Mod1Mask,              XK_f,      setlayout,      {.v = &layouts[1]} },
/*	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },*/
/*	{ MODKEY,                       XK_o, 	   setlayout,      {0} },*/
/*	{ MODKEY,	                    XK_f,      togglefloating, {0} },*/
	{ MODKEY,                       XK_s,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_s,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_z,      focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_x,      focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_z,      tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_x,      tagmon,         {.i = +1 } },
	{ MODKEY,                       XK_equal,  setgaps,        {.i = -4 } },
	{ MODKEY,                       XK_minus,  setgaps,        {.i = +4 } },
	{ MODKEY|ShiftMask,             XK_equal,  setgaps,        {.i = 0 } },
	{ Mod1Mask,            			XK_Tab, 	   	 		   togglescratch,  {.ui = 0 } },
	{ Mod1Mask|ShiftMask,  			XK_Tab, 	   			   togglescratch,  {.ui = 1 } },
	{ Mod1Mask|ControlMask,			XK_Tab,				       togglescratch,  {.ui = 2 } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_q,                      5)
	TAGKEYS(                        XK_w,                      6)
	TAGKEYS(                        XK_e,                      7)
	TAGKEYS(                        XK_r,                      8)
	{ MODKEY|ShiftMask,             XK_Escape, quit,           {0} }, 
	{ MODKEY|ControlMask|ShiftMask, XK_Escape, quit,           {1} }, 
};

/* button definitions */ 
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button1,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};
