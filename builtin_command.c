#include "builtin_command.h"

int find_last_char_index(const char *__s, char __c) {
    int last_index = -1;
    for (int i = 0; __s[i] != '\0'; i++) {
        if (__s[i] == __c) {
            last_index = i;
        }
    }
    return last_index;
}

void exit_command() {
    exit(EXIT_SUCCESS);
}

void cd_command(const char* path) {
    // not argument
    if (path == NULL) {
        const char *home_path = getenv("HOME");
        if (home_path == NULL) {
            perror("Not home");
            return;
        }
        path = (char *)home_path;
    }

    char result_path[1024];
    memset(result_path, '\0', sizeof(result_path));
    if (path != NULL && path[0] == '~') {
        const char *home_path = getenv("HOME");
        if (home_path == NULL) {
            perror("Not home");
            return;
        }
        snprintf(result_path, sizeof(result_path), "%s%s", home_path, path+1);
        path = result_path;
    }

    // relative path
    if (path[0] != '/') {
        char cwd[1024];
        if (getcwd(cwd, sizeof(cwd)) == NULL) {
            perror("getcwd");
            return;
        }
        if (path[0] == ',' && path[1] == '/') {
            snprintf(result_path, sizeof(result_path), "%s%s", cwd, path+1);
            path = result_path;
        } else if (path[0] == '.' && path[1] == '.') {
            cwd[find_last_char_index(cwd, '/')] = '\0';
            snprintf(result_path, sizeof(result_path), "%s%s", cwd, path+2);
        }
    }

    if (chdir(path) != 0) {
        perror("cd");
    }
}
