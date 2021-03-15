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
		char* args1[MAXLINE / 2 + 1];	
		int pid1, pid2;

		//prompt input, read input and store into "input"
		printf("mysh>");
		fflush(stdout);
		char input[MAXLINE];
		fgets(input, MAXLINE, stdin);
    
		//stops program once user types in "exit"
		if (!strcmp(input, "exit\n"))
		{
			break;
		}

		//sets last value of input = 0
		//strtok uses this value to terminate
		//allows program to know when to stop parsing
		input[strlen(input) - 1] = 0;

    
		if (strstr(input, "|"))
		{	
      char* args2[MAXLINE/2 + 1];		
			char* p_cmd1, * p_cmd2;
			char first[MAXLINE / 2], last[MAXLINE / 2];

			//parse input before and after "|"
			strcpy(first, strtok(input, "|"));
			strcpy(last, strtok(NULL, "|"));

			//parse first half of input delimited by " "
			//set values used for execvp
			p_cmd1 = strtok(first, " ");
			int i = 0;
			while (p_cmd1 != NULL)
			{
				args1[i++] = strdup(p_cmd1);
				p_cmd1 = strtok(NULL, " ");
			}
			args1[i] = NULL;			

			//parse last half of input delimited by " "
			//set values used for execvp
			p_cmd2 = strtok(last, " ");
			int j = 0;
			while (p_cmd2 != NULL)
			{
				args2[j++] = strdup(p_cmd2);
				p_cmd2 = strtok(NULL, " ");
			}
			args2[j] = NULL;			
			
			//create pipe
			int pipefd[2];
			pipe(pipefd);
      
			//fork child process
			pid1 = fork();

			if (pid1 == 0)
			{
				close(pipefd[0]);
				dup2(pipefd[1], 1);
				execvp(args1[0], args1);
			}
			else
			{
				pid2 = fork();
				if (pid2 == 0)
				{
					close(pipefd[1]);
					dup2(pipefd[0], 0);
					execvp(args2[0], args2);
				}
			}
			close(pipefd[1]);
			close(pipefd[0]);
			wait(&pid2);
			wait(&pid1);	
			
		}
		else
		{
			pid1 = fork();
			if (pid1 == 0)
			{				
				char* token = strtok(input, " ");
				int i = 0;
				while (token != NULL)
				{
					args1[i++] = token;
					token = strtok(NULL, " ");
				}
				args1[i] = NULL;
				execvp(args1[0], args1);
				exit(0);
			}
			else
			{
				wait(NULL);				
			}
		}		

	}
}