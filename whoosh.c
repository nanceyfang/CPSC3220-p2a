#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_LEN 128

int main(){
  
  char error_message[30] = "An error has occurred\n";

  //write(STDERR_FILENO, error_message, strlen(error_message));

  //char *input = malloc(sizeof(char)*MAX_LEN);
  char input[MAX_LEN];

  const char space[4] = " \t\n";

  printf("whoosh> ");

  while (1) {
    fgets(input, MAX_LEN, stdin);

    char *token;
    token = strtok(input, space);

    pid_t pid = 1; // just to initialize for whoosh>

    while (token!=NULL){
   
      // check if input already implemented

      if ( !(strcmp(token, "exit")) ){
        //free(input);
        exit(0);
      }

      if ( !(strcmp(token, "pwd")) ){
        pid = fork();
        //parent process
        if (pid){
          //wait for child to finish
          int status;
          waitpid(pid, &status, 0);
        }
        else{ //child process
          char cwd[MAX_LEN];
          printf("%s\n", getcwd(cwd, sizeof(cwd)));
          exit(0);
        }

      }

      if ( !(strcmp(token, "cd")) ){
          token = strtok(NULL, space);
          if (token == NULL){
            chdir( getenv("HOME") );
          }
      }

      token = strtok(NULL, space);
      if (pid){
        printf("whoosh> ");
      }
    }
  }

  exit(0);
}
