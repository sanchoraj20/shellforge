
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_INPUT 1024
#define MAX_ARGS 64

int main(void)
{
    char input[MAX_INPUT];
    char *args[MAX_ARGS];

    printf("=================================\n");
    printf("       Welcome to SHELLFORGE\n");
    printf("=================================\n");

    while (1)
    {
        printf("shellforge> ");
        fflush(stdout);

        if (fgets(input, sizeof(input), stdin) == NULL)
        {
            printf("\n");
            break;
        }

        input[strcspn(input, "\n")] = '\0';

        if (input[0] == '\0')
            continue;

        int count = 0;
        char *token = strtok(input, " \t");

        while (token != NULL && count < MAX_ARGS - 1)
        {
            args[count++] = token;
            token = strtok(NULL, " \t");
        }

        args[count] = NULL;

        if (count == 0)
            continue;

        if (strcmp(args[0], "exit") == 0)
        {
            printf("Exiting SHELLFORGE. Goodbye!\n");
            break;
        }

        if (strcmp(args[0], "cd") == 0)
        {
            const char *path =
                (count > 1) ? args[1] : getenv("HOME");

            if (path == NULL || chdir(path) != 0)
                perror("cd");

            continue;
        }

        pid_t pid = fork();

        if (pid < 0)
        {
            perror("fork");
        }
        else if (pid == 0)
        {
            execvp(args[0], args);
            perror("Command execution failed");
            _exit(127);
        }
        else
        {
            int status;
            waitpid(pid, &status, 0);
        }
    }

    return 0;
}
