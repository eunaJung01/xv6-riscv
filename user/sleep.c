#include "user/user.h"

int
main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("[Error] Argument required: number of ticks to sleep\n");
        exit(-1);
    }
    int ticks_to_sleep = atoi(argv[1]);

    if (sleep(ticks_to_sleep) != 0) {
        printf("[Error] Error in sleep system call\n");
    }
    exit(0);
}
