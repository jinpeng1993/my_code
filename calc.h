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
#define LEN_OP_STR 2
#define MAX_SEPARATED_EXPRE 100
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
char *separated_expre[2][MAX_SEPARATED_EXPRE] = {NULL}; //存储分隔后的独立表达式
														//一行分子，一行分母
int sep_expre_num = 0;
char *separated_op[MAX_SEPARATED_EXPRE]; //存储分隔后的操作符
int sep_op_num = 0;

void init_ops();
void init_exps();
void push_ops(char op);
void push_exps(char *expre);
char pop_ops();
char* pop_exps();
char show_ops_top();
char* show_exps_top();
bool is_constant(char ch);
bool is_algebra(char ch);
bool is_last_ch_operator(char ch);
bool is_operator(char ch);
bool judge_op_prior(char op_last, char op_cur);
char* calculate(char *expre1, char *expre_op, char *expre2);
bool is_seperated_expression();
char *parse_expression(char *p_str);

#endif