#include <stdio.h>
#include <sys/types.h>
#include <stdbool.h>
#include <ctype.h>
#include <assert.h>
#include "../deps.h"

#define PROGRAM_NAME "echo"
#define VERSION "0.01"
#define FALLTHROUGH 0
enum { COPYRIGHT_YEAR = 2025 };

const char* AUTHORS = "Authors: Brain Fox, Chet Ramey, and Gurpreet Singh\n";

void usage(int status);
void version_etc(FILE *stream, const char *command_name, const char *authors, const char *version);
static int hextodec(unsigned char c);

int echo(int argc, char **argv);