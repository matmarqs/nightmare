# Elf Exercises

In this folder we have two ELF files, can you tell the differences between them?

1. What are the main differences between these two files?

`legolas` is an executable, `dobby` is a shared library.

2. How are these two files related?

`legolas` depends on `dobby` (`libdobby.so`).

3. Can they both be executed in the docker container? Why or why not?

`dobby` is not an executable. `legolas` can be executable by telling the linker to check `./libdobby.so`:
```bash
cp dobby libdobby.so
LD_LIBRARY_PATH=. ./legolas
```

# Explanation

### **Deep Dive: Shared Libraries, Interpreters, and `LD_*` Environment Variables**

---

## **1. What is a Shared Library?**

A **shared library** (`.so` file, "Shared Object") is a collection of reusable functions that multiple programs can use **without embedding the code directly**.  

- **Example**: `libc.so` (C standard library) provides `printf()`, `malloc()`, etc.  
- **Key Benefits**:  
  - Saves disk space (one copy of the library in memory).  
  - Easier updates (fixing the library updates all programs using it).  

### **How Programs Use Shared Libraries**

1. **At Compile Time**:  
   The linker (`ld`) records which libraries are needed (e.g., `-ldobby`).  
2. **At Runtime**:  
   The **dynamic linker** (`ld-linux.so`) loads the required `.so` files.  

#### **Example: `libdobby.so`**

- Your `dobby` file is a **shared library** (renamed to `libdobby.so`).  
- `legolas` depends on it (but can’t find it by default).  

---

## **2. What is the "Interpreter" (`/lib64/ld-linux-x86-64.so.2`)?**

The **interpreter** (dynamic linker) is a small program that:  

1. **Loads** shared libraries into memory.  
2. **Resolves** function calls (e.g., `printf` → `libc.so`).  
3. **Runs** the main program.  

### **How It Works**

- When you run `./legolas`, the OS first runs the **interpreter** (not `legolas` directly).  
- The interpreter then:  
  1. Reads `legolas`’s list of required libraries (e.g., `libdobby.so`).  
  2. Searches for them in standard paths (`/lib`, `/usr/lib`, etc.).  
  3. Fails if the library is missing (`error while loading shared libraries`).  

#### **Why Your Error Happened**

```sh
./legolas: error while loading shared libraries: libdobby.so: cannot open shared object file
```

- The interpreter (`ld-linux.so`) couldn’t find `libdobby.so` in any standard path.  

---

## **3. What is `LD`? (Linux Dynamic Linker)**
`LD_*` are environment variables that control the **dynamic linker** (`ld-linux.so`).  

### **Key `LD_*` Variables**
| Variable               | Purpose                                                                 |
|------------------------|-------------------------------------------------------------------------|
| `LD_LIBRARY_PATH`      | Adds custom paths to search for `.so` files.                            |
| `LD_PRELOAD`           | Forces loading a specific `.so` before others (for hacking/debugging).  |
| `LD_DEBUG`             | Debugs library loading (e.g., `LD_DEBUG=libs ./legolas`).               |

---

## **4. Fixing Your Issue**
### **Problem**
`legolas` needs `libdobby.so`, but the linker doesn’t check the current directory.  

### **Solutions**
#### **Option 1: Temporary Fix (`LD_LIBRARY_PATH`)**
```sh
LD_LIBRARY_PATH=. ./legolas
```
- **Meaning**: "Look for `.so` files in the current dir (`.`) when running `legolas`."  

#### **Option 2: Permanent Fix (Install `libdobby.so`)**
```sh
sudo cp libdobby.so /usr/lib/
./legolas  # Now works without LD_LIBRARY_PATH
```
- **Why?** `/usr/lib/` is a standard path where `ld-linux.so` looks.  

#### **Option 3: Force-Load (`LD_PRELOAD`)**
```sh
LD_PRELOAD=./libdobby.so ./legolas
```
- **Meaning**: "Load `./libdobby.so` **before** any other libraries."  

---

## **5. Extra: Debugging**

### **Check Dependencies (`ldd`)**
```sh
ldd legolas
```
**Output** (before fix):
```
libdobby.so => not found
```
**After fix**:
```
libdobby.so => /usr/lib/libdobby.so (0x00007f...)
```

### **Debug Library Loading (`LD_DEBUG`)**
```sh
LD_DEBUG=libs ./legolas
```
- Shows **exactly** where `ld-linux.so` searches for libraries.  
