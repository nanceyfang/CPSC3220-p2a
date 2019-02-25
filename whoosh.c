#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <dirent.h>
#include <stdbool.h>

#define MAX_LEN 128

int main(){
  
  char error_message[30] = "An error has occurred\n";

  //write(STDERR_FILENO, error_message, strlen(error_message));

  //char *input = malloc(sizeof(char)*MAX_LEN);
  char input[MAX_LEN+50];
	bool built_in = false;

  const char space[4] = " \t\n";

  char *token;
	char *input_copy = malloc(sizeof(char*)*strlen(input));

  int num_path = 0;
//  putenv("PATH=/");

  char path_name[MAX_LEN];
  strcpy(path_name, "");

  while (1) {
  	printf("whoosh> ");
    
		fgets(input, MAX_LEN+50, stdin);
		strcpy(input_copy, input);
    token = strtok(input_copy, space);
    pid_t pid = 1; // just to initialize for whoosh>

  	if (token!=NULL){	 
      if ( !(strcmp(token, "exit")) ){
        free(input_copy);
        exit(0);
      }
			
			bool too_long = false;

			if ( strlen(input) > MAX_LEN ){
      	write(STDERR_FILENO, error_message, strlen(error_message));
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
            
            //printf("ip: %s\n", input_copy);

            token = strtok(NULL, space);

            // check if redirection > 
            if ( token != NULL && !strcmp(token,">") ){
              token = strtok(NULL, space);
              if (token == NULL){ // no output file
                write(STDERR_FILENO, error_message, strlen(error_message));
                exit(0);
              }

              // output file
              char file_name[strlen(token)+5];
              strcpy(file_name, token);
              strcat(file_name, ".out");
              // too many arguments
              token = strtok(NULL, space);
              if (token != NULL){
                write(STDERR_FILENO, error_message, strlen(error_message));
                exit(0);
              }
              // write to the file 
              FILE *file_ptr;
              file_ptr = fopen(file_name, "w");
              getcwd(cwd, sizeof(cwd));
              fputs(cwd, file_ptr);
              fclose(file_ptr);
              exit(0);
            }
            getcwd(cwd, sizeof(cwd));
            write(STDOUT_FILENO, cwd, strlen(cwd));
            write(STDOUT_FILENO, "\n", 1);
            exit(0);
	        }

  	    }

				if ( !(strcmp(token, "path")) ){
          num_path = 0;
          built_in = true;
  
          char path[MAX_LEN];
          strcpy(path, "PATH=");

          int exists = true;
          token = strtok(NULL, space);
          while (token!=NULL){
  					struct stat fileStat;
	
	  				if(stat(token, &fileStat) < 0 ){
              write(STDERR_FILENO, error_message, strlen(error_message)); 
              exists = false;
              break;
				  	}

            num_path++;
            strcat(path, token);
            strcat(path, " ");
            token = strtok(NULL, space);
          }

          if (exists == true){
            putenv(path);
           // printf("%d\n", num_path);
           strcpy(path_name, getenv("PATH"));
          }
  	    }

				if ( (token != NULL) && !(strcmp(token, "cd")) ){
						built_in = true;
        	  token = strtok(NULL, space);
          	if (token == NULL){
            	chdir( getenv("HOME") );
	          }
						else{ // check if too many arguments
              char tok_copy[MAX_LEN];
              strcpy(tok_copy,token);
              token=strtok(NULL, space);
              if (token == NULL){
                if (chdir( tok_copy ) != 0){
                  write(STDERR_FILENO, error_message, strlen(error_message));
                }
              }
              else{
                write(STDERR_FILENO, error_message, strlen(error_message));
              }
						}
	      }
			
				bool skip = true;
				if (token!=NULL && !built_in){ 
  	    	// check if input already implemented in path

          //printf("path: %s\n", getenv("PATH"));

          char path_copy[MAX_LEN];

          char command[MAX_LEN];
          strcpy(command, "");

          if ( path_name !=NULL && strlen(path_name) <= MAX_LEN ){

            strcpy(path_copy, path_name);
            char *token2;
            token2 = strtok(path_copy, space); // break up path to check directories

  					struct stat file_stat;

            // no path?
            if (token2==NULL){
              strcpy(command, "/");
            }

            while(token2!=NULL){ 
              strcpy(command, "");
              strcpy(command, token2);
              strcat(command, "/");
              strcat(command, token);
              if (stat(command, &file_stat) >= 0){
                skip = false;
                break;
              }
              token2 = strtok(NULL, space);
            }
            
          }
        
					if (skip==false){							
						pid = fork();
						if (pid){
							int status;
				  		waitpid(pid, &status, 0);
						}
						else{
							int argc = 1; //start at 1 bc you need an extra NULL arg
 
              char input_copy2[MAX_LEN];
                // = malloc(sizeof(char*)*strlen(input));
              strcpy(input_copy2, input);

              // go through arguments
							char * token2;
							token2 = strtok(input, space);
							while (token2!=NULL && strcmp(token2, ">") ){
								token2 = strtok(NULL,space);
								argc++;
							}

              //FILE *file_ptr;
              
              if (token2 != NULL && !strcmp(token2, ">")){
                token = strtok(NULL, space);
                if (token == NULL){
      			      write(STDERR_FILENO, error_message, strlen(error_message));		
                }
                else{ // write to a file
                  char file_name[strlen(token2)+5];
                  char file_err[strlen(token2)+5];
                  strcpy(file_name, token);
                  strcat(file_name, ".out");
                  strcpy(file_err, token);
                  strcpy(file_err, ".err");
                  // too many arguments
                  token2 = strtok(NULL, space);
                  if (token2 != NULL){
                    write(STDERR_FILENO, error_message, strlen(error_message));
                    exit(0);
                  }
              
                  // write to the file 
                  int file_d = open(file_name, O_RDWR | O_CREAT, S_IROTH | S_IXOTH);
                  int file_de = open(file_err, O_RDWR | O_CREAT, S_IROTH | S_IXOTH);
                 
                  dup2(file_d,1);
                  dup2(file_de,2);

                  //fputs("here", file_ptr);

                  close(file_d);
                  close(file_de);
                }
              }						

							char *args[argc];

							for (int j = 0; j < argc; j++){
								args[j]=NULL;
							}
 
							// go through input to get arguments
							char * token3;
							int i = 0;
							token3 = strtok(input_copy2, space);
							while (token3!=NULL && strcmp(token3,">")){
								args[i]=token3;
								token3 = strtok(NULL,space);
								i++;
							}

/*              for (int h = 0; h < argc; h++)
              {
                printf("%d: %s\n",h, args[h]);
              }
*/
              execv(command, args);
							exit(0);
						}
					}
          else{
      			write(STDERR_FILENO, error_message, strlen(error_message));		
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
