#ifndef FILO_STRUC_H
# define FILO_STRUC_H

typedef struct s_filo
{
    int				number_of_philosophers;
    int				time_to_die;
    int				time_to_eat;
    int				time_to_sleep;
    int				number_of_times_each_philosopher_must_eat;
    pthread_mutex_t	*forks;
    pthread_t       *philos;
    pthread_mutex_t	print;
}	t_filo;

#endif