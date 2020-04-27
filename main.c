/*
 *
 *   SwitchRes (C) 2020 David Colmenero
 *
 *   This program is Free Software under the GNU Public License
 *
 *   SwitchMode is a simple modeline activator
 *     based on the switchres work of Calamity, Chris Kennedy, Antonio Giner
 */

#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/wait.h>
#include "monitor.h"

FILE *logfd = NULL;
char* binary_arg[MAX_ARGS];


void clean_exit(int exit_number){
    int c;

    /* clean exe args */
    for(c = 0; c < MAX_ARGS; c++ ) {
        if (binary_arg[c] != NULL) {
            free(binary_arg[c]);
            binary_arg[c] = NULL;
        }
    }

    fclose(logfd);
    exit(exit_number);
}

void usage() {
    printf(
        "\nswitchmon - v0.3 - by D_Skywalk for RGBux\n"
        "  -m 'your_modeline_here'\n"
        "  -e 'your_exe with args'\n"
        "\n"
    );
}

int main(int argc, char **argv) {
    ModeLine current_mode;
    char binary[MAX_CHAR];
    char modeline[MAX_CHAR];
    pid_t cpid;
    int c;

    memset(&current_mode, 0, sizeof(struct ModeLine));
    memset(&binary_arg, 0, sizeof(char*) * MAX_ARGS);
    memset(&modeline, 0, sizeof(char) * MAX_CHAR);
    memset(&binary, 0, sizeof(char) * MAX_CHAR);

    logfd = fopen("/tmp/switchmon-log.txt", "w");
    if (logfd == NULL) {
        sr_fprintf(stderr, "Failed opening log\n");
        clean_exit(1);
    }

    GetXrandrDisplay(&current_mode);

    while ((c = getopt(argc, argv, "m:e:")) != -1) {
        switch(c) {
            case 'm':
                strncpy(modeline, optarg, MAX_CHAR);
                modeline[MAX_CHAR-1] = '\0';

                printf(
                    "switchmon for RGBux:\n"
                    "  modeline: %s\n", modeline);
                break;
            case 'e':
                printf("exe: %s\n", optarg);
                if ((strlen(optarg) < 1) ||
                    (split_exeargs(optarg, " ", binary, binary_arg) == -1)
                ){
                    fprintf(stderr, "%c invalid args...\n", c);
                    usage();
                    clean_exit(2);
                }

                break;
            default:
                sr_fprintf(stderr, "%c not found...\n", c);
                usage();
                clean_exit(3);
        }
    }

    if((argc < 4) || (strlen(modeline) < 1) || (strlen(binary) < 1)) {
        fprintf(stderr, "Error: invalid args...\n");
        usage();
        exit(1);
    }

    SetXrandrDisplay((char*) modeline, &current_mode);

    /* Fork and run program, wait for pid to exit */
    cpid = fork();
    switch (cpid) {
        case -1:
            sr_fprintf(stderr, "Failed running %s\n", binary);
            break;

        case 0:
            execvp(binary, (char* const*) binary_arg);
                {
                    char *path = NULL;
                    char *envp = getenv("PATH");
                    path = strtok(envp, ":");
                    while(path != NULL) {
                        char executable[255];
                        sprintf(executable, "%s/%s", path, binary);
                        printf(">> exe: %s\n", executable);
                        execvp(executable,(char* const*) binary);
                        if (VERBOSE)
                            sr_fprintf(stderr, "Failed running %s\n", executable);
                        path = strtok(NULL, ":");
                    }
                }

            sr_fprintf(stderr, "Error running %s\n", binary);
            cpid = -1;
            break;

        default:
            printf(">> cpid: %u\n", cpid);
            waitpid(cpid, NULL, 0);
            break;
    }

    /* Remove modeline with Xrandr */
    DelXrandrDisplay(&current_mode);
    printf("remove modeline...\n");

    clean_exit(0);
}
