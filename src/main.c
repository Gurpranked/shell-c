#include "deps.h"

// NOTE: Update alongside cmd_list 
#define CMD_LIST_SIZE 5
const char* cmd_list[] = {"echo", "exit", "type", "cd", "pwd"};

int main() {
  // Flush after every printf
  setbuf(stdout, NULL);

  if ((homedir = getenv("HOME")) == NULL)
    homedir = getpwuid(getuid())->pw_dir;

  char* input = NULL;
  ssize_t bytes_read;
  size_t len;
  printf("Shell-C (type `exit` to quit)\n");

  while (true) {
    print_prompt();
    bytes_read = getline(&input, &len, stdin);
    if (bytes_read == -1) {
      // EOF or error
      if (feof(stdin))
        printf("Exiting...");
      else
        perror("getline failed");
      // break out of loop and terminate 
      break;
    }
    int argc;
    char** argv = tokenize(input, &argc); 

    if (argc > 0) {
      // exit
      if (strcmp(argv[0], "exit") == 0) {
        int code = 0;
        // Default no argument to exit code 0
        if (argc == 1) {
          free_argv(argv);
          free(input);
          exit(0);
        }
        // Get exit code
        else {
          code = atoi(argv[1]);
        }

        // Validate
        if (code < 0 || code > 255) {
          printf(ANSI_COLOR_RED "Syntax Error: " ANSI_COLOR_RESET "Invalid exit code: %d (0-255)\n", code);
          continue;
        }

        // Exit
        free_argv(argv);
        free(input);
        exit(code);
      }

      // echo
      else if (strcmp(argv[0], "echo") == 0) {
        echo(argc, argv);
      } 

      // type
      else if (strcmp(argv[0], "type") == 0) {
        if (argc > 1) {
          for (int i = 1; i < argc; i++) {
            if (is_alias(argv[1])) {
              printf("%s is a shell alias\n", argv[1]);
            }
            else if (is_keyword(argv[1])) {
              printf("%s is a shell keyword\n", argv[1]);
            }
            else if (is_function(argv[1])) {
              printf("%s is a shell function\n", argv[1]);
            } 
            else if (is_builtin(argv[1])) {
              printf("%s is a shell builtin\n", argv[1]);
            }
            else if (is_executable(argv[1])) {
              printf("%s is an executable program\n", argv[1]);
            }
            else {
              printf(ANSI_COLOR_RED "Unrecognized command: " ANSI_COLOR_RESET "%s\n", argv[1]);
            }
          }
        } 
        else {
          printf(ANSI_COLOR_RED "Syntax error: " ANSI_COLOR_RESET "Invalid arguments\n");
          printf("\tUsage: type COMMAND_NAME\n");
        }
      }
      
      // cd
      else if (strcmp(argv[0], "cd") == 0) {
        if (argc == 1) {
          if (chdir(homedir) != 0) 
            printf(ANSI_COLOR_YELLOW "Error: " ANSI_COLOR_RESET "Invalid Path\n");
        }
        else if (argv[1][0] == '~') {
          if (chdir(homedir) != 0) 
            printf(ANSI_COLOR_YELLOW "Error: " ANSI_COLOR_RESET "Invalid Path\n");
        }
        else if (chdir(argv[1]) != 0) {
          printf(ANSI_COLOR_YELLOW "Error: " ANSI_COLOR_RESET "Invalid Path\n");
        }
      }

      // pwd
      else if (strcmp(argv[0], "pwd") == 0) {
        getcwd(cwd, sizeof(cwd));
        printf("%s\n", cwd);
      }
      
      // Unrecognized command
      else {
        printf(ANSI_COLOR_YELLOW "Unrecognized command: " ANSI_COLOR_RESET "%s\n", argv[0]);
      }
    }
    next:
    // Memory cleanup
    free_argv(argv);   
  }
	free(input);
  return 0;
}

void print_prompt() { 
  getcwd(cwd, sizeof(cwd));
  if (strcmp(cwd, homedir) == 0) {
    printf(ANSI_COLOR_BLUE "~ $ " ANSI_COLOR_RESET); 
    return;
  }
  char* folderName = strrchr(cwd, '/');
  if (folderName != NULL){
    folderName++; // Move past the '/'
  }
  printf(ANSI_COLOR_BLUE "(%s) $ " ANSI_COLOR_RESET, folderName); 
}

int is_builtin(char* input) {
  for(int i = 0; i < CMD_LIST_SIZE; i++) {
    if (strcmp(input, cmd_list[i]) == 0) {
      return 1; 
    }
   }
  
  return 0; 
}


void free_argv(char** argv) {
  if (argv == NULL) {
    return;
  }

  for (int i = 0; argv[i] != NULL; i++) {
    free(argv[i]);
  }
  free(argv);
}

char** tokenize(char* input, int* argc) {  
  if (input == NULL || *input == '\0') {
    *argc = 0;
    return NULL;
  }

  // Temp copy of input to tokenize
  char* input_copy = strdup(input);
  if (input_copy == NULL) {
    perror("strdup failed");
    exit(EXIT_FAILURE);
  } 

  // Calculate argc
  char* temp_input = input_copy;
  char* token = strtok(temp_input, " \t\n");
  int count = 0;
  while (token != NULL) {
    count++;
    token = strtok(NULL, " \t\n");
  }

  // Allocate memory for argv
  char** argv = (char**)malloc(sizeof(char*) * (count + 1));
  if (argv == NULL) {
    perror("malloc failed");
    free(input_copy);
    exit(EXIT_FAILURE);
  }


  // Reset strtok and tokenize original string
  free(input_copy);
  input_copy = strdup(input);
  if (input_copy == NULL) {
    perror("strdup failed");
    exit(EXIT_FAILURE);
  }

  int i = 0;
  token = strtok(input_copy, " \t\n");
  while (token != NULL) {
    argv[i] = strdup(token);
    if (argv[i] == NULL) {
      perror("strdup failed");
      // clean already allocated memory
      for (int j = 0; j < i; j++) 
        free(argv[j]);
      free(argv);
      free(input_copy);
      exit(EXIT_FAILURE);
    }
    i++;
    token = strtok(NULL, " \t\n");
  }
  argv[i] = NULL;
  *argc = count;
  
  free(input_copy);
  return argv;
}


int spawn_child(int argc, char** argv) {
    pid_t p = fork();
    pid_t child_pid;

    // Fork failed
    if (p<0) {
      perror("Error: Fork Failed");
      return PREPARE_FAILURE; 
    }
    // Child proc
    else if (p == 0) {
      // Run command in child process
      execvp(argv[0], argv);
      // In case the parent process execution fails the exec call
      perror("Exec failed");
      free_argv(argv);
      exit(EXIT_FAILURE);
    }
    // Parent proc
    else { 
      int status;
      int child_pid = waitpid(p, &status, 0);
      // Return child process exit status
      if (WIFEXITED(status)) {
        return WEXITSTATUS(status);
      }
    }
}