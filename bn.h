//
// Created by kika on 23.10.2020.
//
#ifndef __BN_H
#define __BN_H

#endif __BN_H
#pragma once
struct bn_s;
typedef struct bn_s bn;
enum bn_codes {
    BN_OK, BN_NULL_OBJECT, BN_NO_MEMORY, BN_DIVIDE_BY_ZERO
};
bn *bn_new(); // Создать новое BN
bn *bn_init(bn const *orig); // Создать копии существующего BN
void bn_print(bn const *num);// Вывод BN
int bn_init_string(bn *t, const char *init_string);// Инициализировать значение BN десятичным представлением строки
int bn_init_int(bn *t, int init_int);
int bn_sign(bn const *t); //-1 если t<0; 0 если t = 0, 1 если t>0
int bn_abs(bn *t);
int bn_neg(bn *t);
int bn_cmp(bn const *left, bn const *right);
int bn_cmp_abs(bn const *left, bn const *right); // сравнение по модулю
int bn_init_string_radix(bn *t, const char *init_string, int radix); // Инициализировать значение BN представлением строки в системе счисления radix
const char *bn_to_string(bn const *t, int radix);
int bn_mul_small (bn *t, const int n); // Умножение на короткое число
int bn_div_small (bn *t, const int n); // Деление на короткое число
bn* bn_add_sign(bn const *left, bn const *right); // Сложение BN одного знака
bn* bn_sub_sign(bn const *left, bn const *right); // Вычитание одного знака
int bn_delete(bn *t); // отчистить память
bn* bn_add(bn const *left, bn const *right);// Сложение BN
bn* bn_sub(bn const *left, bn const *right); //разность BN
bn* bn_mul(bn const *left, bn const *right); // Произведение BN
bn* bn_div(bn const *left, bn const *right);
int bn_pow_to(bn *t, int degree);        //возвести число в степень degree
int bn_root_to(bn *t, int reciprocal);     //корень степени reciprocal
int bn_add_to(bn *t, bn const *right); // +=
int bn_sub_to(bn *t, bn const *right);
int bn_mul_to(bn *t, bn const *right);
int bn_div_to(bn *t, bn const *right);
int bn_mod_to(bn *t, bn const *right);