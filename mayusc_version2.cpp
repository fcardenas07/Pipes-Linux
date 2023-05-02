#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

#define READ_END 0
#define WRITE_END 1

int main(int argc, char *argv[])
{
    int fd[2];
    pid_t pid;

    if (pipe(fd) == -1)
    {
        fprintf(stderr, "Error al crear el pipe\n");
        return EXIT_FAILURE;
    }

    pid = fork();

    if (pid < 0)
    {
        fprintf(stderr, "Error al crear un nuevo proceso\n");
        return EXIT_FAILURE;
    }

    if (pid == 0)
    {                         // Proceso hijo
        close(fd[WRITE_END]); // Cerramos el descriptor de escritura

        dup2(fd[READ_END], STDIN_FILENO); // Redireccionamos la entrada estándar al pipe
        close(fd[READ_END]);              // Cerramos el descriptor de lectura

        execlp("./mayusc", "./mayusc", NULL);                     // Ejecutamos el programa mayus
        fprintf(stderr, "Error al ejecutar el programa mayus\n"); // Este código sólo se ejecuta si hubo un error en execlp
        return EXIT_FAILURE;
    }
    else
    {                        // Proceso padre
        close(fd[READ_END]); // Cerramos el descriptor de lectura

        dup2(fd[WRITE_END], STDOUT_FILENO); // Redireccionamos la salida estándar al pipe
        close(fd[WRITE_END]);               // Cerramos el descriptor de escritura

        execlp("ll", "ll", NULL);                              // Ejecutamos el programa ls
        fprintf(stderr, "Error al ejecutar el programa ls\n"); // Este código sólo se ejecuta si hubo un error en execlp
        return EXIT_FAILURE;
    }

    return 0;
}
