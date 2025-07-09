# Explanations of each file

## x86-ex.S

This file is just an example illustrating the stack in assembly.

## c1.s

This file generates `c1.0` and `c1`.

## c1.0

To produce `c1.0`, assemble `c1.s`:

```bash
nasm -f elf64 c1.s -o c1.0
```

## c1

To produce `c1` executable, link it:
```
ld -nostdlib -static --nmagic c1.0 -o c1
```
