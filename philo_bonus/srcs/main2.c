#include "../includes/philo.h"

int main(void)
{
    pid_t pid;
    sem_t *sem;
    int process;
    int i;
    int status;

    process = 0;
    sem = sem_open("semaforo", O_CREAT);
    i = 0;
    while (i < 10)
    {
        pid = fork();
        if (pid == 0)
        {
            int j = 0;
            while (j < 10)
            {
                usleep(5000);
                sem_wait(sem);
                printf("soy el proceso hijo %d\n", i);
                sem_post(sem);
                j++;
            }
            exit(0); 
        }
        else
        {
            i++;
        }
    }
    waitpid(pid, &status, 0);
    sem_close(sem);
    sem_unlink("semaforo");
    return (0);
    
}