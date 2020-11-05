#include "bn.h"
#include <stdio.h>
#include <stdlib.h>
/*#include <time.h>
struct bn_s {
    int *body;
    int bodysize;
    int sign;
};
typedef struct bn_s bn;
enum bn_codes {
    BN_OK, BN_NULL_OBJECT, BN_NO_MEMORY, BN_DIVIDE_BY_ZERO
};

bn *bn_new(){
    bn *r = malloc(sizeof(bn));
    if (r== NULL) return NULL;
    r->bodysize =1;
    r->sign = 0;
    r->body = malloc(sizeof(int)* r->bodysize);
    if (r->body == NULL){
        free(r);
        return NULL;
    }
    r->body[0]=0;
    return r;
}

int bn_init_int(bn *t, int init_int){
    if (t==NULL) return BN_NULL_OBJECT;
    if (init_int==0){ ///если 0 то всё BN=0
        t->bodysize=1;
        t->sign=0;
        t->body[0]=0;
        return BN_OK;
    }
    if (init_int>0) t->sign = 1;
    else {t->sign = -1; init_int*=-1;} /// если отрицательное то привращаем в положительное
    int a=init_int%10; /// получаем последнюю цифру
    int len = 1;
    while (init_int){
        t->body=realloc(t->body,len*sizeof(int)); ///дозаказываем память
        if (t->body == NULL) return BN_NO_MEMORY;
        t->body[len-1]=a;
        init_int/=10;
        a=init_int%10;
        len++;
    }
    len--; /// len всегда на 1 больше чем длина числа
    t->bodysize=len;
    return BN_OK;

}

bn* bn_add_sign(bn const *left, bn const *right){ ///Сложение BN одного знака
    if (left == NULL || right == NULL) return NULL;
    bn *res = bn_new(); ///создаем новое bn
    int flag =0; ///флаг, нужен для определения числа большей длины
    if (left->sign)
        res->sign = left->sign;
    else
        res->sign = right->sign;
    if (left->bodysize > right->bodysize){
        res->bodysize=left->bodysize;
        flag=1;
    }
    else
        res->bodysize=right->bodysize;
    res->body=realloc(res->body,(res->bodysize+1)*sizeof(int)); ///выделяем память
    if (res->body == NULL) return NULL;
    int j = 0; ///счетчик в котором будет то что мы "откладываем в уме"
    if (flag){
        for (int i = 0;i<left->bodysize;i++){
            if (i<right->bodysize) ///если меньше меньшего, то прибаляем цифры обоих чисел
                j+=left->body[i]+right->body[i];
            else ///иначе только большего
                j+=left->body[i];
            res->body[i]=j%10;
            j/=10;
        }
        if (j){
            res->body[left->bodysize]=j;
            res->bodysize++;
        }
    }
    else{ ///аналогично, но когда правое больше или равно левому
        for (int i = 0;i<right->bodysize;i++){
            if (i<left->bodysize)
                j= j+ left->body[i]+right->body[i];
            else
                j= j+ right->body[i];
            res->body[i]=j%10;
            j/=10;
        }
        if (j){
            res->body[right->bodysize]=j;
            res->bodysize++;
        }
    }
    return res;
}

bn *bn_init(bn const *orig){
    bn *r = malloc(sizeof(bn));
    if (r==NULL) return NULL;
    r->bodysize=orig->bodysize;
    r->sign=orig->sign;
    r->body=malloc(sizeof(int) * r->bodysize);
    if (r->body == NULL){
        free(r);
        return NULL;
    }
    for (int i=0;i<r->bodysize;i++){
        r->body[i]=orig->body[i];
    }
    return r;
}
int bn_delete(bn *t){
    if (t==NULL) return BN_NULL_OBJECT;
    free(t->body);
    free(t);
    return BN_OK;
}
//Вывести на экран значение BN
void bn_print(bn const *num){
    if (num->sign == -1) putchar('-');
    for(int i=num->bodysize-1; i>=0;i--){
        putchar(num->body[i]+48);
    }
    putchar('\n');
}

int main() {
    int n;
    scanf("%d",&n);
    bn *a=bn_new();
    bn *b=bn_new();
    bn *s=bn_new();
    bn_init_int(a,1);
    bn_init_int(b,1);
    //bn_init_string(b,"12200160415121876738");
    //bn_init_string(a,"19740274219868223167"); //F95=31940434634990099905
    time_t start,end;
    volatile long unsigned t;
    start = time(NULL);
    for (int i=2;i<n;i++){
        bn_delete(s);
        s = bn_add_sign(a,b);
        bn_delete(b);
        b=bn_init(a);
        bn_delete(a);
        a=bn_init(s);
    }
    end = time(NULL);
    printf("Цикл использовал %f секунд.\n", difftime(end, start));
    bn_print(s);
    bn_delete(a);
    bn_delete(b);
    bn_delete(s);
    return 0;
}*/



int main(){
    bn *a = bn_new();
    bn *b = bn_new();
    //bn *c = bn_new();
    //printf("%d ", 7%10);
    bn_init_string(a,"123");
    char* init_string = "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFF";
    int code = bn_init_int(b,1048575);
    bn *c=bn_init(b);
    //int code = bn_init_string(a,init_string);
    //code = bn_init_int(b,0);
    bn_init_string_radix(a,init_string,16);
    bn_print(a);
    //code = bn_mul_small(a,13);
    //bn_print(a);
    //bn *c = bn_add_sign(b,a);
    bn_print(c);
    //bn_print(b);
    //code = bn_cmp(a,b);
    printf("cmp ret %d \n",code);
    bn_delete(a);
    bn_delete(b);
    bn_delete(c);
    return 0;
}