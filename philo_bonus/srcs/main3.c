#include "../includes/philo.h"

typedef struct s_sem
{
    sem_t *sem;
    char *name;
} t_sem;

typedef struct s_thread
{
    int nbr;
    pthread_t id_thread;    
    sem_t *sem;

} t_thread;

void *hilo_hijo(void *data)
{
    t_thread *thre;
    int i;

    thre = (t_thread *)data;
    printf("empezando el hijo %d\n", thre->nbr);
    i = 0;
    while (i < 100)
    {
       if (thre->nbr % 2 == 0)
            usleep(2000);
        //sem_wait(thre->sem);
        usleep(1000);
        printf("empezando el hijo %d\n", thre->nbr);        
       // sem_post(thre->sem);
        i++;
    }
    return (0);
}

int main(void)
{
    t_sem semaforos[5];
    t_thread hijos[10];
    int i;

    semaforos[0].name = "sem1";
    semaforos[1].name = "sem2";
    semaforos[2].name = "sem3";
    semaforos[3].name = "sem4";
    semaforos[4].name = "sem5";
    i = 0;
    while (i < 5)
    {
        semaforos[i].sem = sem_open(semaforos[i].name, O_CREAT);
        i++;
    }
    i = 0;
    while (i < 10)
    {
        hijos[i].nbr = i;
        hijos[i].sem = semaforos[0].sem;
        pthread_create(&hijos[i].id_thread, NULL, hilo_hijo, &hijos[i]);
        i++;
    }
    printf("que pasa\n");
    usleep(100000);
    /*
    i = 0;
    while (i < 10)
    {
        if (i % 2 != 0)
        {
            pthread_detach(hijos[i].id_thread);
            i++;
        }
        i++;
    }
    */
    i = 0;
    while (i < 10)
    {
        if (i % 2 == 0)
        {
            pthread_join(hijos[i].id_thread, NULL);
            i++;
        }
        i++;
    }
    return (0);   

}