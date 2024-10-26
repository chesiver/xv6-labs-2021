#include "kernel/types.h"
#include "user/user.h"

void sieve(int fd) {
    int start, cur;
    read(fd, &start, sizeof(int));
    printf("prime %d\n", start);
    if (read(fd, &cur, sizeof(int)) != 0) {
        int p[2];
        pipe(p);
        do {
            if (cur % start != 0) {
                write(p[1], &cur, sizeof(int));
            }
        } while (read(fd, &cur, sizeof(int)) != 0);
        close(p[1]);
        if (fork() == 0) {
            sieve(p[0]);
        }
        close(p[0]);
    }
    wait(0);
    exit(0);
}

int main(int argc, char *argv[]) {
    int p[2];
    pipe(p);
    for (int i = 2; i <= 35; i += 1) {
        write(p[1], &i, sizeof(int));
    }
    close(p[1]);

    if (fork() == 0) {
        sieve(p[0]);
    }
    close(p[0]);
    wait(0);
    exit(0);
}
