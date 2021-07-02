#include "../includes/philo.h"

void *wait_sem(void *data)
{
    t_fork *fork;

    fork = (t_fork *)fork;
    sem_wait(data);
    fork->capture = 1;
    while (1)
        usleep(1000);
}

int run_philo(t_data *data)
{
    int i;
    int has_error;
    int has_two;
    t_fork *fork;

    i = 0;
    
    while (i < data->nbr_philos)
    {
        fork = &data->forks[i];
        has_error = pthread_create(&fork->id_thread, NULL, wait_sem, fork);
        if (has_error)
            return (1);
        i++;
    }
    has_two = 0;
    while (!has_two)
    {
        i = 0;
        while (!has_two && i < data->nbr_philos)
        {
            if (data->forks[i].capture)
            {
                if (!data->fork1)
                {
                    data->fork1 = &data->forks[i];
                    print_change(data, "has taken a fork", get_time());
                }
                else if (!data->fork2 && data->forks[i].nbr != data->fork1->nbr)
                {
                    data->fork2 = &data->forks[i];
                    print_change(data, "has taken a fork", get_time());
                }
                else if (data->fork1 && data->fork2)
                    has_two = 1;
            }
            i++;          
        }      
    }
    i = 0;
    while (i < data->nbr_philos)
    {
        pthread_detach(data->forks[i].id_thread);
        i++;
    }
    i = 0;
    has_two = 0;
    while (i < data->nbr_philos)
    {
        if (data->forks[i].capture)
        {
            if (has_two == 0)
                data->fork1 = &data->forks[i];
            else if (has_two == 1)
                data->fork2 = &data->forks[i];
            else
                sem_post(data->forks[i].sem);
        }
        i++;
    }

    
}