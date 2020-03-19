#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#define max_size 5
#define max_iteration 10

void* producer_f(void* arg);
void* consumer_f(void* arg);

int buffer[max_size];
size_t head = 0;
size_t tail = 0;
int size = 0;

int id = 0;

pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t not_full = PTHREAD_COND_INITIALIZER;
pthread_cond_t not_empty = PTHREAD_COND_INITIALIZER;

int main()
{
    pthread_t producer, consumer;
     
    pthread_create(&producer, NULL, producer_f, NULL);
    pthread_create(&consumer, NULL, consumer_f, NULL);
    
    pthread_join(producer, NULL);
    pthread_join(consumer, NULL);
    
    return 0;
}

void* producer_f(void* arg){
    int n = max_iteration;
    while(n > 0){
        pthread_mutex_lock(&m);
        while(size == max_size) {
            pthread_cond_wait(&not_full, &m);
        }
    
        printf("produce %d\n", id);
        buffer[tail%max_size] = id++;
        tail++;
        size++;
    
        pthread_mutex_unlock(&m);
        // Prevent spurious wakeup if signal after release mutex
        pthread_cond_signal(&not_empty);
    
        n--;
    }
}

void* consumer_f(void* arg){
    int n = max_iteration;
    while(n > 0){
        pthread_mutex_lock(&m);
        while(size == 0) {
            pthread_cond_wait(&not_empty, &m);
        }
    
        printf("consume %d\n", buffer[head%max_size]);
        head++;
        size--;
    
        pthread_mutex_unlock(&m);
        pthread_cond_signal(&not_full);
    
        n--;
    }
}