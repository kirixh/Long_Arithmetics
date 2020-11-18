//
// Created by kika on 23.10.2020.
//

#include <stdlib.h>
#include <stdio.h>
//#include "bn.h"
typedef struct bn_s bn;
enum bn_codes {
    BN_OK, BN_NULL_OBJECT, BN_NO_MEMORY, BN_DIVIDE_BY_ZERO
};
struct bn_s {
    long long int *body;
    int bodysize;
    int sign;
};
typedef long long int ll;
const int base = 1000000000;
const int digits=9;
//Создать новое BN
bn *bn_new(){
    bn *r = malloc(sizeof(bn));
    if (r== NULL) return NULL;
    r->bodysize =1;
    r->sign = 0;
    r->body = malloc(sizeof(ll)* r->bodysize);
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
    r->body=malloc(sizeof(ll) * r->bodysize);
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
    printf("%lld",num->body[num->bodysize-1]);
    for(int i=num->bodysize-2; i>=0;i--){
        printf("%.9lld",num->body[i]);
    }
    putchar('\n');
}
//Инициализировать значение BN десятичным представлением строки
int bn_init_string(bn *t, const char *init_string){
    if (t==NULL || init_string==NULL || init_string[0]=='\0') return BN_NULL_OBJECT;
    char a=init_string[0];
    int flag=0; /// флажок, нужен чтобы обрабатывать по-разному положительные и отрицательные числа.
    int len = -1; /// счётчик длины
    if (a=='-'){t->sign = -1;len++;flag=1;if(init_string[1]=='0'){t->bodysize=1;t->sign=0;t->body[0]=0;return BN_OK;}} ///проверка на - и на -0
    else if(a=='0'){t->bodysize=1;t->sign=0;t->body[0]=0;return BN_OK;}///если 0 то всё BN=0
    else {t->sign = 1;}
    while (a){a=init_string[len+1];len++;} ///Считаем длину числа
    if (flag) {len--;} /// если отрицательное
    t->bodysize = (len-1)/digits+1;
    t->body = realloc(t->body,t->bodysize * sizeof(ll)); ///заказываем память
    if (t->body == NULL) return BN_NO_MEMORY;
    for (int i=0;i<t->bodysize;i++){        ///вычисляю i-ю цифру числа
        t->body[i]=0;
        for (int j=digits;j>0;j--){
            int pos=len - i*digits - j;     ///позиция с которой записываю цифру в BN_цифру
            if (pos>=0)
                t->body[i]=t->body[i]*10 + (init_string[pos+flag]-48);
        }
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
    t->body[0]=init_int%base;
    if (init_int>=base){
        t->body=realloc(t->body,2*sizeof(ll)); ///дозаказываем память
        if (t->body == NULL) return BN_NO_MEMORY;
        t->bodysize++;
        t->body[1]=(init_int - init_int%base)/base;

    }
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
    ll buff =0; /// будем хранить остаток
    ll j;
    for (int i = 0;i<t->bodysize;i++){
        j=t->body[i]*n;
        t->body[i]=(buff + j)%base;
        buff = (buff+j)/base;
    }
    if (buff){
        int reall_size=2; /// для того чтобы дозаказывать память постепенно
        t->body=realloc(t->body,(reall_size*t->bodysize) * sizeof(ll)); ///дозаказываем память
        if (t->body == NULL) return BN_NO_MEMORY;
        int i = 1;
        while(buff){ ///если длина произведения больше чем у t
            if (i==reall_size){
                reall_size*=2;
                i=1;
                t->body=realloc(t->body,(reall_size)*sizeof(ll)); ///дозаказываем память
                if (t->body == NULL) return BN_NO_MEMORY;
            }
            t->body[t->bodysize]=buff%base;
            t->bodysize++;
            buff/=base;
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
    res->body=realloc(res->body,(res->bodysize+1)*sizeof(ll)); ///выделяем память
    if (res->body == NULL) return NULL;
    ll j = 0; ///счетчик в котором будет то что мы "откладываем в уме"
    if (flag){
        for (int i = 0;i<left->bodysize;i++){
            if (i<right->bodysize) ///если меньше меньшего, то прибаляем цифры обоих чисел
                j+=left->body[i]+right->body[i];
            else ///иначе только большего
                j+=left->body[i];
            res->body[i]=j%base;
            j/=base;
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
            res->body[i]=j%base;
            j/=base;
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
    int reall_size = 2;                                              /// для того чтобы дозаказывать память постепенно
    res->body = realloc(res->body, reall_size * sizeof(ll)); ///дозаказываем память
    if (res->body == NULL) return NULL;
    ll buff;                                                        /// промежуточное вычисление
    ll j=0;                                                          ///счетчик в котором храним то что занимаем на предыдущем шаге
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
            res->body = realloc(res->body, reall_size * sizeof(ll)); ///дозаказываем память
            if (res->body == NULL) return NULL;
        }
        if (buff < 0) { /// занимаем у старшего разряда
            j++;
            buff += base;
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
    bn *right_cp_base=bn_init(right); ///Здесь хранится копия правого числа, которая умножается на 10 каждый раз кроме 1 итерации
    res->sign = left->sign * right->sign;
    for (int i=0;i<left->bodysize;i++){
        if (i>0) bn_mul_small(right_cp_base,base);
        bn *right_cp=bn_init(right_cp_base); /// Временная копия, которая нужна для операций чтобы не менять исходное число
        bn_mul_small(right_cp,left->body[i]);
        bn *tmp = bn_add_sign(res,right_cp); /// Переменная для замены значения res
        bn_delete(res);
        bn_delete(right_cp);
        res = bn_init(tmp);
        bn_delete(tmp);
    }
    bn_delete(right_cp_base);
    return res;
}

int bn_add_to(bn *t, bn const *right){
    if (t==NULL || right == NULL)
        return BN_NULL_OBJECT;
    bn *tmp = bn_add(t,right);
    t->sign=tmp->sign;
    t->bodysize=tmp->bodysize;
    t->body=realloc(t->body,sizeof(ll) * t->bodysize);
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

int bn_sub_to(bn *t, bn const *right){
    if (t==NULL || right == NULL)
        return BN_NULL_OBJECT;
    bn *tmp = bn_sub(t,right);
    t->sign=tmp->sign;
    t->bodysize=tmp->bodysize;
    t->body=realloc(t->body,sizeof(ll) * t->bodysize);
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

int bn_sub_to_no_realloc(bn *t, bn const *right){                     ///вспомогательная функция для деления
    if (t==NULL || right == NULL)
        return BN_NULL_OBJECT;
    bn *tmp = bn_sub(t,right);
    t->sign=tmp->sign;
    t->bodysize=tmp->bodysize;
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

int bn_mul_to(bn *t, bn const *right){
    if (t==NULL || right == NULL)
        return BN_NULL_OBJECT;
    bn *tmp = bn_mul(t,right);
    t->sign=tmp->sign;
    t->bodysize=tmp->bodysize;
    t->body=realloc(t->body,sizeof(ll) * t->bodysize);
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
    ll buff =0; /// будем хранить остаток
    for (int i=t->bodysize-1;i>=0;i--){
        buff = buff*base + t->body[i];
        t->body[i]=buff/n;
        buff%=n;
    }
    while (t->body[t->bodysize - 1] == 0){ ///убираем незначащие нули
        if (t->bodysize == 1)
            break;
        t->bodysize--;
    }
    if (t->body[t->bodysize-1]==0) t->sign=0;
    return BN_OK;
}

int bn_mod_small(bn *t, const int n){
    if (t==NULL) return BN_NULL_OBJECT;
    if (n==0) { ///Если n=0 то bn=0
        return BN_DIVIDE_BY_ZERO;
    }
    if (n<0) t->sign*=-1;
    ll buff =0; /// будем хранить остаток
    for (int i=t->bodysize-1;i>=0;i--){
        buff = buff*base + t->body[i];
        t->body[i]=buff/n;
        buff%=n;
    }
    bn *tmp=bn_new();
    bn_init_int(tmp,buff);
    if (buff==0) t->sign=0;
    t->bodysize=tmp->bodysize;
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


bn* bn_div(bn const *left, bn const *right) {
    if (left == NULL || right == NULL) return NULL;
    if (right->sign == 0) return NULL;
    if (right->bodysize == 1 && right->body[0] == 1) {      ///При делении на 1 алгоритм циклится
        bn *m = bn_init(left);
        m->sign = left->sign * right->sign;
        return m;
    }
    if (bn_cmp_abs(left, right) == -1) return bn_new();  ///если модуль делителя больше делимого то 0
    bn *res = bn_new();
    res->body = realloc(res->body, left->bodysize*sizeof(ll));
    res->bodysize=0;
    bn *current = bn_new();
    current->body = realloc(current->body, (left->bodysize+1)*sizeof(ll)); ///буду хранить текущее, от чего вычитаю
    int k=0;
    int zero_flag=0;
    for (int i = left->bodysize-1; i>= 0; i--) {
        if (i-k<0){break;}                                           ///если закончились разряды
        current->sign=1;
        k--;
        int flag=-1;
        int dig_counter=0;
        while (bn_cmp_abs(current,right)==-1){                         ///записываем в current пока оно меньше чем right
            k++;
            if (i-k<0){
                if (flag!=-1)                                   /// если добавляли нули в ответ, то bodysize на 1 больше чем нужно
                    res->bodysize--;
                break;}
            if (current->bodysize>=2 || ((k>=1 || res->bodysize>0) && current->body[current->bodysize-1]!=0)) {///из-за обратного хранения сдвигаем все цифры вправо и записываем в начало
            dig_counter++;
                for (int j = current->bodysize; j > 0; j--) {
                    current->body[j]=current->body[j-1];
                }
                current->bodysize++;
            }
            ///если в left есть цифры

            current->body[0] = left->body[i-k];
            if ((res->bodysize>0 && current->body[current->bodysize-1]==0)||(res->bodysize>0 && dig_counter>=2)){ ///если первая цифра в current постоянно 0, то дописываем нули в ответ
                flag++;                                                                                         ///или делаем заем >1 раза
                res->body[left->bodysize-i-1+flag+zero_flag] = 0;
                res->bodysize++;
            }
        }
        if (flag>-1){                                           ///нужно дописать еще один 0 в ответ после цикла
            if (i-k<0 && current->body[current->bodysize-1]!=0){
            flag++;
            res->body[left->bodysize-i-1+flag+zero_flag] = 0;
            res->bodysize++;
            zero_flag+=flag+1;
            }
            else if(current->body[current->bodysize-1]!=0)
                zero_flag+=flag+1;
            else
                zero_flag+=flag;
        }
        ll l = 0;
        ll r = base;
        ll g = 0;
        while (l <= r) {
            ll m = (l + r) / 2;
            bn *t = bn_init(right);
            bn_abs(t);
            bn_mul_small(t, m);
            if (bn_cmp_abs(t,current)<1) {   ///если t<= current
                g = m;
                l = m + 1;
            }
            else r = m - 1;
            bn_delete(t);
        }
        res->body[left->bodysize-i-1+zero_flag] = g;
        res->bodysize++;
        bn *t=bn_init(right);
        bn_abs(t);
        bn_mul_small(t,g);
        bn_sub_to_no_realloc(current,t);           ///так как мы уже выделили под current left->bodysize + 1 ячейку
        bn_delete(t);
    }
    res->sign=left->sign*right->sign;
    ll tmp;
    ll o=res->bodysize/2;
    for(int j=0;j<o;j++){ ///меняем местами цифры, так как они записаны в прямом порядке
        tmp=res->body[res->bodysize-1-j];
        res->body[res->bodysize-1-j]=res->body[j];
        res->body[j]=tmp;
    }
    while(res->body[res->bodysize-1]==0)          ///избавляюсь от незначащих нулей
        res->bodysize--;
    if (res->sign==-1 && current->body[current->bodysize-1]!=0){
        bn *y=bn_new();
        bn_init_int(y,-1);
        bn_add_to(res,y);
        bn_delete(y);
    }
    bn_delete(current);
    return res;
}

bn* bn_mod(bn const *left, bn const *right){
    if (left == NULL || right == NULL) return NULL;
    if (right->sign == 0) return NULL;
    if (right->bodysize == 1 && right->body[0] == 1) {      ///При делении на 1 алгоритм циклится
        bn *m = bn_init(left);
        m->sign = left->sign * right->sign;
        return m;
    }
    if (bn_cmp_abs(left, right) == -1){bn *tmp =bn_init(left); tmp->sign=right->sign; return tmp;}  ///если модуль делителя больше делимого то остаток само число
    bn *res = bn_new();
    res->body = realloc(res->body, left->bodysize*sizeof(ll));
    res->bodysize=0;
    bn *current = bn_new();
    current->body = realloc(current->body, (left->bodysize+1)*sizeof(ll)); ///буду хранить текущее, от чего вычитаю
    int k=0;
    int zero_flag=0;
    for (int i = left->bodysize-1; i>= 0; i--) {
        if (i-k<0){break;}                                           ///если закончились разряды
        current->sign=1;
        k--;
        int flag=-1;
        int dig_counter=0;
        while (bn_cmp_abs(current,right)==-1){                         ///записываем в current пока оно меньше чем right
            k++;
            if (i-k<0){
                if (flag!=-1)                                   /// если добавляли нули в ответ, то bodysize на 1 больше чем нужно
                    res->bodysize--;
                break;}
            if (current->bodysize>=2 || ((k>=1 || res->bodysize>0) && current->body[current->bodysize-1]!=0)) {///из-за обратного хранения сдвигаем все цифры вправо и записываем в начало
                dig_counter++;
                for (int j = current->bodysize; j > 0; j--) {
                    current->body[j]=current->body[j-1];
                }
                current->bodysize++;
            }
            ///если в left есть цифры

            current->body[0] = left->body[i-k];
            if ((res->bodysize>0 && current->body[current->bodysize-1]==0)||(res->bodysize>0 && dig_counter>=2)){ ///если первая цифра в current постоянно 0, то дописываем нули в ответ
                flag++;                                                                                         ///или делаем заем >1 раза
                res->body[left->bodysize-i-1+flag+zero_flag] = 0;
                res->bodysize++;
            }
        }
        if (flag>-1){                                           ///нужно дописать еще один 0 в ответ после цикла
            if (i-k>=0){
                flag++;
                res->body[left->bodysize-i-1+flag] = 0;
                res->bodysize++;
            }
            zero_flag=flag+1;
        }
        ll l = 0;
        ll r = base;
        ll g = 0;
        while (l <= r) {
            ll m = (l + r) / 2;
            bn *t = bn_init(right);
            bn_abs(t);
            bn_mul_small(t, m);
            if (bn_cmp_abs(t,current)<1) {   ///если t<= current
                g = m;
                l = m + 1;
            }
            else r = m - 1;
            bn_delete(t);
        }
        res->body[left->bodysize-i-1] = g;
        res->bodysize++;
        bn *t=bn_init(right);
        bn_abs(t);
        bn_mul_small(t,g);
        bn_sub_to_no_realloc(current,t);           ///так как мы уже выделили под current left->bodysize + 1 ячейку
        bn_delete(t);
    }
    if (current->body[current->bodysize-1]!=0) current->sign=right->sign;
    if (left->sign*right->sign==-1 && current->body[0]!=0){
        bn *cp_current=bn_sub(right,current);
        bn_delete(current);
        current=bn_init(cp_current);
        bn_delete(cp_current);
    }
    bn_delete(res);
    return current;
}

int bn_div_to(bn *t, bn const *right){
    if (t==NULL || right == NULL)
        return BN_NULL_OBJECT;
    bn *tmp = bn_div(t,right);
    t->sign=tmp->sign;
    t->bodysize=tmp->bodysize;
    t->body=realloc(t->body,sizeof(ll) * t->bodysize);
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

int bn_mod_to(bn *t, bn const *right){
    if (t==NULL || right == NULL)
        return BN_NULL_OBJECT;
    bn *tmp = bn_mod(t,right);
    t->sign=tmp->sign;
    t->bodysize=tmp->bodysize;
    t->body=realloc(t->body,sizeof(ll) * t->bodysize);
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

int bn_pow_to(bn *t, int degree) {            ///метод быстрого возведения в степень
    if (t == NULL) return BN_NULL_OBJECT;
    if (degree < 0) {                       ///если степень отрицательная то 1
        t->sign=0;
        t->bodysize=1;
        t->body[0]=1;
        return BN_OK;
    }
    bn *ans=bn_new();
    bn_init_int(ans,1);
    while(degree){
        if (degree%2==0){           ///если четная, то умножаем степени исходного t
            degree/=2;
            bn *cp_t = bn_init(t);
            bn *tmp = bn_mul(t, cp_t);  ///код ниже копирует в t из tmp
            t->sign=tmp->sign;
            t->bodysize=tmp->bodysize;
            t->body=realloc(t->body,sizeof(ll) * t->bodysize);
            if (t->body == NULL){
                free(t);
                return BN_NO_MEMORY;
            }
            for (int i=0;i<t->bodysize;i++){
                t->body[i]=tmp->body[i];
            }
            bn_delete(tmp);
            bn_delete(cp_t);
        }
        else{                                               ///если степень нечетная, то умножаем просто на степень исходного t
            degree--;
            bn *tmp = bn_mul(ans, t);
            ans->sign=tmp->sign;
            ans->bodysize=tmp->bodysize;
            ans->body=realloc(ans->body,sizeof(ll) * ans->bodysize);
            if (ans->body == NULL){
                free(ans);
                return BN_NO_MEMORY;
            }
            for (int i=0;i<ans->bodysize;i++){
                ans->body[i]=tmp->body[i];
            }
            bn_delete(tmp);

        }
    }                                                                       ///копируем из ans в t
    t->sign=ans->sign;
    t->bodysize=ans->bodysize;
    t->body=realloc(t->body,sizeof(ll) * t->bodysize);
    if (t->body == NULL){
        free(t);
        return BN_NO_MEMORY;
    }
    for (int i=0;i<t->bodysize;i++){
        t->body[i]=ans->body[i];
    }
    bn_delete(ans);
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
    bn *l=bn_new();
    bn_init_int(l,2);
    bn_pow_to(l,15*t->bodysize);
    int sign=t->sign;
    bn_abs(t);
    int p;
    int flag=0;
    ///формула Ньютона
    while (1){
        bn *l_cp1=bn_init(l);
        bn *l_cp2=bn_init(l);
        bn_mul_small(l_cp1,reciprocal-1);
        bn_pow_to(l_cp2,reciprocal-1);
        bn *tmp=bn_div(t,l_cp2);
        bn *lnext=bn_add(l_cp1,tmp);
        bn_div_small(lnext,reciprocal);
        p = bn_cmp_abs(lnext,l);
        if (p==0 || (p>0 && flag)){                          ///если числа равны, либо предыдущее было меньше, а сейчас стало увеличиваться, то выход
            t->sign=sign;
            t->bodysize=lnext->bodysize;
            t->body=realloc(t->body,sizeof(ll) * t->bodysize);
            if (t->body == NULL){
                free(t);
                return BN_NO_MEMORY;
            }
            for (int i=0;i<t->bodysize;i++){
                t->body[i]=lnext->body[i];
            }
            bn_delete(lnext);
            bn_delete(l);
            bn_delete(l_cp2);
            bn_delete(l_cp1);
            bn_delete(tmp);
            break;
        }
        if (p==-1) flag =1;                                 ///если следующее число меньше, то продолжаем
        else flag =0;
        bn_delete(l);
        l=bn_init(lnext);
        bn_delete(lnext);
        bn_delete(l_cp1);
        bn_delete(l_cp2);
        bn_delete(tmp);
    }
    return BN_OK;
}


/// Инициализировать значение BN представлением строки в системе счисления radix
int bn_init_string_radix(bn *t, const char *init_string, int radix){
    if (t==NULL || init_string==NULL || init_string[0]=='\0') return BN_NULL_OBJECT;
    char a = init_string[0];
    int i =0;
    if (a=='-') {t->sign=-1;i =1;if(init_string[1]==0){t->bodysize=1;t->sign=0;t->body[0]=0;return BN_OK;}} ///проверка на - и -0
    else if(a=='0'){t->bodysize=1;t->sign=0;t->body[0]=0;return BN_OK;}///если 0 то всё BN=0
    else
        t->sign=1;
    bn *sum=bn_new();
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
    t->bodysize=sum->bodysize;
    t->body=realloc(t->body,sizeof(ll) * t->bodysize);
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
/// Выдать представление BN в системе счисления radix в виде строки
const char *bn_to_string(bn const *t, int radix){
    if (t==NULL) return NULL;
    char* result_string=(char*)malloc(1);
    int reall_size =1;
    int i =0;
    if (t->sign==0) {result_string[0]='0';i++;}
    bn *cp_t=bn_init(t);
    bn_abs(cp_t);
    while(cp_t->sign!=0){
        bn *rest=bn_init(cp_t);
        bn_mod_small(rest,radix);                                   ///получаем остаток
        if (i==reall_size){                                         ///если память заполнилась
            reall_size*=2;
            result_string=(char *)realloc(result_string,reall_size);
        }

        result_string[i]="0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"[rest->body[0]]; ///добавляем цифру в строку
        bn_div_small(cp_t,radix);                                               ///переходим в следующий разряд
        i++;
        bn_delete(rest);
    }
    if (t->sign==-1){                                                           ///если отрицательное то добавляем - в конец строки
        if (i==reall_size) result_string=(char *)realloc(result_string,reall_size*=2); ///если память переполнилась
        result_string[i]='-';
        i++;
    }
    for(int j=0;j<i/2;j++)                                                      ///меняем порядок на обратный
    {
        char a=result_string[j];
        result_string[j]=result_string[i-1-j];
        result_string[i-1-j]=a;
    }
    if (i==reall_size) result_string=(char *)realloc(result_string,reall_size+1); ///если память переполнилась
    result_string[i]='\000';                                                          ///дописываем нулевой символ в конец строки
    bn_delete(cp_t);
    return result_string;
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
