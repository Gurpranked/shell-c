#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

typedef struct {
    char* buffer;
    size_t buffer_length;
    ssize_t input_length;
} InputBuffer;

typedef enum { PREPARE_SUCCESS, PREPARE_SYNTAX_ERROR, PREPARE_UNRECOGNIZED_COMMAND } PrepareResult;

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
    Arg: (InputBuffer*)
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
    (meta) Arg: (optional) text (default = '\n')
        text is a null-terminated char array

    Desc: Prints command argument to stdout
            NOTE: Does NOT support options
    returns:
        PREPARE_UNRECOGNIZED_STATEMENT
        PREPARE_SUCCESS

*/
PrepareResult process_echo(InputBuffer* input_buffer);