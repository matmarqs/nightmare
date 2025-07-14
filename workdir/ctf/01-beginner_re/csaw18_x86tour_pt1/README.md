### **Bootloader Assembly and Execution Guide (Void Linux)**

#### **1. Install Dependencies**
Ensure you have `nasm` (assembler) and `qemu` (emulator) installed:
```bash
sudo xbps-install nasm qemu
```

---

#### **2. Assemble the Bootloader**
Compile `stage-1.asm` into a raw binary (`boot.bin`). Define `NUM_SECTORS=1` (`stage-2.bin` is 1 sector, 512 bytes):
```bash
nasm -f bin -o boot.bin -D NUM_SECTORS=1 stage-1.asm
```

---

#### **3. Create a Bootable Disk Image**
Create a blank disk image (1.44MB floppy size) and write the bootloader to the first sector:
```bash
# Create empty image (2880 sectors = 1.44MB)
dd if=/dev/zero of=disk.img bs=512 count=2880

# Write bootloader to sector 0
dd if=boot.bin of=disk.img conv=notrunc
```

---

#### **4. Add the Second Stage (`stage-2.bin`)**
Write `stage-2.bin` starting at **sector 1** (immediately after the bootloader):
```bash
dd if=stage-2.bin of=disk.img bs=512 seek=1 conv=notrunc
```

---

#### **5. Run in QEMU**
Test the bootloader with the second stage:
```bash
qemu-system-x86_64 -fda disk.img
```
- **Expected Behavior**:
  1. Bootloader prints initial messages (e.g., "tacOS by Elyk").
  2. Loads `stage-2.bin` from disk and jumps to it.

---

#### **6. Debugging (Optional)**
- **View Disk Contents**:
  ```bash
  hexdump -C disk.img | less
  ```
- **Log QEMU Output**:
  ```bash
  qemu-system-x86_64 -fda disk.img -d int,cpu_reset -D qemu.log
  ```

---

#### **Key Notes**
- **Boot Signature**: The bootloader must end with `0xAA55` (handled by the assembly code).
- **Sector Alignment**: Ensure `stage-2.bin` is padded to a multiple of 512 bytes.
- **Real Mode**: The code runs in 16-bit real mode (BIOS interrupts like `int 0x10` work).

---

### **Final Directory Structure**
```
.
├── stage-1.asm        # Bootloader source
├── boot.bin          # Compiled bootloader
├── stage-2.bin       # Second stage binary
├── disk.img          # Bootable disk image
└── README.md         # This guide
```
