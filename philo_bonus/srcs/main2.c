#include "../includes/philo.h"

int main(void)
{
    pid_t pid;
    sem_t *sem;
    int i;
    int status;
    int error;
    char name[] = "semaforo213344";

    printf("empieza el programa\n");
    sem = sem_open(name, O_CREAT | O_EXCL, 0644, 1);
    if (sem == (sem_t *)-1)
    {
        printf("Error al crear el semaforo\n");
        exit (0);
    }
    i = 0;
    while (i < 10)
    {
        i++;
        pid = fork();
        if (pid == -1)
        {
            printf("No se ha creado el hijo %d\n", i);
            exit (0);
        }
        else if (pid == 0)
            break;
    }

    if (pid == 0)
    {
        // printf("Se crea el hijo %d\n", i);
        //sem = sem_open(name, O_CREAT, 0666,0);
        int j = 0;
        int max = 20;
        if (i == 8)
            max = 10;
        printf("soy el hijo %d y he sido creado\n", i);
        while (j < max)
        {
            /*
            error = sem_wait(sem);
            if (error == -1)
            {
                printf("Error en wait en hijo %d\n", i);
                exit (1);
            }    
            */                   
            printf("soy el proceso hijo %d en iteración %d y error devuelve %d\n", i, j, error); 
            /*                  
            error = sem_post(sem);
            if (error == -1)
            {
                printf("Error en post en hijo %d\n", i);
                exit (1);
            }
            */        
            usleep(500000); 
            j++;
            
        }
        
        /*
        if (i == 8)
            sem_unlink(name);
            */
        sem_close(sem);
        exit(0); 
    }
    else
    {
        waitpid(pid, &status, 0);
        printf("terminan los hijos\n");
        sem_unlink(name);
        sem_close(sem);        
        exit (0);
    }
    
}