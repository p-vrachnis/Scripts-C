#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>




int command_cd(char **args);
char *builtin_str[] = {
  "cd"
};
 
int command_cd(char **args)
{
  if (args[1] == NULL) {
    fprintf(stderr, "Command not found \"cd\"\n");
  } else {
    if (chdir(args[1]) != 0) {
      perror("");
    }
    
  }
  return 1;
}

int (*builtin_func[]) (char **) = {
  &command_cd
};



static char* args[512];
pid_t pid;
int array_pipe[2];
 

static int execcmd(int input, int first, int check)
{int i=0;
	int pipes[2];
 
	
	pipe( pipes );	
	pid = fork();
 
        
 
	if (pid == 0) {
		if (first == 1 && check == 0 && input == 0) {
			
			dup2( pipes[1], STDOUT_FILENO );
		} else if (first == 0 && check == 0 && input != 0) {
			
			dup2(input, STDIN_FILENO);
			dup2(pipes[1], STDOUT_FILENO);
          
		} else {
                        
                         if (strcmp(args[0], builtin_str[i]) == 0) {
                               return (*builtin_func[i])(args);
                         }
                               
                
                 
			dup2( input, STDIN_FILENO );
                 }
 
		if (execvp( args[0], args) == -1)
			_exit(EXIT_FAILURE); 
	}
 
	if (input != 0) 
		close(input);
	close(pipes[1]);
     if (check == 1)
       
		close(pipes[0]);
 
	return pipes[0];
	
}
 
static int split(char* command, int input, int first, int check);
static char line[1024];
static int n = 0; 

static int split(char* command, int input, int first, int check)
{ int i = 0;
 
  while (isspace(*command)) ++command;
        
  
  char* temp = strchr(command, ' ');
  while(temp != NULL) {
		temp[0] = '\0';
                args[i] = command;
		++i;
                temp = temp + 1;
                while (isspace(*temp)) ++temp;
                command = temp ;  		
                temp = strchr(command, ' ');
} 
	if (command[0] != '\0') { 
		args[i] = command;
		temp = strchr(command, '\n'); 
		temp[0] = '\0';
		++i; 
}	
 
	args[i] = NULL;
                
                
	        if (strcmp(args[0], "exit") == 0) 
			exit(0);
		n += 1;
		
                return execcmd(input, first,check );
	return 0;

 }


 
int main()
{
	while (1) {
		
		printf("$ ");
		fflush(NULL);
 
		
		if (!fgets(line, 1024, stdin)) 
			return 0;
 
		int input = 0;
		int first = 1;
 
		char* command = line;
		char* temp = strchr(command, '|'); 
 
		while (temp != NULL) {
			
			*temp = '\0';

			input = split(command, input, first, 0);
                        
			command = temp + 1;
			temp = strchr(command, '|'); 
			first = 0;
		}
		input = split(command, input, first, 1);
               
		int i;
	for (i = 0; i < n; ++i) 
		wait(NULL); 
		n = 0;
	}
	return 0;
}
 
 

