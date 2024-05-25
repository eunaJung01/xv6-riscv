#include "types.h"
#include "user.h"

int
main(int argc, char *argv[]) {
    if (argc != 2) {
        printf(2, "You should pass an argument: number of ticks to sleep\n");
        exit();
    }

    int ticks_to_sleep = atoi(argv[1]);
    if (sleep(ticks_to_sleep) != 0) {
        printf(2, "Error in sleep sys_call\n");
    }
    exit();
}
