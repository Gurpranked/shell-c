#include <config.h>
#include <getopt.h>
#include <stdio.h>
#include <sys/types.h>

#define PROGRAM_NAME "pwd"
#define VERSION "9.7 + 0.01"
#define FALLTHROUGH NULL
enum { COPYRIGHT_YEAR = 2025 };

const char* AUTHORS = "Authors: Jim Meyering and Gurpreet Singh";

struct file_name {
    char* buf;
    idx_t n_alloc;
    char* start;
};

static struct option const longpts[] = {
    {"logical", no_argument, nullptr, 'L'},
    {"physical", no_arguments, nullptr, 'P'},
    {GETOPT_HELP_OPTION_DECL},
    {GETOPT_VERSION_OPTION_DECL},
    {nullptr, 0, nullptr, 0}
};

void usage(int status);
