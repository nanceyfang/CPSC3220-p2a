#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_LEN 128

int main(){
  
  //char *input = malloc(sizeof(char)*MAX_LEN);
  char input[MAX_LEN];

  printf("Shell start...\n");

  const char space[4] = " \t\n";

  while (1) {
    fgets(input, MAX_LEN, stdin);

    char *token;
    token = strtok(input, space);

    while (token!=NULL){
      printf("%s\n",token);
      if ( !(strcmp(token, "exit")) ){
        //free(input);
        exit(0);
      }

      if ( !(strcmp(token, "pwd")) ){
        pid_t pid = fork();
        //parent process
        if (pid){
          //wait i guess?
        }
        else{ //child process
          char cwd[MAX_LEN];
          printf("%s\n", getcwd(cwd, sizeof(cwd)));
          exit(0);
        }

      }

      token = strtok(NULL, space);
    }
  }

  exit(0);
}
