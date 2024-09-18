#ifndef FILO_STRUC_H
# define FILO_STRUC_H

typedef struct s_filo
{
    long                number_of_philosophers;
    long                time_to_die;
    long                time_to_eat;
    long                time_to_sleep;
    long                number_of_times_each_philosopher_must_eat;
    long                *last_meal_time;
    int                 eat;
    pthread_mutex_t     *forks;
    pthread_t           *philos;
    pthread_mutex_t     print;
    pthread_mutex_t     *last_meal_mutex;
    int                 is_dead;
    pthread_mutex_t     dead_mutex;
    pthread_mutex_t     id_mutex;
    int                 philosopher_id;
    long                init_program;
}   t_filo;
#endif