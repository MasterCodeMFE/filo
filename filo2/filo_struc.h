#ifndef FILO_STRUC_H
# define FILO_STRUC_H

typedef struct s_filo
{
    long number_of_philosophers;
    long time_to_die;
    long time_to_eat;
    long time_to_sleep;
    long number_of_times_each_philosopher_must_eat;
    long *last_meal_time;
    long start_time;
    int philosopher_id;
    int *eat;
    int philosopher_dead;
    pthread_mutex_t *forks;
    pthread_mutex_t print;
    pthread_mutex_t id_mutex;
    pthread_mutex_t meal_time_mutex;
    pthread_mutex_t death_mutex; 
    pthread_t monitoring_thread;
    pthread_t *philos;
}   t_filo;

#endif