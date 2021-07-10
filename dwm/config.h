#include <X11/XF86keysym.h>
/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 0;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int swallowfloating    = 0;        /* 1 means swallow floating windows by default */
static const unsigned int gappih    = 10;       /* horiz inner gap between windows */
static const unsigned int gappiv    = 10;       /* vert inner gap between windows */
static const unsigned int gappoh    = 10;       /* horiz outer gap between windows and screen edge */
static const unsigned int gappov    = 10;       /* vert outer gap between windows and screen edge */
static const int smartgaps          = 0;        /* 1 means no outer gap when there is only one window */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const int usealtbar          = 1;        /* 1 means use non-dwm status bar */
static const char *altbarclass      = "Polybar"; /* Alternate bar class name */
static const char *alttrayname      = "tray";    /* Polybar tray instance name */
static const char *altbarcmd        = "$HOME/.config/polybar/launch.sh"; /* Alternate bar launch command */
static const Bool viewontag         = True;     /* Switch view on tag switch */
static const char *fonts[]          = { "JetBrainsMono Nerd Font:style=Bold:size=12", "feather:size=13", "Material:size=13", "Material Design Icons:size=13"};
//static const char *fonts[]          = { "JetBrainsMono Nerd Font:style=Bold:size=12", "JoyPixels:size=12:antialias=true:autohint=true"};
static const char dmenufont[]       = "JetBrainsMono Nerd Font:size=12";
static const char col_gray1[]       = "#111111";  
static const char col_gray2[]       = "#444444";  
static const char col_gray3[]       = "#9c9c9c";  
static const char col_gray4[]       = "#eaeaea";  
static const char col_cyan[]        = "#214375";  
static const char col_black[]       = "#000000";  
static const char col_red[]         = "#ff0000";  
static const char col_yellow[]      = "#ffff00";  
static const char col_white[]       = "#ffffff";  
static const char col_blue[]        = "#42A5F5";  
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm]     = { col_gray4, col_gray1, col_gray2 },
	[SchemeSel]      = { col_white,     col_gray1, col_blue  },
};

static const unsigned int alphas[][3]      = {
	/*               fg      bg        border     */
	[SchemeNorm]     = { 180, 180, OPAQUE },
	[SchemeSel]      = { 100, 100, OPAQUE },
};
/* tagging */
static const char *tags[] = { "", "", "", "", "", "", "", "", "" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class            instance        title           tags mask  isfloating  isterminal  noswallow  monitor */
	{ "Gimp",           NULL,           NULL,           0,         1,          0,           0,        -1 },
	{ "scrcpy",         NULL,           NULL,           0,         1,          0,           0,        -1 },
	{ "Brave",          NULL,           NULL,           0,         0,          0,          -1,        -1 },
	{ "Chromium",       NULL,           NULL,           1 << 8,    0,          0,          -1,        -1 },
	{ "st-256color",    NULL,           NULL,           0,         0,          1,           0,        -1 },
	{ NULL,             "spterm",       NULL,           SPTAG(0),  1,          1,           0,        -1 },
	{ NULL,             "splatindic",   NULL,           SPTAG(1),  1,          1,           0,        -1 },
	{ NULL,             "spemoji",      NULL,           SPTAG(2),  1,          1,           0,        -1 },
	{ NULL,             "spcalc",       NULL,           SPTAG(3),  1,          1,           0,        -1 },
	{ NULL,             NULL,           "Event Tester", 0,         0,          0,           1,        -1 }, /* xev */
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

#define FORCE_VSPLIT 1  /* nrowgrid layout: force two clients to always split vertically */
#include "vanitygaps.c"

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "%{F#ffb900}󰙀",      tile },    /* first entry is default */
	{ "󰄶",      NULL },    /* no layout function means floating behavior */
	{ "󰄮",      monocle },
	{ "",      centeredmaster },
	{ "",      centeredfloatingmaster },
	{ "",      spiral },
 	{ "",      dwindle },
	{ "󰄮",      bstack },
	{ "󰄮",      bstackhoriz },
	{ "󰄮",      deck },
	{ "󰄮",      nrowgrid },
	{ "󰄮",      grid },
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
static const char *dmenucmd[] = { "dmenu_run", "-l", "20", "-c", "-bw", "3", "-p", "Run: ", NULL };
static const char *rofiruncmd[] = { "rofi", "-modi", "run,drun,window","-show", "drun", "-show-icons", "-columns", "3", NULL };
static const char *termcmd[]  = { "st", NULL };
static const char scratchpadname[] = "scratchpad";
static const char *browsercmd[]  = { "brave", NULL };

typedef struct {
	const char *name;
	const void *cmd;
} Sp;

const char *spcmd1[] = { "st", "-n", "spterm", "-g", "120x34", NULL };
const char *spcmd2[] = { "st", "-n", "splatindic","-g", "80x28", "-e", "la", NULL };
const char *spcmd3[] = { "st", "-n", "spemoji","-g", "38x15", "-e", "emoji", NULL };
const char *spcmd4[] = { "st", "-n", "spcalc","-g", "80x28", "-e", "calc", NULL };

static Sp scratchpads[] = {
	/* name          cmd  */
	{"spterm",      spcmd1},
	{"splatindic",  spcmd2},
	{"spemoji",     spcmd3},
	{"spcalc",      spcmd4},
};

static Key keys[] = {
	/* modifier                     key                        function        argument */
    TAGKEYS(			            XK_1,		                               0)
	TAGKEYS(			            XK_2,		                               1)
	TAGKEYS(			            XK_3,		                               2)
	TAGKEYS(			            XK_4,		                               3)
	TAGKEYS(			            XK_5,		                               4)
	TAGKEYS(			            XK_6,		                               5)
	TAGKEYS(			            XK_7,		                               6)
	TAGKEYS(			            XK_8,		                               7)
	TAGKEYS(			            XK_9,		                               8)
	{ MODKEY,                       XK_d,                      spawn,          {.v = rofiruncmd } },
	{ MODKEY,        			    XK_Return,                 spawn,          {.v = termcmd } },
	{ MODKEY,        			    XK_w, 	                   spawn,          {.v = browsercmd } },
	{ 0,							XF86XK_AudioLowerVolume,   spawn,          SHCMD("vol down") },
	{ 0,							XF86XK_AudioRaiseVolume,   spawn,          SHCMD("vol up") },
	{ 0,							XF86XK_AudioMute, 		   spawn,          SHCMD("vol toggle") },
	{ 0,							XF86XK_MonBrightnessUp,    spawn,          SHCMD("bright up") },
	{ 0,							XF86XK_MonBrightnessDown,  spawn,          SHCMD("bright down") },
	{ MODKEY|ShiftMask,			    XK_r,                      spawn,          SHCMD("kill -HUP $(pgrep -u $USER 'dwm$')") },
//	{ 0,							XK_Print,                  spawn,          {.v = screenshotcmd } },
	{ MODKEY,                       XK_b,                      togglebar,      {0} },
	{ MODKEY,                       XK_s,  	                   togglescratch,  {.ui = 0 } },
	{ MODKEY|ShiftMask,             XK_l,  	                   togglescratch,  {.ui = 1 } },
	{ MODKEY|ShiftMask,             XK_e,  	                   togglescratch,  {.ui = 2 } },
	{ MODKEY,                       XK_g,  	                   togglescratch,  {.ui = 3 } },
	{ MODKEY|ShiftMask,             XK_s,  	                   togglesticky,   {0} },
	{ MODKEY|ShiftMask,             XK_j,                      rotatestack,    {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_k,                      rotatestack,    {.i = -1 } },
	{ MODKEY,                       XK_j,                      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,                      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_slash,                  incnmaster,     {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_slash,                  incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,                      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,                      setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_Tab,                    view,           {0} },
	{ MODKEY,            			XK_q,                      killclient,     {0} },
	{ MODKEY|ShiftMask,            	XK_q,                      spawn,          SHCMD("sysact") },
	{ MODKEY,                       XK_t,                      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_i,                      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_r,                      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_u,                      setlayout,      {.v = &layouts[3]} },
	{ MODKEY|ShiftMask,             XK_u,                      setlayout,      {.v = &layouts[4]} },
	{ MODKEY,                       XK_y,                      setlayout,      {.v = &layouts[5]} },
	{ MODKEY,                       XK_o,                      setlayout,      {.v = &layouts[6]} },
	{ MODKEY|ShiftMask,             XK_o,                      setlayout,      {.v = &layouts[7]} },
	{ MODKEY,                       XK_p,                      setlayout,      {.v = &layouts[8]} },
	{ MODKEY|ShiftMask,             XK_p,                      setlayout,      {.v = &layouts[9]} },
	{ MODKEY|ShiftMask,             XK_i,                      setlayout,      {.v = &layouts[10]} },
	{ MODKEY|ShiftMask,             XK_t,                      setlayout,      {.v = &layouts[11]} },
	{ MODKEY,             		    XK_f,                      togglefullscr,  {0} },
//	{ MODKEY,                       XK_space,                  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_f,  	                   togglefloating, {0} },
	{ MODKEY,                       XK_0,                      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,                      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,                  focusmon,       {.i = -1 } },
    { MODKEY,                       XK_semicolon,              focusmaster,    {0} },
	{ MODKEY,                       XK_period,                 focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,                  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period,                 tagmon,         {.i = +1 } },
	{ MODKEY,             		    XK_x,                      incrgaps,       {.i = +1 } },
	{ MODKEY,            	   	    XK_z,                      incrgaps,       {.i = -1 } },
	{ MODKEY|ShiftMask,   			XK_x,                      incrogaps,      {.i = +1 } },
	{ MODKEY|ShiftMask, 	    	XK_z,                      incrogaps,      {.i = -1 } },
	{ MODKEY|ControlMask, 			XK_x,                      incrigaps,      {.i = +1 } },
	{ MODKEY|ControlMask,  			XK_z,                      incrigaps,      {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_a,                      togglegaps,     {0} },
    { MODKEY,			            XK_F10,	                   spawn,		   SHCMD("rofirecord") },
	{ MODKEY|ShiftMask,		        XK_F10,	                   spawn,		   SHCMD("rofirecord kill") },
    { MODKEY,			            XK_F11,	                   spawn,		   SHCMD("mpv --no-cache --no-osc --no-input-default-bindings --input-conf=/dev/null --title=webcam $(ls /dev/video[0,2,4,6,8] | tail -n 1)") },
	/* { MODKEY,			XK_F12,		xrdb,		{.v = NULL } }, */
	{ MODKEY,			            XK_space,	               zoom,		   {0} },
	{ MODKEY|ShiftMask,		        XK_space,	               togglefloating, {0} },

	{ 0,				            XK_Print,	               spawn,		   SHCMD("maim pic-full-$(date '+%y%m%d-%H%M-%S').png") },
	{ ShiftMask,			        XK_Print,	               spawn,		   SHCMD("maimpick") },
	{ MODKEY,			            XK_Scroll_Lock,	           spawn,		   SHCMD("killall screenkey || screenkey &") },

	{ 0,                            XF86XK_AudioPrev,		   spawn,		   SHCMD("mpc prev && pkill -RTMIN+4 dwmblocks") },
	{ 0,                            XF86XK_AudioNext,		   spawn,		   SHCMD("mpc next && pkill -RTMIN+4 dwmblocks") },
	{ 0,                            XF86XK_AudioPause,		   spawn,		   SHCMD("mpc pause && pkill -RTMIN+4 dwmblocks") },
	{ 0,                            XF86XK_AudioPlay,		   spawn,		   SHCMD("mpc toggle && pkill -RTMIN+4 dwmblocks") },
	{ 0,                            XF86XK_AudioStop,		   spawn,		   SHCMD("mpc stop && pkill -RTMIN+4 dwmblocks") },
	{ 0,                            XF86XK_AudioRewind,	       spawn,		   SHCMD("mpc seek -10 && pkill -RTMIN+4 dwmblocks") },
	{ 0,                            XF86XK_AudioForward,	   spawn,		   SHCMD("mpc seek +10 && pkill -RTMIN+4 dwmblocks") },
	{ 0,                            XF86XK_AudioMedia,		   spawn,		   SHCMD("st -e ncmpcpp") },
	{ 0,                            XF86XK_AudioMicMute,	   spawn,		   SHCMD("pactl set-source-mute @DEFAULT_SOURCE@ toggle") },
	{ 0,                            XF86XK_PowerOff,		   spawn,		   SHCMD("sysact") },
	{ 0,                            XF86XK_Calculator,		   spawn,		   SHCMD("rofi -modi calc -show calc -config /home/peter/.config/rofi/themes/calculator.rasi") },
	{ 0,                            XF86XK_Sleep,	           spawn,		   SHCMD("sudo -A zzz") },
	{ 0,                            XF86XK_WWW,		           spawn,		   SHCMD("$BROWSER") },
	{ 0,                            XF86XK_DOS,		           spawn,		   SHCMD("st") },
	{ 0,                            XF86XK_ScreenSaver,	       spawn,		   SHCMD("slock & xset dpms force off; mpc pause; pauseallmpv") },
	{ 0,                            XF86XK_TaskPane,		   spawn,		   SHCMD("st -e htop") },
	{ 0,                            XF86XK_Mail,		       spawn,		   SHCMD("st -e neomutt ; pkill -RTMIN+12 dwmblocks") },
	{ 0,                            XF86XK_MyComputer,		   spawn,		   SHCMD("st -e lf /") },
	{ 0,                            XF86XK_Battery,		       spawn,		   SHCMD("") }, 
	{ 0,                            XF86XK_Launch1,		       spawn,		   SHCMD("xset dpms force off") },
	{ 0,                            XF86XK_TouchpadToggle,	   spawn,		   SHCMD("(synclient | grep 'TouchpadOff.*1' && synclient TouchpadOff=0) || synclient TouchpadOff=1") },
	{ 0,                            XF86XK_TouchpadOff,	       spawn,		   SHCMD("synclient TouchpadOff=1") },
	{ 0,                            XF86XK_TouchpadOn,		   spawn,		   SHCMD("synclient TouchpadOff=0") },
	{ MODKEY,    					XK_a,                      defaultgaps,    {0} },
	{ 0,				            XK_Print,	               spawn,		   SHCMD("maim pic-full-$(date '+%y%m%d-%H%M-%S').png") },
	{ MODKEY|ShiftMask,			    XK_semicolon,	           spawn,		   SHCMD("maimpick") },
	{ MODKEY,			            XK_e,		               spawn,		   SHCMD("st -e neomutt ; pkill -RTMIN+12 dwmblocks; rmdir ~/.abook") },
    { MODKEY|ShiftMask,			    XK_n,		               spawn,		   SHCMD("st -e nvim -c VimwikiIndex") },
	{ MODKEY,		                XK_n,		               spawn,		   SHCMD("st -e newsboat; pkill -RTMIN+6 dwmblocks") },
	{ MODKEY,			            XK_m,		               spawn,		   SHCMD("st -e ncmpcpp") },


	{ MODKEY|Mod2Mask,              XK_z,                      incrihgaps,     {.i = +1 } },
	{ MODKEY|Mod2Mask,              XK_x,                      incrihgaps,     {.i = -1 } },
	{ MODKEY|ControlMask,           XK_z,                      incrivgaps,     {.i = +1 } },
	{ MODKEY|ControlMask,           XK_x,                      incrivgaps,     {.i = -1 } },
	{ MODKEY|Mod1Mask,              XK_z,                      incrohgaps,     {.i = +1 } },
	{ MODKEY|Mod1Mask,              XK_x,                      incrohgaps,     {.i = -1 } },
	{ MODKEY|Mod3Mask,              XK_z,                      incrovgaps,     {.i = +1 } },
	{ MODKEY|Mod3Mask,              XK_x,                      incrovgaps,     {.i = -1 } },

};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
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
};

static const char *ipcsockpath = "/tmp/dwm.sock";
static IPCCommand ipccommands[] = {
  IPCCOMMAND(  view,                1,      {ARG_TYPE_UINT}   ),
  IPCCOMMAND(  toggleview,          1,      {ARG_TYPE_UINT}   ),
  IPCCOMMAND(  tag,                 1,      {ARG_TYPE_UINT}   ),
  IPCCOMMAND(  toggletag,           1,      {ARG_TYPE_UINT}   ),
  IPCCOMMAND(  tagmon,              1,      {ARG_TYPE_UINT}   ),
  IPCCOMMAND(  focusmon,            1,      {ARG_TYPE_SINT}   ),
  IPCCOMMAND(  focusstack,          1,      {ARG_TYPE_SINT}   ),
  IPCCOMMAND(  zoom,                1,      {ARG_TYPE_NONE}   ),
  IPCCOMMAND(  incnmaster,          1,      {ARG_TYPE_SINT}   ),
  IPCCOMMAND(  killclient,          1,      {ARG_TYPE_SINT}   ),
  IPCCOMMAND(  togglefloating,      1,      {ARG_TYPE_NONE}   ),
  IPCCOMMAND(  setmfact,            1,      {ARG_TYPE_FLOAT}  ),
  IPCCOMMAND(  setlayoutsafe,       1,      {ARG_TYPE_PTR}    ),
  IPCCOMMAND(  quit,                1,      {ARG_TYPE_NONE}   ),
  IPCCOMMAND(  togglebar,           1,      {ARG_TYPE_UINT}   ),
};

