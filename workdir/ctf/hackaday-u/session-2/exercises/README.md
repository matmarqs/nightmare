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
