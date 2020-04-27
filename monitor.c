/*
 *
 *   SwitchMode (C) 2020 David Colmenero
 *
 *   This program is Free Software under the GNU Public License
 *
 *   SwitchMode is a simple modeline activator
 *     based on the switchres work of Calamity, Chris Kennedy, Antonio Giner
 */

#include "monitor.h"

char * PrintModeline(ModeLine *mode, char *modeline) {
	sprintf(modeline, "     \"%s\" %.6f %d %d %d %d %d %d %d %.0f %s %s%s%s",
		mode->name, ((double)mode->pclock/1000000),
		mode->hactive, mode->hbegin, mode->hend, mode->htotal,
		mode->vactive, mode->vbegin, mode->vend, mode->vtotal,
		mode->hsync?"+HSync":"-HSync", mode->vsync?"+VSync":"-VSync",
		mode->doublescan?" doublescan":"", mode->interlace?" interlace":"");

	return modeline;
}

int GetXrandrDisplay(ModeLine *defaultMode) {
        char cmd[64*1024]={'\x00'};
        char *cmdptr = cmd;

        /* Get Xrandr display information */
        sprintf(cmd, "xrandr --current | grep ' connected' | cut -d' ' -f1");
        RunProgram(cmdptr);
        sscanf(cmd, "%s", (char *)&defaultMode->label);

        /* Get Xrandr default resolution information */
        sprintf(cmd, "xrandr --current | grep '*'");
        RunProgram(cmdptr);
	defaultMode->vfreq = 0;
        if (sscanf(cmd, "   %dx%dx%lf   %lf*",
                &defaultMode->hactive, &defaultMode->vactive, &defaultMode->vfreq,
                &defaultMode->a_vfreq) != 4 || !defaultMode->vfreq)
	{
        	sscanf(cmd, "   %dx%d   %lf*",
                	&defaultMode->hactive, &defaultMode->vactive, &defaultMode->a_vfreq);
        	sprintf(defaultMode->name, "%dx%d",
                	defaultMode->hactive, defaultMode->vactive);
		defaultMode->vfreq = defaultMode->a_vfreq;
	} else {
        	sprintf(defaultMode->name, "%dx%dx%.2f",
                	defaultMode->hactive, defaultMode->vactive, defaultMode->vfreq);
	}

        if (VERBOSE)
                sr_fprintf(stderr, "Got display %s '%s' with %dx%d@%.2f resolution\n",
                        defaultMode->label, defaultMode->name,
                        defaultMode->hactive, defaultMode->vactive, defaultMode->vfreq);
	return 0;
}

int SetXrandrDisplay(char * modeline, ModeLine *current) {
        char cmd[64*1024]={'\x00'};
        char *cmdptr = cmd;

        /* Add new modeline */
        sprintf(cmd, "xrandr --nograb --newmode \"%s\" %s",
            MODELINE_NAME, modeline);
        RunProgram(cmdptr);

        /* Add modeline to interface */
        sprintf(cmd, "xrandr --nograb --addmode %s \"%s\"",
                current->label, MODELINE_NAME);
        RunProgram(cmdptr);

        if (XRANDR) {
                /* Switch to mode */
                sprintf(cmd, "xrandr --nograb --output %s --mode \"%s\"",
                        current->label, MODELINE_NAME);
                RunProgram(cmdptr);
        }
	return 0;
}

int DelXrandrDisplay(ModeLine *baseMode) {
        char cmd[64*1024]={'\x00'};
        char *cmdptr = cmd;

        /* Switch to default mode */
        sprintf(cmd, "xrandr --nograb --output %s --mode \"%s\"",
                baseMode->label, MODELINE_BASE);
        RunProgram(cmdptr);

        /* Delete  modeline from interface */
        sprintf(cmd, "xrandr --nograb --delmode %s \"%s\"",
                baseMode->label, MODELINE_NAME);
        RunProgram(cmdptr);

        /* Print out modeline information */
        if (VERBOSE) {
                sprintf(cmd, "xrandr --nograb --current");
                RunProgram(cmdptr);
                sr_fprintf(stderr, "%s", cmd);
        }

        /* remove modeline */
        sprintf(cmd, "xrandr --nograb --rmmode \"%s\"", MODELINE_NAME);
        RunProgram(cmdptr);

	return 0;
}
