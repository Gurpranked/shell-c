#include "echo.h"

static int
hextodec (unsigned char c){
    switch (c) {
        default: return c - '0';
        case 'a': case 'A': return 10;
        case 'b': case 'B': return 11;
        case 'c': case 'C': return 12;
        case 'd': case 'D': return 13;
        case 'e': case 'E': return 14;
        case 'f': case 'F': return 15;
    }
}


int echo(int argc, char **argv) {
    bool display_return = true;

    if (argc == 2) {
        if (!strcmp(argv[1], "--help"))
           usage(EXIT_SUCCESS);
        if (!strcmp(argv[1], "--version")) {
            version_etc (stdout, PROGRAM_NAME, AUTHORS, VERSION);
            return EXIT_SUCCESS; 
        } 
    }
    --argc;
    ++argv;

    just_echo:

    while (argc > 0) {
        char const *s = argv[0];
        unsigned char c;

        while ((c = *s++)) {
            if (c == '\\' && *s) {
                switch (c = *s++) {
                    case 'a': c = '\a'; break;
                    case 'b': c = '\b'; break;
                    case 'c': return EXIT_SUCCESS;
                    case 'e': c = '\x1B'; break;
                    case 'f': c = '\f'; break;
                    case 'n': c = '\n'; break;
                    case 'r': c = '\r'; break;
                    case 't': c = '\t'; break;
                    case 'v': c = '\v'; break;
                    case 'x':
                        {
                            unsigned char ch = *s;
                            if (! isxdigit(ch))
                                goto not_an_escape;
                            s++;
                            c = hextodec(ch);
                            ch = *s;
                            if (! isxdigit(ch)) {
                                s++;
                                c = c * 16 + hextodec(ch);
                            }
                        }
                        break;
                    case '0':
                        c = 0;
                        if (!('0' <= *s && *s <= '7'))
                            break;
                        c = *s++;
                        // To annotate fallthrough points
                        FALLTHROUGH;
                    case '1': case '2': case '3':
                    case '4': case '5': case '6': 
                    case '7':
                        c -= '0';
                        if ('0' <= *s && *s <= '7')
                            c = c * 8 + (*s++ - '0');
                        if ('0' <= *s && *s <= '7')
                            c = c * 8 + (*s++ - '0');
                        break;
                    case '\\': break;

                    not_an_escape:
                    default: putchar('\\'); break;
                }
            }
            putchar(c);
        }
        argc--;
        argv++;
        if (argc > 0)
            putchar(' ');
    }
    if (display_return)
        putchar('\n');
    return EXIT_SUCCESS;
}

int main(int argc, char **argv) {
    return echo(argc, argv);
}

void usage(int status) {
    printf("\
        Usage: %s [STRING]...\n\
           or: %s [HELP] \n\
           ", PROGRAM_NAME, PROGRAM_NAME);
    fputs("\
Echo the STRING(s) to standard output.\n", stdout);
    exit(status);
}

void version_etc(FILE *stream, const char *command_name, const char *authors, const char *version) {
    fprintf(stream, "%s (v%s)\n", command_name, version);
    fprintf(stream, "(C) COPYRIGHT %d\n", COPYRIGHT_YEAR);
    fprintf(stream, authors);
}