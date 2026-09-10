#include "array.h"
#include <stdio.h>

int main() {
    Array a = array_new(4);
    for(int i = 0; i < 8; i++){
        array_insertBack(&a, 1);
        array_print(a);
        printf("%lu\n", a.back);
    }
    printf("%lu\n", a.capacity);
    return 0;
}