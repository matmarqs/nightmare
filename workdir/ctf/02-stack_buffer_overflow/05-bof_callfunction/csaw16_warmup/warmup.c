#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

char *gets(char *);

void easy() {
    system("cat flag.txt");
}

int main() {
    char buf[64];
    char input[64];
    write(1, "-Warm Up-\n", 10);
    write(1, "WOW:", 4);
    /* buf: rbp-0x80 */
    sprintf(buf, "%p\n", easy);
    /* 0x40060d is the address of the easy() function */
    write(1, buf, 9);
    write(1, ">", 1);
    /* input: rbp-0x40 */
    gets(input);
    /* for the stack buffer overflow:
     * we want to add (0x40 + 0x8) of padding and then the address of the easy() function 0x40060d
     * the 0x40 is because input = rbp-x040
     * the 0x8 is the pushed rbp
     * the next one is the pushed rip to return after main() is finished
     * */
}
