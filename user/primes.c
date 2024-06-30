#include "user/user.h"

int
is_prime(int number) {
    if (number <= 1) {
        return 0;
    }

    for (int i = 2; i < number / 2; i++) {  // actually.. while i < sqrt(number) + 1
        if (number % i == 0) {
            return 0;
        }
    }
    return 1;
}

int
main() {
    int buffer[1];

    int pipe_for_numbers[2];
    pipe(pipe_for_numbers);

    if (fork() != 0) {
        // parent process
        close(pipe_for_numbers[0]);

        for (int i = 2; i <= 35; i++) {
            buffer[0] = i;

            if (write(pipe_for_numbers[1], buffer, sizeof(buffer)) != sizeof(buffer)) {
                fprintf(2, "parent: write error\n");
                exit(1);
            }
        }
        close(pipe_for_numbers[1]);
        wait(0);
        exit(0);

    } else {
        // child process
        close(pipe_for_numbers[1]);

        while (read(pipe_for_numbers[0], buffer, sizeof(buffer) > 0)) {

            if (is_prime(buffer[0])) {
                int pipe_for_prime[2];
                pipe(pipe_for_prime);

                if (fork() != 0) {
                    close(pipe_for_prime[0]);

                    if (write(pipe_for_prime[1], buffer, sizeof(buffer)) != sizeof(buffer)) {
                        fprintf(2, "child: write error\n");
                        exit(1);
                    }
                    close(pipe_for_prime[1]);
                    wait(0);

                } else {
                    // grandchild process
                    close(pipe_for_prime[1]);

                    while (read(pipe_for_prime[0], buffer, sizeof(buffer)) != sizeof(buffer)) {
                        fprintf(2, "grandchild: read error\n");
                        exit(1);
                    }
                    printf("prime %d\n", buffer[0]);

                    close(pipe_for_prime[0]);
                    exit(0);
                }
            }
        }
        close(pipe_for_numbers[0]);
        exit(0);
    }
}
