#include <stdio.h>

struct timeval
{
    long tv_sec;
    long tv_usec;
};


int main()
{
    struct timeval time;

    if(gettimeofday(&time, NULL))
    {
        return 1;
    }
    printf("%ld segundos\n", time.tv_sec);
    printf("%ld microsegundos\n", time.tv_usec);
}