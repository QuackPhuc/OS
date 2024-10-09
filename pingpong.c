#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main() {
    int p[2];
    pipe(p);

    int check = 1;
    int pid = fork();
    if (pid > 0) {
        write(p[1], &check, sizeof(int));

        wait(0);

        int tmp;
        read(p[0], &tmp, sizeof(int));
        if (tmp == check) {
            printf("%d: received pong\n", getpid());
        } else {
            printf("%d: received error\n", getpid());
        }
        close(p[0]);
        close(p[1]); 
        exit(0);
    } else if (pid == 0) {
        int tmp;
        read(p[0], &tmp, sizeof(int));
        printf("%d: received ping\n", getpid());
        write(p[1], &tmp, sizeof(int));
    }
    exit(0);
}