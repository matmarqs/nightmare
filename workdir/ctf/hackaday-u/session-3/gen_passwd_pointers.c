#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

typedef struct {
    int key;
    char *username;
    char *password;
    int realKey;
} User;

void gen_password(User *user) {
    size_t len;
    char *gen_passwd;
    int i;

    len = strlen(user->username);
    gen_passwd = (char *)malloc(len);
    for (i = 0; i < len; i++) {
        gen_passwd[i] = user->realKey ^ (user->key + user->username[i]);
        gen_passwd[i] = gen_passwd[i] - 0x13;
    }
    gen_passwd[len] = '\0';
    user->password = gen_passwd;
    return;
}

int add_mult_8(int a,int b) {
  return (b + a) * 8;
}

void swapNames(char **a, char **b) {
    char *aux = *a;
    *a = *b;
    *b = aux;
}

int main() {
    User user;
    user.key = 1;
    user.realKey = add_mult_8(user.key,user.key + 0xbeef);
    user.username = strdup("matmarqs");
    user.password = strdup("initial");  // Just placeholder
    gen_password(&user);
    for (size_t i = 0; i < strlen(user.password); i++) {
        printf("\\x%02x", (unsigned char)user.password[i]);
    }
    printf("\n");
}
