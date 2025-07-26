#include <stdio.h>

int main() {
    char password[11] = {0};
    char *hackaday_u = "hackaday-u";
    for (int i = 0; i < 10; i++) {
        password[i] = hackaday_u[i] + 2;
    }
    puts(password);
}
