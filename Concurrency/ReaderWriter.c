#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#define READERS 6

void* read_resource(void*);
void* write_resource(void*);

int resource_counter = 0;

int resource = 100;

pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t read_ready = PTHREAD_COND_INITIALIZER;
pthread_cond_t write_ready = PTHREAD_COND_INITIALIZER;

int main()
{
    pthread_t writer;
    pthread_t readers[READERS];
    
    int i;
    for (i=0;i<READERS;i++)
    pthread_create(&readers[i], NULL, read_resource, NULL);
    pthread_create(&writer, NULL, write_resource, NULL);

    for (i=0;i<READERS;i++)
    pthread_join(readers[i], NULL);
    
    pthread_join(writer, NULL);
    return 0;
}

void* read_resource(void* arg){
    pthread_mutex_lock(&m);
    while(resource_counter == -1) pthread_cond_wait(&read_ready, &m);
    
    resource_counter++;
    pthread_mutex_unlock(&m);
    
    printf("start read resource %d\n", resource);
    sleep(5);
    printf("end read resource %d\n", resource);
    
    pthread_mutex_lock(&m);
    resource_counter--;
    
    if (resource_counter == 0){
        pthread_cond_broadcast(&write_ready);
    }
    pthread_mutex_unlock(&m);
}

void* write_resource(void* arg){
    pthread_mutex_lock(&m);
    while(resource_counter != 0) pthread_cond_wait(&write_ready, &m);
    
    resource_counter = -1;
    pthread_mutex_unlock(&m);
    
    printf("start modify resource %d\n", resource);
    resource++;
    printf("end modify resource %d\n", resource);
    
    pthread_mutex_lock(&m);
    resource_counter = 0;
    pthread_mutex_unlock(&m);
    
    pthread_cond_broadcast(&write_ready);
    pthread_cond_broadcast(&read_ready);
}