/* user and group to drop privileges to */
static const char *user  = "karson777";
static const char *group = "wheel";

static const char *colorname[NUMCOLS] = {
	[INIT] =   "#282a36",     /* after initialization */
	[INPUT] =  "#282a36",   /* during input */
	[FAILED] = "#ff5555",   /* wrong password */
};

/* treat a cleared input like a wrong password (color) */
static const int failonclear = 1;
