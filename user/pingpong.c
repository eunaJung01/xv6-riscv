#include "user/user.h"

int
main() {
    int parent_to_child[2];
    int child_to_parent[2];
    char buffer[1];

    pipe(parent_to_child);
    pipe(child_to_parent);

    if (fork() == 0) {
        // child process
        close(parent_to_child[1]);
        close(child_to_parent[0]);

        while (read(parent_to_child[0], buffer, sizeof(buffer)) != sizeof(buffer)) {
            fprintf(2, "child: read error\n");
            exit(1);
        }
        printf("<%d>: received ping\n", getpid());

        if (write(child_to_parent[1], buffer, sizeof(buffer)) != sizeof(buffer)) {
            fprintf(2, "child: write error\n");
            exit(1);
        }

        close(parent_to_child[0]);
        close(child_to_parent[1]);
        exit(0);

    } else {
        // parent process
        close(parent_to_child[0]);
        close(child_to_parent[1]);

        buffer[0] = 'a';
        if (write(parent_to_child[1], buffer, sizeof(buffer)) != sizeof(buffer)) {
            fprintf(2, "parent: write error\n");
            exit(1);
        }

        while (read(child_to_parent[0], buffer, sizeof(buffer)) != sizeof(buffer)) {
            fprintf(2, "parent: read error\n");
            exit(1);
        }
        printf("<%d>: received pong\n", getpid());

        close(parent_to_child[0]);
        close(child_to_parent[1]);
        exit(0);
    }
}
