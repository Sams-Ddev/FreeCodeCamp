#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void remove_newline(char *cmd, ssize_t chars_read) {
    if (cmd[chars_read - 1] == '\n') {
        cmd[chars_read - 1] = '\0';
    }
}

int main(void) {
    char *cmd;
    size_t buffsize = 0; // Set proper value for buffsize
    ssize_t chars_read;
    pid_t child_pid;

    cmd = (char *)malloc(buffsize * sizeof(char));
    if (cmd == NULL) {
        perror("Unable to allocate command buffer");
        exit(EXIT_FAILURE);
    }

    while (1) {
        printf("#cisfun$ ");
        chars_read = getline(&cmd, &buffsize, stdin);
        if (chars_read == -1) {
            perror("getline");
            free(cmd);
            exit(EXIT_FAILURE);
        }
        remove_newline(cmd, chars_read);
        child_pid = fork();
        if (child_pid == -1) {
            perror("fork failed");
            free(cmd);
            exit(EXIT_FAILURE);
        } else if (child_pid == 0) {
            execve(cmd, (char *const []){cmd, NULL}, NULL);
            perror("execve");
            free(cmd);
            exit(EXIT_FAILURE);
        } else {
            wait(NULL);
        }
    }

    free(cmd);
    return EXIT_SUCCESS;
}

