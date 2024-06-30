#include "user/user.h"
#include "kernel/stat.h"
#include "kernel/fs.h"

#define SIZE_OF_BUFFER 512

const char *file_name;

void
find_file(const char *path) {
    // validate the directory path
    int fd;
    if ((fd = open(path, 0)) < 0) {
        fprintf(2, "cannot open %s\n", path);
        return;
    }

    struct stat dir_stat;
    if (fstat(fd, &dir_stat) < 0) {
        fprintf(2, "cannot open stat %s\n", path);
        close(fd);
        return;
    }
    if (dir_stat.type != T_DIR) {
        close(fd);
        return;
    }

    if (strlen(path) + 1 + DIRSIZ + 1 > SIZE_OF_BUFFER) {
        fprintf(2, "path name is too long\n");
        return;
    }

    // set a buffer which stores the path
    char buffer[SIZE_OF_BUFFER];
    strcpy(buffer, path);

    char *p = buffer + strlen(buffer);
    *p++ = '/';

    // strcmp if the entry is a file || recurse if the entry is a directory
    struct dirent entry;
    while (read(fd, &entry, sizeof(entry)) == sizeof(entry)) {

        if (entry.inum == 0) {
            continue;
        }
        if (strcmp(entry.name, ".") == 0 || strcmp(entry.name, "..") == 0) {
            continue;
        }

        memmove(p, entry.name, DIRSIZ);
        p[DIRSIZ] = 0;

        struct stat entry_stat;
        if (stat(buffer, &entry_stat) < 0) {
            fprintf(2, "cannot open stat %s\n", buffer);
            continue;
        }

        if (entry_stat.type == T_FILE && strcmp(entry.name, file_name) == 0) {
            printf("%s\n", buffer);
            continue;
        }
        if (entry_stat.type == T_DIR) {
            find_file(buffer);
        }
    }
}

int
main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(2, "Usage: find <path> <file_name>\n");
        exit(1);
    }

    file_name = argv[2];
    find_file(argv[1]);
}
