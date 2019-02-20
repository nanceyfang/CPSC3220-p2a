#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <dirent.h>
#include <stdbool.h>

#define MAX_LEN 128

int main(){
  
  char error_message[30] = "An error has occurred\n";


  //write(STDERR_FILENO, error_message, strlen(error_message));

  //char *input = malloc(sizeof(char)*MAX_LEN);
  char input[MAX_LEN];
	bool built_in = false;

  const char space[4] = " \t\n";

  printf("whoosh> ");

  while (1) {
    fgets(input, MAX_LEN, stdin);

    char *token;
    token = strtok(input, space);

    pid_t pid = 1; // just to initialize for whoosh>

    while (token!=NULL){
  		 
      if ( !(strcmp(token, "exit")) ){
        //free(input);
        exit(0);
      }

			built_in = false;

      if ( !(strcmp(token, "pwd")) ){
				built_in = true;
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

			if ( !(strcmp(token, "path")) ){
				built_in = true;
				//uh i think i need to fix this somehow
				printf("%s\n", getenv("PATH") );
      }

			if ( !(strcmp(token, "cd")) ){
					built_in = true;
          token = strtok(NULL, space);
          if (token == NULL){
            chdir( getenv("HOME") );
          }
					else{ // cd to token
						chdir( token );
					}
      }

			if (token!=NULL && !built_in){ 
      	// check if input already implemented
				// go to /bin or /usr/bin and check
				DIR *d; // like a file?
				struct dirent *directory;
				d = opendir("/bin");
				if(d){
					directory=readdir(d);
					while (directory != NULL){
						// command in /bin
						if ( !(strcmp(token,directory->d_name)) ){
							//printf("match");

							int argc = 0;
							
							printf("%s\n", input);

							pid = fork();
							if (pid){
								int status;
			          waitpid(pid, &status, 0);
							}
							else{
								char command[MAX_LEN];
								strcpy(command, "/bin/");
								strcat(command, token);

								// go through input for arguments?
								for (int i = 0; i < strlen(input); i++){
									printf("%c", input[i]);
								}			

								char *args[] = {token, NULL, NULL, NULL};
								execv(command, args);
							}			
							break;
						}		
						directory=readdir(d);
					}
					closedir(d);
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
