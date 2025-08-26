#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pwd.h>

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

#define MAX_PATH_LENGTH 4096

typedef struct {
    char** argv;
    int argc;
} InputBuffer;

typedef enum { PREPARE_SUCCESS, PREPARE_FAILURE, PREPARE_SYNTAX_ERROR, PREPARE_UNRECOGNIZED_COMMAND = -1 } PrepareResult;

// Declarations

/*
    Prints shell prompt symbol
    Arg: 
        None
    
    Desc: Prints the shell prompt symbol to stdout (default '$')

    returns:
        None
*/
void print_prompt();


/*
    Reads from stdin into InputBuffer object
    Arg: (required) InputBuffer*
        input_buffer
    
    Desc: Reads from stdin into the InputBuffer object's buffer. 
            Updates input_length field.
    
    returns:
        None
*/
void read_input(InputBuffer* input_buffer);


/*
    Create new InputBuffer object
    Arg: 
        None
    
    Desc: Allocates a new InputBuffer object with default NULL buffer,
            input_lenth of 0, and buffer_length of 0.
    
    returns:
        InputBuffer*
*/
InputBuffer* new_input_buffer();


/*
    Closes input buffer object
    Arg: (required) (InputBuffer*)
        input_buffer
    
    Desc: Given InputBuffer object, frees
            internal buffer and entire object itself from heap
    
    returns:
        None
*/
void close_input_buffer(InputBuffer* input_buffer);


/*
    Builtin exit command 
    (meta) Arg: (optional) status_code (default = 0)
        status_code is a positive integer ranged from 0 to 255
    
    Desc: Reads from InputBuffer object to interpret for exit command
            NOTE: To avoid memory leaks, the provided InputBuffer object is 
                attempted to be closed on a successful exit command.
    
    Arg: (InputBuffer*)
        input_buffer

    returns (PrepareResult):
        PREPARE_SYNTAX_ERROR 
        PREPARE_UNRECONGIZED_COMMAND
        OR 
        Successfully exits
    
*/
PrepareResult process_exit(InputBuffer* input_buffer);


/*
    Builtin echo command
    Args: (required) int, (required) char**
        int argc, char** argv
        Just as the arguments produced by the OS ABI in C
    
    Desc: Prints command argument to stdout
            NOTE: Does NOT support options
    returns: int
        EXIT_SUCCESS
*/
int echo(int argc, char **argv);

/*
    Spawns child process to run programs. 
    Arg: (required) (InputBuffer*)
        input_buffer
    
    Desc: Spawns a child process to execute the command from the input_buffer

    returns: int
        process exit status OR process creation error code
    
*/
int spawn_child(int argc, char** argv);


/*
    Tokenizes buffer into an argv like array delimited 
    Arg: (char*) 
        input
    
    Desc: Tokenizes a character array into an argv like array of strings. 

    returns: (char**)
        argv
*/
char** tokenize(char* input, int* argc);

void process_type(InputBuffer* input_buffer);

int is_command(char* command);


void free_argv(char** argv);