#include <stdio.h>
#include <string.h>

int main(
    int argc,   // rbp-0x14
    char *argv[] // rbp-0x20
) {
    int i;    // rbp-0x10
    int passlen;   // rbp-0xc
    char *hackaday_u = "hackaday-u";    // rbp-0x8

    if (argc != 2) {
        puts("Please provide the secret password!");
        return -1;
    }

    if (strlen(argv[1]) < 10) {
        puts("Come on now ...  you should expect better from us!");
        return -1;
    }

    passlen = strlen(argv[1]);
    for (i = 0; i < passlen; i++) {
        if (hackaday_u[i] + 2 != argv[1][i]) {
            puts("Wrong Password!");
            return -1;
        }
    }

    puts("Correct! You've entered the right password ... you're getting better at this!");
    return 0;
}
