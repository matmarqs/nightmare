#include <stdio.h>
#include <sys/ptrace.h>

int main() {
    int x = ~0xffffffff;
    printf("0x%x\n", x);
    printf("%d\n", x);

    //printf("%d\n", 29 - (24 | 5));

    //int x = ptrace(0, 1, 1, 0);
    //printf("%d\n", x);
}
