#include <stdio.h>
#include <string.h>

#define is_odd(x) ((x) & 1)

int main(
    int argc,       // var_44h, rbp-0x44
    char *argv[]    // var_50h, rbp-0x50
) {
    char *good_luck_next_time = "good luck next time";

    if (argc != 3) {
        puts("hmm... i'm not sure you know what the word \"twins\" mean :/");
        puts(good_luck_next_time);    // "good luck next time"
        return 0;
    }

    char *abby = argv[1];
    char *gabby = argv[2];

    if (strlen(abby) <= 6) {
        puts("Abby: i'm older than that :(");
        puts(good_luck_next_time);
        return 0;
    }
    if (strlen(gabby) <= 6) {
        puts("Gabby: i'm older than that :(");
        puts(good_luck_next_time);
        return 0;
    }

    if (strlen(gabby) != strlen(abby)) {
        puts("Abby & Gabby: for god's sake we are TWINS! we were born the same night!!");
        puts(good_luck_next_time);
        return 0;
    }

    if (is_odd(strlen(abby))) {
        puts("Abby & Gabby: we are not \"odd\" years old :(");
        puts(good_luck_next_time);
        return 0;
    }

    int i = 0;
    int half_len = strlen(abby) >> 1;
    for (; i < half_len; i++) {
        if (gabby[i] != abby[i]) {
            puts("Abby & Gabby: we're half twins you know...");
            puts(good_luck_next_time);
            return 0;
        }
    }

    int full_len = strlen(abby);
    for (; i < full_len; i++) {
        if (gabby[i] == abby[i]) {
            puts("Abby & Gabby: we are only HALF twins... :3 ");
            puts(good_luck_next_time);
            return 0;
        }
    }

    puts("Abby & Gabby: yaayy!! nice job! :D");
    return 0;
}
