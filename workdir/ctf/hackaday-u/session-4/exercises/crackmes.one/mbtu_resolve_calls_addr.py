import re

# Base virtual address where code is loaded
base_address = 0x804860d

# Raw decrypted disassembly
disassembly = """
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
0x00000022      740a           je 0x2e
0x00000024      83ec0c         sub esp, 0xc
0x00000027      6a00           push 0
0x00000029      e8e5fdffff     call 0xfffffffffffffe13
0x0000002e      6a00           push 0
0x00000030      6a01           push 1
0x00000032      6a00           push 0
0x00000034      6a00           push 0
0x00000036      e8a8fdffff     call 0xfffffffffffffde3
0x0000003b      83c410         add esp, 0x10
0x0000003e      85c0           test eax, eax
0x00000040      740a           je 0x4c
0x00000042      83ec0c         sub esp, 0xc
0x00000045      6a00           push 0
0x00000047      e8c7fdffff     call 0xfffffffffffffe13
0x0000004c      e8b2feffff     call 0xffffffffffffff03
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
0x00000079      750a           jne 0x85
0x0000007b      83ec0c         sub esp, 0xc
0x0000007e      6a01           push 1
0x00000080      e88efdffff     call 0xfffffffffffffe13
0x00000085      e879feffff     call 0xffffffffffffff03
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
0x000000c1      7510           jne 0xd3
0x000000c3      83ec0c         sub esp, 0xc
0x000000c6      6872880408     push 0x8048872
0x000000cb      e833fdffff     call 0xfffffffffffffe03
0x000000d0      83c410         add esp, 0x10
0x000000d3      e82bfeffff     call 0xffffffffffffff03
0x000000d8      83ec0c         sub esp, 0xc
0x000000db      ff75f4         push qword [rbp - 0xc]
0x000000de      e8d0fcffff     call 0xfffffffffffffdb3
0x000000e3      83c410         add esp, 0x10
0x000000e6      b800000000     mov eax, 0
0x000000eb      c9             leave
0x000000ec      c3             ret
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
