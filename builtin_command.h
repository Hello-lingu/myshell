#ifndef BUILTIN_COMMAND_H
#define BUILTIN_COMMAND_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void exit_command();
void cd_command(const char *path);

#endif
