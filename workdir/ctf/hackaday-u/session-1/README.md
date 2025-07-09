# Solutions

## c1

The password is `hackadayu`.

We can get it through `strings`, `ltrace`, `ghidra` etc.

## c2

```c
int main(int argc,char **argv) {
  int ret;
  size_t input_len;
  
  if (argc == 2) {
    input_len = strlen(argv[1]);
    if (input_len < 5) {
      puts("We\'d never use a password that short!");
      ret = -1;
    }
    else if ((*argv[1] == 'h') && (argv[1][4] == 'u')) {
      puts("Correct -- maybe we should pay attention to more characters...");
      ret = 0;
    }
    else {
      puts("Wrong answer! Try again");
      ret = -1;
    }
  }
  else {
    puts("Please supply the password!\r");
    ret = -1;
  }
  return ret;
}
```

We just need to input a `char s[5]` where `s[0] = 'h'` and `s[4] = 'u'`.

## c3

```c
int main(int argc,char **argv) {
  int ret;
  size_t inputlen;
  
  if (argc == 2) {
    inputlen = strlen(argv[1]);
    if (inputlen < 5) {
      puts("Come on now ...  you should expect betTer from us!");
      ret = -1;
    }
    else if ((uint)(byte)argv[1][2] - (uint)(byte)argv[1][3] == 32) {
      puts("Correct! You figured it out ... looks like we have to upgrade our security...");
      ret = 0;
    }
    else {
      puts("IncorRect pasSword!");
      ret = -1;
    }
  }
  else {
    puts("Please provide the secret pasSword!");
    ret = -1;
  }
  return ret;
}
```

We just need a input string `s` of length at least `5` where `s[2] - s[3] = 32`. This happens if we use a lowercase for `s[2]` and an uppercase for `s[3]`, for example `"aabBa"`.

## c4

```c
int main(int argc,char **argv) {
  int ret;
  size_t inputlen;
  int i;
  
  if (argc == 2) {
    inputlen = strlen(argv[1]);
    if (inputlen < 10) {
      puts("Come on now ...  you should expect better from us!");
      ret = -1;
    }
    else {
      inputlen = strlen(argv[1]);
      for (i = 0; i < (int)inputlen; i = i + 1) {
        if ("hackaday-u"[i] + 2 != (int)argv[1][i]) {
          puts("Wrong Password!");
          return -1;
        }
      }
      puts("Correct! You\'ve entered the right password ... you\'re getting better at this!");
      ret = 0;
    }
  }
  else {
    puts("Please provide the secret password!");
    ret = -1;
  }
  return ret;
}
```

We just need to shift 2 for each of the characters in the string `hackaday-u`.

```python
s = "hackaday-u"
passwd = ""

for c in s:
    passwd += chr(ord(c) + 2)

print(passwd)
# Output: 'jcemcfc{/w'
```

## nasm_crack

Just run `strings`, we see that the password is `supersecret`.

## SimpleKeyGen

```c
int checkSerial(char *input) {
  int iVar1;
  size_t input_len;
  int i;
  
  input_len = strlen(input);
  if (input_len == 16) {
    for (i = 0; input_len = strlen(input), (ulong)(long)i < input_len; i = i + 2) {
      if ((int)input[i] - (int)input[(long)i + 1] != -1) {
        return -1;
      }
    }
    iVar1 = 0;
  }
  else {
    iVar1 = -1;
  }
  return iVar1;
}

```

Our input

* Has to make this function `checkSerial()` return `0`.
* Has to be `16` in length.
* Has to satisfy `input[i+1] - input[i] == 1` for every pair `(i, i+1)`, with `i` incrementing `i += 2`.

Such an input is `input = "abababababababab"`.

## skele

```c
int main(int argc,char **argv) {
  int ret;
  size_t inputlen;
  size_t eight;
  long in_FS_OFFSET;
  int i;
  byte xor [8];
  long stackCanary;
  
  stackCanary = *(long *)(in_FS_OFFSET + 0x28);
  xor[0] = 8;
  xor[1] = 9;
  xor[2] = 10;
  xor[3] = 0xb;
  xor[4] = 0xc;
  xor[5] = 0xd;
  xor[6] = 0xe;
  xor[7] = 0xf;
  if (argc == 2) {
    inputlen = strlen(argv[1]);
    eight = strlen("skeletor");
    if (inputlen < eight) {
      puts("Wrong length, better luck next time!\r");
      ret = -1;
    }
    else {
      for (i = 0; i < 8; i = i + 1) {
        if ((byte)("skeletor"[i] ^ xor[i]) != argv[1][i]) {
          puts("Wrong answer\r");
          ret = -1;
          goto end;
        }
      }
      puts("Correct! You have the power!\r");
      ret = 0;
    }
  }
  else {
    puts("Please provide the password to enter castle greyskull!\r");
    ret = 0;
  }
end:
  if (stackCanary != *(long *)(in_FS_OFFSET + 0x28)) {
    __stack_chk_fail();
  }
  return ret;
}
```

Our input has to be at least 8 characters (the length of `"skeletor"`).

The array to be xored with `"skeletor"` is `xor = [8, 9, 10, 11, 12, 13, 14, 15]`.

```python
xor = [8, 9, 10, 11, 12, 13, 14, 15]
skeletor = "skeletor"

passwd = ""

for i in range(8):
    passwd += chr(ord(skeletor[i]) ^ xor[i])


print(passwd)
# Out[5]: '{bogiya}'
```

