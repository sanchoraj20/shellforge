
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_ARGS 64

typedef struct {
    char *args[MAX_ARGS];
    int count;
} Command;

void parse_command(char *line, Command *cmd)
{
    cmd->count = 0;

    char *token = strtok(line, " \t");

    while (token != NULL && cmd->count < MAX_ARGS - 1)
    {
        cmd->args[cmd->count] = token;
        cmd->count++;
        token = strtok(NULL, " \t");
    }

    cmd->args[cmd->count] = NULL;
}

int main(void)
{
    char *line = NULL;
    size_t len = 0;
    Command cmd;

    printf("=================================\n");
    printf("       Welcome to SHELLFORGE\n");
    printf("=================================\n");

    while (1)
    {
        printf("shellforge$ ");
        fflush(stdout);

        if (getline(&line, &len, stdin) == -1)
        {
            printf("\n");
            break;
        }

        line[strcspn(line, "\n")] = '\0';

        parse_command(line, &cmd);

        if (cmd.count == 0)
            continue;

        if (strcmp(cmd.args[0], "exit") == 0)
        {
            printf("Exiting SHELLFORGE. Goodbye!\n");
            break;
        }

        /* Milestone 3: Display the organized command */
        printf("Structure Log -> command: %s | Arguments found: %d\n",
               cmd.args[0], cmd.count - 1);

        /* Built-in command: cd */
        if (strcmp(cmd.args[0], "cd") == 0)
        {
            const char *path =
                (cmd.count > 1) ? cmd.args[1] : getenv("HOME");

            if (path == NULL || chdir(path) != 0)
                perror("cd");

            continue;
        }

        /* Execute external commands */
        pid_t pid = fork();

        if (pid < 0)
        {
            perror("fork");
        }
        else if (pid == 0)
        {
            execvp(cmd.args[0], cmd.args);
            perror("Command execution failed");
            _exit(127);
        }
        else
        {
            int status;
            waitpid(pid, &status, 0);
        }
    }

    free(line);
    return 0;
}

