/* This is the deobfuscated version after RE */

#include <stdio.h>  /* puts */
#include <stdlib.h> /* exit */
#include <string.h> /* strncmp */
#include <sys/ptrace.h> /* ptrace */

int main(int argc,char **argv) {
    int being_traced = ptrace(PTRACE_TRACEME, 1, 1, 0); /* returns -1 if being traced, 0 otherwise */

    /* the next if block is always executed */
    if (being_traced != 1337) {  /* (0 or -1) != 1337  ->  always true */
        puts("Bad!");
        exit(1);
    }

    if (argc != 2) {
        puts("Bad!");
        exit(1);
    }

    if (strncmp(argv[1], "__gmon_start__", 14) == 0) {
        puts("Good!");
        exit(0);
    }
    else {
        puts("Bad!");
        exit(1);
    }
    return 0;
}
