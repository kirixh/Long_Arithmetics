//
// Created by kika on 23.10.2020.
//
#include <stdlib.h>
#include <stdio.h>
#include "bn.h"
//#include <string.h>
struct bn_s {
    int *body;
    int bodysize;
    int sign;
};
//Создать новое BN
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
//Скопировать существующее BN
bn *bn_init(bn const *orig){
    bn *r = malloc(sizeof(bn));
    if (r==NULL) return NULL;
    r->bodysize=orig->bodysize;
    r->sign=orig->sign;
    r->body=orig->body;
}
//Вывести на экран значение BN
void bn_print(bn const *num){
    if (num->sign == -1) putchar('-');
    for(int i=0; i<num->bodysize;i++){
        putchar(num->body[i]+48);
    }
    putchar('\n');
}
//Инициализировать значение BN десятичным представлением строки
int bn_init_string(bn *t, const char *init_string){
    if (t==NULL || init_string==NULL || init_string[0]=='\0') return BN_NULL_OBJECT;
    char a=init_string[0];
    int flag=0; // флажок, нужен чтобы обрабатывать по-разному положительные и отрицательные числа.
    int fl=1; // еще флажок, та же причина
    if (a=='-'){t->sign = -1;a=init_string[1];flag=1;}
    else if(a=='0'){t->bodysize=1;t->sign=0;t->body[0]=0;return BN_OK;}//если 0 то всё BN=0
    else {t->sign = 1;}
    int len = 1; //счётчик длины
    while (a){
        if(flag){ //если отрицательное
            t->body[0]=a-48;
            flag=0;
            a = init_string[2];
            fl=0;
        }
        else {
            len++;
            t->body = realloc(t->body, (len) * sizeof(int)); //дозакаываем память
            if (t->body == NULL) return BN_NO_MEMORY;
            t->body[len-1-fl] = a - 48;
            a = init_string[len+1-2*fl];
        }
    }
    if (fl) //если отрицательное
        t->bodysize = len-1;
    else
        t->bodysize = len;
    return BN_OK;
}
// Инициализировать значение BN заданным целым числом
int bn_init_int(bn *t, int init_int){
    if (t==NULL) return BN_NULL_OBJECT;
    if (init_int==0){ //если 0 то всё BN=0
        t->bodysize=1;
        t->sign=0;
        t->body[0]=0;
        return BN_OK;
    }
    if (init_int>0) t->sign = 1;
    else {t->sign = -1; init_int*=-1;} // если отрицательное то привращаем в положительное
    int a=init_int%10; // получаем последнюю цифру
    int len = 1;
    while (init_int){
        t->body=realloc(t->body,len*sizeof(int));//дозаказываем память
        if (t->body == NULL) return BN_NO_MEMORY;
        t->body[len-1]=a;
        init_int/=10;
        a=init_int%10;
        len++;
    }
    len--; // len всегда на 1 больше чем длина числа
    t->bodysize=len;
    int tmp;
    init_int = len/2; // используем отслужившую переменную
    for(int j=0;j<init_int;j++){ //меняем местами цифры, так как они записаны в обратном порядке
        tmp=t->body[len-1];
        t->body[len-1]=t->body[j];
        t->body[j]=tmp;
        len--;
    }
    return BN_OK;

}
int bn_sign(bn const *t){ //-1 если t<0; 0 если t = 0, 1 если t>0
    if (t==NULL) return BN_NULL_OBJECT;
    return t->sign;
}
int bn_abs(bn *t){ //взять модуль
    if (t==NULL) return BN_NULL_OBJECT;
    if (t->sign==-1) t->sign = 1;
    return BN_OK;
}
int bn_neg(bn *t){
    if (t==NULL) return BN_NULL_OBJECT;
    t->sign *=-1;
    return BN_OK;
}
int bn_cmp(bn const *left, bn const *right){ // Если левое меньше, вернуть <0; если равны, вернуть 0; иначе >0
    if (left==NULL || right == NULL) return BN_NULL_OBJECT;
    int ret = 1;
    if (left->sign > right->sign) return 1;
    if (left->sign < right->sign) return -1;
    if (left->sign == right->sign){
        if (left->sign==0) return 0;
        if (left->sign==-1) ret=-1;
        if (left->bodysize > right->bodysize) return ret;
        if (left->bodysize < right->bodysize) return -ret;
        for (int i=0;i<left->bodysize;i++){
            if (left->body[i] > right->body[i]) return ret;
            if (left->body[i] < right->body[i]) return -ret;
        }
        return 0;
    }
}

int bn_mul_small (bn *t, const long long int n){
    char * ans = "";
    long long int buff =0;
    long long j = 0 ;
 for (int i = t->bodysize;i>0;i--){
     j=t->body[i]*n;
 }
}

int bn_init_string_radix(bn *t, const char *init_string, int radix){ // Инициализировать значение BN представлением строки
char * p="0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";// в системе счисления radix
if (t==NULL || init_string==NULL || init_string[0]=='\0') return BN_NULL_OBJECT;
char a = init_string[0];
int i =0;
int sum=0;
int flag =1; // для того чтобы потом инициализировать от отрицательного int
if (a=='-') {flag = -1;i =1;}
while (a){
    a=init_string[i];
    if (a=='\0') break;
    int k=0;
    for (k;k<32;k++){ // находим номер цифры в строке p
        if (p[k]==a) break;
    }
    sum=sum*radix + k;
    i++;
}
return bn_init_int(t,flag*sum);
}
