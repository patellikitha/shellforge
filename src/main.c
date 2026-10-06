#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void)
{
    char *line = NULL;
    size_t len = 0;
    char *args[64];

    while (1)
    {
        /* Display prompt */
        printf("shellforge$ ");
        fflush(stdout);

        /* Read user input */
        if (getline(&line, &len, stdin) == -1)
        {
            break;
        }

        /* Remove newline */
        line[strcspn(line, "\n")] = '\0';

        int i = 0;

        /* Split command into words */
        char *token = strtok(line, " \t");

        while (token != NULL && i < 63)
        {
            args[i++] = token;
            token = strtok(NULL, " \t");
        }

        /* NULL terminate args */
        args[i] = NULL;

        /* Ignore empty input */
        if (i == 0)
        {
            continue;
        }

        /* Exit ShellForge */
        if (strcmp(args[0], "exit") == 0)
        {
            break;
        }

        /* Milestone 5: Directory Navigation */
        if (strcmp(args[0], "cd") == 0)
        {
            if (args[1] == NULL)
            {
                fprintf(stderr, "shellforge: missing path parameter\n");
            }
            else
            {
                if (chdir(args[1]) != 0)
                {
                    perror("Directory change failed");
                }
            }

            continue;
        }

        /* Create child process for external commands */
        pid_t pid = fork();

        if (pid == 0)
        {
            /* Child process */
            execvp(args[0], args);

            /* Runs only if execvp fails */
            perror("Execution error");
            exit(1);
        }
        else if (pid > 0)
        {
            /* Parent process */
            waitpid(pid, NULL, 0);
        }
        else
        {
            /* fork() failed */
            perror("Fork creation error");
        }
    }

    free(line);

    return 0;
}
