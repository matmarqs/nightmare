# Session Three Exercises

These exercises are designed to illusrate the principals that are covered in the course materials, for session three we have the following exercises:

* structs
* pointers
* syscall
* files

**Note** You will need to mark these binaries as executable before trying to run them, ex: ```chmod u+x structs```

Each of these exercises are designed to accept user input from the command line - when the proper input is provided you will get a Success or Congratulatory message.

**Remember** you can provide non ascii input as follows:

```./pointers `python -c 'print "\xDE\xAD\xBE\xEF"'` ```

This example will provide the value 0xDEADBEEF into argv[1]

**Hint**: For the files exercise, you may need to edit binary files, to do this you can use any standard hex editor, below are some reccomendations:
* 010Editor
* HxD
* xxd

It is up to you to determine the format and number of these arguments for each exercise by reverse engineering them - if you get stuck please feel free to reach out on the [hackaday.io](https://hackaday.io/project/172292-introduction-to-reverse-engineering-with-ghidra) page, or reach out to me on [twitter](https://twitter.com/wrongbaud/).


# `structs`

## Reversing

Manually decompiling the C code by examining assembly from `gdb`:

```c
typedef struct {
    int key;
    char *name;
    char *pass;
} User;

int main (
    int argc,    // rbp-0x54
    char *argv[] // rbp-0x60
)
{
    int i;   // rbp-0x48

    int input_key = 0;   // rbp-0x44
    char *input_user; // rbp-0x40
    char *input_pass; // rbp-0x38

    User user;  // hidden struct, contiguous area of memory
    //int s_key; // rbp-0x30
    //char *s_user; // rbp-0x28
    //char *s_pass; // rbp-0x20

    int double_key; // rbp-0x18

    if (argc != 4) {
        puts("Please provide your key, username and password!\r\nExample: 12738 wrongbaud P@55W0rd1\r");
        return -1;
    }

    // input_key has to be a valid signed integer
    int input_key = atoi(argv[1]);   // rbp-0x44
    if (input_key == 0) {    // atoi() returns 0 on error, also the key can't be 0
        puts("Improper key provided, please provide and integer key!\r");
        return -1;
    }

    input_user = argv[2];
    if (strlen(input_user) > 597 || strlen(input_user) <= 7) {    // 597 = 0x255
        puts("Improper username provided, please check the length!\r");
        return -1;
    }

    input_pass = argv[3];
    if (strlen(input_pass) > 597 || strlen(input_pass) <= 7) {
        puts("Improper password provided, please check the length!\r");
        return -1;
    }

    user.key = input_key;
    double_key = add(user.key, user.key);
    user.name = input_user;

    gen_password(&user); // user.pass = generated_password

    for (int i = 0; i < strlen(user.pass); i++) {
        if (input_pass[i] != user.pass[i]) {
            puts("Invalid character in password detected, exiting now!\r");
            return -1;
        }
    }

    puts("Correct! Access granted!\r");
    free(user.pass);

    return 0;
}

int add(int a, int b) {
    return a + b;
}

// arg[0] = key, arg[1] = username, arg[2] = generated_password
// this function generate a password given the key and username
// it actually takes a struct User
void gen_password(
    User *user // rbp-0x18
)
{
    int i;   // rbp-0x10
    int len;   // rbp-0xc
    char *generated_passwd;   // rbp-0x8

    len = strlen(user->name);
    generated_passwd = malloc(len);

    for (i = 0; i < len; i++) {
        generated_passwd[i] = (char) (user->key ^ user->name[i]);
        generated_passwd[i] -= 19;
    }

    user->pass = generated_passwd;
}
```

## Pwning

* To get access, we need `input_pass` to be equal to the `generated_passwd`.

* We are free to choose an `username` and `key`.

* The username has to have length at least `8`. So, let's pick the username `matmarqs`.

* The key can not be `0`, so let's take `key = 1`.

We just have to replicate the function. I will write in C because we already have the C code for `gen_passwd()`:

```c
// gen_passwd.c

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct {
    int key;
    char *name;
    char *pass;
} User;

void gen_password(User *user)
{
    int i;   // rbp-0x10
    int len;   // rbp-0xc
    char *generated_passwd;   // rbp-0x8

    len = strlen(user->name);
    generated_passwd = malloc(len + 1); // patch the code to add '\0' string

    for (i = 0; i < len; i++) {
        generated_passwd[i] = (char) (user->key ^ user->name[i]);
        generated_passwd[i] -= 19;
    }

    user->pass = generated_passwd;
}

int main() {
    User user;
    user.key = 1;
    user.name = "matmarqs";
    gen_password(&user);
    printf("%s\n", user.pass);
    free(user.pass);
}
```

```bash
$ gcc gen_passwd.c && ./a.out
YMbYM`]_
$ ./structs 1 matmarqs 'YMbYM`]_'
Correct! Access granted!
```


# `pointers`

Using Ghidra, we get

```c
typedef struct {
    int key;
    char *username;
    char *password;
    int realKey;
} User;

int main(int argc,char **argv) {
    int key;
    size_t len;
    int i;
    User user;
    char *password;
    char *username;

    if (argc != 4) {
        puts("Please provide your key, username and password!\r\nExample: 12738 wrongbaud P@55W0rd1\r");
        return -1;
    }

    key = atoi(argv[1]);
    if (key == 0) {
        puts("Improper key provided, please provide and integer key!\r");
        return -1;

    username = argv[2];
    len = strlen(username);
    if (len < 8 || len > 597) {
        puts("Improper username provided, please check the length!\r");
        return -1;
    }

    password = argv[3];
    len = strlen(password);
    if (len < 8 || len > 597) {
        puts("Improper password provided, please check the length!\r");
        return -1;
    }

    user.key = key;
    user.realKey = add_mult_8(key,key + 0xbeef);
    user.username = username;
    user.password = password;
    swapNames(&user.username,&user.password);
    gen_password(&user);
    i = 0;
    while( true ) {
        len = strlen(user.password);
        if (i >= len) {
            puts("Correct! Access granted!\r");
            free(user.password);
            return 0;
        }
        if (username[i] != user.password[i]) break;
        i = i + 1;
    }
    puts("Invalid character in password detected, exiting now!\r");
    return -1;
}

int add_mult_8(int a,int b) {
  return (b + a) * 8;
}

void gen_password(User *user) {
  size_t len;
  char *gen_passwd;
  int i;

  len = strlen(user->username);
  gen_passwd = (char *)malloc((long)(int)len);
  for (i = 0; i < (int)len; i = i + 1) {
    gen_passwd[i] = (byte)user->realKey ^ (char)user->key + user->username[i];
    gen_passwd[i] = gen_passwd[i] + -0x13;
  }
  user->password = gen_passwd;
  return;
}

void swapNames(char **a,char **b) {
  char *aux = malloc(8);
  aux = *(char *)a;
  *(char *)a = *(char *)b;
  *(char *)b = aux;
  return;
}
```

Here is the code to generate the password:
```c
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

typedef struct {
    int key;
    char *username;
    char *password;
    int realKey;
} User;

void gen_password(User *user) {
    size_t len;
    char *gen_passwd;
    int i;

    len = strlen(user->username);
    gen_passwd = (char *)malloc(len);
    for (i = 0; i < len; i++) {
        gen_passwd[i] = user->realKey ^ (user->key + user->username[i]);
        gen_passwd[i] = gen_passwd[i] - 0x13;
    }
    gen_passwd[len] = '\0';
    user->password = gen_passwd;
    return;
}

int add_mult_8(int a,int b) {
  return (b + a) * 8;
}

void swapNames(char **a, char **b) {
    char *aux = *a;
    *a = *b;
    *b = aux;
}

int main() {
    User user;
    user.key = 1;
    user.realKey = add_mult_8(user.key,user.key + 0xbeef);
    user.username = strdup("matmarqs");
    user.password = strdup("initial");  // Just placeholder
    gen_password(&user);
    for (size_t i = 0; i < strlen(user.password); i++) {
        printf("\\x%02x", (unsigned char)user.password[i]);
    }
    printf("\n");
}
```

To get access, we need to swap the username and password:
```bash
$ c gen_passwd_pointers.c
\xd3\xd7\xea\xd3\xd7\xe8\xe7\xe9
$ ./pointers 1 $(echo -e "\xd3\xd7\xea\xd3\xd7\xe8\xe7\xe9") "matmarqs"
Correct! Access granted!
```

# `syscall`

The disassembly is:

```asm
mov    eax,0x2  ; open(char *filename, int flags, int mode) syscall
movabs rdi,0x600124 ; "syscalls.txt"
mov    rsi,QWORD PTR ds:0x600131    ; *0x600131 = 0x42, which is the flag O_CREAT | O_RDWR
mov    rdx,QWORD PTR ds:0x600139    ; *0x600139 = 0x180, mode '600' = rw------- in Linux. octal 0600 = 0x180
syscall
mov    QWORD PTR ds:0x600141,rax    ; file descriptor is stored at 0x600141
mov    eax,0x1  ; write(int fd, char *buf, size_t count) syscall
mov    rdi,QWORD PTR ds:0x600141    ; file descriptor of file "syscalls.txt"
movabs rsi,0x400114 ; buffer from which we write in the file, it has the string "hello-hackaday\0"
mov    edx,0xe  ; write 14 bytes
syscall
mov    eax,0x3  ; close(int fd) syscall
mov    rdi,QWORD PTR ds:0x600141    ; the file descriptor from before
syscall
mov    edi,0x0  ; exit(0)
mov    eax,0x3c ; exit(int error_code) syscall
syscall
```

* There are 4 syscalls performed: `open 2`, `write 1`, `close 3`, and `exit 60`.
* The program just creates a file with read/write permissions (mode `600`) named `syscalls.txt` and
writes `hello-hackaday` to it. It closes the file and `exit(0)`.
* The entry point is `_start`.


# `files`

From Ghidra, we get:

```c
typedef struct {
    int key;
    char *name;
    int dontUse;
    char *pass;
    int calcKey;
} User;

void gen_password(User *user) {
  size_t len_username;
  char *gen_pass;
  int i;

  len_username = strlen(user->name);
  gen_pass = (char *)malloc((long)(int)len_username);
  for (i = 0; i < (int)len_username; i = i + 1) {
    gen_pass[i] = (byte)user->calcKey ^ (char)user->key + user->name[i];
    gen_pass[i] = gen_pass[i] + -0x13;
  }
  user->pass = gen_pass;
  return;
}

int add_mult_8(int a,int b) {
  return (b + a) * 8;
}

int main(void) {
  int ret;
  ssize_t num_bytes;
  size_t len_pass;
  ulong ii;
  int key;
  int i;
  int dont_use;
  int key_fd;
  int int_num_bytes;
  int username_fd;
  int password_fd;
  char *username;
  char *password;
  User s_key;

  key = 0;
  dont_use = 0;
                    /* "key.y" */
  key_fd = open(keyFile,0);
  if (key_fd == -1) {
    puts("Could not find key file, please try again!\r");
    ret = -1;
  }
  else {
    num_bytes = read(key_fd,&key,4);
    int_num_bytes = (int)num_bytes;
    if (int_num_bytes < 4) {
      puts("Not enough values in keyfile, please try again!\r");
      ret = -1;
    }
    else {
      dont_use = int_num_bytes + -1;
      s_key.key = key;
      s_key.dontUse = dont_use;
                    /* uname.x */
      username_fd = open(unameFile,0);
      if (username_fd == -1) {
        puts("Could not find username file, please try again!\r");
        ret = -1;
      }
      else {
        username = (char *)malloc(0x255);
        num_bytes = read(username_fd,username,0x255);
        int_num_bytes = (int)num_bytes;
        if (int_num_bytes < 8) {
          puts("Not enough values in keyfile, please try again!\r");
          ret = -1;
        }
        else {
                    /* pword.z */
          password_fd = open(pwordFile,0);
          if (password_fd == -1) {
            puts("Could not find password file, please try again!\r");
            ret = -1;
          }
          else {
            password = (char *)malloc(0x255);
            num_bytes = read(password_fd,password,0x255);
            int_num_bytes = (int)num_bytes;
            if (int_num_bytes < 8) {
              puts("Not enough values in keyfile, please try again!\r");
              ret = -1;
            }
            else {
                    /* conteudo de verdade, quando os arquivos existem */
              s_key.calcKey = add_mult_8(s_key.key,s_key.key + 0xbeef);
              s_key.name = username;
              s_key.pass = password;
              gen_password(&s_key);
              for (i = 0; ii = (ulong)i, len_pass = strlen(s_key.pass), ii < len_pass; i = i + 1) {
                if (password[i] != s_key.pass[i]) {
                  puts("Invalid character in password detected, exiting now!\r");
                  return -1;
                }
              }
              puts("Correct! Access granted!\r");
              free(s_key.pass);
              ret = 0;
            }
          }
        }
      }
    }
  }
  return ret;
}
```

We just have to create the files `key.y`, `uname.x` and `pword.z` with respective `key`, `username` and `password` contents. We chose:

* In `key.y`: `1111` (has to be length at least 4), which is transformed to the `int` value `0x31313131`.
* In `uname.x`: `matmarqs`.
* In `pword.z`: we generate it with the following C code

```c
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct {
    int key;
    char *name;
    int dontUse;
    char *pass;
    int calcKey;
} User;

void gen_password(User *user) {
  size_t len_username;
  char *gen_pass;
  int i;
  
  len_username = strlen(user->name);
  gen_pass = (char *)malloc((long)(int)len_username);
  for (i = 0; i < (int)len_username; i = i + 1) {
    gen_pass[i] = user->calcKey ^ ((char)user->key + user->name[i]);
    gen_pass[i] = gen_pass[i] + -0x13;
  }
  user->pass = gen_pass;
  return;
}

int add_mult_8(int a,int b) {
  return (b + a) * 8;
}

int main() {
  User user;
  user.key = 0x31313131;
  user.name = strdup("matmarqs");
  user.calcKey = add_mult_8(user.key, user.key + 0xbeef);
  gen_password(&user);
  printf("%s", user.pass);
}
```

```bash
$ gcc gen_passwd_files.c && ./a.out > pword.z
$ ./files
Correct! Access granted!
$ xxd key.y
00000000: 3131 3131                                1111
$ xxd uname.x
00000000: 6d61 746d 6172 7173                      matmarqs
$ xxd pword.z
00000000: 0307 1a03 0718 1719                      ........
```
