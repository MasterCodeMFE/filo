#include <stdio.h>

struct timeval
{
    long tv_sec;
    long tv_usec;
};


int main()
{
    struct timeval tiempo_inicial;
    struct timeval tiempo_final;

    if(gettimeofday(&tiempo_inicial, NULL))
    {
        return 1;
    }
    usleep(5000000);
    if(gettimeofday(&tiempo_final, NULL))
    {
        return 1;
    }
    printf("%ld segundos\n", tiempo_inicial.tv_sec - tiempo_inicial.tv_sec);
    printf("%ld segundos\n", tiempo_final.tv_sec- tiempo_inicial.tv_sec);
}