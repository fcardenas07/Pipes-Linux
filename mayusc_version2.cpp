#include <iostream>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
    int pipefd[2];
    pid_t pid;

    if (pipe(pipefd) == -1)
    {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid = fork();

    if (pid == -1)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0)
    {                     // Proceso hijo - mayusc
        close(pipefd[1]); // Cerramos la escritura en el pipe

        dup2(pipefd[0], STDIN_FILENO); // Redireccionamos la entrada estándar al pipe

        execlp("mayusc", "mayusc", NULL); // Ejecutamos el programa mayusc
        perror("execlp");                 // Si execlp retorna, hubo un error
        exit(EXIT_FAILURE);
    }
    else
    {                     // Proceso padre - ll
        close(pipefd[0]); // Cerramos la lectura en el pipe

        dup2(pipefd[1], STDOUT_FILENO); // Redireccionamos la salida estándar al pipe

        execlp("ll", "ll", NULL); // Ejecutamos el programa ll
        perror("execlp");         // Si execlp retorna, hubo un error
        exit(EXIT_FAILURE);
    }

    return 0;
}
