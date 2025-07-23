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
