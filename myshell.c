#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <pwd.h>
#include "builtin_command.h"

#define SIZE 1024

char *command_argv[SIZE];

char copy_env[SIZE];

void set_cmd() {
    char cwd[1024];
    char hostname[128];

    gethostname(hostname, sizeof(hostname));

    memset(cwd, '\0', sizeof(cwd));
    getcwd(cwd, sizeof(cwd));
    printf("%s@%s-[%s]\n> ", getlogin(), hostname, cwd);  //  This is CMD
}

int main() {
    while (1) {
        char command[SIZE];
        memset(command, '\0', SIZE);

        set_cmd();
        fgets(command, SIZE, stdin);  //  Get user inputed command
        command[strcspn(command, "\n")] = '\0'; // remove '\n'

        //  cut the command[]
        command_argv[0] = strtok(command, " ");
        int index = 1;

        //  return NULL when function strtok cannot cut string, it can as loop break condition
        while ( command_argv[index++] = strtok(NULL, " ") );

        //  command ls set colors
        if ( strcmp(command_argv[0], "ls") == 0 ) {
            command_argv[index++] = "--color=auto";
        }

        //  alias ls -l -> ll
        if ( strcmp(command_argv[0], "ll") == 0 ) {
            command_argv[0] = "ls";
            command_argv[index++] = "-l";
            command_argv[index++] = "--color=auto";
        }

        //  build in shell
        if ( strcmp(command_argv[0], "cd") == 0) {
            cd_command(command_argv[1]);
            continue;
        }
        if ( strcmp(command_argv[0], "exit") == 0 ) {
            exit_command();
        }

        //  create a child process and instead
        pid_t fpid = fork();
        if ( fpid == 0 ) {
            if (execvp(command_argv[0], command_argv)) {
                printf("command not found: %s\n", command_argv[0]);
            }
            exit(-1);  //  when instead failed, exit and return -1
        }
        //  The parent process recovered child process
        int status = 0;
        pid_t ret = waitpid(fpid, &status, 0);
        if ( ret < 0 ) {
            exit(-1);
        }
    }
    return 0;
}
