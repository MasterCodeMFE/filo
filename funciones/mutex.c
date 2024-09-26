#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

typedef struct s_mutex
{
    int num;
    pthread_mutex_t mutex;
}   t_mutex;

void *print_numbers(void *arg)
{
    t_mutex *data = (t_mutex *)arg;
    int j = 0;

    while (j < 100000)
    {   
        pthread_mutex_lock(&data->mutex);
        data->num++;
        pthread_mutex_unlock(&data->mutex);
        j++;
    }
    return (NULL);
}

void example_1(void)
{
    t_mutex *mutex;
    pthread_t thread1, thread2;

    mutex = malloc(sizeof(t_mutex));
    mutex->num = 0;
    pthread_mutex_init(&mutex->mutex, NULL); // Inicializa el mutex

    pthread_create(&thread1, NULL, print_numbers, mutex);
    pthread_create(&thread2, NULL, print_numbers, mutex);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    printf("num = %d\n", mutex->num);

    pthread_mutex_destroy(&mutex->mutex); // Destruye el mutex al finalizar
    free(mutex);
}

int main()
{
    example_1();
    return 0;
}