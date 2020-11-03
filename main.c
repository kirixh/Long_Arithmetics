#include "bn.h"
#include <stdio.h>
int main() {
    bn *a = bn_new();
    bn *b = bn_init(a);
    char* init_string = "FFFFFFF";
    int code = bn_init_string_radix(a,init_string,32);
    bn_print(a);
    //code = bn_init_int(b,-1234567);
    //bn_print(b);
    //code = bn_cmp(a,b);
    printf("cmp ret %d \n",code);
    return 0;
}