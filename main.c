//#include "bn.h"
#include <stdio.h>
#include <stdlib.h>
//#include <string.h>
//#include <time.h>
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
///Фибоначчи
/*int main() {
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
    int j;                                                          ///счетчик в котором храним то что занимаем на предыдущем шаге
    for (int i = 0; i < left->bodysize; i++) {
        res->bodysize++;
        if (i < right->bodysize) {                                     ///если меньше меньшего, то отнимаем цифры обоих чисел
            j = left->body[i] - right->body[i];
        } else                                                          ///иначе в ответ идут цифры большего
            j = left->body[i];
        if (i == reall_size - 1) {                                  /// если массив переполнился
            reall_size *= 2;
            res->body = realloc(res->body, reall_size * sizeof(int)); ///дозаказываем память
            if (res->body == NULL) return NULL;
        }
        if (j < 0) { /// занимаем у старшего разряда
            left->body[i + 1]--;
            j += 10;
        }
        res->body[i] = j;
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

char *getstring(int *len){
    *len=0;
    int real_num=1;
    char * init_string1 = (char *)malloc(1);
    char a=getchar();
    while (a!='\n'){
        init_string1[(*len)++]=a;
        if (*len>=real_num){
            real_num*=2;
            init_string1=(char *)realloc(init_string1,real_num);
        }
        a=getchar();
    }
    init_string1[*len]='\0';
    return init_string1;
}
/*int main(){ ///Длинное слож/вычитание
    int len;
    char *init_string1=getstring(&len);
    char *oper=getstring(&len);
    char *init_string2=getstring(&len);
    bn *a=bn_new();
    bn *b=bn_new();
    bn *c=bn_new();
    bn_init_string(a,init_string1);
    bn_init_string(b,init_string2);
    if (oper[0]=='+'){
        bn_delete(c);
        c=bn_add(a,b);}
    else{
        bn_delete(c);
        c=bn_sub(a,b);}
    bn_print(c);
    bn_delete(c);
    bn_delete(a);
    bn_delete(b);
    free (init_string1);
    free (oper);
    free (init_string2);
    return 0;
}
*/

int main(){ ///Длинное умножение
    int len;
    char *init_string1=getstring(&len);
    char *oper=getstring(&len);
    char *init_string2=getstring(&len);
    bn *a=bn_new();
    bn *b=bn_new();
    bn_init_string(a,init_string1);
    bn_init_string(b,init_string2);
    bn *c= bn_mul(a,b);
    bn_print(c);
    bn_delete(c);
    bn_delete(a);
    bn_delete(b);
    free (init_string1);
    free (oper);
    free (init_string2);
    return 0;
}


/*
int main(){
    bn *a = bn_new();
    bn *b = bn_new();
    //bn *c = bn_new();
    //printf("%d ", 1^-1);
    bn_init_string(a,"-1");
    //char* init_string = "FFFFFFFZKSQE";
    int code = bn_init_int(b,1);
    //bn *c=bn_init(b);
    //int code = bn_init_string(a,init_string);
    //code = bn_init_int(b,0);
    //bn_init_string_radix(a,init_string,36);
    bn_print(a);
    //code = bn_mul_small(a,13);
    //bn_print(a);
    //bn *c = bn_add_sign(b,a);
    //bn_print(c);
    bn_print(b);
    printf("сравнили %d \n",bn_cmp_abs(a,b));
    bn *c = bn_mul(a,b);
    bn_print(c);
    //code = bn_cmp(a,b);
    printf("cmp ret %d \n",code);
    bn_delete(a);
    bn_delete(b);
    bn_delete(c);
    return 0;
}*/