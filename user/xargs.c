#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fcntl.h"
#include "kernel/fs.h"
#include "kernel/param.h"
#include "user/user.h"

char *readline(char *buf, int max) {
    int i, cc;
    char c;
    for (i = 0; i + 1 < max;) {
        cc = read(0, &c, 1);
        if (cc < 1) break;
        if (c == '\n' || c == '\r') break;
        buf[i++] = c;
    }
    buf[i] = '\0';
    return buf;
}

int main(int argc, char *argv[]) {
    char buf[256];
    char *extra_argv[MAXARG + 1];
    extra_argv[0] = "xargs";
    for (int i = 2; i < argc; i += 1) {
        extra_argv[i - 1] = malloc(256);
        strcpy(extra_argv[i - 1], argv[i]);
    }
    extra_argv[argc - 1] = malloc(256);
    extra_argv[argc] = 0;
    while (readline(buf, 256) && strlen(buf) > 0) {
        strcpy(extra_argv[argc - 1], buf);
        if (fork() == 0) {
            exec(argv[1], extra_argv);
        }
        wait(0);
    }
    exit(0);
}