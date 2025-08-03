// compile with:
// gcc -m32 -B /usr/lib32/gcc/i686-pc-linux-gnu/14.2/ -shared -fPIC -o fake fake.c
// solve with:
// LD_PRELOAD=./fake ./mbtu_patched

int ptrace(int request, int pid, int addr, int data) { return 0; }
char *getenv(const char *name) { return 0; }
