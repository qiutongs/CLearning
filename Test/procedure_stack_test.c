#include <stdio.h>

long f0(long a, long b) {
    long local = 188, d = a + b;
    int arr[5];
    return 100;
}

int main()
{
    printf("main address: %ld\n", &main);
    printf("f0 address: %ld\n", &f0);
    printf("\n");
    
    short a = 88;
    printf("main: &a: %ld, %ld\n", &a, a);
    
    for (long i = 5; i >= 0; i--) {
        printf("address %ld, value %ld\n", &a + i, *(long*)(&a + i));
    }
    printf("--\n");
    
    f0(101, 102);
    return 999;
}