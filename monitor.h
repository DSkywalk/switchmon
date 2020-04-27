/*
 *
 *   SwitchMode (C) 2020 David Colmenero
 *
 *   This program is Free Software under the GNU Public License
 *
 *   SwitchMode is a simple modeline activator
 *     based on the switchres work of Calamity, Chris Kennedy, Antonio Giner
 */

#ifndef __MONITOR_H__
#define __MONITOR_H__

#include <stdio.h>
#include <stdlib.h>

typedef struct ModeLine {
	char   name[256];
	int    pclock;
	int    hactive;
	int    hbegin;
	int    hend;
	int    htotal;
	int    vactive;
	int    vbegin;
	int    vend;
	double vtotal;
	int    interlace;
	int    doublescan;
	int    hsync;
	int    vsync;
	//
	int    result;
	int    weight;
	int    vpad;
	char   label[256];
	char   resolution[256];
	int    depth;
	//
	double vfreq;
	int    a_width;
	int    a_height;
	double a_vfreq;
	double hfreq;
	//
	char   game[256];
	//
	int    desktop;
	int    custom;
	//
	char   regdata[256];
	int    regdata_size;
} ModeLine;

/* Xrandr commands */
int GetXrandrDisplay(ModeLine *defaultMode);
int SetXrandrDisplay(char * modeline, ModeLine *defaultMode);
int DelXrandrDisplay(ModeLine *defaultMode);

/* Utilities */
//double Normalize(double a, double b);
int RunProgram(char *temp);
//int get_output(char *command, char *output);
int split_exeargs(char* string, const char* delim, char * dest_binary, char** dest_args);
void sr_fprintf(FILE *fd, const char *fmt, ...);

#ifdef _VERBOSE
#define VERBOSE 1
#else
#define VERBOSE 0
#endif

#define XRANDR 1
#define MODELINE_NAME "switchmon_rgbux"
#define MODELINE_BASE "15Khz_desktop"
#define MAX_ARGS 64
#define MAX_CHAR 256

extern FILE *logfd;

#endif
