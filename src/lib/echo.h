#include <stdbool.h>
#include <ctype.h>
#include "../deps.h"

#define PROGRAM_NAME "echo"
#define VERSION "9.7 + 0.01"
#define FALLTHROUGH NULL
enum { COPYRIGHT_YEAR = 2025 };

const char* AUTHORS = "Authors: Brain Fox, Chet Ramey, and Gurpreet Singh\n";

void usage(int status);
void version_etc(FILE *stream, const char *command_name, const char *authors, const char *version);
static int hextodec(unsigned char c);
