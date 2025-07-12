# `control-flow-1`: Exercise 1

*  How many compare statements are in main?

`main` has 4 `cmp` statements, one for `argc == 2` and 3 checks for the input.

*  What are the three values that are being compared?

`argc` (on the first check); `argv[1]`, `argv[2]` (on the other 3 checks).

*  Can you pass all three checks?

Yes. Just have to supply `100 <= x2 < x1 <= 2 * x2`.

# `loop-example-1`: Exercise

* How many times does this loop run?

15 times, which is the only valid input length.

* What is this loop looking for?

It is checking if the input has exactly 8 uppercase letters.

* Do the values used represent anything?

Yes, they represent `'A' <= c <= 'Z'`, uppercase ASCII characters.

* Can you get access?

Yes, for example by using the input `ABCDEFGH@@@@@@@`.

# `variables-example`: Example

* How many global variables are being used?

By the `main` function, only 2 global variables: `long XorMe` and `char globalVar[8]`.

* How many local variables are in use?

By the `main` function, we have the 5 named local variables: `argc`, `argv` (arguments); `ret`, `i` and `input_len`.

Now, in the assembly code, we have 6 unamed local variables:

* undefined4 `rbp-0x10`
* undefined4 `rbp-0x14`
* undefined1 `rbp-0x15`
* undefined1 `rbp-0x16`
* undefined4 `rbp-0x1c`
* undefined8 `rbp-0x28`.

* Can you find the proper keycode?

Yes, it is `J0(kb$!R`. To get it, just apply the operations with the global variables `XorMe` and `globalVar`.

# `func-example-1`: Exercise 1

* How many functions does the auto analysis discover?
* How many local variables are present in the each function?
* What are their values?
* Do any of these functions take arguments?
* If so, what are the arguments?

# `heap-example-1`: Exercise 1

* How much memory is being allocated via malloc?
* How is this program different than the loop example?

# `array-example`: Exercise

* How many different arrays are in use?
* What is their scope, are they global?
* Can you solve the password for all 4 index values?
