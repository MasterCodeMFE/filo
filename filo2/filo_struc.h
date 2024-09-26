#ifndef FILO_STRUC_H
# define FILO_STRUC_H

typedef struct s_filo
{
    long number_of_philosophers;
    long time_to_die;
    long time_to_eat;
    long time_to_sleep;
    long number_of_times_each_philosopher_must_eat;
    int philosopher_id;
    pthread_mutex_t *forks;
    pthread_mutex_t print;
    pthread_mutex_t id_mutex;
    pthread_t *philos;
}   t_filo;

#endif