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
