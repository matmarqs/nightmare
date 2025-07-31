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
