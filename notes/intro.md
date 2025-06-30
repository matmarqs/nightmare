# Assembly

## Registers

* `rbp`: Base Pointer, points to the bottom of the current stack frame
* `rsp`: Stack Pointer, points to the top of the current stack frame
* `rip`: Instruction Pointer, points to the instruction to be executed

* General Purpose Registers. These can be used for a variety of different things:
`rax`, `rbx`, `rcx`, `rdx`, `rsi`, `rdi`, `r8`, `r9`, `r10`, `r11`, `r12`, `r13`, `r14`, `r15`.

In **x64**, Linux arguments are passed via registers. The first few args are passed by the registers:

| Register | Argument
| -------- | --------
| rdi      | First Argument
| rsi      | Second Argument
| rdx      | Third Argument
| rcx      | Fourth Argument
| r8       | Fifth Argument
| r9       | Sixth Argument

With the **x86** elf architecture, arguments are passed on the stack.

When a C function return a value, this value is passed in the `rax` register on **x64** and `eax` on **x86**.

### Register sizes

| 8 Byte Register | Lower 4 Bytes | Lower 2 Bytes | Lower Byte |
|-----------------|---------------|---------------|------------|
|   rbp           |     ebp       |     bp        |     bpl    |
|   rsp           |     esp       |     sp        |     spl    |
|   rip           |     eip       |               |            |
|   rax           |     eax       |     ax        |     al     |
|   rbx           |     ebx       |     bx        |     bl     |
|   rcx           |     ecx       |     cx        |     cl     |
|   rdx           |     edx       |     dx        |     dl     |
|   rsi           |     esi       |     si        |     sil    |
|   rdi           |     edi       |     di        |     dil    |
|   r8            |     r8d       |     r8w       |     r8b    |
|   r9            |     r9d       |     r9w       |     r9b    |
|   r10           |     r10d      |     r10w      |     r10b   |
|   r11           |     r11d      |     r11w      |     r11b   |
|   r12           |     r12d      |     r12w      |     r12b   |
|   r13           |     r13d      |     r13w      |     r13b   |
|   r14           |     r14d      |     r14w      |     r14b   |
|   r15           |     r15d      |     r15w      |     r15b   |

### Words

A *word* is just 2 bytes of data

### Stacks

In this code the variable x is stored in the stack:

```c
#include <stdio.h>

void main(void)
{
    int x = 5;
    puts("hi");
}
```

Which generates this assembly code:
```asm
0000000000001135 <main>:
    1135:       55                      push   rbp
    1136:       48 89 e5                mov    rbp,rsp
    1139:       48 8d 3d c4 0e 00 00    lea    rdi,[rip+0xec4]        # 2004 <_IO_stdin_used+0x4>
    1140:       e8 eb fe ff ff          call   1030 <puts@plt>
    1145:       90                      nop
    1146:       5d                      pop    rbp
    1147:       c3                      ret    
    1148:       0f 1f 84 00 00 00 00    nop    DWORD PTR [rax+rax*1+0x0]
    114f:       00
```

Values on the stack are moved on by either pushing them onto the stack, or popping them off. That is the only way to add or remove values from the stack (it is a LIFO data structure). However we can reference values on the stack.

The exact bounds of the stack is recorded by two registers, `rbp` and `rsp`. The base pointer `rbp` points to the bottom of the stack. The stack pointer `rsp` points to the top of the stack.


### Flags

There is one register that contains flags. A flag is a particular bit of this register. If it is set or not, will typically mean something. Here is the list of flags.

```
00:     Carry Flag
01:     always 1
02:     Parity Flag
03:     always 0
04:     Adjust Flag
05:     always 0
06:     Zero Flag
07:     Sign Flag
08:     Trap Flag
09:     Interruption Flag     
10:     Direction Flag
11:     Overflow Flag
12:     I/O Privilege Field lower bit
13:     I/O Privilege Field higher bit
14:     Nested Task Flag
15:     Resume Flag
```

### Instructions

#### mov

```bash
mov rax, rdx
```

This will just move the data from the rdx register to the rax register.

#### dereference

Dereferencing a pointer means to treat a pointer like the value it points to.

```asm
mov rax, [rdx]
```

Will move the value pointed to by `rdx` into the `rax` register. On the flipside:

```asm
mov [rax], rdx
```

Will move the value of the `rdx` register into whatever memory is pointed to by the `rax` register. The actual value of the `rax` register does not change.

#### lea

The lea instruction calculates the address of the second operand, and moves that address in the first. For instance:

```asm
lea rdi, [rbx+0x10]
```

This will move the address `rbx+0x10` into the `rdi` register.

#### add

```asm
add rax, rdx
```

That will set `rax` equal to `rax + rdx`.

#### sub

```asm
sub rsp, 0x10
```

This will set the rsp register equal to `rsp - 0x10`.

#### xor

```asm
xor rdx, rax
```

That will set the rdx register equal to `rdx ^ rax`.

#### push

The push instruction will grow the stack by either `8` bytes (for **x64**, `4` for **x86**), then push the contents of a register onto the new stack space. For instance:

```asm
push rax
```

This will grow the stack by `8` bytes, and the contents of the `rax` register will be on top of the stack.

#### jmp

The `jmp` instruction will jump to an instruction address. It is used to redirect code execution. For instance:

```asm
jmp 0x602010
```

#### call & ret

This is similar to the `jmp` instruction. The difference is it will push the values of `rbp` and `rip` onto the stack, then jump to whatever address it is given. This is used for calling functions. After the function is finished, a `ret` instruction is called which uses the pushed values of `rbp` and `rip` (saved base and instruction pointers) it can continue execution right where it left off

#### cmp

The cmp instruction is similar to that of the sub instruction. Except it doesn't store the result in the first argument. It checks if the result is less than zero, greater than zero, or equal to zero. Depending on the value it will set the flags accordingly.

#### jnz / jz

This jump if not zero and jump if zero `(jnz/jz)` instructions are pretty similar to the jump instruction. The difference is they will only execute the jump depending on the status of the zero flag. For `jz` it will only jump if the zero flag is set. The opposite is true for `jnz`.


## Dissassembling

```bash
objdump -D a.out -M intel | less
```

### Hello World

First let's take a look at the assembly code:

`$    objdump -D hello_world -M intel | less`

After searching through for the string `main` to find the main function, we see this:

```asm
080483fb <main>:
 80483fb:       8d 4c 24 04             lea    ecx,[esp+0x4]
 80483ff:       83 e4 f0                and    esp,0xfffffff0
 8048402:       ff 71 fc                push   DWORD PTR [ecx-0x4]
 8048405:       55                      push   ebp
 8048406:       89 e5                   mov    ebp,esp
 8048408:       51                      push   ecx
 8048409:       83 ec 04                sub    esp,0x4
 804840c:       83 ec 0c                sub    esp,0xc
 804840f:       68 b0 84 04 08          push   0x80484b0
 8048414:       e8 b7 fe ff ff          call   80482d0 <puts@plt>
 8048419:       83 c4 10                add    esp,0x10
 804841c:       b8 00 00 00 00          mov    eax,0x0
 8048421:       8b 4d fc                mov    ecx,DWORD PTR [ebp-0x4]
 8048424:       c9                      leave  
 8048425:       8d 61 fc                lea    esp,[ecx-0x4]
 8048428:       c3                      ret    
 8048429:       66 90                   xchg   ax,ax
 804842b:       66 90                   xchg   ax,ax
 804842d:       66 90                   xchg   ax,ax
 804842f:       90                      nop
```

Looking at the code, we see a function call to `puts`:

```
push   0x80484b0
call   80482d0 <puts@plt>
```

Looking through the rest of the code, we really don't see much else that is interesting for our perspective. So this code probably just prints a string. When we run the binary, we see that is correct:

```bash
./hello_world
hello world!
```


### If then

We start off by viewing the assembly code with `objdump`:

```
objdump -D if_then -M intel | less`
```


After parsing through for the `main` function, we see this.

```asm
080483fb <main>:
 80483fb:       8d 4c 24 04             lea    ecx,[esp+0x4]
 80483ff:       83 e4 f0                and    esp,0xfffffff0
 8048402:       ff 71 fc                push   DWORD PTR [ecx-0x4]
 8048405:       55                      push   ebp
 8048406:       89 e5                   mov    ebp,esp
 8048408:       51                      push   ecx
 8048409:       83 ec 14                sub    esp,0x14
 804840c:       c7 45 f4 0a 00 00 00    mov    DWORD PTR [ebp-0xc],0xa
 8048413:       83 7d f4 0a             cmp    DWORD PTR [ebp-0xc],0xa
 8048417:       75 10                   jne    8048429 <main+0x2e>
 8048419:       83 ec 0c                sub    esp,0xc
 804841c:       68 c0 84 04 08          push   0x80484c0
 8048421:       e8 aa fe ff ff          call   80482d0 <puts@plt>
 8048426:       83 c4 10                add    esp,0x10
 8048429:       b8 00 00 00 00          mov    eax,0x0
 804842e:       8b 4d fc                mov    ecx,DWORD PTR [ebp-0x4]
 8048431:       c9                      leave  
 8048432:       8d 61 fc                lea    esp,[ecx-0x4]
 8048435:       c3                      ret    
 8048436:       66 90                   xchg   ax,ax
 8048438:       66 90                   xchg   ax,ax
 804843a:       66 90                   xchg   ax,ax
 804843c:       66 90                   xchg   ax,ax
 804843e:       66 90                   xchg   ax,ax
```

We can see that it loads the value `0xa` into `ebp-0xc`:

```asm
mov    DWORD PTR [ebp-0xc],0xa
```

Immediately proceeding that, we see that it runs a `cmp` instruction on it to check if it is equal. If they are not equal it will jump to `main+0x2e`. Since it was just loaded with the value `0xa`, it should not make the jump:

```asm
cmp    DWORD PTR [ebp-0xc],0xa jne    8048429 <main+0x2e>
```

proceeding that it should make a call to puts:

```asm
sub    esp,0xc push   0x80484c0 call   80482d0 <puts@plt>
```

So after looking at this code, we see that it should make that `puts` call. When we run it, we see that is what it does:

```
./if_then
x = ten
```


### Loop

Let's take a look at the assembly code:

```bash
objdump -D loop -M intel | less
```

Quickly searching for the main function, we find it:

```asm
080483fb <main>:
 80483fb:       8d 4c 24 04             lea    ecx,[esp+0x4]
 80483ff:       83 e4 f0                and    esp,0xfffffff0
 8048402:       ff 71 fc                push   DWORD PTR [ecx-0x4]
 8048405:       55                      push   ebp
 8048406:       89 e5                   mov    ebp,esp
 8048408:       51                      push   ecx
 8048409:       83 ec 14                sub    esp,0x14
 804840c:       c7 45 f4 00 00 00 00    mov    DWORD PTR [ebp-0xc],0x0
 8048413:       eb 17                   jmp    804842c <main+0x31>
 8048415:       83 ec 08                sub    esp,0x8
 8048418:       ff 75 f4                push   DWORD PTR [ebp-0xc]
 804841b:       68 c0 84 04 08          push   0x80484c0
 8048420:       e8 ab fe ff ff          call   80482d0 <printf@plt>
 8048425:       83 c4 10                add    esp,0x10
 8048428:       83 45 f4 01             add    DWORD PTR [ebp-0xc],0x1
 804842c:       83 7d f4 13             cmp    DWORD PTR [ebp-0xc],0x13
 8048430:       7e e3                   jle    8048415 <main+0x1a>
 8048432:       b8 00 00 00 00          mov    eax,0x0
 8048437:       8b 4d fc                mov    ecx,DWORD PTR [ebp-0x4]
 804843a:       c9                      leave  
 804843b:       8d 61 fc                lea    esp,[ecx-0x4]
 804843e:       c3                      ret    
 804843f:       90                      nop
```

In this function, we can see that it will initialize a stack variable at `ebp-0xc` to `0`, then jump to `0x804842c` (`main+0x31`):

```asm
mov    DWORD PTR [ebp-0xc],0x0 jmp    804842c <main+0x31>
```


Looking at the instructions at `0x804842c` we see this:

```asm
cmp    DWORD PTR [ebp-0xc],0x13 jle    8048415 <main+0x1a>
```


We see that it compares the stack value at `ebp-0xc` against `0x13`, and if it is less than or equal then it will jump to `0x8048415` (`0x80483fb + 0x1a`). That brings us to a printf call:

```asm
sub    esp,0x8 push   DWORD PTR [ebp-0xc] push   0x80484c0 call   80482d0 <printf@plt>
```


It looks like it is printing out the contents of `ebp-0xc` in some sort of format string. After that we can see that it increments the value of `ebp-0xc`, before doing the `cmp` again:

```asm
add    DWORD PTR [ebp-0xc],0x1
```


So right, putting all of the pieces together, now we are probably looking at a for loop that will run `20` times, and print the iteration counter each time. Something that looks similar to this:

```c
int i = 0;
for (i = 0; i < 20; i ++)
{
    printf("%d ", i);
}
```

When we run the binary, we see that it is true:

```
./loop
0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19
```
