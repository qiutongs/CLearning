#include <stdio.h>
#include <pthread.h>

void *f(void *p);

int main()
{

    pthread_t t1;

    pthread_create(
        &t1,
        NULL,
        f,
        NULL
    );

    pthread_join(t1, NULL);

    return 0;
}

void *f(void *p)
{
    printf("Hello, World!\n");
}