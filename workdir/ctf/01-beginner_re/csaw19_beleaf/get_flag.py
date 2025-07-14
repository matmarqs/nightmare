# The asterik '*' mean 0xFFFFFFFF, 32 bit int equals -1

base_array = ['w', 'f', '{', '_', 'n', 'y', '}', '*', 'b', 'l', 'r', '*', '*', '*', '*', '*', '*', 'a', 'e', 'i', '*', 'o', 't',
              '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', 'g', '*', '*', '*', '*', '*', '*', 'u',]

# to output 0, c needs to be 'w'
# to output 1, c needs to be 'f'
# to output 2, c needs to be '{'
# and so on...

compare_array = [0x01, 0x09, 0x11, 0x27, 0x02, 0x00, 0x12, 0x03, 0x08, 0x12, 0x09, 0x12, 0x11, 0x01, 0x03, 0x13,
                 0x04, 0x03, 0x05, 0x15, 0x2E, 0x0A, 0x03, 0x0A, 0x12, 0x03, 0x01, 0x2E, 0x16, 0x2E, 0x0A, 0x12, 0x06,]

print(len(base_array))
print(len(compare_array))

flag = ''

for i in compare_array:
    flag += base_array[i]

print(flag)


# Ghidra disassembly output (each line is one byte):
### compare_array                   ### base_array
# 1h                                # 77h    w
# 00h                               # 00h
# 00h                               # 00h
# 00h                               # 00h
# 00h                               # 66h    f
# 00h                               # 00h
# 00h                               # 00h
# 00h                               # 00h
# 09h                               # 7Bh    {
# 00h                               # 00h
# 00h                               # 00h
# 00h                               # 00h
# 00h                               # 5Fh    _
# 00h                               # 00h
# 00h                               # 00h
# 00h                               # 00h
# 11h                               # 6Eh    n
# 00h                               # 00h
# 00h                               # 00h
# 00h                               # 00h
# 00h                               # 79h    y
# 00h                               # 00h
# 00h                               # 00h
# 00h                               # 00h
# 27h    '                          # 7Dh    }
# 00h                               # 00h
# 00h                               # 00h
# 00h                               # 00h
# 00h                               # FFh    *
# 00h                               # FFh
# 00h                               # FFh
# 00h                               # FFh
# 02h                               # 62h    b
# 00h                               # 00h
# 00h                               # 00h
# 00h                               # 00h
# 00h                               # 6Ch    l
# 00h                               # 00h
# 00h                               # 00h
# 00h                               # 00h
# 00h                               # 72h    r
# 00h                               # 00h
# 00h                               # 00h
# 00h                               # 00h
# 00h                               # FFh    *
# 00h                               # FFh
# 00h                               # FFh
# 00h                               # FFh
# 12h                               # FFh    *
# 00h                               # FFh
# 00h                               # FFh
# 00h                               # FFh
# 00h                               # FFh    *
# 00h                               # FFh
# 00h                               # FFh
# 00h                               # FFh
# 03h                               # FFh    *
# 00h                               # FFh
# 00h                               # FFh
# 00h                               # FFh
# 00h                               # FFh    *
# 00h                               # FFh
# 00h                               # FFh
# 00h                               # FFh
# 08h                               # FFh    *
# 00h                               # FFh
# 00h                               # FFh
# 00h                               # FFh
# 00h                               # 61h    a
# 00h                               # 00h
# 00h                               # 00h
# 00h                               # 00h
# 12h                               # 65h    e
# 00h                               # 00h
# 00h                               # 00h
# 00h                               # 00h
# 00h                               # 69h    i
# 00h                               # 00h
# 00h                               # 00h
# 00h                               # 00h
# 09h                               # FFh    *
# 00h                               # FFh
# 00h                               # FFh
# 00h                               # FFh
# 00h                               # 6Fh    o
# 00h                               # 00h
# 00h                               # 00h
# 00h                               # 00h
# 12h                               # 74h    t
# 00h                               # 00h
# 00h                               # 00h
# 00h                               # 00h
# 00h                               # FFh    *
# 00h                               # FFh
# 00h                               # FFh
# 00h                               # FFh
# 11h                               # FFh    *
# 00h                               # FFh
# 00h                               # FFh
# 00h                               # FFh
# 00h                               # FFh    *
# 00h                               # FFh
# 00h                               # FFh
# 00h                               # FFh
# 01h                               # FFh    *
# 00h                               # FFh
# 00h                               # FFh
# 00h                               # FFh
# 00h                               # FFh    *
# 00h                               # FFh
# 00h                               # FFh
# 00h                               # FFh
# 03h                               # FFh    *
# 00h                               # FFh
# 00h                               # FFh
# 00h                               # FFh
# 00h                               # FFh    *
# 00h                               # FFh
# 00h                               # FFh
# 00h                               # FFh
# 13h                               # FFh    *
# 00h                               # FFh
# 00h                               # FFh
# 00h                               # FFh
# 00h                               # FFh    *
# 00h                               # FFh
# 00h                               # FFh
# 00h                               # FFh
# 04h                               # FFh    *
# 00h                               # FFh
# 00h                               # FFh
# 00h                               # FFh
# 00h                               # FFh    *
# 00h                               # FFh
# 00h                               # FFh
# 00h                               # FFh
# 03h                               # FFh    *
# 00h                               # FFh
# 00h                               # FFh
# 00h                               # FFh
# 00h                               # FFh    *
# 00h                               # FFh
# 00h                               # FFh
# 00h                               # FFh
# 05h                               # FFh    *
# 00h                               # FFh
# 00h                               # FFh
# 00h                               # FFh
# 00h                               # FFh    *
# 00h                               # FFh
# 00h                               # FFh
# 00h                               # FFh
# 15h                               # FFh    *
# 00h                               # FFh
# 00h                               # FFh
# 00h                               # FFh
# 00h                               # 67h    g
# 00h                               # 00h
# 00h                               # 00h
# 00h                               # 00h
# 2Eh    .                          # FFh    *
# 00h                               # FFh
# 00h                               # FFh
# 00h                               # FFh
# 00h                               # FFh    *
# 00h                               # FFh
# 00h                               # FFh
# 00h                               # FFh
# 0Ah                               # FFh    *
# 00h                               # FFh
# 00h                               # FFh
# 00h                               # FFh
# 00h                               # FFh    *
# 00h                               # FFh
# 00h                               # FFh
# 00h                               # FFh
# 03h                               # FFh    *
# 00h                               # FFh
# 00h                               # FFh
# 00h                               # FFh
# 00h                               # FFh    *
# 00h                               # FFh
# 00h                               # FFh
# 00h                               # FFh
# 0Ah                               # 75h    u
# 00h                               # 00h
# 00h                               # 00h
# 00h                               # 00h
# 00h
# 00h
# 00h
# 00h
# 12h
# 00h
# 00h
# 00h
# 00h
# 00h
# 00h
# 00h
# 03h
# 00h
# 00h
# 00h
# 00h
# 00h
# 00h
# 00h
# 01h
# 00h
# 00h
# 00h
# 00h
# 00h
# 00h
# 00h
# 2Eh    .
# 00h
# 00h
# 00h
# 00h
# 00h
# 00h
# 00h
# 16h
# 00h
# 00h
# 00h
# 00h
# 00h
# 00h
# 00h
# 2Eh    .
# 00h
# 00h
# 00h
# 00h
# 00h
# 00h
# 00h
# 0Ah
# 00h
# 00h
# 00h
# 00h
# 00h
# 00h
# 00h
# 12h
# 00h
# 00h
# 00h
# 00h
# 00h
# 00h
# 00h
# 06h
# 00h
# 00h
# 00h
# 00h
# 00h
# 00h
# 00h
