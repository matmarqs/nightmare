#include <stdint.h>
#include <stdio.h>
#include <sys/ptrace.h>

int convert(uint64_t offset) {
    return -((int) ~0xfffffffffffffdc3);
}

int main() {
    uint64_t offset = 0xfffffffffffffdc3;
    uint32_t addr = 0x0000001d;
    uint32_t base = 0x804860d;

    printf("0x%x\n", base + addr + convert(offset));

    //printf("%c\n", 0x65);

    //int x = ~0xffffffff;
    //printf("0x%x\n", x);
    //printf("%d\n", x);

    //printf("%d\n", 29 - (24 | 5));

    //int x = ptrace(0, 1, 1, 0);
    //printf("%d\n", x);
}
