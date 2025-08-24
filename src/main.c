#include "deps.h"

int main() {
  // Flush after every printf
  setbuf(stdout, NULL);
  InputBuffer* input_buffer = new_input_buffer();

  while (true) {
    print_prompt();
    read_input(input_buffer);
    if (process_exit(input_buffer) == PREPARE_UNRECOGNIZED_COMMAND) {
      printf(ANSI_COLOR_YELLOW "Unrecognized command: " ANSI_COLOR_RESET "%s\n", input_buffer->buffer);
    } 
  }
  close_input_buffer(input_buffer);
	return 0;
}

void print_prompt() { printf("$ "); }

void read_input(InputBuffer* input_buffer) {
  ssize_t bytes_read = getline(&(input_buffer->buffer), &(input_buffer->buffer_length), stdin);
  if (bytes_read <= 0) {
    printf("Error reading input\n");
    exit(EXIT_FAILURE);
  }

  // Ignore trailing newline
  input_buffer->input_length = bytes_read - 1;
  input_buffer->buffer[bytes_read - 1] = 0;
}

InputBuffer* new_input_buffer() {
  InputBuffer* input_buffer = (InputBuffer*) malloc(sizeof(InputBuffer));
  input_buffer->buffer = NULL;
  input_buffer->buffer_length = 0;
  input_buffer->input_length = 0;
  return input_buffer;
}

void close_input_buffer(InputBuffer* input_buffer) {
  free(input_buffer->buffer);
  input_buffer->buffer = NULL;
  free(input_buffer);
  input_buffer = NULL;
}

PrepareResult process_exit(InputBuffer* input_buffer) {
  if (strncmp(input_buffer->buffer, "exit", 4) == 0) {
    char char_code[4];
    sscanf((input_buffer->buffer) + 4, "%3s", char_code);
    // atoi ensures that invalid or null characters are defaulted to 0
    int code = atoi(char_code);
    if (code < 0 || code > 255) {
      printf(ANSI_COLOR_RED "Syntax Error: " ANSI_COLOR_RESET "Invalid exit code: %d (0-255)\n", code);
      return PREPARE_SYNTAX_ERROR;
    }
    close_input_buffer(input_buffer);
    exit(code);
  }
  else {
    return PREPARE_UNRECOGNIZED_COMMAND;
  }
}

int is_command(char* command) {
  for(int i = 0; i < CMD_LIST_SIZE; i++) {
    if (strcmp(command, cmd_list[i]) == 0) {
      return i;
    }
    else if(strcmp(command, cmd_list[i]) < 0) {
      return -1;
    }
  }
  return -1;
}

void process_type(InputBuffer* input_buffer) {
  if (strncmp(input_buffer->buffer, "type", 4) == 0) {
    char* rest = input_buffer->buffer;
    char* arg = strtok_r(input_buffer->buffer, " ", &rest);
    int cmd_int;
    while (arg != NULL) {
      cmd_int = is_command(arg);
      switch(cmd_int){
        case 0:
          fprintf(stdout, "%s\n")
      } 

      // Get next argument
      arg = strtok_r(NULL, " ", &rest);
    }
  }
}


int tokenize(char* input, char ***tokens) {  
  int argc = 0;
  // Get the first token
  char* rest = input;
  char* token = strtok_r(input, " ", &rest);

  // Get the rest of the tokens
  while (token != NULL) {
    (*tokens)[argc++] = token;
    token = strtok_r(NULL, " ", &rest);
  }

  return argc;
}


void spawn_child(InputBuffer* input_buffer) {
    char* executable_filename = get_executable(input_buffer);
    
    // Tokenize to argv for command
    char*** argv;
    int argc = tokenize(input_buffer->buffer, argv);
    pid_t pid = fork();
    pid_t child_pid;

    if (p<0) {
      perror("Error: Fork Failed");
      exit(EXIT_FAILURE);
    }
    // Child process
    else if (p == 0) {
      // Run command in child process
      execvp(executable_filename, argv);
    }
    else
      child_pid = wait(NULL);


}

