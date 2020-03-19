#include <stdio.h>
#include <string.h>
#include <stdint.h>

int main()
{
    //printf("Hello, World!\n");
    //char * a = "秋";
    char var[] = "宋";
    //printf("%ld\n", strlen(a));
    
    printf("%u\n", (uint8_t)var[0]);
    printf("%u\n", (uint8_t)var[1]);
    printf("%u\n", (uint8_t)var[2]);

    return 0;
}