#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
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

  char *token;
	char *input_copy = malloc(sizeof(char*)*strlen(input));

  while (1) {
  	printf("whoosh> ");
    
		fgets(input, MAX_LEN, stdin);
		strcpy(input_copy, input);
    token = strtok(input_copy, space);
    pid_t pid = 1; // just to initialize for whoosh>

  	if (token!=NULL){	 
      if ( !(strcmp(token, "exit")) ){
        free(input_copy);
        exit(0);
      }
			
			bool too_long = false;

			printf("\nsize: %d\ninput: %s\n", sizeof(input), input);
			if (sizeof(input) > 128){
      	write(STDERR_FILENO, error_message, strlen(error_message));
				printf("\ntoolong\n");
				too_long=true;
			}

			if (too_long == false){

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
			
				bool skip = true;
				if (token!=NULL && !built_in){ 
  	    	// check if input already implemented
					// go to /bin or /usr/bin and check
					char command1[MAX_LEN];
					strcpy(command1, "/bin/");
					strcat(command1, token);
	
					char command2[MAX_LEN];
					strcpy(command2, "usr/bin/");
					strcat(command2, token);

					char command[MAX_LEN];
					struct stat fileStat;
	
					if(stat(command1, &fileStat) >= 0 ){
						skip = false;
						strcpy(command, command1);
					}
					else if(stat(command2, &fileStat) >= 0 ){
						skip = false;
						strcpy(command,command2);
					}
					else{
						write(STDERR_FILENO, error_message, strlen(error_message));	
						//skip = true;
					}

					if (skip==false){							
						pid = fork();
						if (pid){
							int status;
				  		waitpid(pid, &status, 0);
						}
						else{
							int argc = 1; //start at 1 bc apparently you need an extra NULL arg
							// go through input for arguments?
							while (token!=NULL) {
								token = strtok(NULL, space);
								argc++;
							}
							
							char *args[argc];

							for (int j = 0; j < argc; j++){
								args[j]=NULL;
							}

							// go through input to get arguments
							char * token2;
							int i = 0;
							token2 = strtok(input, space);
							while (token2!=NULL){
								args[i]=token2;
								token2 = strtok(NULL,space);
								i++;
							}
						
							execv(command, args);
							exit(0);
						}
					}		
				}	
			}

      if (pid){
        int status;
        waitpid(pid, &status, 0);
      }
//this over here officer
    }
  }

  exit(0);
}
