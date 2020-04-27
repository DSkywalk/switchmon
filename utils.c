/*
 *
 *   SwitchMode (C) 2020 David Colmenero
 *
 *   This program is Free Software under the GNU Public License
 *
 *   SwitchMode is a simple modeline activator
 *     based on the switchres work of Calamity, Chris Kennedy, Antonio Giner
 */

#include <stdarg.h>
#include <string.h>

#include "monitor.h"

void sr_fprintf(FILE *fd, const char *fmt, ...) {
        va_list vl;

        va_start(vl, fmt);

	if (logfd)
        	vfprintf(logfd, fmt, vl);
	else
        	vfprintf(fd, fmt, vl);

        va_end(vl);

        return;
}

int split_exeargs(char* string, const char* delim, char * dest_binary, char* dest_args[]) {
    int current_ptr = 0;
    char *token = NULL;

    /* get the first executable */
    token = strtok(string, delim);
    if (token == NULL)
        return -1;

    strncpy(dest_binary, token, MAX_CHAR);
    dest_binary[MAX_CHAR-1] = '\0';

    /* walk through other tokens */
    while( token != NULL ) {
        dest_args[current_ptr] = (char *) malloc(sizeof(char) * MAX_ARGS);
        printf( " %s\n", token );
        strncpy(dest_args[current_ptr], token, MAX_ARGS);
        dest_args[current_ptr][MAX_ARGS-1] = '\0';

        /* get next token */
        token = strtok(NULL, delim);
        current_ptr ++;
    }

    return 0;
}


int RunProgram(char *temp) {
        FILE *pi;
        char c;

        if (VERBOSE)
                sr_fprintf(stderr, "Running: %s\n", temp);

        pi=popen(temp, "r");
        if(pi != NULL) {
                int i = 0;
                c=fgetc(pi);
                while(c != '\xff' && i < 64*1024) {
                        temp[i]=c;
                        c=fgetc(pi);
                        i++;
                }
                temp[i]='\x00';
                pclose(pi);
        } else
                return -1;

        if (VERBOSE)
                sr_fprintf(stderr, "Output from command is:\n '%s'\n", temp);

        return 0;
}
