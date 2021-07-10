/* time interval in seconds to sleep before looking for updates in the main loop */
#define SLEEPINTERVAL			1

#define PATH(name)			"/home/peter/.local/bin/statusbar/"name

/* If interval of a block is set to 0, the block will only be updated once at startup.
 * If interval is set to a negative value, the block will never be updated in the main loop.
 * Set signal to 0 if signalling is not required for the block.
 * Signal must be less than 10 for clickable blocks.
 * If multiple realtime signals are pending, then the lowest numbered signal is delivered first. */

/* pathu - path of the program whose output is to be used for status text
 * pathc - path of the program to be executed on clicks */
static Block blocks[] = {
/*	pathu				pathc					interval	signal */

	{PATH("traffic"), NULL,	2,	11},
//	{PATH("cpu_usage"), NULL,	2,	8},
//	{PATH("memory"), NULL,	2,	7},
//	{PATH("cpu"), NULL,	5,	6},
	{PATH("volume"), NULL,	1,	5},
	{PATH("mic"), NULL,	1,	5}, // to be fix -- interval cannot be set to 0
	{PATH("battery"), NULL,	30,	3},
	{PATH("music"), PATH("music"),	1,	4},
	{PATH("wifi"), PATH("wifi_button"),	5,	2},
//	{PATH("calendar"), NULL,	300,	10},
	{PATH("clock"), PATH("clock"),	1,	1},
	{PATH("recording"), NULL, 1,	9},


        { NULL } /* just to mark the end of the array */
};

static const char *delim = "\x0b";
