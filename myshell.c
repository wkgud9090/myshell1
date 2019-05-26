#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <pwd.h>

#define MAX_LEN_LINE    20
#define LEN_HOSTNAME	30
int main(void)
{
    char command[MAX_LEN_LINE]={0x00,};
    char *args[] = {command, NULL};
    int ret, status;
    pid_t pid, cpid;
    char *ptr;

    char hostname[LEN_HOSTNAME+1] = {0x00,};
    gethostname(hostname, LEN_HOSTNAME);
    
    while (true) {
        char *s;
        int len;
        
        //printf("MyShell $ ");
	// 3
	printf("%s@%s $ ",getpwuid(getuid())->pw_name,hostname);
        s = fgets(command, MAX_LEN_LINE, stdin);
        if (s == NULL) {
            fprintf(stderr, "fgets failed\n");
            exit(1);
        }
        
        len = strlen(command);
        printf("%d\n", len);
        if (command[len - 1] == '\n' || command[len-1] == '\r') {
            command[len - 1] = '\0'; 
        }
	// 1
        if(!strcmp("exit",command)){
		printf("exit!");
		return 0;
	}

        printf("[%s]\n", command);

	ptr = command;
	int i=0;
	    for(i=0;i<len;i++)
	    {
		char *save;         
		save = ptr;
		if (*ptr == ' ')
		{
		    char *next;
		    for (next = ptr + 1 ; *next != '\0'; next++, ptr++)
			*ptr = *next;
		    ptr = save;
		}
		ptr++;
	    }
	printf("command: %s \n",command);
	// 2
       	char *p = strtok(command, ";");
	while(p != NULL){
		printf("%s \n",p);
//		execve(p, NULL, NULL);

		pid = fork();
		if (pid < 0) {
		    fprintf(stderr, "fork failed\n");
		    exit(1);
		} 
		if (pid != 0) {  /* parent */
		    cpid = waitpid(pid, &status, 0);
		    if (cpid != pid) {
			fprintf(stderr, "waitpid failed\n");        
		    }
		    printf("Child process terminated\n");
		    if (WIFEXITED(status)) {
			printf("Exit status is %d\n", WEXITSTATUS(status)); 
		    }
		}
		else {  /* child */
		    if(p){
			execve(p, NULL, NULL);
		    }else{
		    	ret = execve(args[0], args, NULL);
	            }
		    
		    if (ret < 0) {
			fprintf(stderr, "execve failed\n");   
			return 1;
		    }
		}
		if(p)
			p = strtok(NULL, ";");
	}	
    }
    return 0;
}
