#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int bst_array[47] = { 'w', 'f', '{', '_', 'n', 'y', '}',  -1, 'b', 'l', 'r',  -1,  -1,  -1,  -1,  -1,  -1, 'a', 'e', 'i',  -1, 'o', 't',
                       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 'g',  -1,  -1,  -1,  -1,  -1,  -1, 'u' };

/* bst_array is an implict BST (DeepSeek generated the diagram) */
/* -1 are replaced with '*' characters */
/*
                                [0] 'w' (ROOT)
                              /               \
              [1] 'f'                             [2] '{'
            /         \                         /         \
      [3] '_'          [4] 'n'          [5] 'y'          [6] '}'
     /       \         /       \        /       \        /       \
[8] 'b'      [9] 'l' [10] 'r'   *      *         *      *         *
 |            |       |
 *         [17]'a'    *
           /    \
          *      *
               [19]'i'
               /    \
              *      *
                    [21]'o'
                    /    \
                   *    [22]'t'
                        /    \
                       *    [40]'g'
                            /    \
                           *    [46]'u'
*/

/* these are the indices of the characters of the flag "flag{we_beleaf_in_your_re_future}" in the BST bst_array */
long compare_array[33] = { 0x01, 0x09, 0x11, 0x27, 0x02, 0x00, 0x12, 0x03, 0x08, 0x12, 0x09, 0x12, 0x11, 0x01, 0x03, 0x13,
                           0x04, 0x03, 0x05, 0x15, 0x2E, 0x0A, 0x03, 0x0A, 0x12, 0x03, 0x01, 0x2E, 0x16, 0x2E, 0x0A, 0x12, 0x06,};

#define left(x) (2*(x) + 1)
#define right(x) (2*((x)+1))

long transform(char c) {
  long i = 0;
  int k = (int) c;

  /* this is an implicit Binary Search Tree traversal to find the index of character c */
  /* leave the while loop when (int) c = bst_array[i] */
  while ((i != -1 && (k != bst_array[i]))) {
    if (k < bst_array[i]) {
      i = left(i); /* go to left child */
    }
    else if (k > bst_array[i]) {
      i = right(i);  /* go to right child */
    }
  }
  return i;
}


int main(void) {
  size_t input_len;
  long c;
  /* long in_FS_OFFSET; */
  unsigned long i;
  char input [136];
  //long stackCanary = *(long *)(in_FS_OFFSET + 0x28);

  printf("Enter the flag\n>>> ");
  scanf("%s", input);
  input_len = strlen(input);
                    /* the input has to be 33 chars */
  if (input_len < 33) {
    puts("Incorrect!");
    exit(1);
  }
  for (i = 0; i < input_len; i = i + 1) {
    c = transform(input[i]);
    /* compare each transformed(input[i]) with compare_array[i] (long type, 8 bytes) */
    if (c != compare_array[i]) {
      puts("Incorrect!");
      exit(1);
    }
  }
  puts("Correct!");

  /*
  if (stackCanary != *(long *)(in_FS_OFFSET + 0x28)) {
    __stack_chk_fail();
  }
  */

  return 0;
}
