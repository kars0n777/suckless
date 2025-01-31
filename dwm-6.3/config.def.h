/* See LICENSE file for copyright and license details. */

/* appearance */ 
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int gappx     = 8;       /* gaps between windows */
static const unsigned int snap      = 0;        /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = {"mononoki Nerd Font-Regular:size=10", "Apple Color Emoji:pixelsize=16:antialias=true:autohint=true"};
static const char dmenufont[]       = "mononoki Nerd Font-Regular:size=10"; 
static const char col_gray1[]       = "#282a36";
static const char col_gray2[]       = "#bd93f9";
static const char col_gray3[]       = "#ff79c6";
static const char col_gray4[]       = "#bd93f9";
static const char col_cyan[]        = "#282a36";
static const unsigned int baralpha = 0xd0;
static const unsigned int borderalpha = OPAQUE;
static const char *colors[][3]      = {
  /*               fg         bg         border   */
  [SchemeNorm] = { col_gray3, col_gray1, col_cyan  },
  [SchemeSel]  = { col_gray4, col_cyan,  col_gray2 },
};
static const unsigned int alphas[][3]      = {
  /*               fg      bg        border     */
  [SchemeNorm] = { OPAQUE, baralpha, borderalpha },
  [SchemeSel]  = { OPAQUE, baralpha, borderalpha },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class                  instance    title       tags mask     isfloating   monitor */
	{ "rustdesk",	  NULL,			NULL,		0,				1,			 -1 },
	{ "Rustdesk",	  NULL,			NULL,		0,				1,			 -1 },
};

/* layout(s) */
static const float mfact     = 0.50; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

#include "horizgrid.c"
static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "",      tile },    /* first entry is default */
	{ "",      NULL }, /* no layout function means floating behavior */
	{ "",      monocle },
	{ "",      horizgrid },
	{ "TTT",      bstack },
	{ "",      bstackhoriz },
	{ "󱪷",      horizontal },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|Mod1Mask,           			KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *termcmd[]  = { "st", NULL };
static const char *sxhkdcmd[]  = { "sxhkd-toggle.sh", NULL };


static Key keys[] = {
  /* modifier                     key        function        argument */
  { MODKEY|ShiftMask,		            XK_Return, spawn,          {.v = dmenucmd } },
  { MODKEY, 			                  XK_Return, spawn,          {.v = termcmd } },
  { ControlMask|Mod1Mask,           XK_s,      spawn,          {.v = sxhkdcmd} },
  { MODKEY,                         XK_BackSpace, togglebar,   {0} },
  { MODKEY,                         XK_t,      focusstack,     {.i = +1 } },
  { MODKEY,                         XK_k,      focusstack,     {.i = +1 } },
  { MODKEY,	            	          XK_g,      focusstack,     {.i = -1 } },
  { MODKEY,	            	          XK_j,      focusstack,     {.i = -1 } },
  { MODKEY,                         XK_a,      setmfact,       {.f = -0.05} },
  { MODKEY,                         XK_d,      setmfact,       {.f = +0.05} },
  { MODKEY,                         XK_space,  zoom,           {0} },
  { MODKEY,	                        XK_Tab,    view,           {0} }, /* only switches to most recent workspace */ 
  { MODKEY|ShiftMask, 			        XK_c,      killclient,     {0} },
  { ControlMask|Mod1Mask,           XK_g,      setlayout,      {.v = &layouts[0]} },
  { MODKEY|Mod1Mask,	              XK_g,      setlayout,      {.v = &layouts[1]} },
  /*	{ Mod1Mask|ShiftMask,           XK_g,      setlayout,      {.v = &layouts[2]} },*/
  { MODKEY|ShiftMask, 			        XK_g,      setlayout,      {.v = &layouts[3]} },
  { Mod1Mask|ShiftMask,			        XK_g,	     setlayout,      {.v = &layouts[5]} },
  { ControlMask|MODKEY,			        XK_g,	     setlayout,      {.v = &layouts[6]} },
  /*	{ MODKEY|ShiftMask,             XK_f, 	   setlayout,      {0} },*/
  /*	{ MODKEY|ShiftMask,	            XK_f,      togglefloating, {0} },*/
  { MODKEY,                         XK_s,      view,           {.ui = ~0 } },
  { MODKEY|ShiftMask,               XK_s,      tag,            {.ui = ~0 } },
  { MODKEY,                         XK_x,      focusmon,       {.i = -1 } },
  { MODKEY,                         XK_z,      focusmon,       {.i = +1 } },
  { MODKEY|ShiftMask,               XK_x,      tagmon,         {.i = -1 } },
  { MODKEY|ShiftMask,               XK_z,      tagmon,         {.i = +1 } },
  { MODKEY,                         XK_l,      focusmon,       {.i = -1 } },
  { MODKEY,                         XK_h,      focusmon,       {.i = +1 } },
  { MODKEY|ShiftMask,               XK_l,      tagmon,         {.i = -1 } },
  { MODKEY|ShiftMask,               XK_h,      tagmon,         {.i = +1 } },
  { MODKEY,                         XK_equal,  setgaps,        {.i = -4 } },
  { MODKEY,                         XK_minus,  setgaps,        {.i = +4 } },
  { MODKEY|ShiftMask,               XK_equal,  setgaps,        {.i = 0 } },
  TAGKEYS(                          XK_1,                      0)
    TAGKEYS(                        XK_2,                      1)
    TAGKEYS(                        XK_3,                      2)
    TAGKEYS(                        XK_4,                      3)
    TAGKEYS(                        XK_5,                      4)
    TAGKEYS(                        XK_q,                      5)
    TAGKEYS(                        XK_w,                      6)
    TAGKEYS(                        XK_e,                      7)
    TAGKEYS(                        XK_r,                      8)
    TAGKEYS(                        XK_6,                      9)
    TAGKEYS(                        XK_7,                      10)
    TAGKEYS(                        XK_8,                      11)
    TAGKEYS(                        XK_9,                      12)
    TAGKEYS(                        XK_0,                      13)
    { MODKEY|ShiftMask,             XK_Escape, quit,           {1} }, 
    { MODKEY|ControlMask|ShiftMask, XK_Escape, quit,           {0} }, 
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
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};
