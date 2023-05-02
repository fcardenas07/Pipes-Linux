#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <iostream>
#include <cstring>

using namespace std;

int main() {
    int fd[2];
    pid_t pid;
    char input[100];
    memset(input, 0, sizeof(input));

    if (pipe(fd) == -1) {
        cerr << "Error: Failed to create pipe" << endl;
        return 1;
    }

    pid = fork();
    if (pid < 0) {
        cerr << "Error: Failed to fork" << endl;
        return 1;
    }
    else if (pid == 0) {
        // Child process - will execute mayusc
        close(fd[1]);  // Close unused write end
        dup2(fd[0], STDIN_FILENO);  // Redirect read end to stdin
        close(fd[0]);  // Close read end
        execlp("./mayusc", "./mayusc", NULL);  // Execute mayusc
    }
    else {
        // Parent process - will read input from user
        close(fd[0]);  // Close unused read end
        cout << "Enter text to be converted to uppercase:" << endl;
        cin.getline(input, sizeof(input));
        write(fd[1], input, strlen(input));  // Write input to pipe
        close(fd[1]);  // Close write end
        wait(NULL);  // Wait for child process to complete
    }

    return 0;
}
