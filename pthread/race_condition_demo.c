#include <stdio.h>
#include <pthread.h>

void *f(void *p);

/* 
 * Below program has race condition on the global variable "i",
 * so output is unpredictable.
 */
int main()
{

    pthread_t t[4];

    int i;
    for (i = 0; i < 4; i++)
    {
        pthread_create(
            &t[i],
            NULL,
            f,
            &i
        );
    }

    for (i = 0; i < 4; i++)
    {
        pthread_join(t[i], NULL);
    }

    return 0;
}

void *f(void *p)
{
    int *i = (int *)p;
    printf("Hello, World %d!\n", *i);
}