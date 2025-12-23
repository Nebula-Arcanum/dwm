/* See LICENSE file for copyright and license details. */
#include <X11/XF86keysym.h>

/* appearance */
static const unsigned int borderpx  = 3;        /* border pixel of windows */
static const int startwithgaps[]    = { 0 };	/* 1 means gaps are used by default, this can be customized for each tag */
static const unsigned int gappx[]   = { 10 };   /* default gap between windows in pixels, this can be customized for each tag */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const int vertpad            = 0;       /* vertical padding of bar */
static const int sidepad            = 0;       /* horizontal padding of bar */
static const char *fonts[]          = { "3270NerdFontMono:size=12" };
static const char dmenufont[]       = "3270NerdFontMono:size=12";
static const char col_white[]        = "#FFFFFF";
static const char col_black[]        = "#000000";
static const unsigned int baralpha = OPAQUE;
static const unsigned int borderalpha = OPAQUE;
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_white, col_black, col_black },
	[SchemeSel]  = { col_black, col_white,  col_white  },
};
static const unsigned int alphas[][3]      = {
    /*               fg      bg        border*/
    [SchemeNorm] = { OPAQUE, baralpha, borderalpha },
	[SchemeSel]  = { OPAQUE, baralpha, borderalpha },
};

static const char *const autostart[] = {
	"dwmblocks", NULL,
	NULL /* terminate */
};

/* tagging */
// static const char *tags[] = { "", "󰖟", "󰯃", "", "󰘹", "", "7", "", "" };

/* tagging: refer to https://github.com/bakkeby/patches/wiki/tagicons */
static const char *tags[NUMTAGS] = { NULL };  /* left for compatibility reasons, i.e. code that checks LENGTH(tags) */
static char *tagicons[][NUMTAGS*2] = {
	[IconsDefault]        = { "", "󰖟", "󰯃", "", "󰘹", "", "7", "8", "9",
                             "", "󰖟", "", "", "E", "F", "G", "H", "I" },
	[IconsVacant]         = { NULL },
	[IconsOccupied]       = { NULL },
};

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	// { "Gimp",     NULL,       NULL,       0,            1,           -1 },
	// { "firefox",  NULL,       NULL,       1 << 1,       0,           -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int attachdirection = 4; /* 0 default, 1 above, 2 aside, 3 below, 4 bottom, 5 top */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
	{ "TTT",      bstack },
	{ "===",      bstackhoriz },
	{ "|M|",      centeredmaster },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_black, "-nf", col_white, "-sb", col_white, "-sf", col_black, NULL };
							/* updates dwmblocks */
/* Volume control */
static const char *volup[] = { "mixer", "vol=+1%", NULL };
static const char *voldown[] = { "mixer", "vol=-1%", NULL };
static const char *volmute[] = { "mixer", "vol.mute=toggle", NULL };
static const char *volupdate[] = { "pkill", "-76", "dwmblocks", NULL };

/* MusicPD pause/play */
static const char *mpdtoggle[] = { "mpc", "toggle", NULL };

/* Program launch definitions for autostarttaglist */
static const char *termcmd[]  = { "st", NULL };
static const char *browsercmd[] = { "firefox", NULL };
static const char *wikicmd[] = { "st", "-e", "zsh", "-l", "-c", "cd /home/leon/Wiki/ && nvim", NULL };
static const char *ankicmd[] = { "anki", NULL };
static const char *vitcmd[] = { "st", "-e", "vit", NULL };
static const char *wyrdcmd[] = { "st", "-e", "wyrd", "/home/leon/.local/share/remind/reminders", NULL };

static const char *screenshotcmd[] = { "flameshot", "gui", NULL };

Autostarttag autostarttaglist[] = {
	{.cmd = termcmd, .tags = 1 << 0 },
	{.cmd = browsercmd, .tags = 1 << 1 },
	{.cmd = wikicmd, .tags = 1 << 2 },
	{.cmd = vitcmd, .tags = 1 << 7 },
	// {.cmd = khalcmd, .tags = 1 << 8 },
	{.cmd = wyrdcmd, .tags = 1 << 8 },
	{.cmd = ankicmd, .tags = 1 << 4 },
	{.cmd = NULL, .tags = 0 },
};

static const Key keys[] = {
	/* modifier                     key        function        argument */
	{ 0,	                        XK_KP_Add, spawn,                {.v = volup } },
	{ 0,	                        XK_KP_Subtract, spawn,           {.v = voldown } },
	{ 0,	                        XK_KP_Multiply, spawn,           {.v = volmute } },
	{ 0,	                        XK_KP_Add, spawn,                {.v = volupdate } },
	{ 0,	                        XK_KP_Subtract, spawn,           {.v = volupdate } },
	{ 0,	                        XK_KP_Multiply, spawn,           {.v = volupdate } },
	{ 0,	                        XF86XK_AudioRaiseVolume, spawn,  {.v = volup } },
	{ 0,	                        XF86XK_AudioLowerVolume, spawn,  {.v = voldown } },
	{ 0,	                        XF86XK_AudioMute, spawn,         {.v = volmute } },
	{ 0,	                        XF86XK_AudioRaiseVolume, spawn,  {.v = volupdate } },
	{ 0,	                        XF86XK_AudioLowerVolume, spawn,  {.v = volupdate } },
	{ 0,	                        XF86XK_AudioMute, spawn,         {.v = volupdate } },
	{ 0,	                        XK_KP_Divide, spawn,             {.v = mpdtoggle } },
	{ 0,	                        XK_Print,  spawn,	   {.v = screenshotcmd } },
	{ MODKEY,                       XK_p,      spawn,          {.v = dmenucmd } },
	{ MODKEY|ShiftMask,             XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY|ShiftMask,             XK_c,      killclient,     {0} },
    { MODKEY|ControlMask,           XK_c,      killclient,     {.ui = 1} },  // kill unselect
    { MODKEY|ShiftMask|ControlMask, XK_c,      killclient,     {.ui = 2} },  // killall
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_u,      setlayout,      {.v = &layouts[3]} },
	{ MODKEY,                       XK_o,      setlayout,      {.v = &layouts[4]} },
	{ MODKEY,                       XK_c,      setlayout,      {.v = &layouts[5]} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	{ MODKEY,                       XK_minus,  setgaps,        {.i = -5 } },
	{ MODKEY,                       XK_equal,  setgaps,        {.i = +5 } },
	{ MODKEY|ShiftMask,             XK_minus,  setgaps,        {.i = GAP_RESET } },
	{ MODKEY|ShiftMask,             XK_equal,  setgaps,        {.i = GAP_TOGGLE} },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY|ShiftMask,             XK_q,      quit,           {0} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
	{ ClkTagBar,            0,              Button4,        cycleiconset,   {.i = +1 } },
	{ ClkTagBar,            0,              Button5,        cycleiconset,   {.i = -1 } },
};

