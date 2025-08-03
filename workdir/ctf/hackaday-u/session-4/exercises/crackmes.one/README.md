# CrackMes

Within this folder you will find four crackmes all sources from crackmes.one, the original authors will be credited below

1. kellek's noprelo: https://crackmes.one/crackme/5b9d312233c5d45fc286ae03
2. m3hd1's half-twins: https://crackmes.one/crackme/5dce805c33c5d419aa0131ae
3. crackmes.de's moreboredthanyou by stefanie: https://crackmes.one/crackme/5ab77f5a33c5d40ad448c504
4. BitFriends's auth: https://crackmes.one/crackme/5e8349b033c5d4439bb2e040


# `noprelo`

From Ghidra, we get:

```c
int not(int x) {
    return ~x;
}

void main(int argc,char **argv) {
    uint uVar1;
    uint uVar2;
    uint uVar3;
    int iVar4;
    long lVar5;

    not(0xfffffdb1); /* dead code */
    uVar1 = not(0xffffffff);    /* 0 */
    uVar2 = not(0xfffffffe);    /* 1 */
    uVar3 = not(0xffffffff);    /* 1 */
    iVar4 = not(0xffffffe2);    /* 29 = 0x1d */
    // By inspecting /usr/include/sys/ptrace.h, we see
    // PTRACE_SYSCALL = 24
    // PTRACE_POKEDATA = 5
    // PTRACE_TRACEME = 0
    // long ptrace(enum __ptrace_request request, pid_t pid, void *addr, void *data);
    lVar5 = ptrace(iVar4 - (PTRACE_SYSCALL|PTRACE_POKEDATA),(ulong)uVar3,(ulong)uVar2,(ulong)uVar1); /* -1 if being traced, 0 otherwise */
    //    = ptrace(29 - (24 | 5), 1, 1, 0);
    //    = ptrace(29 - 29, 1, 1, 0);
    //    = ptrace(0, 1, 1, 0);
    // Basically, ptrace(PTRACE_TRACEME) returns 0 if the process is not being traced
    // and returns -1 if it is being traced
    //
    // The ptrace(PTRACE_TRACEME) system call is used by a child process to indicate to its parent that it should be traced.
    // Return Value:
    // 
    //     On success, ptrace(PTRACE_TRACEME) returns 0.
    //     On error, it returns -1,
    iVar4 = not(0xfffffac6);    /* 1337 = 0x539 */
    /* the next if block is always executed */
    if ((int)lVar5 != iVar4) {  /* (0 or -1) != 1337  ->  always true */
        puts("Bad!");
        iVar4 = not(0xfffffffe); /* 1 */
        exit(iVar4);
    }
    iVar4 = not(0xfffffffd); /* 2 */
    /* needs to have argv[1] */
    if (argc != iVar4) { /* if (argc != 2) */
        puts("Bad!");
        iVar4 = not(0xfffffffe); /* 1 */
        exit(iVar4);
    }
    iVar4 = not(0xfffffffe); /* 1 */
    real_main(argv[iVar4]);
    not(0xffffffff); /* 0 */
    return;
}

void real_main(char *argv1) {
  int iVar1 = not(0xfffffff1);  /* 14 = 0xe */
  iVar1 = strncmp(argv1,"__gmon_start__",(long)iVar1);  /* compare 14 bytes of "__gmon_start__" */
  if (iVar1 == 0) { /* if the strings are equal */
    puts("Good!");
    iVar1 = not(0xffffffff); /* 0 */
    exit(iVar1);
  }
  puts("Bad!");
  iVar1 = not(0xfffffffe);  /* 1 */
  exit(iVar1);
}
```

After deobfuscating the code, we get something like this in functionality:

```c
void main(int argc,char **argv) {
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
}
```

Therefore, in order to get the executable to output `"Good!"`, we have two alternatives:

* Run the binary with a debugger, set a breakpoint at `if (being_traced != 1337)`, and change the variables accordingly to not exit rightaway.
* Patch the binary, effectively removing the `if block` associated with the `ptrace` call.

## Using the debugger `radare`

```
[sekai@void crackmes.one]$ r2 -d ./noprelo __gmon_start__
WARN: Relocs has not been applied. Please use `-e bin.relocs.apply=true` or `-e bin.cache=true` next time
[0x7f36fd5c5cc0]> aa
INFO: Analyze all flags starting with sym. and entry0 (aa)
INFO: Analyze imports (af@@@i)
INFO: Analyze entrypoint (af@ entry0)
INFO: Analyze symbols (af@@@s)
INFO: Recovering variables (afva@@@F)
INFO: Analyze all functions arguments/locals (afva@@@F)
[0x7f36fd5c5cc0]> afl
0x561227a63030    1      6 sym.imp.strncmp
0x561227a63040    1      6 sym.imp.puts
0x561227a63050    1      6 sym.imp.ptrace
0x561227a63060    1      6 sym.imp.exit
0x561227a63070    1     46 entry0
0x561227a65fe0    1     24 reloc.__libc_start_main
0x561227a631e1    5    237 main
0x561227a63169    1     14 fcn.561227a63169
0x561227a63177    3    106 fcn.561227a63177
0x561227a63160    5     60 entry.init0
0x561227a63110    5     55 entry.fini0
0x561227a65ff8    1   4104 reloc.__cxa_finalize
0x561227a630a0    4     34 fcn.561227a630a0
[0x7f36fd5c5cc0]> db main
[0x7f36fd5c5cc0]> dc
INFO: hit breakpoint at: 0x561227a631e1
[0x561227a631e1]> pd 50
            ;-- rax:
            ;-- rip:
            ; DATA XREF from entry0 @ 0x561227a63091(r)
┌ 237: int main (int argc, char **argv, char **envp);
│ `- args(rdi, rsi, rdx) vars(4:sp[0x2c..0x50])
│           0x561227a631e1 b    55             push rbp
│           0x561227a631e2      4889e5         mov rbp, rsp
│           0x561227a631e5      4155           push r13
│           0x561227a631e7      4154           push r12
│           0x561227a631e9      53             push rbx
│           0x561227a631ea      4883ec38       sub rsp, 0x38
│           0x561227a631ee      897dcc         mov dword [var_34h], edi ; argc
│           0x561227a631f1      488975c0       mov qword [var_40h], rsi ; argv
│           0x561227a631f5      488955b8       mov qword [var_48h], rdx ; envp
│           0x561227a631f9      bfb1fdffff     mov edi, 0xfffffdb1     ; 4294966705
│           0x561227a631fe      e866ffffff     call fcn.561227a63169
│           0x561227a63203      8945dc         mov dword [var_24h], eax
│           0x561227a63206      bfffffffff     mov edi, 0xffffffff     ; -1
│           0x561227a6320b      e859ffffff     call fcn.561227a63169
│           0x561227a63210      4189c5         mov r13d, eax
│           0x561227a63213      bffeffffff     mov edi, 0xfffffffe     ; 4294967294
│           0x561227a63218      e84cffffff     call fcn.561227a63169
│           0x561227a6321d      4189c4         mov r12d, eax
│           0x561227a63220      bfffffffff     mov edi, 0xffffffff     ; -1
│           0x561227a63225      e83fffffff     call fcn.561227a63169
│           0x561227a6322a      89c3           mov ebx, eax
│           0x561227a6322c      bfe2ffffff     mov edi, 0xffffffe2     ; 4294967266
│           0x561227a63231      e833ffffff     call fcn.561227a63169
│           0x561227a63236      83e81d         sub eax, 0x1d           ; 29
│           0x561227a63239      4489e9         mov ecx, r13d
│           0x561227a6323c      4489e2         mov edx, r12d
│           0x561227a6323f      89de           mov esi, ebx
│           0x561227a63241      89c7           mov edi, eax
│           0x561227a63243      b800000000     mov eax, 0
│           0x561227a63248      e803feffff     call sym.imp.ptrace     ; long ptrace(__ptrace_request request, pid_t pid, void*addr, void*data)
│           0x561227a6324d      8945dc         mov dword [var_24h], eax
│           0x561227a63250      bfc6faffff     mov edi, 0xfffffac6     ; 4294965958
│           0x561227a63255      e80fffffff     call fcn.561227a63169
│           0x561227a6325a      3945dc         cmp dword [var_24h], eax
│       ┌─< 0x561227a6325d      741d           je 0x561227a6327c
│       │   0x561227a6325f      488d3db30d..   lea rdi, str.Bad_       ; 0x561227a64019 ; "Bad!"
│       │   0x561227a63266      e8d5fdffff     call sym.imp.puts       ; int puts(const char *s)
│       │   0x561227a6326b      bffeffffff     mov edi, 0xfffffffe     ; 4294967294
│       │   0x561227a63270      e8f4feffff     call fcn.561227a63169
│       │   0x561227a63275      89c7           mov edi, eax
│       │   0x561227a63277      e8e4fdffff     call sym.imp.exit       ; void exit(int status)
│       └─> 0x561227a6327c      bffdffffff     mov edi, 0xfffffffd     ; 4294967293
│           0x561227a63281      e8e3feffff     call fcn.561227a63169
│           0x561227a63286      3945cc         cmp dword [var_34h], eax
│       ┌─< 0x561227a63289      741d           je 0x561227a632a8
│       │   0x561227a6328b      488d3d870d..   lea rdi, str.Bad_       ; 0x561227a64019 ; "Bad!"
│       │   0x561227a63292      e8a9fdffff     call sym.imp.puts       ; int puts(const char *s)
│       │   0x561227a63297      bffeffffff     mov edi, 0xfffffffe     ; 4294967294
│       │   0x561227a6329c      e8c8feffff     call fcn.561227a63169
│       │   0x561227a632a1      89c7           mov edi, eax
[0x561227a631e1]> db 0x561227a6325a
[0x561227a631e1]> dc
INFO: hit breakpoint at: 0x561227a6325a
[0x561227a6325a]> pf d @ rbp - 0x24 ;  printing [var_24h]
0x7ffc8085ae2c = -1
[0x561227a6325a]> pd 20
│           ;-- rip:
│           0x561227a6325a b    3945dc         cmp dword [var_24h], eax
│       ┌─< 0x561227a6325d      741d           je 0x561227a6327c
│       │   0x561227a6325f      488d3db30d..   lea rdi, str.Bad_       ; 0x561227a64019 ; "Bad!"
│       │   0x561227a63266      e8d5fdffff     call sym.imp.puts       ; int puts(const char *s)
│       │   0x561227a6326b      bffeffffff     mov edi, 0xfffffffe     ; 4294967294
│       │   0x561227a63270      e8f4feffff     call fcn.561227a63169
│       │   0x561227a63275      89c7           mov edi, eax
│       │   0x561227a63277      e8e4fdffff     call sym.imp.exit       ; void exit(int status)
│       └─> 0x561227a6327c      bffdffffff     mov edi, 0xfffffffd     ; 4294967293
│           0x561227a63281      e8e3feffff     call fcn.561227a63169
│           0x561227a63286      3945cc         cmp dword [var_34h], eax
│       ┌─< 0x561227a63289      741d           je 0x561227a632a8
│       │   0x561227a6328b      488d3d870d..   lea rdi, str.Bad_       ; 0x561227a64019 ; "Bad!"
│       │   0x561227a63292      e8a9fdffff     call sym.imp.puts       ; int puts(const char *s)
│       │   0x561227a63297      bffeffffff     mov edi, 0xfffffffe     ; 4294967294
│       │   0x561227a6329c      e8c8feffff     call fcn.561227a63169
│       │   0x561227a632a1      89c7           mov edi, eax
│       │   0x561227a632a3      e8b8fdffff     call sym.imp.exit       ; void exit(int status)
│       └─> 0x561227a632a8      bffeffffff     mov edi, 0xfffffffe     ; 4294967294
│           0x561227a632ad      e8b7feffff     call fcn.561227a63169
[0x561227a6325a]> dr eax
0x00000539
[0x561227a6325a]> dr eax = -1
0x00000539 -> 0xffffffffffffffff -> 0xffffffff
[0x561227a6325a]> dr eax
0xffffffff
[0x561227a6325a]> pf d @ rbp - 0x24
0x7ffc8085ae2c = -1
[0x561227a6325a]> dc
Good!
(22774) Process exited with status=0x0
[0x7f36fd479d25]> 
```

## Patching the binary with `radare`

In the following, we replace the `ptrace` call with 5 `nop`'s at address `0x00001248`.
Before these `nop`'s, the `eax` register is set to `0` and is saved at the local variable `[var_24h]`.
Continuing, at address `0x00001250`, the `edi` register is set to `0xfffffac6` by `mov edi, 0xfffffac6` and then the `not()` function is called.
This will set `eax = not(0xfffffac6) = 0x539 = 1337`. Instead, we just patch the instruction to be `mov edi, 0xffffffff`.
In result, we will have `eax = not(0xffffffff) = 0`, which will be compared to `[var_24h]` which is also `0`.
The `0x0000125d  je 0x127c` will be taken, and program will reach the `real_main()` function normally.

From this, we get the program to output `"Good!"` when we run `./noprelo_patched "__gmon_start__"`.

```
[sekai@void crackmes.one]$ cp noprelo noprelo_patched 
[sekai@void crackmes.one]$ r2 -w ./noprelo_patched 
WARN: Relocs has not been applied. Please use `-e bin.relocs.apply=true` or `-e bin.cache=true` next time
[0x00001070]> aaa
INFO: Analyze all flags starting with sym. and entry0 (aa)
INFO: Analyze imports (af@@@i)
INFO: Analyze entrypoint (af@ entry0)
INFO: Analyze symbols (af@@@s)
INFO: Analyze all functions arguments/locals (afva@@@F)
INFO: Analyze function calls (aac)
INFO: Analyze len bytes of instructions for references (aar)
INFO: Finding and parsing C++ vtables (avrr)
INFO: Analyzing methods (af @@ method.*)
INFO: Recovering local variables (afva@@@F)
INFO: Type matching analysis for all functions (aaft)
INFO: Propagate noreturn information (aanr)
INFO: Use -AA or aaaa to perform additional experimental analysis
[0x00001070]> afl
0x00001030    1      6 sym.imp.strncmp
0x00001040    1      6 sym.imp.puts
0x00001050    1      6 sym.imp.ptrace
0x00001060    1      6 sym.imp.exit
0x00001070    1     46 entry0
0x000011e1    5    237 main
0x00001169    1     14 fcn.00001169
0x00001177    3    106 fcn.00001177
0x00001160    5     60 entry.init0
0x00001110    5     55 entry.fini0
0x000010a0    4     34 fcn.000010a0
0x00001000    3     27 fcn.00001000
[0x00001070]> s main
[0x000011e1]> pd 50
            ; DATA XREF from entry0 @ 0x1091(r)
┌ 237: int main (uint32_t argc, char **argv, char **envp);
│ `- args(rdi, rsi, rdx) vars(4:sp[0x2c..0x50])
│           0x000011e1      55             push rbp
│           0x000011e2      4889e5         mov rbp, rsp
│           0x000011e5      4155           push r13
│           0x000011e7      4154           push r12
│           0x000011e9      53             push rbx
│           0x000011ea      4883ec38       sub rsp, 0x38
│           0x000011ee      897dcc         mov dword [var_34h], edi    ; argc
│           0x000011f1      488975c0       mov qword [var_40h], rsi    ; argv
│           0x000011f5      488955b8       mov qword [var_48h], rdx    ; envp
│           0x000011f9      bfb1fdffff     mov edi, 0xfffffdb1         ; 4294966705 ; int64_t arg1
│           0x000011fe      e866ffffff     call fcn.00001169
│           0x00001203      8945dc         mov dword [var_24h], eax
│           0x00001206      bfffffffff     mov edi, 0xffffffff         ; -1 ; int64_t arg1
│           0x0000120b      e859ffffff     call fcn.00001169
│           0x00001210      4189c5         mov r13d, eax
│           0x00001213      bffeffffff     mov edi, 0xfffffffe         ; 4294967294 ; int64_t arg1
│           0x00001218      e84cffffff     call fcn.00001169
│           0x0000121d      4189c4         mov r12d, eax
│           0x00001220      bfffffffff     mov edi, 0xffffffff         ; -1 ; int64_t arg1
│           0x00001225      e83fffffff     call fcn.00001169
│           0x0000122a      89c3           mov ebx, eax
│           0x0000122c      bfe2ffffff     mov edi, 0xffffffe2         ; 4294967266 ; int64_t arg1
│           0x00001231      e833ffffff     call fcn.00001169
│           0x00001236      83e81d         sub eax, 0x1d
│           0x00001239      4489e9         mov ecx, r13d               ; void*data
│           0x0000123c      4489e2         mov edx, r12d               ; void*addr
│           0x0000123f      89de           mov esi, ebx                ; pid_t pid
│           0x00001241      89c7           mov edi, eax                ; __ptrace_request request
│           0x00001243      b800000000     mov eax, 0
│           0x00001248      e803feffff     call sym.imp.ptrace         ; long ptrace(__ptrace_request request, pid_t pid, void*addr, void*data)
│           0x0000124d      8945dc         mov dword [var_24h], eax
│           0x00001250      bfc6faffff     mov edi, 0xfffffac6         ; 4294965958 ; int64_t arg1
│           0x00001255      e80fffffff     call fcn.00001169
│           0x0000125a      3945dc         cmp dword [var_24h], eax
│       ┌─< 0x0000125d      741d           je 0x127c
│       │   0x0000125f      488d3db30d..   lea rdi, str.Bad_           ; 0x2019 ; "Bad!" ; const char *s
│       │   0x00001266      e8d5fdffff     call sym.imp.puts           ; int puts(const char *s)
│       │   0x0000126b      bffeffffff     mov edi, 0xfffffffe         ; 4294967294 ; int64_t arg1
│       │   0x00001270      e8f4feffff     call fcn.00001169
│       │   0x00001275      89c7           mov edi, eax                ; int status
│       │   0x00001277      e8e4fdffff     call sym.imp.exit           ; void exit(int status)
│       │   ; CODE XREF from main @ 0x125d(x)
│       └─> 0x0000127c      bffdffffff     mov edi, 0xfffffffd         ; 4294967293 ; int64_t arg1
│           0x00001281      e8e3feffff     call fcn.00001169
│           0x00001286      3945cc         cmp dword [var_34h], eax
│       ┌─< 0x00001289      741d           je 0x12a8
│       │   0x0000128b      488d3d870d..   lea rdi, str.Bad_           ; 0x2019 ; "Bad!" ; const char *s
│       │   0x00001292      e8a9fdffff     call sym.imp.puts           ; int puts(const char *s)
│       │   0x00001297      bffeffffff     mov edi, 0xfffffffe         ; 4294967294 ; int64_t arg1
│       │   0x0000129c      e8c8feffff     call fcn.00001169
│       │   0x000012a1      89c7           mov edi, eax                ; int status
[0x000011e1]> s 0x00001248
[0x00001248]> wx 9090909090
[0x00001248]> pd 20
│           0x00001248      90             nop
│           0x00001249      90             nop
│           0x0000124a      90             nop
│           0x0000124b      90             nop
│           0x0000124c      90             nop
│           0x0000124d      8945dc         mov dword [var_24h], eax
│           0x00001250      bfc6faffff     mov edi, 0xfffffac6         ; 4294965958 ; int64_t arg1
│           0x00001255      e80fffffff     call fcn.00001169
│           0x0000125a      3945dc         cmp dword [var_24h], eax
│       ┌─< 0x0000125d      741d           je 0x127c
│       │   0x0000125f      488d3db30d..   lea rdi, str.Bad_           ; 0x2019 ; "Bad!" ; const char *s
│       │   0x00001266      e8d5fdffff     call sym.imp.puts           ; int puts(const char *s)
│       │   0x0000126b      bffeffffff     mov edi, 0xfffffffe         ; 4294967294 ; int64_t arg1
│       │   0x00001270      e8f4feffff     call fcn.00001169
│       │   0x00001275      89c7           mov edi, eax                ; int status
│       │   0x00001277      e8e4fdffff     call sym.imp.exit           ; void exit(int status)
│       │   ; CODE XREF from main @ 0x125d(x)
│       └─> 0x0000127c      bffdffffff     mov edi, 0xfffffffd         ; 4294967293 ; int64_t arg1
│           0x00001281      e8e3feffff     call fcn.00001169
│           0x00001286      3945cc         cmp dword [var_34h], eax
│       ┌─< 0x00001289      741d           je 0x12a8
[0x00001248]> s 0x00001250
[0x00001250]> wa mov edi, 0xffffffff
INFO: Written 5 byte(s) (mov edi, 0xffffffff) = wx bfffffffff @ 0x00001250
[0x00001250]> q
[sekai@void crackmes.one]$ ./noprelo_patched 
Bad!
[sekai@void crackmes.one]$ ./noprelo_patched "__gmon_start__"
Good!
```


# `half-twins`

Static analysis with `radare2`:

```c
#define is_odd(x) ((x) & 1)

int main(
    int argc,       // var_44h, rbp-0x44
    char *argv[]    // var_50h, rbp-0x50
) {
    char *good_luck_next_time = "good luck next time";

    if (argc != 3) {
        puts("hmm... i'm not sure you know what the word \"twins\" mean :/");
        puts(good_luck_next_time);    // "good luck next time"
        return 0;
    }

    char *abby = argv[1];
    char *gabby = argv[2];

    if (strlen(abby) <= 6) {
        puts("Abby: i'm older than that :(");
        puts(good_luck_next_time);
        return 0;
    }
    if (strlen(gabby) <= 6) {
        puts("Gabby: i'm older than that :(");
        puts(good_luck_next_time);
        return 0;
    }

    if (strlen(gabby) != strlen(abby)) {
        puts("Abby & Gabby: for god's sake we are TWINS! we were born the same night!!");
        puts(good_luck_next_time);
        return 0;
    }

    if (is_odd(strlen(abby))) {
        puts("Abby & Gabby: we are not \"odd\" years old :(");
        puts(good_luck_next_time);
        return 0;
    }

    int half_len = strlen(abby) >> 1;
    for (int i = 0; i < half_len; i++) {
        if (gabby[i] != abby[i]) {
            puts("Abby & Gabby: we're half twins you know...");
            puts(good_luck_next_time);
            return 0;
        }
    }

    full_len = strlen(abby);
    for (; i < full_len; i++) {
        if (gabby[i] == abby[i]) {
            puts("Abby & Gabby: we are only HALF twins... :3 ");
            puts(good_luck_next_time);
            return 0;
        }
    }

    puts("Abby & Gabby: yaayy!! nice job! :D");
    return 0;
}
```

```bash
$ ./half-twins aaaabbbb aaaacccc
Abby & Gabby: yaayy!! nice job! :D
```


# `mbtu`

The binary is 32-bit and has **No PIE enabled**.

```
$ file mbtu
mbtu: ELF 32-bit LSB executable, Intel i386, version 1, for GNU/Linux 2.4.1, dynamically linked, interpreter /lib/ld-linux.so.2, no section header
$ pwn checksec mbtu
[!] Did not find any GOT entries
[*] '/home/sekai/code/nightmare/workdir/ctf/hackaday-u/session-4/exercises/crackmes.one/mbtu'
    Arch:       i386-32-little
    RELRO:      No RELRO
    Stack:      No canary found
    NX:         NX enabled
    PIE:        No PIE (0x8048000)
```

## Patching the corrupted binary

First of all, I tried to run the binary directly. It always **segfaults**. Inspecting in GDB, it segfaults in address `0x80485c1`.

```
$ gdb ./mbtu
Reading symbols from ./mbtu...
(No debugging symbols found in ./mbtu)
(gdb) starti
Starting program: /home/sekai/code/nightmare/workdir/ctf/hackaday-u/session-4/exercises/crackmes.one/mbtu

Program stopped.
0xf7fe58f0 in _start () from /lib/ld-linux.so.2
(gdb) ni
0xf7fe58f2 in _start () from /lib/ld-linux.so.2
(gdb)
0xf7fe58f5 in _start () from /lib/ld-linux.so.2
(gdb)
0xf7fe58f6 in _start () from /lib/ld-linux.so.2
(gdb)
[Thread debugging using libthread_db enabled]
Using host libthread_db library "/usr/lib64/libthread_db.so.1".
0xf7fe58fb in _start () from /lib/ld-linux.so.2
(gdb)
0xf7fe58fe in _dl_start_user () from /lib/ld-linux.so.2
(gdb)
0xf7fe5900 in _dl_start_user () from /lib/ld-linux.so.2
(gdb)
0xf7fe58e0 in ?? () from /lib/ld-linux.so.2

<SNIP>

(gdb)
0x0804840b in ?? ()
(gdb)
0x080483b0 in ?? ()
(gdb)
0x080483b6 in ?? ()
(gdb)

Program received signal SIGSEGV, Segmentation fault.
0x080485c1 in ?? ()
(gdb)

Program terminated with signal SIGSEGV, Segmentation fault.
The program no longer exists.
```

The output of `strace` is:
```
$ strace ./mbtu
execve("./mbtu", ["./mbtu"], 0x7ffe71364e70 /* 71 vars */) = 0
[ Process PID=16119 runs in 32 bit mode. ]
brk(NULL)                               = 0x8892000
mmap2(NULL, 8192, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0) = 0xf7ef9000
access("/etc/ld.so.preload", R_OK)      = -1 ENOENT (No such file or directory)
openat(AT_FDCWD, "/etc/ld.so.cache", O_RDONLY|O_LARGEFILE|O_CLOEXEC) = 3
statx(3, "", AT_STATX_SYNC_AS_STAT|AT_NO_AUTOMOUNT|AT_EMPTY_PATH, STATX_BASIC_STATS, {stx_mask=STATX_BASIC_STATS|STATX_MNT_ID, stx_attributes=0, stx_mode=S_IFREG|0644, stx_size=80263, ...}) = 0
mmap2(NULL, 80263, PROT_READ, MAP_PRIVATE, 3, 0) = 0xf7ee5000
close(3)                                = 0
openat(AT_FDCWD, "/usr/lib32/libc.so.6", O_RDONLY|O_LARGEFILE|O_CLOEXEC) = 3
read(3, "\177ELF\1\1\1\3\0\0\0\0\0\0\0\0\3\0\3\0\1\0\0\0\340d\2\0004\0\0\0"..., 512) = 512
statx(3, "", AT_STATX_SYNC_AS_STAT|AT_NO_AUTOMOUNT|AT_EMPTY_PATH, STATX_BASIC_STATS, {stx_mask=STATX_BASIC_STATS|STATX_MNT_ID, stx_attributes=0, stx_mode=S_IFREG|0755, stx_size=13015676, ...}) = 0
mmap2(NULL, 2275600, PROT_READ, MAP_PRIVATE|MAP_DENYWRITE, 3, 0) = 0xf7cb9000
mmap2(0xf7cdc000, 1703936, PROT_READ|PROT_EXEC, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x23000) = 0xf7cdc000
mmap2(0xf7e7c000, 376832, PROT_READ, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x1c3000) = 0xf7e7c000
mmap2(0xf7ed8000, 12288, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x21f000) = 0xf7ed8000
mmap2(0xf7edb000, 39184, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_FIXED|MAP_ANONYMOUS, -1, 0) = 0xf7edb000
close(3)                                = 0
set_thread_area({entry_number=-1, base_addr=0xf7efa440, limit=0x0fffff, seg_32bit=1, contents=0, read_exec_only=0, limit_in_pages=1, seg_not_present=0, useable=1}) = 0 (entry_number=12)
set_tid_address(0xf7efa4a8)             = 16119
set_robust_list(0xf7efa4ac, 12)         = 0
rseq(0xf7efa3c0, 0x20, 0, 0x53053053)   = 0
mprotect(0xf7ed8000, 8192, PROT_READ)   = 0
mprotect(0xf7f35000, 8192, PROT_READ)   = 0
ugetrlimit(RLIMIT_STACK, {rlim_cur=8192*1024, rlim_max=RLIM_INFINITY}) = 0
munmap(0xf7ee5000, 80263)               = 0
mprotect(0x8048000, 242, PROT_READ|PROT_WRITE) = 0
--- SIGSEGV {si_signo=SIGSEGV, si_code=SEGV_ACCERR, si_addr=0x80485c1} ---
+++ killed by SIGSEGV +++
Segmentation fault
```

The key line is
```
mprotect(0x8048000, 242, PROT_READ|PROT_WRITE) = 0
--- SIGSEGV {si_signo=SIGSEGV, si_code=SEGV_ACCERR, si_addr=0x80485c1} ---
```

From Linux man pages:
```
mprotect(2)                   System Calls Manual                  mprotect(2)

NAME
       mprotect, pkey_mprotect - set protection on a region of memory

SYNOPSIS
       #include <sys/mman.h>

       int mprotect(size_t size;
                    void addr[size], size_t size, int prot);

DESCRIPTION
       mprotect() changes the access protections for the calling process's
       memory pages containing any part of the address range in the interval
       [addr, addr+size-1].  addr must be aligned to a page boundary.

       If the calling process tries to access memory in a manner that violates
       the protections, then the kernel generates a SIGSEGV signal for the
       process.

       prot is a combination of the following access flags: PROT_NONE or a
       bitwise OR of the other values in the following list:

       PROT_NONE
              The memory cannot be accessed at all.

       PROT_READ
              The memory can be read.

       PROT_WRITE
              The memory can be modified.

       PROT_EXEC
              The memory can be executed.
```

ChatGPT says:

The binary successfully requested read-write permission on the code page, but it immediately segfaults on a memory access error (`SEGV_ACCERR`) at address `0x80485c1`.

Why this happens despite `mprotect` succeeding?

* The binary tries to execute code in a page that currently does not have execute permission.
* The binary calls mprotect with only `PROT_READ|PROT_WRITE` but not `PROT_EXEC`.
* Modern Linux kernel enforces **W^X (write xor execute)** policies strictly. Pages with write permission but without execute permission will cause a fault on code execution.
* The binary **never adds the `PROT_EXEC` flag**, so the CPU forbids instruction fetches at `0x80485c1` causing the fault.

How to fix it?

* Patch the binary to add `PROT_EXEC` to the `mprotect` call
* Find the `mprotect` syscall invocation in the binary.
* Change the flags parameter from `PROT_READ|PROT_WRITE` (`0x3`) to `PROT_READ|PROT_WRITE|PROT_EXEC` (`0x7`).

In `radare2` we get the following disassembly around this region:
```
[0x080485c1]> pd-- 10
            0x080485a7      8945fc         mov dword [ebp - 4], eax
            0x080485aa      83ec04         sub esp, 4
            0x080485ad      6a03           push 3                      ; 3
            0x080485af      8b55f4         mov edx, dword [ebp - 0xc]
            0x080485b2      8b45f0         mov eax, dword [ebp - 0x10]
            0x080485b5      29d0           sub eax, edx
            0x080485b7      48             dec eax
            0x080485b8      50             push eax
            0x080485b9      ff75fc         push dword [ebp - 4]
            0x080485bc      e81ffeffff     call sym.imp.mprotect
            0x080485c1      83c410         add esp, 0x10               ; PROGRAM SEGFAULTS HERE
            0x080485c4      85c0           test eax, eax
        ┌─< 0x080485c6      740a           je 0x80485d2
        │   0x080485c8      83ec0c         sub esp, 0xc
        │   0x080485cb      6a01           push 1                      ; 1
        │   0x080485cd      e84efeffff     call sym.imp.exit
        └─> 0x080485d2      c745f80100..   mov dword [ebp - 8], 1
            0x080485d9      8b45f0         mov eax, dword [ebp - 0x10]
            0x080485dc      48             dec eax
            0x080485dd      3945f4         cmp dword [ebp - 0xc], eax
```

The instruction we want to patch is:
```asm
0x080485ad      6a03           push 3
```

In `radare2`:

```
(.venv) [sekai@void crackmes.one]$ cp mbtu mbtu_patched
(.venv) [sekai@void crackmes.one]$ r2 -w ./mbtu_patched
[0x08048460]> aaa
INFO: Analyze all flags starting with sym. and entry0 (aa)
INFO: Analyze imports (af@@@i)
INFO: Analyze entrypoint (af@ entry0)
INFO: Analyze symbols (af@@@s)
INFO: Analyze all functions arguments/locals (afva@@@F)
INFO: Analyze function calls (aac)
INFO: Analyze len bytes of instructions for references (aar)
INFO: Finding and parsing C++ vtables (avrr)
INFO: Analyzing methods (af @@ method.*)
INFO: Recovering local variables (afva@@@F)
INFO: Type matching analysis for all functions (aaft)
INFO: Propagate noreturn information (aanr)
INFO: Use -AA or aaaa to perform additional experimental analysis
[0x08048460]> s 0x80485ad
[0x080485ad]> pd 20
            0x080485ad      6a03           push 3                      ; 3
            0x080485af      8b55f4         mov edx, dword [ebp - 0xc]
            0x080485b2      8b45f0         mov eax, dword [ebp - 0x10]
            0x080485b5      29d0           sub eax, edx
            0x080485b7      48             dec eax
            0x080485b8      50             push eax
            0x080485b9      ff75fc         push dword [ebp - 4]
            0x080485bc      e81ffeffff     call sym.imp.mprotect
            0x080485c1      83c410         add esp, 0x10
            0x080485c4      85c0           test eax, eax
        ┌─< 0x080485c6      740a           je 0x80485d2
        │   0x080485c8      83ec0c         sub esp, 0xc
        │   0x080485cb      6a01           push 1                      ; 1
        │   0x080485cd      e84efeffff     call sym.imp.exit           ; void exit(int status)
        │   ; CODE XREF from fcn.080484e4 @ +0xe2(x)
        └─> 0x080485d2      c745f80100..   mov dword [ebp - 8], 1
│           ; CODE XREF from main @ 0x8048609(x)
│           0x080485d9      8b45f0         mov eax, dword [ebp - 0x10]
│           0x080485dc      48             dec eax
│           0x080485dd      3945f4         cmp dword [ebp - 0xc], eax
│       ┌─< 0x080485e0      7202           jb 0x80485e4
│      ┌──< 0x080485e2      eb27           jmp 0x804860b
[0x080485ad]> wa push 7
INFO: Written 2 byte(s) (push 7) = wx 6a07 @ 0x080485ad
[0x080485ad]> q
```

Now the binary runs normally:
```
(.venv) [sekai@void crackmes.one]$ ./mbtu_patched
..:: MoreBoredThanYou by niel anthony acuna ::..
```

## Cracking it

If we seek to the `main` (address `0x08048460`) function in `radare2`, we see that the assembly looks weird. There are some instructions that do not make any sense.
It seems they are **encrypted**.

```
[0x08048460]> afl
0x080483c0    1      6 sym.imp.close
0x080483d0    1      6 sym.imp.getenv
0x080483e0    1      6 sym.imp.mprotect
0x080483f0    1      6 sym.imp.ptrace
0x08048400    1      6 sym.imp.__libc_start_main
0x08048410    1      6 sym.imp.printf
0x08048420    1      6 sym.imp.exit
0x08048430    1      6 sym.imp.htons
0x08048440    1      6 sym.imp.connect
0x08048450    1      6 sym.imp.socket
0x08048460    1     33 entry0
0x0804860d    9    109 main
0x080484e4    4     42 fcn.080484e4
0x080487b4    3     36 fcn.080487b4
0x0804848d    3     25 fcn.0804848d
0x080487b0    1      4 fcn.080487b0
0x08048398    1     23 fcn.08048398
0x08048484    1      9 fcn.08048484
0x080487d8    1     27 fcn.080487d8
0x080484a8    6     58 fcn.080484a8
[0x08048460]> s main
[0x0804860d]> pdf
       ╎    ; DATA XREF from entry0 @ 0x8048477(r)
       ╎    ; DATA XREFS from fcn.080484e4 @ +0x32(r), +0xad(w)
┌ 109: int main (int argc, char **argv, char **envp);
│      ╎    0x0804860d      54             push esp
│      ╎    0x0804860e      8ae0           mov ah, al
│      ╎    0x08048610      84e5           test ch, ah
│      ╎    0x08048612      238eebe1ab15   and ecx, dword [esi + 0x15abe1eb]
│      ╎    0x08048618      17             pop ss
│      ╎    0x08048619      191b           sbb dword [ebx], ebx
│      ╎    0x0804861b      34db           xor al, 0xdb                ; 219
│      ╎    0x0804861d      a2cf294f09     mov byte [0x94f29cf], al    ; [0x94f29cf:1]=255
│      ╎    0x08048622      a32927d995     mov dword [0x95d92729], eax ; [0x95d92729:4]=-1
│      ╎    0x08048627      c8c8c6b8       enter 0xffffffffffffc6c8, 0xffffffffffffffb8
│      ╎    0x0804862b      f9             stc
│      ╎    0x0804862c      2f             das
│      ╎    0x0804862d      c483314dcaa7   les eax, [ebx - 0x5835b2cf]
│      ╎    0x08048633      41             inc ecx
│      ╎    0x08048634      2551bbb0aa     and eax, 0xaab0bb51
│      ╎    0x08048639      a6             cmpsb byte [esi], byte es:[edi]
│      ╎    0x0804863a      a4             movsb byte es:[edi], byte [esi]
│      ╎    0x0804863b      37             aaa
│      ╎    0x0804863c      5f             pop edi
│      ╎    0x0804863d      0b626b         or esp, dword [edx + 0x6b]
│      ╎    0x08048640      036f07         add ebp, dword [edi + 7]
│      └──< 0x08048643      e1a3           loope 0x80485e8
│           0x08048645      f0             invalid
..
```

Looking at `_DT_INIT` in Ghidra, we see:
```c
void _DT_INIT(void)

{
  FUN_08048484();
  FUN_080484e4();
  FUN_080487b4();
  return;
}
```

In particular, the function `FUN_080487b4()` looks suspicious:

```c
undefined4 __regparm3 FUN_080487b4(undefined4 param_1,undefined4 param_2)
{
  code *pcVar1;
  undefined **ppuVar2;

  ppuVar2 = &PTR_FUN_08049888;
  pcVar1 = (code *)PTR_FUN_08049888;
  if (PTR_FUN_08049888 != (undefined *)0xffffffff) {
    do {
      ppuVar2 = ppuVar2 + -1;
      (*pcVar1)();
      pcVar1 = (code *)*ppuVar2;
    } while (pcVar1 != (code *)0xffffffff);
  }
  return param_2;
}
```

In Ghidra, we see this data layout:
```
                         //
                         // segment_3 
                         // Loadable segment  [0x8049880 - 0x80499af] (zero-ex
                         // ram:08049880-ram:080499af
                         //
                         DAT_08049880                              XREF[6]:   0804809c(*), 
                                                                               FUN_08048700:0804871b(*), 
                                                                               FUN_08048700:08048721(*), 
                                                                               FUN_08048700:08048740(R), 
                                                                               FUN_08048760:08048778(*), 
                                                                               FUN_08048760:0804877e(*)  
      08049880 ff ff ff      undefin   FFFFFFFFh
                         PTR_FUN_08049884                          XREF[2]:   FUN_08048700:08048740(R), 
                                                                               pre_entry:080487cd(R)  
      08049884 10 85 04      addr      FUN_08048510
                         PTR_FUN_08049888                          XREF[2]:   pre_entry:080487b9(R), 
                                                                               pre_entry:080487c1(*)  
      08049888 8b 85 04      addr      FUN_0804858b
      0804988c 00            ??        00h
      0804988d 00            ??        00h
      0804988e 00            ??        00h
      0804988f 00            ??        00h
      08049890 ff            ??        FFh
      08049891 ff            ??        FFh
      08049892 ff            ??        FFh
      08049893 ff            ??        FFh
```

The pointer `PTR_FUN_08049888` storages the function `FUN_0804858b`. We see that first `pcVar1 = FUN_0804858b`.

The following code basically executes the two functions in sequence: `FUN_0804858b`, `FUN_08048510`. It stops at `DAT_08049880` because it points to the data `FFFFFFFFh`.
```c
ppuVar2 = &PTR_FUN_08049888;
pcVar1 = (code *)PTR_FUN_08049888;
if (PTR_FUN_08049888 != (undefined *)0xffffffff) {
  do {
    ppuVar2 = ppuVar2 + -1;
    (*pcVar1)();
    pcVar1 = (code *)*ppuVar2;
  } while (pcVar1 != (code *)0xffffffff);
}
```

The first function `FUN_0804858b` is renamed to `mprotect_and_xor_on_main()` and is reversed as follows:
```c
void mprotect_and_xor_on_main(void)
{
  int mprotect_ret;
  byte *main_i;
  int count;

  main_i = &main;   // main = 0x804860d
  mprotect_ret = mprotect(&Elf32_Ehdr_08048000,0xf2,7);
  if (mprotect_ret != 0) {
    exit(1);
  }
  count = 1;
  for (; main_i < &DAT_080486ff; main_i = main_i + 1) { // 0x80486ff is supposedly the final of the main() function
    if (100 < count) {
      count = 1;
    }
    *main_i = (byte)count ^ *main_i;
    count = count + 2;
  }
  return;
}
```

The first function:
* Sets the memory region `0x8048000 -- 0x80480f2` to have read, write and execute permissions.
* Applies XOR operations to the `main` function region `0x804860d -- 0x80486fe`, effectively **decrypting it**.

The second funtion `FUN_08048510` is renamed to `checksum_main()` is reversed as follows:
```c
void checksum_main(void)
{
    byte *b;
    uint i;
    uint checksum;

    checksum = 0;
    i = 0;
    for (b = &main; b < &end_of_main; b = b + 1) {
        checksum = checksum ^ *b ^ i;
        i = i + 1;
    }
    if (checksum != precalculated_main_checksum) {  // precalculated_main_checksum = 0xec
        exit(0);
    }
    return;
}
```

* It verifies if the `main()` function, after decryption, has the correct checksum. If not, the program exits.

Now we decrypt the `main()` function with the following program:
```c
#include <stdio.h>

char main_bytes[] = {
0x54, 0x8a, 0xe0, 0x84, 0xe5, 0x23, 0x8e, 0xeb, 0xe1, 0xab, 0x15, 0x17, 0x19, 0x1b, 0x34, 0xdb, 0xa2, 0xcf, 0x29, 0x4f, 0x09, 0xa3, 0x29, 0x27,
0xd9, 0x95, 0xc8, 0xc8, 0xc6, 0xb8, 0xf9, 0x2f, 0xc4, 0x83, 0x31, 0x4d, 0xca, 0xa7, 0x41, 0x25, 0x51, 0xbb, 0xb0, 0xaa, 0xa6, 0xa4, 0x37, 0x5f,
0x0b, 0x62, 0x6b, 0x03, 0x6f, 0x07, 0xe1, 0xa3, 0xf0, 0xf0, 0xee, 0x90, 0xd1, 0x07, 0x9c, 0xdb, 0x69, 0x15, 0xa2, 0xcf, 0x29, 0x4d, 0x29, 0xc3,
0xea, 0xd2, 0xce, 0xcc, 0xdd, 0x85, 0xc7, 0xc4, 0xc2, 0xbc, 0xad, 0x4f, 0x2d, 0x07, 0xc1, 0x4f, 0x45, 0xa7, 0xf4, 0xae, 0xaa, 0xa8, 0xda, 0x9f,
0x4d, 0xdc, 0x8d, 0x67, 0x6b, 0x03, 0x6f, 0x06, 0x63, 0x09, 0xe5, 0xdb, 0xec, 0xec, 0xea, 0x94, 0xdd, 0x0b, 0x94, 0x5a, 0xd5, 0xa0, 0x58, 0xd3,
0xd6, 0x5e, 0x27, 0xac, 0xdd, 0x3f, 0x5f, 0x36, 0xd1, 0xb5, 0xc0, 0xc0, 0xbe, 0xab, 0x3c, 0xb9, 0xb6, 0xb4, 0x2b, 0x88, 0x14, 0x8b, 0x57, 0x57,
0xda, 0xb7, 0x51, 0x37, 0x76, 0x77, 0x01, 0x03, 0xed, 0x81, 0xf4, 0xf4, 0xf2, 0x8c, 0xd5, 0x03, 0x73, 0x9e, 0x5c, 0xc1, 0xda, 0x5a, 0xfd, 0x5c,
0x25, 0x27, 0x28, 0xa8, 0xc1, 0x2b, 0x5b, 0x23, 0xb8, 0x72, 0xe1, 0x6b, 0xc2, 0x4a, 0xb5, 0xab, 0x32, 0xba, 0xb6, 0xb4, 0xce, 0x8b, 0x41, 0xd6,
0x95, 0x22, 0x49, 0xd8, 0xb1, 0x53, 0x09, 0x11, 0x89, 0x07, 0x0d, 0xef, 0x3a, 0xf6, 0xf2, 0xf0, 0x92, 0xd7, 0x05, 0xff, 0x32, 0xe5, 0xe2, 0xe0,
0xa2, 0xcf, 0x29, 0xd8, 0x5c, 0xdf, 0xc5, 0xff, 0xcd, 0xcc, 0xca, 0xb4, 0xfd, 0x2b, 0x85, 0x3f, 0x41, 0x43, 0x45, 0x8e, 0x8a, 0xdb, 0xdd, 0xdf,
0xc1, 0xc3,
0x00, // extra null byte, just to mark the end
};

int main() {
    int count = 1;
    for (int i = 0; main_bytes[i] != '\0'; i++) {
        if (count > 100) {
            count = 1;
        }
        putchar(count ^ main_bytes[i]);
        count = count + 2;
    }
}
```

We get from `radare2` the following disassembly:

```
[sekai@void crackmes.one]$ gcc decrypt_mbtu.c && ./a.out > decrypted_mbtu_main.rawbin
[sekai@void crackmes.one]$ r2 decrypted_mbtu_main.rawbin
[0x00000000]> pd
            0x00000000      55             push rbp
            0x00000001      89e5           mov ebp, esp
            0x00000003      83ec28         sub esp, 0x28
            0x00000006      83e4f0         and esp, 0xfffffff0
            0x00000009      b800000000     mov eax, 0
            0x0000000e      29c4           sub esp, eax
            0x00000010      83ec0c         sub esp, 0xc
            0x00000013      6820880408     push 0x8048820
            0x00000018      e8a6fdffff     call 0xfffffffffffffdc3
            0x0000001d      83c410         add esp, 0x10
            0x00000020      85c0           test eax, eax
        ┌─< 0x00000022      740a           je 0x2e
        │   0x00000024      83ec0c         sub esp, 0xc
        │   0x00000027      6a00           push 0
        │   0x00000029      e8e5fdffff     call 0xfffffffffffffe13
        └─> 0x0000002e      6a00           push 0
            0x00000030      6a01           push 1
            0x00000032      6a00           push 0
            0x00000034      6a00           push 0
            0x00000036      e8a8fdffff     call 0xfffffffffffffde3
            0x0000003b      83c410         add esp, 0x10
            0x0000003e      85c0           test eax, eax
        ┌─< 0x00000040      740a           je 0x4c
        │   0x00000042      83ec0c         sub esp, 0xc
        │   0x00000045      6a00           push 0
        │   0x00000047      e8c7fdffff     call 0xfffffffffffffe13
        └─> 0x0000004c      e8b2feffff     call 0xffffffffffffff03
            0x00000051      83ec0c         sub esp, 0xc
            0x00000054      6840880408     push 0x8048840
            0x00000059      e8a5fdffff     call 0xfffffffffffffe03
            0x0000005e      83c410         add esp, 0x10
            0x00000061      83ec04         sub esp, 4
            0x00000064      6a00           push 0
            0x00000066      6a01           push 1
            0x00000068      6a02           push 2
            0x0000006a      e8d4fdffff     call 0xfffffffffffffe43
            0x0000006f      83c410         add esp, 0x10
            0x00000072      8945f4         mov dword [rbp - 0xc], eax
            0x00000075      837df4ff       cmp dword [rbp - 0xc], 0xffffffff
        ┌─< 0x00000079      750a           jne 0x85
        │   0x0000007b      83ec0c         sub esp, 0xc
        │   0x0000007e      6a01           push 1
        │   0x00000080      e88efdffff     call 0xfffffffffffffe13
        └─> 0x00000085      e879feffff     call 0xffffffffffffff03
            0x0000008a      66c745d80200   mov word [rbp - 0x28], 2
            0x00000090      83ec0c         sub esp, 0xc
            0x00000093      6817140000     push 0x1417                 ; '\x17\x14'
            0x00000098      e886fdffff     call 0xfffffffffffffe23
            0x0000009d      83c410         add esp, 0x10
            0x000000a0      668945da       mov word [rbp - 0x26], ax
            0x000000a4      c745dc7f00..   mov dword [rbp - 0x24], 0x100007f ; '\x7f'
            0x000000ab      83ec04         sub esp, 4
            0x000000ae      6a10           push 0x10
            0x000000b0      8d45d8         lea eax, [rbp - 0x28]
            0x000000b3      50             push rax
            0x000000b4      ff75f4         push qword [rbp - 0xc]
            0x000000b7      e877fdffff     call 0xfffffffffffffe33
            0x000000bc      83c410         add esp, 0x10
            0x000000bf      85c0           test eax, eax
        ┌─< 0x000000c1      7510           jne 0xd3
        │   0x000000c3      83ec0c         sub esp, 0xc
        │   0x000000c6      6872880408     push 0x8048872
        │   0x000000cb      e833fdffff     call 0xfffffffffffffe03
        │   0x000000d0      83c410         add esp, 0x10
        └─> 0x000000d3      e82bfeffff     call 0xffffffffffffff03
            0x000000d8      83ec0c         sub esp, 0xc
            0x000000db      ff75f4         push qword [rbp - 0xc]
            0x000000de      e8d0fcffff     call 0xfffffffffffffdb3
            0x000000e3      83c410         add esp, 0x10
            0x000000e6      b800000000     mov eax, 0
            0x000000eb      c9             leave
            0x000000ec      c3             ret
```

The instructions like `call 0xfffffffffffffdc3` are because of *position independent code*. The offset of `main` in the original program is `0x804860d`.
I asked ChatGPT to write a `python` script to get the real addresses of the function calls.

```py
import re

# Base virtual address where code is loaded
base_address = 0x804860d

# Raw decrypted disassembly
disassembly = """
<PASTE-ASSEMBLY-HERE>
"""

# Regular expression to match call instructions and extract offset
call_pattern = re.compile(r'0x([0-9a-f]+)\s+e8([0-9a-f]{8})\s+call\s+0x([0-9a-f]+)', re.IGNORECASE)

def resolve_call_targets(disassembly, base_address):
    results = []

    for match in call_pattern.finditer(disassembly):
        offset_str, rel_bytes, target_str = match.groups()
        offset = int(offset_str, 16)
        rel = int.from_bytes(bytes.fromhex(rel_bytes), byteorder='little', signed=True)

        instruction_addr = base_address + offset
        next_instr_addr = instruction_addr + 5
        resolved_addr = next_instr_addr + rel

        results.append({
            "instruction_offset": offset,
            "relative_offset": rel,
            "resolved_address": resolved_addr
        })

    return results

# Run the resolver
resolved_calls = resolve_call_targets(disassembly, base_address)

# Print results
for call in resolved_calls:
    print(f"0x{call['instruction_offset']:08x} -> call 0x{call['resolved_address']:08x}")
```

The output is
```
$ python3 mbtu_resolve_calls_addr.py
0x00000018 -> call 0x080483d0
0x00000029 -> call 0x08048420
0x00000036 -> call 0x080483f0
0x00000047 -> call 0x08048420
0x0000004c -> call 0x08048510
0x00000059 -> call 0x08048410
0x0000006a -> call 0x08048450
0x00000080 -> call 0x08048420
0x00000085 -> call 0x08048510
0x00000098 -> call 0x08048430
0x000000b7 -> call 0x08048440
0x000000cb -> call 0x08048410
0x000000d3 -> call 0x08048510
0x000000de -> call 0x080483c0
```

Each of these addresses resolute to some `libc` function, which we can see in Ghidra:
```
0x00000018 -> call 0x080483d0 -> getenv
0x00000029 -> call 0x08048420 -> exit
0x00000036 -> call 0x080483f0 -> ptrace
0x00000047 -> call 0x08048420 -> exit
0x0000004c -> call 0x08048510 -> checksum_main
0x00000059 -> call 0x08048410 -> printf
0x0000006a -> call 0x08048450 -> socket
0x00000080 -> call 0x08048420 -> exit
0x00000085 -> call 0x08048510 -> checksum_main
0x00000098 -> call 0x08048430 -> htons
0x000000b7 -> call 0x08048440 -> connect
0x000000cb -> call 0x08048410 -> printf
0x000000d3 -> call 0x08048510 -> checksum_main
0x000000de -> call 0x080483c0 -> close
```

From this we can reverse the disassembly.
```asm
            0x00000000      55             push rbp
            0x00000001      89e5           mov ebp, esp
            0x00000003      83ec28         sub esp, 0x28
            0x00000006      83e4f0         and esp, 0xfffffff0
            0x00000009      b800000000     mov eax, 0
            0x0000000e      29c4           sub esp, eax
            0x00000010      83ec0c         sub esp, 0xc
            0x00000013      6820880408     push 0x8048820   ; LD_PRELOAD
            0x00000018      e8a6fdffff     call getenv      ; char *getenv(const char *name);
            0x0000001d      83c410         add esp, 0x10
            0x00000020      85c0           test eax, eax
        ┌─< 0x00000022      740a           je 0x2e
        │   0x00000024      83ec0c         sub esp, 0xc
        │   0x00000027      6a00           push 0
        │   0x00000029      e8e5fdffff     call exit        ; [[noreturn]] void exit(int status);
        └─> 0x0000002e      6a00           push 0
            0x00000030      6a01           push 1
            0x00000032      6a00           push 0
            0x00000034      6a00           push 0
            0x00000036      e8a8fdffff     call ptrace      ; long ptrace(enum __ptrace_request op, pid_t pid, void *addr, void *data);
            0x0000003b      83c410         add esp, 0x10
            0x0000003e      85c0           test eax, eax
        ┌─< 0x00000040      740a           je 0x4c
        │   0x00000042      83ec0c         sub esp, 0xc
        │   0x00000045      6a00           push 0
        │   0x00000047      e8c7fdffff     call exit        ; [[noreturn]] void exit(int status);
        └─> 0x0000004c      e8b2feffff     call checksum_main   ; void checksum_main(void)
            0x00000051      83ec0c         sub esp, 0xc
            0x00000054      6840880408     push 0x8048840
            0x00000059      e8a5fdffff     call printf  ; int printf(const char *restrict format, ...);
            0x0000005e      83c410         add esp, 0x10
            0x00000061      83ec04         sub esp, 4
            0x00000064      6a00           push 0
            0x00000066      6a01           push 1
            0x00000068      6a02           push 2
            0x0000006a      e8d4fdffff     call socket  ; int socket(int domain, int type, int protocol);
            0x0000006f      83c410         add esp, 0x10
            0x00000072      8945f4         mov dword [rbp - 0xc], eax
            0x00000075      837df4ff       cmp dword [rbp - 0xc], 0xffffffff
        ┌─< 0x00000079      750a           jne 0x85
        │   0x0000007b      83ec0c         sub esp, 0xc
        │   0x0000007e      6a01           push 1
        │   0x00000080      e88efdffff     call exit        ; [[noreturn]] void exit(int status);
        └─> 0x00000085      e879feffff     call checksum_main   ; void checksum_main(void);
            0x0000008a      66c745d80200   mov word [rbp - 0x28], 2
            0x00000090      83ec0c         sub esp, 0xc
            0x00000093      6817140000     push 0x1417                 ; '\x17\x14'
            0x00000098      e886fdffff     call htons   ; uint16_t htons(uint16_t hostshort);
            0x0000009d      83c410         add esp, 0x10
            0x000000a0      668945da       mov word [rbp - 0x26], ax
            0x000000a4      c745dc7f00..   mov dword [rbp - 0x24], 0x100007f ; '\x7f'
            0x000000ab      83ec04         sub esp, 4
            0x000000ae      6a10           push 0x10
            0x000000b0      8d45d8         lea eax, [rbp - 0x28]
            0x000000b3      50             push rax
            0x000000b4      ff75f4         push qword [rbp - 0xc]
            0x000000b7      e877fdffff     call connect ; int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
            0x000000bc      83c410         add esp, 0x10
            0x000000bf      85c0           test eax, eax
        ┌─< 0x000000c1      7510           jne 0xd3
        │   0x000000c3      83ec0c         sub esp, 0xc
        │   0x000000c6      6872880408     push 0x8048872
        │   0x000000cb      e833fdffff     call printf  ; int printf(const char *restrict format, ...);
        │   0x000000d0      83c410         add esp, 0x10
        └─> 0x000000d3      e82bfeffff     call checksum_main   ; void checksum_main(void);
            0x000000d8      83ec0c         sub esp, 0xc
            0x000000db      ff75f4         push qword [rbp - 0xc]
            0x000000de      e8d0fcffff     call close   ; int close(int fd);
            0x000000e3      83c410         add esp, 0x10
            0x000000e6      b800000000     mov eax, 0
            0x000000eb      c9             leave
            0x000000ec      c3             ret
```

Manually reversing this:
```c
#include <stdio.h>
#include <stdlib.h>
#include <sys/ptrace.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>

/* this function does not matter */
void checksum_main() {
    ;
}

int main() {
    if (getenv("LD_PRELOAD")) {   // at address 0x8048820
        exit(0);
    }

    if (ptrace(PTRACE_TRACEME, 0, 1, 0)) {  // PTRACE_TRACEME = 0
        // exits if the program is being traced
        exit(0);
    }

    checksum_main();

    printf("..:: MoreBoredThanYou by niel anthony acuna ::..\n");

 /* socket(2, 1, 0); */
    int fd = socket(PF_INET, SOCK_STREAM, 0);   // ebp-0xc returns file descriptor on success, -1 if error
    // The protocol specifies a particular protocol to be used with the
    // socket.  Normally only a single protocol exists to support a particular
    // socket type within a given protocol family, in which case protocol can
    // be specified as 0.
    if (fd == -1) {
        exit(1);
    }

    checksum_main();

    // connect to port 127.0.0.1:5143
    struct sockaddr_in addr = {    // rbp-0x28
        .sin_family = AF_INET,
        .sin_port = htons(0x1417),
        .sin_addr.s_addr = htonl(0x7f000001)  // 127.0.0.1
    };
    // int var_28h = 2;    // AF_INET = PF_INET = 2
    // uint16_t var_26h = htons(0x1417);   // port 5143
    // int var_24h = 0x100007f;    // 0x7f.0x00.0x00.0x01 = 127.0.0.1 = localhost
    // Explanation: man 7 ip, man sockaddr
    // struct sockaddr_in {
    //     sa_family_t    sin_family; /* address family: AF_INET */
    //     in_port_t      sin_port;   /* port in network byte order */
    //     struct in_addr sin_addr;   /* internet address */
    // };

    if (connect(fd, (struct sockaddr *) &addr, 0x10) == 0) {
        /* enter here if the connection to port 5143 on localhost in successful */
        printf("cracked!\n");
    }

    checksum_main();
    close(fd);
    return 0;
}
```

Therefore, in order to crack, we just need that the program successfully connects to port `5143` on `localhost`:
```
[sekai@void crackmes.one]$ nc -lvnp 5143
Ncat: Version 7.95 ( https://nmap.org/ncat )
Ncat: Listening on [::]:5143
Ncat: Listening on 0.0.0.0:5143
^Z
[1]+  Stopped                 nc -lvnp 5143
[sekai@void crackmes.one]$ ./mbtu_patched
..:: MoreBoredThanYou by niel anthony acuna ::..
cracked!
[sekai@void crackmes.one]$ fg
nc -lvnp 5143
Ncat: Connection from 127.0.0.1:33628.
```

