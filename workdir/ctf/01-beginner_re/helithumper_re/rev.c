#include <stdlib.h>

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

/* glibc stack smashing detection */
//extern void __stack_chk_fail(void) __attribute__((noreturn));

bool validate(char *input) {
    //long in_FS_OFFSET = 0;
    //long stack_canary = *(long *)(in_FS_OFFSET + 0x28);
    int flag[14];
    bool check;
    size_t input_len;
    int i;

    // Build flag array in stack
    flag[0] = 'f'; flag[1] = 'l'; flag[2] = 'a'; flag[3] = 'g';
    flag[4] = '{'; flag[5] = 'H'; flag[6] = 'u'; flag[7] = 'C';
    flag[8] = 'f'; flag[9] = '_'; flag[10] = 'l'; flag[11] = 'A';
    flag[12] = 'b'; flag[13] = '}';

    input_len = strlen(input);
    i = 0;
    
    do {
        // Check if we've processed entire input
        if ((int)input_len <= i) {
            check = true;
            goto stack_check;
        }
        
        // Compare character with flag
        if (input[i] != flag[i]) {
            check = false;
            goto stack_check;
        }
        
        i++;
    } while (true);

stack_check:
    //// Verify stack canary hasn't been modified
    //if (stack_canary != *(long *)(in_FS_OFFSET + 0x28)) {
    //    __stack_chk_fail();
    //}
    return check;
}

int main(void) {
    char *intro_text = "Welcome to the Salty Spitoon™, How tough are ya?";
    char *failed_str = "Yeah right. Back to Weenie Hut Jr™ with ya";
    char *input = (char *)calloc(50, 1);
    
    puts(intro_text);
    scanf("%s", input);
    
    bool validation_result = validate(input);
    
    // Inverted logic for message printing
    if (!validation_result) {
        puts(failed_str);
    } else {
        puts("Right this way...");
    }
    
    free(input);
    return !validation_result;
}
