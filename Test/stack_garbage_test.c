#include <stdio.h>

void f0() {
    long local = 188;
}

void f1()
{
    long local;
    // output 188
    printf("%ld", local);
}

int main()
{
    f0();
    f1();
    return 0;
}