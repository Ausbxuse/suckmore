/* See LICENSE file for copyright and license details. */
/* Default settings; can be overriden by command line. */

static int topbar = 1;                      /* -b  option; if 0, dmenu appears at bottom     */

static int fuzzy = 1;                      /* -F  option; if 0, dmenu doesn't use fuzzy matching     */
static int centered = 0;                    /* -c option; centers dmenu on screen */
static int min_width = 1000;                    /* minimum width when centered */
/* -fn option overrides fonts[0]; default X11 font or font set */
static const char *fonts[] = {
	"JetBrainsMonoNF-Bold:size=12",
	"JoyPixels:pixelsize=17:antialias=true:autohint=true",
};
static const char *prompt      = NULL;      /* -p  option; prompt to the left of input field */
static const char *colors[SchemeLast][2] = {
	/*     fg         bg       */
	[SchemeNorm] = { "#eaeaea", "#111111" },
	[SchemeSel] = { "#eaeaea", "#42a5f5" },
	[SchemeSelHighlight] = { "#b7fd4b", "#42a5f5" },
	[SchemeNormHighlight] = { "#b7fd4b", "#222222" },
	[SchemeOut] = { "#000000", "#6ffaed" },
};
/* -l option; if nonzero, dmenu uses vertical list with given number of lines */
static unsigned int lines      = 0;
static unsigned int lineheight = 24;         /* -h option; minimum height of a menu line     */

/*
 * Characters not considered part of a word while deleting words
 * for example: " /?\"&[]"
 */
static const char worddelimiters[] = " ";

/* Size of the window border */
static unsigned int border_width = 0;
