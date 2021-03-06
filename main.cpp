#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <cstdio>
#include <cstddef>

#define MAXLINE 80 
using namespace std;

int main()
{	
	while (1)
	{
		char* args0[MAXLINE/2+1],* args1[MAXLINE / 2 + 1], * args2[MAXLINE / 2 + 1]; 
		char command1[MAXLINE / 2], command2[MAXLINE / 2];
		int pid1, pid2;
		char* p_cmd1, * p_cmd2;

		printf("mysh>");
		fflush(stdout);

		char input[MAXLINE];
		fgets(input, MAXLINE, stdin);

		if (!strcmp(input, "exit\n"))
		{
			break;
		}

		input[strlen(input) - 1] = 0;

		if (strstr(input, "|"))
		{			

			char first[MAXLINE / 2], last[MAXLINE / 2];
			strcpy(first, strtok(input, "|"));
			strcpy(last, strtok(NULL, "|"));			
      //parse input before "|", set values for execvp
			p_cmd1 = strtok(first, " ");
			int i = 0;
			while (p_cmd1 != NULL)
			{
				args1[i++] = strdup(p_cmd1);
				p_cmd1 = strtok(NULL, " ");
			}
      args1[i] = NULL;
			strcpy(command1, args1[0]);	
			//parse input after "|", set values for execvp
			p_cmd2 = strtok(last, " ");
			int j = 0;
			while (p_cmd2 != NULL)
			{
				args2[j++] = strdup(p_cmd2);
				p_cmd2 = strtok(NULL, " \n");
			}
      args2[j] = NULL;
			strcpy(command2, args2[0]);
			
			int pipefd[2];
			pipe(pipefd);
      
			pid1 = fork();

			if (pid1 == 0)
			{
				close(pipefd[0]);
				dup2(pipefd[1], 1);
				execvp(command1, args1);
			}
			else
			{
				pid2 = fork();
				if (pid2 == 0)
				{
					close(pipefd[1]);
					dup2(pipefd[0], 0);
					execvp(command2, args2);
				}
			}
			close(pipefd[1]);
			close(pipefd[0]);
			wait(&pid2);
			wait(&pid1);	
			
		}
		else
		{
			int child_pid = fork();
			if (child_pid == 0)
			{				
				char* token = strtok(input, " ");
				int i = 0;
				while (token != NULL)
				{
					args0[i++] = token;
					token = strtok(NULL, " ");
				}
				args0[i] = NULL;
				execvp(args0[0], args0);
				exit(0);
			}
			else
			{
				wait(NULL);				
			}
		}		

	}
}