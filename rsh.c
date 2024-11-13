#include <stdio.h>
#include <stdlib.h>
#include <spawn.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

#define N 12

extern char **environ;

char *allowed[N] = {"cp","touch","mkdir","ls","pwd","cat","grep","chmod","diff","cd","exit","help"};

int isAllowed(const char*cmd) {
	// TODO
	// return 1 if cmd is one of the allowed commands
	// return 0 otherwise
	for(int i=0; i < 12; i++)
    {
        if(strcmp(cmd, allowed[i])==0) return 1;
    }
	return 0;
}

int lastThree(const char*cmd)
{
    for(int i=9; i < 12; i++)
    {
        if(strcmp(cmd, allowed[i]) == 0) 
        {return 1;}
    }
}

int main() {

    // TODO
    // Add variables as needed

    pid_t pid;
    int status;
    posix_spawnattr_t attr;

    char line[256];
    char * splittingLine;
    char *argv[20];
    char  * lineTok;

    while (1) 
    {
        int i = 0;
        fprintf(stderr,"rsh>");

        if (fgets(line,256,stdin)==NULL) continue;

        if (strcmp(line,"\n")==0) continue;

        line[strlen(line)-1]='\0';
        
        splittingLine = line;
        lineTok = strtok(splittingLine, " ");

        while (lineTok != NULL)
        {
            argv[i] = lineTok;
            i++;
            lineTok = strtok(NULL, " ");
        }

        argv[i] = NULL;

        if(isAllowed(argv[0])== 0)
        {
            printf("NOT ALLOWED!\n");
            continue;
        }
        if(lastThree(argv[0]) == 1)
        {
            if(strcmp(argv[0], allowed[11]) == 0)
            {
                printf("The allowed commands are:\n");
                for(int i = 0; i < 12; i++)
                {
                    printf("%d: %s\n", i+1, allowed[i]);
                }
            }
            else if (strcmp(argv[0], allowed[9])== 0)
            {
                if (i>2)
                {
                    printf("-rsh: cd: too many arguments \n");
                }
                else
                {
                    chdir(argv[1]);
                }
            }
            else
            {
                return 0;
            }
        }
        else{
            posix_spawnattr_init(&attr);
            if(posix_spawnp(&pid, argv[0], NULL, &attr, argv, environ) != 0)
            {
                perror("spawn failed");
                exit(EXIT_FAILURE);
            }
            if(waitpid(pid, &status, 0)== -1)
            {
                perror("waitpid failed");
                exit(EXIT_FAILURE);
            }
            posix_spawnattr_destroy(&attr);
        }

	// TODO
	// Add code to spawn processes for the first 9 commands
	// And add code to execute cd, exit, help commands
	// Use the example provided in myspawn.c

    }
    return 0;
}