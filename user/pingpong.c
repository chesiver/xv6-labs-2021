#include "kernel/types.h"
#include "user/user.h"

void panic(char *s) {
    fprintf(2, "panic in %s\n", s);
    exit(1);
}

int main(int argc, char *argv[]) {
    int p[2];
    if (pipe(p) < 0) {
        panic("pipe");
    }
    uchar buf[1];
    write(p[0], buf, 1);
    close(p[0]);
    if (fork() == 0) {
        read(p[0], buf, 1);
        write(p[1], buf, 1);
        close(p[1]);
        fprintf(1, "%d: received ping\n", getpid());
    } else {
        wait(0);
        read(p[1], buf, 1);
        fprintf(1, "%d: received pong\n", getpid());
    }
    exit(0);
}
