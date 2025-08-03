#include <stdio.h>
#include <stdlib.h>
#include <sys/ptrace.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>

/* this function does not matter */
void checksum_main() {
    ;
}

int main() {
    if (getenv("LD_PRELOAD")) {   // at address 0x8048820
        exit(0);
    }

    if (ptrace(PTRACE_TRACEME, 0, 1, 0)) {  // PTRACE_TRACEME = 0
        // exits if the program is being traced
        exit(0);
    }

    checksum_main();

    printf("..:: MoreBoredThanYou by niel anthony acuna ::..\n");

 /* socket(2, 1, 0); */
    int fd = socket(PF_INET, SOCK_STREAM, 0);   // ebp-0xc returns file descriptor on success, -1 if error
    // The protocol specifies a particular protocol to be used with the
    // socket.  Normally only a single protocol exists to support a particular
    // socket type within a given protocol family, in which case protocol can
    // be specified as 0.
    if (fd == -1) {
        exit(1);
    }

    checksum_main();

    // connect to port 127.0.0.1:5143
    struct sockaddr_in addr = {    // rbp-0x28
        .sin_family = AF_INET,
        .sin_port = htons(0x1417),
        .sin_addr.s_addr = htonl(0x7f000001)  // 127.0.0.1
    };
    // int var_28h = 2;    // AF_INET = PF_INET = 2
    // uint16_t var_26h = htons(0x1417);   // port 5143
    // int var_24h = 0x100007f;    // 0x7f.0x00.0x00.0x01 = 127.0.0.1 = localhost
    // Explanation: man 7 ip, man sockaddr
    // struct sockaddr_in {
    //     sa_family_t    sin_family; /* address family: AF_INET */
    //     in_port_t      sin_port;   /* port in network byte order */
    //     struct in_addr sin_addr;   /* internet address */
    // };

    if (connect(fd, (struct sockaddr *) &addr, 0x10) == 0) {
        printf("cracked!\n");
    }

    checksum_main();
    close(fd);
    return 0;
}
