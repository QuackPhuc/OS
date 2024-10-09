#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void seive(int p[]) __attribute__((noreturn));

void seive(int p[]) {
    close(p[1]);
    
    int prime;
    if (!read(p[0], &prime, sizeof(prime))) {
        close(p[0]);
        exit(0);
    }

    printf("prime %d\n", prime);

    int rp[2];
    pipe(rp);

    if (fork()) {
        close(p[0]);
        seive(rp);
    } else {
        close(rp[0]);
        
        int num;

        while (read(p[0], &num, sizeof(num)) != 0) {
            if (num % prime != 0) {
                write(rp[1], &num, sizeof(num));
            }
        }
        close(p[0]);
        close(rp[1]);
        wait(0);
    }

    exit(0);
}

int main(int argc, char *argv[]) {
    int p[2];
    pipe(p);

    if (fork() == 0) {
        close(p[0]);
        for (int i = 2; i <= 280; i++) {
            write(p[1], &i, sizeof(i));
        }
        close(p[1]);
        exit(0);
    } else {
        close(p[1]);
        seive(p);
        close(p[0]);
        wait(0);
    }
    exit(0);
}

