#include<stdio.h>

int main(int argc, char *argv[]){
    char *processnum = argv[0];
    printf("This is the child %s\n", processnum);
    for(int c = 1; c < argc; c++)
        printf(">> argc %s\n", argv[c]);

    return 0;
}
