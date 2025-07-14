# `control-flow-1`

```c
int main(int argc,char **argv) {
  int x1;
  int x2;
  
  if (argc == 3) {
                    /* two arguments are integers */
    x1 = atoi(argv[1]);
    x2 = atoi(argv[2]);
                    /* first arguments needs to be greater than the second */
    if (x2 < x1) {
                    /* x1 <= 2 * x2 */
      if (x2 << 1 < x1) {
        puts("Second check failed, try again!\r");
        x2 = -1;
      }
      else {
                    /* x2 >= 100 */
        if ((x2 + x1) - x1 < 100) {
          puts("Third check failed, try again!\r");
          x2 = -1;
        }
        else {
          puts("Proper values provided! Great work!\r");
          x2 = 0;
        }
      }
    }
    else {
      puts("First check failed, try again!\r");
      x2 = -1;
    }
  }
  else {
    puts("Please provide two values to generate a code!\r\nExample: ./control-flow-1 100 4300\r");
    x2 = -1;
  }
  return x2;
}
```

Let `x1` be the first argument and `x2` the second argument. We have 3 conditions:

* `x1 > x2`
* `x1 <= 2 x2`
* `x2 >= 100`

Organizing these inequalities, we get:

$$
100 \leq x_2 < x_1 \leq 2 x_2.
$$

If we choose $x_2 = 100$, we can use any $101 \leq x_1 \leq 200$, for example $x_1 = 200$.


# `loop-example-1`

```c
int main(int argc,char **argv) {
  int ret;
  size_t input_len;
  int i;
  int count_upper_letters;
  
  if (argc == 2) {
    input_len = strlen(argv[1]);
    if ((int)input_len == 15) {
      count_upper_letters = 0;
      for (i = 0; i < 15; i = i + 1) {
        if (('@' < argv[1][i]) && (argv[1][i] < '[')) {
          count_upper_letters = count_upper_letters + 1;
        }
      }
      if (count_upper_letters == 8) {
        puts("Congratulations, access granted!\r");
      }
      else {
        puts("Not quite what we\'re looking for ... maybe try again?\r");
      }
      ret = 0;
    }
    else {
      puts("Wrong length! Try again!\r");
      ret = -1;
    }
  }
  else {
    puts("Please provide a string!\r");
    ret = -1;
  }
  return ret;
}
```

Basically, the code checks if the input (in **argv[1]**) has length 15 and has exactly **8** characters in the range `'A' <= input[i] <= 'B'`, the remaining 7 characters have to be outside of this range. This functionality is indicated by the variable `count_upper_letters`. Note that the line

```c
'@' < argv[1][i]) && (argv[1][i] < '[')
```

is equivalent to

```c
'A' < argv[1][i]) && (argv[1][i] < 'Z')
```

because `'@' = 'A' - 1` and `'[' = 'Z' + 1` in ASCII.

So, a sample valid input is `"ABCDEFGH@@@@@@@"`.


# `variables-example`

```c
int main(int argc,char **argv) {
  int ret;
  size_t input_len;
  int i;
  
  if (argc == 2) {
    input_len = strlen(argv[1]);
                    /* input has to be length 8 */
    if (input_len < 8) {
      puts("Too short, try again!\r");
    }
    for (i = 0; i < 8; i = i + 1) {
      if ((char)((char)(XorMe >> ((byte)(i << 3) & 0x3f)) + globalVar[i] + 1) != argv[1][i]) {
        puts("Improper character in keycode detected, try again!\r");
        return -1;
      }
    }
    puts("Proper keycode supplied, well done!\r");
    ret = 0;
  }
  else {
    printf("Please prvide the 8 character keycode");
    ret = -1;
  }
  return ret;
}
```

Global variables `long XorMe = 0xdeadbeeffacecafe` and `char globalVar[8] = "KeYpress"`.

For each `input[i]` has to be equal to `(XorMe >> (i << 3) & 0x3f) + globalVar[i] + 1`.

```python
def get_char(x):
    return chr(x % 256)

XorMe = 0xdeadbeeffacecafe
globalVar = "KeYpress"
string = ""
for i in range(8):
    string += get_char((XorMe >> ((i << 3) & 0x3f)) + ord(globalVar[i]) + 1)
print(string)
```

This outputs the keycode `'J0(kb$!R'`.


# `func-example-1`

All the code is doing is getting the lower and uppercase parts of the input string. The passcode will pass if their lengths are equal.
Therefore, an input like `aB` is enough.

```c
int main(int argc,char **argv) {
  int ret;
  char *upper;
  char *lower;
  size_t up_len;
  size_t lo_len;
  
  if (argc == 2) {
    upper = getUpperCase(argv[1]);
    lower = getLowerCase(argv[1]);
    up_len = strlen(upper);
    lo_len = strlen(lower);
    if (up_len == lo_len) {
      puts("Passcode generator passed, good job!\r");
      free(upper);
      free(lower);
      ret = 0;
    }
    else {
      puts("Passcode doesn\'t have enough variety! Please try again\r");
      free(upper);
      free(lower);
      ret = -1;
    }
  }
  else {
    puts("Please generate a passcode for system usage!\r");
    ret = -1;
  }
  return ret;
}
```

The functions `getUpperCase` and `getLowerCase` simply get the upper and lowercase parts of the input string.

```c
char * getUpperCase(char *input) {
  size_t input_len;
  char *uppercase;
  int i;
  int count;
  
  input_len = strlen(input);
                    /* allocating sizeof(char) * input_len */
  uppercase = (char *)calloc((long)(int)input_len,1);
  count = 0;
  for (i = 0; i < (int)input_len; i = i + 1) {
                    /* check if it is uppercase 'A' <= c <= 'Z' */
    if (('@' < input[i]) && (input[i] < '[')) {
      uppercase[count] = input[i];
      count = count + 1;
    }
  }
  return uppercase;
}

char * getLowerCase(char *input) {
  size_t input_len;
  char *lowercase;
  int i;
  int count;
  
  input_len = strlen(input);
  lowercase = (char *)calloc((long)(int)input_len,1);
  count = 0;
  for (i = 0; i < (int)input_len; i = i + 1) {
                    /* check if it is lowercase, 'a' <= c <= 'z' */
    if (('`' < input[i]) && (input[i] < '{')) {
      lowercase[count] = input[i];
      count = count + 1;
    }
  }
  return lowercase;
}
```


# `heap-example-1`

```c
int main(int argc,char **argv) {
  int ret;
  size_t input_len;
  char *upper;
  int i;
  int count;
  
  if (argc == 2) {
    input_len = strlen(argv[1]);
    upper = (char *)calloc((long)(int)input_len,1);
    count = 0;
    for (i = 0; i < (int)input_len; i = i + 1) {
      if (('@' < argv[1][i]) && (argv[1][i] < '[')) {
        upper[count] = argv[1][i];
        count = count + 1;
      }
    }
    if (count == 12) {
      printf("The result is: %s\r\n",upper);
    }
    else {
      puts("Not quite what we\'re looking for ... maybe try again?\r");
    }
    free(upper);
    ret = 0;
  }
  else {
    puts("Please provide a string!\r");
    ret = -1;
  }
  return ret;
}
```

Basically, the result is the uppercase part of the input string. If the length is of the uppercase part is 12, it outputs positively.


# `array-example`


```c
int main(int argc,char **argv) {
  int index;
  size_t keyword_len;
  char curr;
  char next;
  uint i;
  
  if (argc == 3) {
    index = atoi(argv[1]);
                    /* 0 <= argv1 <= 4 */
    if (index == -1 || index > 4) {
      puts("Improper index provided, try again!\r");
      index = -1;
    }
    else {
      keyword_len = strlen((char *)keywords[index]);
      for (i = 0; i < keyword_len; i++) {
        curr = keywords[index][i];
        if (i == keyword_len - 1) {
          next = keywords[index][0];
        }
        else {
          next = keywords[index][i+1];
        }
        if (next < c) {
          curr = curr - next;
        }
        else {
          curr = next - c;
        }
        curr = curr + '`';
                    /* transformed char has to match input keycode chars */
        if (c != argv[2][i]) {
          printf("Wrong value detected at character %x!\r\n", i);
          return -1;
        }
      }
      printf("Congratulations, you\'ve unlocked the code for value %x, can you get them all?\r\n", index + 1);
      index = 0;
    }
  }
  else {
    puts("Please provide a password index and keycode, ex: 1 p@55w0rd!\r");
    index = -1;
  }
  return index;
}
```

In Ghidra we see the `keycodes` array, which we declare as an array and transform the data type to `char **`. Then we reimplement the algorithm below in Python to get all the 5 codes:

```py
# solve_array.py
keycodes = ["hackadayu", "software", "reverse", "engineering", "ghidra"]

def transform(i, keyword):
    curr = keyword[i]

    if i == len(keyword) - 1:
        next = keyword[0]
    else:
        next = keyword[i+1]

    if ord(next) < ord(curr):
        c = ord(curr) - ord(next)
    else:
        c = ord(next) - ord(curr)

    return chr(c + ord('`'))

for index in range(5):
    result = ""
    for i in range(len(keycodes[index])):
        result += transform(i, keycodes[index])
    print(result)
```

```bash
$ python3 solve_array.py
gbhjccxdm
dincvqmn
mqqmanm
igbei`miegb
aaenqf
```
