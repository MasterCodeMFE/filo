#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

void print_numbers(void *num)
{
    int *i;
    int j;

    i = (int *)num;
    j = 0;
    while (j < 10000000)
    {
        (*i)++;
        j++;
    }
}

void print_hellow(void)
{
    int i  = 0;

    while (i < 100)
    {
        printf("Hello\n");
        i++;
        usleep(10000);
    }
}
void print_world(void)
{
    int i  = 0;

    while (i < 100)
    {
        printf("            World\n");
        i++;
        usleep(10000);
    }
}

void example_1(void)
{
    print_hellow();
    print_world();
}
void example_2(void)
{
    pthread_t thread1;
    pthread_t thread2;

    pthread_create(&thread1, NULL, (void *)print_hellow, NULL);
    pthread_create(&thread2, NULL, (void *)print_world, NULL);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
}

void example_3(void)
{
    int *num;
    pthread_t thread1;
    pthread_t thread2;

    num = malloc(sizeof(int));
    *num = 0;
    pthread_create(&thread1, NULL, (void *)print_numbers, num);
    pthread_create(&thread2, NULL, (void *)print_numbers, num);
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    printf("num = %d\n", *num);
}
int main()
{
    example_3();
    return 0;
}