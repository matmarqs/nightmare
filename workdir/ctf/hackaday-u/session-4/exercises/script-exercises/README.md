# Scripting / API Review

In this folder you will find three examples of the same code compiled for different architectures. For each of these, see if you can do the following:

1. Write a Java script that calculates instruction frequency for all of the instructions used in the main function
2. Write a Python script to examine and write the PCode out to a file using the API calls covered in class
3. Write a Java or Python script that counts the amount of branches taken during the main function


# `pcode-ex-x86_64`

```c
int main(
    int argc,   // rbp-0x14
    char *argv[] // rbp-0x20
) {
    int i;    // rbp-0x10
    int passlen;   // rbp-0xc
    char *hackaday_u = "hackaday-u";    // rbp-0x8

    if (argc != 2) {
        puts("Please provide the secret password!");
        return -1;
    }

    if (strlen(argv[1]) < 10) {
        puts("Come on now ...  you should expect better from us!");
        return -1;
    }

    passlen = strlen(argv[1]);
    for (i = 0; i < passlen; i++) {
        if (hackaday_u[i] + 2 != argv[1][i]) {
            puts("Wrong Password!");
            return -1;
        }
    }

    puts("Correct! You've entered the right password ... you're getting better at this!");
    return 0;
}
```

From the (manually) decompiled above, we know the password is the `ROT 2` of `"hackaday-u"`, so it is `"jcemcfc{/w"`.

```bash
$ ./pcode-ex-x86_64 'jcemcfc{/w'
Correct! You've entered the right password ... you're getting better at this!
```
