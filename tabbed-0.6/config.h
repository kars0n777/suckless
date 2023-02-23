/* See LICENSE file for copyright and license details. */

/* appearance */
static const char font[]        = "mononoki Nerd Font-Regular:size=10";
static const char* normbgcolor  = "#282a36";
static const char* normfgcolor  = "#ff79c6";
static const char* selbgcolor   = "#282a36";
static const char* selfgcolor   = "#bd93f9";
static const char* urgbgcolor   = "#282a36";
static const char* urgfgcolor   = "#bd93f9";
static const char before[]      = "<";
static const char after[]       = ">";
static const char titletrim[]   = "...";
static const int  tabwidth      = 200;
static const Bool foreground    = True;
static       Bool urgentswitch  = True;

/*
 * Where to place a new tab when it is opened. When npisrelative is True,
 * then the current position is changed + newposition. If npisrelative
 * is False, then newposition is an absolute position.
 */
static int  newposition   = 1;
static Bool npisrelative  = True;

#define SETPROP(p) { \
        .v = (char *[]){ "/bin/sh", "-c", \
                "prop=\"$(xwininfo -children -id $1 | grep '^     0x' |" \
                "sed -e's@^ *\\(0x[0-9a-f]*\\) \"\\([^\"]*\\)\".*@\\1 \\2@' |" \
                "tail -n +2 | dmenu -i -l 10 -p 'Switch to: ')\" &&" \
                "xprop -id $1 -f $0 8s -set $0 \"$prop\"", \
                p, winid, NULL \
        } \
}

/* Modify the following line to match your terminal and software list */
#define OPENTERMSOFT(p) { \
	.v = (char *[]){ "/bin/sh", "-c", \
		"term='st' && titlearg='-t' && embedarg='--embed' &&" \
		"softlist=$(printf '%s\n' \"htop\" \"ncdu\" \"nvim\" \"fzf\" \"nnn\" \"ncmpcpp\" \"nmtui\") &&" \
		"printf '%s' \"$softlist\" |" \
		"dmenu -i -p 'Softwares to run: ' |" \
		"xargs -I {} $term $titlearg \"{}\" $embedarg $1 -e \"{}\"", \
		p, winid, NULL \
	} \
}


/* Modify the following line to match your terminal*/
#define OPENTERM(p) { \
	.v = (char *[]){ "/bin/sh", "-c", \
		"term='st' && embedarg='--embed' &&" \
		"cd \"$(xwininfo -children -id $1 | grep '^     0x' |" \
                "sed -e's@^ *\\(0x[0-9a-f]*\\) \"\\([^\"]*\\)\".*@\\1 \\2@' |" \
		"dmenu -i -l 10 -p 'New term path based on: ' |" \
		"cut -f 1 | xargs -I {} xprop -id \"{}\" | grep _NET_WM_PID |" \
		"cut -d ' ' -f 3 | xargs -I {} pstree -p \"{}\" |" \
		"cut -d '(' -f 3 | cut -d ')' -f 1 |" \
		"xargs -I {} readlink -e /proc/\"{}\"/cwd/)\" &&" \
		"$term $embedarg $1", \
		p, winid, NULL \
	} \
}

/* deskid: id for current workspace */
/* rootid: id for root window */
/* window: data for chosen window by dmenu */
/* wid: chosen window's window id */
/* wname: chosen window's name */
/* cwid: chosen window's child window id (tabbed window only) */
#define ATTACHWIN(p) { \
	.v = (char *[]){ "/bin/sh", "-c", \
		"deskid=$(xdotool get_desktop) &&" \
		"rootid=\"$(xwininfo -root | grep \"Window id\" | cut -d ' ' -f 4)\" &&" \
		"window=\"$(wmctrl -x -l | grep -E \" $deskid \" |" \
		"grep -v $(printf '0x0%x' \"$1\") |" \
		"cut -d ' ' -f 1,4 | dmenu -i -l 5 -p \"Attach: \")\" &&" \
		"wid=$(printf '%s' \"$window\" | cut -d ' ' -f 1) &&" \
		"wname=$(printf '%s' \"$window\" | cut -d ' ' -f 2) &&" \
		"[ \"$wname\" = \"tabbed.tabbed\" ] &&" \
		"cwid=$(xwininfo -children -id \"$wid\" | grep '^     0x' |" \
                "sed -e 's@^ *\\(0x[0-9a-f]*\\) \"\\([^\"]*\\)\".*@\\1@') &&" \
		"for id in $(printf '%s' \"$cwid\"); do xdotool windowreparent \"$id\" \"$rootid\"; done &&" \
		"for id in $(printf '%s' \"$cwid\"); do xdotool windowreparent \"$id\" \"$1\"; done ||" \
		"xdotool windowreparent \"$wid\" $1", \
		p, winid, NULL \
	} \
}

#define ATTACHSELECTWIN(p) { \
	.v = (char *[]){ "/bin/sh", "-c", \
		"rootid=\"$(xwininfo -root | grep \"Window id\" | cut -d ' ' -f 4)\" &&" \
		"wid=$(xdotool selectwindow) &&" \
		"wname=$(xwininfo -id \"$wid\" | grep 'Window id:' | cut -d ' ' -f 5-) &&" \
		"[ \"$wname\" = \"(has no name)\" ] &&" \
		"cwid=$(xwininfo -children -id \"$wid\" | grep '^     0x' |" \
                "sed -e 's@^ *\\(0x[0-9a-f]*\\) \"\\([^\"]*\\)\".*@\\1@') &&" \
		"for id in $(printf '%s' \"$cwid\"); do xdotool windowreparent \"$id\" \"$rootid\"; done &&" \
		"for id in $(printf '%s' \"$cwid\"); do xdotool windowreparent \"$id\" \"$1\"; done ||" \
		"xdotool windowreparent \"$wid\" $1", \
		p, winid, NULL \
	} \
}

#define ATTACHALL(p) { \
	.v = (char *[]){ "/bin/sh", "-c", \
		"deskid=$(xdotool get_desktop) &&" \
		"rootid=\"$(xwininfo -root | grep \"Window id\" | cut -d ' ' -f 4)\" &&" \
		"window=\"$(wmctrl -x -l | grep -E \" $deskid \" |" \
		"grep -v $(printf '0x0%x' \"$1\") | cut -d ' ' -f 1,4)\" &&" \
		"IFS=':' &&" \
		"for win in $(printf '%s' \"$window\" | tr '\n' ':'); do unset IFS &&" \
		    "wid=$(printf '%s' \"$win\" | cut -d ' ' -f 1) &&" \
		    "wname=$(printf '%s' \"$win\" | cut -d ' ' -f 2) &&" \
		    "[ \"$wname\" = \"tabbed.tabbed\" ] &&" \
		    "{ cwid=$(xwininfo -children -id \"$wid\" | grep '^     0x' |" \
		    "sed -e 's@^ *\\(0x[0-9a-f]*\\) \"\\([^\"]*\\)\".*@\\1@') &&" \
		    "for id in $(printf '%s' \"$cwid\"); do xdotool windowreparent \"$id\" \"$rootid\"; done &&" \
		    "for id in $(printf '%s' \"$cwid\"); do xdotool windowreparent \"$id\" \"$1\"; done; } ||" \
		"xdotool windowreparent \"$wid\" $1; done", \
		p, winid, NULL \
	} \
}


#define DETACHWIN(p) { \
        .v = (char *[]){ "/bin/sh", "-c", \
		"rootid=\"$(xwininfo -root | grep \"Window id\" | cut -d ' ' -f 4)\" &&" \
                "wid=\"$(xwininfo -children -id $1 | grep '^     0x' |" \
                "sed -e 's@^ *\\(0x[0-9a-f]*\\) \"\\([^\"]*\\)\".*@\\1 \\2@' |" \
                "dmenu -i -l 5 -p 'Detach: ' | cut -d ' ' -f 1)\" &&" \
		"xwininfo -id $wid -stats | grep -q 'IsUnMapped' && xdotool windowmap $wid;" \
		"xdotool windowreparent \"$wid\" \"$rootid\" &&" \
		"xdotool windowactivate $1", \
                p, winid, NULL \
        } \
}



#define DETACHALL(p) { \
        .v = (char *[]){ "/bin/sh", "-c", \
		"rootid=\"$(xwininfo -root | grep \"Window id\" | cut -d ' ' -f 4)\" &&" \
                "wid=\"$(xwininfo -children -id $1 | grep '^     0x' |" \
                "sed -e 's@^ *\\(0x[0-9a-f]*\\) \"\\([^\"]*\\)\".*@\\1@')\" &&" \
		"IFS=':' &&" \
		"for id in $(printf '%s' \"$wid\" | tr '\n' ':'); do unset IFS &&" \
		    "xdotool windowreparent \"$id\" \"$rootid\" &&" \
		    "xwininfo -id $id -stats |" \
		    "grep -q 'IsUnMapped' &&" \
		    "xdotool windowmap $id; done", \
                p, winid, NULL \
        } \
}



#define SHOWHIDDEN(p) { \
        .v = (char *[]){ "/bin/sh", "-c", \
                "cwin=\"$(xwininfo -children -id $1 | grep '^     0x' |" \
                "sed -e 's@^ *\\(0x[0-9a-f]*\\) \"\\([^\"]*\\)\".*@\\1 \\2@')\" &&" \
		"IFS=':' &&" \
		"for win in $(printf '%s' \"$cwin\" | tr '\n' ':'); do unset IFS &&" \
		    "cwid=$(printf '%s' \"$win\" | cut -d ' ' -f 1) &&" \
		    "xwininfo -id $cwid -stats |" \
		    "grep -q 'IsUnMapped' &&" \
		    "printf '%s\n' \"$win\"; done |" \
		"dmenu -i -l 5 -p \"Show hidden window:\" |" \
		"cut -d ' ' -f 1 |" \
		"xargs -I {} xdotool windowmap \"{}\"", \
                p, winid, NULL \
        } \
}

#define SHOWHIDDENALL(p) { \
        .v = (char *[]){ "/bin/sh", "-c", \
                "cwid=\"$(xwininfo -children -id $1 | grep '^     0x' |" \
                "sed -e 's@^ *\\(0x[0-9a-f]*\\) \"\\([^\"]*\\)\".*@\\1@')\" &&" \
		"IFS=':' &&" \
		"for id in $(printf '%s' \"$cwid\" | tr '\n' ':'); do unset IFS &&" \
		    "xwininfo -id $id -stats | " \
		    "grep -q 'IsUnMapped' &&" \
		    "xdotool windowmap $id; done", \
                p, winid, NULL \
        } \
}

#define HIDEWINDOW(p) { \
        .v = (char *[]){ "/bin/sh", "-c", \
                "cwid=\"$(xwininfo -children -id $1 | grep '^     0x' |" \
                "sed -e 's@^ *\\(0x[0-9a-f]*\\) \"\\([^\"]*\\)\".*@\\1@')\" &&" \
		"IFS=':' && winnum=0 &&" \
		"for id in $(printf '%s' \"$cwid\" | tr '\n' ':'); do unset IFS &&" \
		    "xwininfo -id $id -stats | " \
		    "grep -q 'IsViewable' &&" \
		    "winnum=$(($winnum+1)); done;" \
                "[ $winnum -gt 1 ] &&" \
		"{ xwininfo -children -id $1 | grep '^     0x' | head -n 1 |" \
                "sed -e 's@^ *\\(0x[0-9a-f]*\\) \"\\([^\"]*\\)\".*@\\1@' |" \
		"xargs -I {} xdotool windowunmap \"{}\"; }", \
                p, winid, NULL \
        } \
}



#define MODKEY ControlMask 
static Key keys[] = {
	/* modifier            key              function     argument */
	{ MODKEY|ShiftMask,    XK_t,       	focusonce,   { 0 } },
	{ MODKEY, 	       XK_t,            spawn,       { 0 } },
	{ MODKEY, 	       XK_Return,            spawn,       { 0 } },

	{ MODKEY, 	       XK_Tab,            rotate,      { .i = +1 } },
	{ MODKEY|ShiftMask,    XK_Tab,            rotate,      { .i = -1 } },
	{ Mod1Mask,	       XK_Tab,            movetab,     { .i = +1 } },
	{ Mod1Mask|ShiftMask,  XK_Tab,            movetab,     { .i = -1 } },
	/*{ ControlMask,       XK_Tab,         	  rotate,      { .i = 0 } },*/

	{ MODKEY,   	       XK_1,            move,        { .i = 0 } },
	{ MODKEY,   	       XK_2,            move,        { .i = 1 } },
	{ MODKEY,   	       XK_3,            move,        { .i = 2 } },
	{ MODKEY,   	       XK_4,            move,        { .i = 3 } },
	{ MODKEY,   	       XK_5,            move,        { .i = 4 } },
	{ MODKEY,   	       XK_6,            move,        { .i = 5 } },
	{ MODKEY,   	       XK_7,            move,        { .i = 6 } },
	{ MODKEY,   	       XK_8,            move,        { .i = 7 } },
	{ MODKEY,   	       XK_9,            move,        { .i = 8 } },
	{ MODKEY,   	       XK_0,            move,        { .i = 9 } },

	{ MODKEY, 	       XK_w,            killclient,  { 0 } },

	{ MODKEY,              XK_u,            focusurgent, { 0 } },
	{ MODKEY|ShiftMask,    XK_u,            toggle,      { .v = (void*) &urgentswitch } },

	{ 0,                   XK_F11,          fullscreen,  { 0 } },

	{ MODKEY|ShiftMask,    XK_comma,        spawn,       SETPROP("_TABBED_SELECT_TAB") },

	{ MODKEY,              XK_b,            showbar,     { .i = 1 } },
	{ MODKEY,    	       XK_n,            showbar,     { .i = 0 } },

	/* Unique functionality */
	/*{ MODKEY|ShiftMask,  XK_period,       spawn,       OPENTERMSOFT("_TABBED_SELECT_TERMAPP") },
	{ MODKEY|ShiftMask,    XK_slash,        spawn,       OPENTERM("_TABBED_TERM") },
	{ MODKEY|ShiftMask,    XK_a,	        spawn,       ATTACHWIN("_TABBED_ATTACH_WIN") },
	{ MODKEY|ShiftMask,    XK_s,	        spawn,       ATTACHSELECTWIN("_TABBED_ATTACH_WIN") },
	{ MODKEY|ShiftMask,    XK_equal,        spawn,       ATTACHALL("_TABBED_ATTACH_ALL") },
	{ MODKEY|ShiftMask,    XK_d,	        spawn,       DETACHWIN("_TABBED_DETACH_WIN") },
	{ MODKEY|ShiftMask,    XK_minus,        spawn,       DETACHALL("_TABBED_DETACH_ALL") },
	{ MODKEY|ShiftMask,    XK_bracketleft,  spawn,       HIDEWINDOW("_TABBED_HIDE_WINDOW") },
	{ MODKEY|ShiftMask,    XK_bracketright, spawn,       SHOWHIDDEN("_TABBED_SHOW_HIDDEN") },
	{ MODKEY|ShiftMask,    XK_backslash,    spawn,       SHOWHIDDENALL("_TABBED_SHOW_HIDDEN_ALL") },*/
};

static Key keyreleases[] = {
	/* modifier            key              function     argument */
	{ MODKEY|ShiftMask,    XK_Alt_L,      showbar,     { .i = 0 } },
};
