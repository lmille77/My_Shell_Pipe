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
	int should_run = 1;

	while (should_run == 1)
	{
		char* args0[MAXLINE/2+1],* args1[MAXLINE / 2 + 1], * args2[MAXLINE / 2 + 1], * array1[MAXLINE / 2], * array2[MAXLINE / 2];
		char command1[MAXLINE / 2], command2[MAXLINE / 2];
		int pid1, pid2;
		char* pch0, * pch1;

		printf("mysh>");
		fflush(stdout);

		char input[MAXLINE];
		fgets(input, MAXLINE, stdin);


		if (!strcmp(input, "exit\n"))
		{
			should_run = 0;
			break;
		}

		const int len = strlen(input) - 1;
		if (input[len] == '\n')
		{
			input[len] = '\0';
		}

		if (strstr(input, "|"))
		{
			//printf("Pipe Confirmed.\n");

			char first[MAXLINE / 2], last[MAXLINE / 2];
			strcpy(first, strtok(input, "|"));
			strcpy(last, strtok(NULL, "|"));

			//printf("First: %s\n", first);
			//printf("Last: %s\n", last);

			pch0 = strtok(first, " ");
			int i = 0;
			while (pch0 != NULL)
			{
				array1[i++] = strdup(pch0);
				pch0 = strtok(NULL, " ");
			}
			strcpy(command1, array1[0]);
			for (int a = 0; a < i; a++)
			{
				args1[a] = array1[a];
			}
			args1[i] = NULL;

			pch1 = strtok(last, " ");
			int j = 0;
			while (pch1 != NULL)
			{
				array2[j++] = strdup(pch1);
				pch1 = strtok(NULL, " \n");
			}
			strcpy(command2, array2[0]);
			for (int b = 0; b < j; b++)
			{
				args2[b] = array1[b];
			}
			args2[j] = NULL;

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
				//input[strlen(input) - 1] = 0;
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