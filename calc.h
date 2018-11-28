#ifndef _CALC_H_
#define _CALC_H_

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <math.h>
#include <string.h>

#define DEBUG 1

#if DEBUG
#define DBG_PRINTF(...) printf(__VA_ARGS__)
#else
#define DBG_PRINTF(...) 
#endif

#define MAX_STACK 100
#define NUM_LEN 32
#define MAX_EXPRE 200
#define END '\n'

typedef int bool;
#define true 1
#define false 0
#define pi  3.14159
#define exp 2.71828

char ops[MAX_STACK]; //运算符栈
int ops_top;       //运算符栈顶
char *exps[MAX_STACK]; //表达式栈
int exps_top;       //表达式栈顶
bool cal_flag = false; //表示当前字符入栈前是否有出栈运算的过程
char last_ch; //保存上一次记录的字符

char pop_expre[MAX_EXPRE];
char *separated_expre[2][MAX_EXPRE] = {NULL}; //存储分隔后的独立表达式
														//一行分子，一行分母
int sep_expre_num = 0;
char *separated_op[MAX_EXPRE]; //存储分隔后的操作符
int sep_op_num = 0;

char temp_separated_expre[MAX_EXPRE]; //临时保存分隔表达式的过程数据

void init_ops();
void init_exps();
void push_ops(char op);
void push_exps(char *expre);
char pop_ops();
char* pop_exps();
char show_ops_top();
char* show_exps_top();

/**
*判断是否为常数字符
*/
bool is_constant(char ch);

/**
*判断是否为代数字符
*/
bool is_algebra(char ch);

/**
*判断上一个字符是不是操作符
*/
bool is_last_ch_operator(char ch);

/**
*判断当前字符是否为操作符
*/
bool is_operator(char ch);

/**
*判断上一个操作符和当前操作符的优先级
*/
bool judge_op_prior(char op_last, char op_cur);

/**
*对弹出栈的表达式和符号进行合并(如果是常数则计算)
*/
char* calculate(char *expre1, char *expre_op, char *expre2);

/**
*是不是独立的表达式
*/
bool is_seperated_expression();

/**
*分隔成独立的表达式
*/
void to_seperated_expression(char *p_str);

/**
*表达式中是否有除号
*/
bool is_have_divisor(char * p_str);

/**
*截取表达式中第一个'/'之前的部分
*/
void before_divisor(char *dst_str, char *src_str);

/**
*截取表达式中第一个'/'之后的部分
*/
void after_divisor(char *dst_str, char *src_str);

/**
*解析表达式
*/
void parse_expression(char *p_str);

#endif