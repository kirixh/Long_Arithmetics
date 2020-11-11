//
// Created by kika on 23.10.2020.
//
/*
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
//Инициализировать значение BN десятичным представлением строки
int bn_init_string(bn *t, const char *init_string){
    if (t==NULL || init_string==NULL || init_string[0]=='\0') return BN_NULL_OBJECT;
    char a=init_string[0];
    int flag=0; /// флажок, нужен чтобы обрабатывать по-разному положительные и отрицательные числа.
    int len = -1; /// счётчик длины
    if (a=='-'){t->sign = -1;len++;flag=1;}
    else if(a=='0'){t->bodysize=1;t->sign=0;t->body[0]=0;return BN_OK;}///если 0 то всё BN=0
    else {t->sign = 1;}
    while (a){a=init_string[len+1];len++;} ///Считаем длину числа
    int i =0;
    if (flag) {i =1;len--;} /// если отрицательное
    t->bodysize = len;
    t->body = realloc(t->body,(len) * sizeof(int)); ///закаываем память
    if (t->body == NULL) return BN_NO_MEMORY;
    while(i<len+flag){
        a = init_string[i];
        t->body[i-flag] = a - 48;
        i++;
    }
    int tmp;
    i=len/2; /// используем старую переменную
    for(int j=0;j<i;j++){ ///меняем местами цифры, так как они записаны в прямом порядке
        tmp=t->body[len-1];
        t->body[len-1]=t->body[j];
        t->body[j]=tmp;
        len--;
    }
    return BN_OK;

}
// Инициализировать значение BN заданным целым числом
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
int bn_sign(bn const *t){ ///-1 если t<0; 0 если t = 0, 1 если t>0
    if (t==NULL) return BN_NULL_OBJECT;
    return t->sign;
}
int bn_abs(bn *t){ ///взять модуль
    if (t==NULL) return BN_NULL_OBJECT;
    if (t->sign==-1) t->sign = 1;
    return BN_OK;
}
int bn_neg(bn *t){
    if (t==NULL) return BN_NULL_OBJECT;
    t->sign *=-1;
    return BN_OK;
}
int bn_cmp(bn const *left, bn const *right){ /// Если левое меньше, вернуть <0; если равны, вернуть 0; иначе >0
    if (left==NULL || right == NULL) return BN_NULL_OBJECT;
    int ret = 1;
    if (left->sign > right->sign) return 1;
    if (left->sign < right->sign) return -1;
    if (left->sign == right->sign){
        if (left->sign==0) return 0;
        if (left->sign==-1) ret=-1;
        if (left->bodysize > right->bodysize) return ret;
        if (left->bodysize < right->bodysize) return -ret;
        for (int i=left->bodysize-1;i>=0;i--){
            if (left->body[i] > right->body[i]) return ret;
            if (left->body[i] < right->body[i]) return -ret;
        }
    }
    return 0;
}

int bn_cmp_abs(bn const *left, bn const *right){ /// Сравнение по модулю
    if (left==NULL || right == NULL) return BN_NULL_OBJECT;
    int ret = 1;
    if (left->sign==0 && right->sign==0) return 0;
    if (right->sign==0) return ret;
    if (left->sign==0) return -ret;
    if (left->bodysize > right->bodysize) return ret;
    if (left->bodysize < right->bodysize) return -ret;
    for (int i=left->bodysize-1;i>=0;i--){
        if (left->body[i] > right->body[i]) return ret;
        if (left->body[i] < right->body[i]) return -ret;
        }
    return 0;
}

int bn_mul_small (bn *t, const int n){
    if (t==NULL) return BN_NULL_OBJECT;
    if (n==0) { ///Если n=0 то bn=0
        t->bodysize=1;
        t->sign=0;
        t->body[0]=0;
        return BN_OK;
    }
    int buff =0; /// будем хранить остаток
    int j;
 for (int i = 0;i<t->bodysize;i++){
     j=t->body[i]*n;
     t->body[i]=(buff + j)%10;
     buff = (buff+j)/10;
 }
 if (buff){
     int reall_size=4; /// для того чтобы дозаказывать память постепенно
     t->body=realloc(t->body,(reall_size+t->bodysize) * sizeof(int)); ///дозаказываем память
     if (t->body == NULL) return BN_NO_MEMORY;
     int i = 1;
     while(buff){ ///если длина произведения больше чем у t
         if (i==reall_size){
             reall_size*=2;
             i=1;
             t->body=realloc(t->body,(reall_size+t->bodysize)*sizeof(int)); ///дозаказываем память
             if (t->body == NULL) return BN_NO_MEMORY;
         }
         t->body[t->bodysize]=buff%10;
         t->bodysize++;
         buff/=10;
         i++;
     }
 }
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
bn* bn_sub_sign(bn const *left, bn const *right) { ///Вычитание BN одного знака left - right
    if (left == NULL || right == NULL) return NULL;
    int flag = bn_cmp_abs(left, right);
    bn *res = bn_new();
    if (flag == 0) return res; /// если числа равны то разность 0
    if (left->sign)
        res->sign = left->sign;
    else
        res->sign = right->sign;
    int reall_size = 4;                                              /// для того чтобы дозаказывать память постепенно
    res->body = realloc(res->body, reall_size * sizeof(int)); ///дозаказываем память
    if (res->body == NULL) return NULL;
    int buff;                                                        /// промежуточное вычисление
    int j=0;                                                          ///счетчик в котором храним то что занимаем на предыдущем шаге
    for (int i = 0; i < left->bodysize; i++) {
        res->bodysize++;
        if (i < right->bodysize) {                                     ///если меньше меньшего, то отнимаем цифры обоих чисел
            buff = left->body[i] - right->body[i] - j;
            j=0;
        } else{                                                       ///иначе в ответ идут цифры большего
            buff = left->body[i] - j;
            j=0;
        }
        if (i == reall_size - 1) {                                  /// если массив переполнился
            reall_size *= 2;
            res->body = realloc(res->body, reall_size * sizeof(int)); ///дозаказываем память
            if (res->body == NULL) return NULL;
        }
        if (buff < 0) { /// занимаем у старшего разряда
            j++;
            buff += 10;
        }
        res->body[i] = buff;
    }
    res->bodysize--;
    while (res->body[res->bodysize - 1] == 0) ///убираем незначащие нули
        res->bodysize--;

    return res;
}

bn* bn_add(bn const *left, bn const *right){
    if (left == NULL || right == NULL) return NULL;
    if (left->sign==right->sign){
            return bn_add_sign(left,right);
    }
    else{
        int k = bn_cmp_abs(left,right);
        if (k==1)
            return bn_sub_sign(left,right);
        else if (k==0)
            return bn_new();
        else
            return bn_sub_sign(right,left);
    }
}
bn* bn_sub(bn const *left, bn const *right){
    if (left == NULL || right == NULL) return NULL;
    int cmp_abs = bn_cmp_abs(left,right);
    if (left->sign==right->sign){
        if (cmp_abs==1) return bn_sub_sign(left,right);
        else if (cmp_abs==0) return bn_new();
        else {
            bn *res = bn_sub_sign(right,left);
            res->sign*=-1;
            return res;
        }
    }
    else if (left->sign>right->sign){ /// 1 : 0 ; 1 : -1; 0 : -1 .
        bn *res = bn_add_sign(left,right);
        if (left->sign==0) res->sign*=-1;
        return res;
    }
    else { /// -1 : 0 ; -1 : 1; 0 : 1 .
        bn * res = bn_add_sign(left,right);
        if (left->sign==0) res->sign*=-1;
        return res;
    }
}

bn* bn_mul(bn const *left, bn const *right){
    if (left==NULL|| right==NULL) return NULL;
    if (left->sign == 0 || right->sign==0) return bn_new(); ///Если один множитель 0 то все произведение 0
    bn *res = bn_new();
    bn *right_cp_10=bn_init(right); ///Здесь хранится копия правого числа, которая умножается на 10 каждый раз кроме 1 итерации
    res->sign = left->sign * right->sign;
    for (int i=0;i<left->bodysize;i++){
        if (i>0) bn_mul_small(right_cp_10,10);
        bn *right_cp=bn_init(right_cp_10); /// Временная копия, которая нужна для операций чтобы не менять исходное число
        bn_mul_small(right_cp,left->body[i]);
        bn *tmp = bn_add_sign(res,right_cp); /// Переменная для замены значения res
        bn_delete(res);
        bn_delete(right_cp);
        res = bn_init(tmp);
        bn_delete(tmp);
    }
    bn_delete(right_cp_10);
    return res;
}

int bn_add_to(bn *t, bn const *right){
    if (t==NULL || right == NULL)
        return BN_NULL_OBJECT;
    bn *tmp = bn_add_sign(t,right);
    t->sign=tmp->sign;
    t->bodysize=tmp->bodysize;
    t->body=realloc(t->body,sizeof(int) * t->bodysize);
    if (t->body == NULL){
        free(t);
        return BN_NO_MEMORY;
    }
    for (int i=0;i<t->bodysize;i++){
        t->body[i]=tmp->body[i];
    }
    bn_delete(tmp);
    return BN_OK;
}

int bn_div_small(bn *t, const int n){
    if (t==NULL) return BN_NULL_OBJECT;
    if (n==0) { ///Если n=0 то bn=0
        return BN_DIVIDE_BY_ZERO;
    }
    if (n<0) t->sign*=-1;
    int buff =0; /// будем хранить остаток
    for (int i=t->bodysize-1;i>=0;i--){
        buff = buff*10 + t->body[i];
        t->body[i]=buff/n;
        buff%=n;
    }
    while (t->body[t->bodysize - 1] == 0){ ///убираем незначащие нули
        if (t->bodysize == 1)
            break;
        t->bodysize--;
    }
    return BN_OK;
}

bn* bn_div(bn const *left, bn const *right){
    if (left==NULL || right==NULL) return NULL;
    if (right->sign == 0) return NULL;
    if (right->bodysize==1 && right->body[0]==1){      ///При делении на 1 бин поиск циклиться
        bn *m=bn_init(left);
        m->sign=left->sign * right->sign;
        return m;
    }
    if (bn_cmp_abs(left,right)==-1) return bn_new();  ///если модуль делителя больше делимого то 0
    bn *right1 = bn_init(right);
    bn_abs(right1);                                 ///создаю копии входных данных, так как с отрицательными я работаю так же, как и с положительными
    bn *left1 = bn_init(left);
    bn_abs(left1);
    bn *r=bn_init(left1);                           /// l и r границы бин поиска
    bn *l=bn_new();
    bn_init_int(l,1);
    int p;
    ///пока разность между (произведением ответа и делителя) и делимым не станет меньше чем делимое
    while (1){
        bn *m = bn_add(l,r);
        bn_div_small(m,2);                ///нахожу середину
        bn *tmp =bn_mul(m,right1);
        p=bn_cmp(tmp,left1);                ///сравниваю с делимым
        bn *diff = bn_sub(left1,tmp);       ///разность нужна для выхода из цикла
        /*printf("l= ");
        bn_print(l);
        printf("r= ");
        bn_print(r);
        printf("m= ");
        bn_print(m);
        printf("p= %d",p);
        printf("\n\n");*/
        /*
        if (p>0){               ///если ответ больше чем делимое, то правую границу уменьшаю
            bn_delete(r);
            r=bn_init(m);
            bn_delete(m);
            bn_delete(tmp);
            bn_delete(diff);
        }
        else if (bn_cmp(diff,right1)<1) {  ///если разность меньше или равна делителю то число найдено
            bn_delete(r);
            bn_delete(l);
            bn_delete(tmp);
            bn_delete(diff);
            bn_delete(left1);
            bn_delete(right1);
            m->sign=right->sign*left->sign;
            return m;
        }
        else{                          ///иначе увеличиваю левую границу
            bn_delete(l);
            l=bn_init(m);
            bn_delete(m);
            bn_delete(tmp);
            bn_delete(diff);
        }
    }
}
int bn_pow_to(bn *t, int degree) {
    if (t == NULL) return BN_NULL_OBJECT;
    if (degree < 0) {
        t->sign=0;
        t->bodysize=1;
        t->body[0]=1;
        return BN_OK;
    }
    bn *cp_t = bn_init(t);
    for (int i = 0; i < degree-1; i++) {
        bn *tmp = bn_mul(t, cp_t);
        t->sign=tmp->sign;
        t->bodysize=tmp->bodysize;
        t->body=realloc(t->body,sizeof(int) * t->bodysize);
        if (t->body == NULL){
            free(t);
            return BN_NO_MEMORY;
        }
        for (int i=0;i<t->bodysize;i++){
            t->body[i]=tmp->body[i];
        }
        bn_delete(tmp);
    }
    bn_delete(cp_t);
    return BN_OK;
}

int bn_root_to(bn *t, int reciprocal){
    if (t==NULL) return BN_NULL_OBJECT;
    if (reciprocal==0){
        t->sign=0;
        t->bodysize=1;
        t->body[0]=0;
        return BN_OK;
    }
    if (reciprocal<0) return BN_NULL_OBJECT;
    if (reciprocal==1) return BN_OK;
    if (t->sign==-1 && reciprocal % 2 == 0) {
        return BN_NULL_OBJECT;
    }
    if (t->bodysize==1 && (t->sign==0 || (t->body[0]==1)))       ///если 0, 1, -1
        return BN_OK;
    bn *r=bn_init(t);                           /// l и r границы бин поиска
    bn_abs(r);
    bn *l=bn_new();
    int p;
    ///пока разность между (произведением ответа и делителя) и делимым не станет меньше чем делимое
    while (1){
        bn *m = bn_add(l,r);
        bn_div_small(m,2);                ///нахожу середину
        bn *m1=bn_init(m);
        bn *m1_1=bn_init(m);
        bn *tmp=bn_new();
        if (t->sign==1)
            bn_init_int(tmp,1);
        else
            bn_init_int(tmp,-1);
        bn_add_to(m1_1,tmp);
        bn_delete(tmp);
        bn_pow_to(m1,reciprocal);
        bn_pow_to(m1_1,reciprocal);
        p=bn_cmp_abs(m1,t);
        int diff=bn_cmp_abs(m1_1,t);
        /*
        printf("l= ");
        bn_print(l);
        printf("r= ");
        bn_print(r);
        printf("m= ");
        bn_print(m);
        printf("p= %d\n",p);
        printf("m1= ");
        bn_print(m1);
        printf("m1_1= ");
        bn_print(m1_1);
        printf("\n\n");

        bn_delete(m1);
        bn_delete(m1_1);
        if (p>0){               ///если ответ больше чем делимое, то правую границу уменьшаю
            bn_delete(r);
            r=bn_init(m);
            bn_delete(m);
        }
        else if (diff==1) {  ///если разность меньше или равна делителю то число найдено
            bn_delete(r);
            bn_delete(l);
            t->bodysize=m->bodysize;
            t->body=realloc(t->body,sizeof(int) * t->bodysize);
            if (t->body == NULL){
                free(t);
                return BN_NO_MEMORY;
            }
            for (int i=0;i<t->bodysize;i++){
                t->body[i]=m->body[i];
            }
            bn_delete(m);
            return BN_OK;
        }
        else{                          ///иначе увеличиваю левую границу
            bn_delete(l);
            l=bn_init(m);
            bn_delete(m);
        }
    }
}



/// Инициализировать значение BN представлением строки в системе счисления radix
int bn_init_string_radix(bn *t, const char *init_string, int radix){
if (t==NULL || init_string==NULL || init_string[0]=='\0') return BN_NULL_OBJECT;
char a = init_string[0];
int i =0;
bn *sum=bn_new();
if (a=='-') {t->sign=-1;i =1;}
else if(a=='0'){t->bodysize=1;t->sign=0;t->body[0]=0;return BN_OK;}///если 0 то всё BN=0
while (a){
    a=init_string[i];
    int k;
    if (a=='\0') break;
    if (a >= '0' && a <= '9')
        k = a - '0';
    else
        k= a - 'A' + 10;
    bn_mul_small(sum,radix);
    bn *g = bn_new();
    bn_init_int(g,k);
    bn *res = bn_add_sign(sum,g);
    bn_delete(sum);
    sum=bn_init(res);
    bn_delete(res);
    bn_delete(g);
    i++;
}
t->sign=sum->sign;
t->bodysize=sum->bodysize;
t->body=realloc(t->body,sizeof(int) * t->bodysize);
    if (t->body == NULL){
        free(t);
        return BN_NO_MEMORY;
    }
    for (int i=0;i<t->bodysize;i++){
        t->body[i]=sum->body[i];
    }
bn_delete(sum);
return BN_OK;
}
*/