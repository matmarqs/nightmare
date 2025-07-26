#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct {
    int key;
    char *name;
    int dontUse;
    char *pass;
    int calcKey;
} User;

void gen_password(User *user) {
  size_t len_username;
  char *gen_pass;
  int i;
  
  len_username = strlen(user->name);
  gen_pass = (char *)malloc((long)(int)len_username);
  for (i = 0; i < (int)len_username; i = i + 1) {
    gen_pass[i] = user->calcKey ^ ((char)user->key + user->name[i]);
    gen_pass[i] = gen_pass[i] + -0x13;
  }
  user->pass = gen_pass;
  return;
}

int add_mult_8(int a,int b) {
  return (b + a) * 8;
}

int main() {
  User user;
  user.key = 0x31313131;
  user.name = strdup("matmarqs");
  user.calcKey = add_mult_8(user.key, user.key + 0xbeef);
  gen_password(&user);
  printf("%s", user.pass);
}
