#include "calc.h"

void init_ops() {
	ops_top = -1;
}

void init_exps() {
	exps_top = -1;
}

void push_ops(char op) {
	DBG_PRINTF("%s:%d: push_ops : push op(%c)\n", __FILE__, __LINE__ , op);
	if (ops_top == MAX_STACK-1) {
		DBG_PRINTF("%s:%d: 运算符栈已满!\n", __FILE__, __LINE__);
		exit(-1);
	} else {
		ops_top++;
		ops[ops_top] = op;
	}
}

void push_exps(char *expre) {
	DBG_PRINTF("%s:%d: push_exps : push expre(%s) strlen of expre(%d)\n",
			__FILE__, __LINE__ , expre, (int)strlen(expre) + 1);
	if (exps_top == MAX_STACK-1) {
		DBG_PRINTF("%s:%d: 表达式栈已满!\n", __FILE__, __LINE__);
		exit(-1);
	} else {
		exps_top++;
		exps[exps_top] = malloc(strlen(expre) + 1);
		memset(exps[exps_top], 0, strlen(expre) + 1);
		strcpy(exps[exps_top], expre);
	}
}

char pop_ops() {
	char pop_op;
	if(ops_top == -1) {
		DBG_PRINTF("%s:%d: 运算符栈为空\n", __FILE__, __LINE__);
		exit(-1);
	} else {
		pop_op = ops[ops_top];
		ops_top--;
	}
	return pop_op;
}

char* pop_exps() {
	memset(pop_expre, 0, MAX_EXPRE);
	if(exps_top == -1) {
		DBG_PRINTF("%s:%d: 表达式栈为空\n", __FILE__, __LINE__);
		exit(-1);
	} else {
		strcpy(pop_expre, exps[exps_top]);
		DBG_PRINTF("%s:%d: exps_top(%s)\n", __FILE__, __LINE__, exps[exps_top]);
		free(exps[exps_top]);
		exps_top--;
	}
	return pop_expre;
}


char show_ops_top() {
	if (ops_top != -1) {
		return ops[ops_top];
	} else {
		DBG_PRINTF("%s:%d: Error\n", __FILE__, __LINE__);
		exit(-1);
	}
}

char* show_exps_top() {
	if (exps_top != -1) {
		return exps[exps_top];
	} else {
		DBG_PRINTF("%s:%d: Error\n", __FILE__, __LINE__);
		exit(-1);
	}
}

bool is_constant(char ch) {
	if (ch >= '0' && ch <= '9' || ch == '.' || ch == 'p' || ch == 'i' \
			|| ch =='e' || ch == 'x' || ch == 'p') {
		return true;
	} else
		return false;
}

bool is_algebra(char ch) {
	switch (ch) {
		case 'a':
		case 'b':
		case 'c':
		case 'd':
		case 'l':
		case 'm':
		case 'n':
		case 'x':
		case 'y':
		case 'z':
			return true;
		
		default:
			return false;
	}
}

bool is_last_ch_operator(char ch) {
	if (ch == show_ops_top())
		return true;
	else
		return false;
}

bool is_operator(char ch) {
	DBG_PRINTF("%s:%d: is_operator: current ch(%c) last_ch(%c) cal_flag(%d)\n",
			__FILE__, __LINE__, ch, last_ch, cal_flag);
	switch (ch) {
		case '+':
		case '-':
			if (!cal_flag && (is_last_ch_operator(last_ch) || last_ch == END)) {
				/*如果在此运算符之前，没有进行过出栈计算的运算，并且
				这是第一个输入的字符或者上一次输入的字符也是操作符，
				向操作数栈压入一个"0"表示正或负数运算*/
				DBG_PRINTF("%s:%d: is_operator: push 0 into exps[]\n",
						__FILE__, __LINE__);
				push_exps("0");
			}
			return true;
		case '(':
			if (!cal_flag) {
				if (last_ch == ')' || is_constant(last_ch)
						|| is_algebra(last_ch)) {
					
					DBG_PRINTF("%s:%d: is_operator: Wrong expression\n",
							__FILE__, __LINE__);
					exit(-1);
				}
			}
			return true;
		case ')':
			if (!cal_flag) {
				if (!(is_constant(last_ch) || is_algebra(last_ch))) {
					DBG_PRINTF("%s:%d: is_operator: Wrong expression\n",
							__FILE__, __LINE__);
					exit(-1);
				}
			}
			return true;
		case '*':
		case '/':
		case '^':
		case '=':
		case END:
			return true;
			
		default:
			return false;
	}
}

bool judge_op_prior(char op_last, char op_cur) {
	char prior;
	DBG_PRINTF("%s:%d: judge_op_prior: op_last(%c) op_cur(%c)\n",
			__FILE__, __LINE__, op_last, op_cur);
	switch (op_cur) {
		case '+':
		case '-':
			if (op_last == '(' || op_last == END)
				prior = '<';
			else
				prior = '>';
			break;
		case '*':
		case '/':
			if (op_last == '*' || op_last == '/' || op_last == '^')
				prior = '>';
			else
				prior = '<';
			break;
		case '^':
			prior = '<';
			break;
		case '(':
			if (op_last == ')') {
				DBG_PRINTF("%s:%d: judge_op_prior: Wrong expression\n",
						__FILE__, __LINE__);
				exit(-1);
			} else
				prior = '<';
			break;
		case ')':
			switch (op_last) {
				case '(':
					prior = '=';
					break;
				case END:
					DBG_PRINTF("%s:%d: judge_op_prior: Wrong expression\n",
							__FILE__, __LINE__);
					exit(-1);
				default:
					prior = '>';
			}
			break;
		case END:
			switch (op_last) {
				case END:
					prior = '=';
					break;
				case '(':
					DBG_PRINTF("%s:%d: judge_op_prior: Wrong expression\n",
							__FILE__, __LINE__);
					exit(-1);
				default:
					prior = '>';
			}
			break;
		case '=':
			prior = '=';
			break;

		default:
			DBG_PRINTF("%s:%d: judge_op_prior: Wrong operator!\n",
					__FILE__, __LINE__);
			exit(-1);
	}
	DBG_PRINTF("%s:%d: judge_op_prior: prior(%c)\n", __FILE__, __LINE__, prior);
	return prior;
}

char* calculate(char *expre1, char *expre_op, char *expre2) {
	DBG_PRINTF("%s:%d: calculate: expre1(%s) expre_op(%s) expre2(%s)\n",
				__FILE__, __LINE__, expre1, expre_op, expre2);
	strcat(expre1, expre_op);
	strcat(expre1, expre2);
	DBG_PRINTF("%s:%d: calculate: expre1(%s)\n", __FILE__, __LINE__, expre1);
	return expre1;
}

bool is_seperated_expression() {
	int ops_top_temp = ops_top;
	if (show_ops_top() != '+' && show_ops_top() != '-') {
		return false;
	}
	while (--ops_top_temp != -1) {
		if (ops[ops_top_temp] == '(') {
			return false;
		}
	}
	return true;
}

bool is_have_divisor(char *p_str) {
	DBG_PRINTF("%s:%d: is_have_divisor: p_str(%s) len of p_str(%d)\n",
			__FILE__, __LINE__, p_str, (int)strlen(p_str)+1);
	if (NULL == strchr(p_str, '/')) {
		DBG_PRINTF("%s:%d: is_have_divisor: p_str(%s) len of p_str(%d)\n",
			__FILE__, __LINE__, p_str, (int)strlen(p_str)+1);
		return false;
	} else {
		DBG_PRINTF("%s:%d: is_have_divisor: p_str(%s) len of p_str(%d)\n",
			__FILE__, __LINE__, p_str, (int)strlen(p_str)+1);
		return true;
	}
}

void before_divisor(char *dst_str, char *src_str) {
	unsigned int tmp;
	tmp = (unsigned int)(strchr(src_str, '/') - src_str);
	strncpy(dst_str, src_str, tmp);
	DBG_PRINTF("%s:%d: before_divisor: dst_str(%s) src_str(%s)\n",
			__FILE__, __LINE__, dst_str, src_str);
}

void after_divisor(char *dst_str, char *src_str) {
	char *str_temp = strchr(src_str, '/');
	str_temp++; //不包含‘/’，所以指针向后移一位
	strcpy(dst_str, str_temp);
	DBG_PRINTF("%s:%d: after_divisor: dst_str(%s) src_str(%s)\n",
			__FILE__, __LINE__, dst_str, src_str);
}

void to_seperated_expression(char *p_str) {
	char get_ch, top_ops_ch;
	int num_len = 0; //连续输入数字的个数
	char num_buff[NUM_LEN];
	char expre1[MAX_EXPRE];
	char expre2[MAX_EXPRE];
	char expre_op[MAX_EXPRE];

	init_ops();
	push_ops(END);
	init_exps();
	
	if (p_str != NULL) {
		get_ch = *p_str;
	} else {
		DBG_PRINTF("%s:%d: to_seperated_expression: Wrong expression!\n", \
				__FILE__, __LINE__);
	}
	top_ops_ch = show_ops_top();
	DBG_PRINTF("%s:%d: to_seperated_expression: get_ch(%c) top_ops_ch(%c)\n", \
			__FILE__, __LINE__, get_ch, top_ops_ch);
		
	while (get_ch != END || top_ops_ch != END) {
		DBG_PRINTF("%s:%d: to_seperated_expression: get_ch(%c) cal_flag(%d), num_len(%d)\n", \
				__FILE__, __LINE__, get_ch, cal_flag, num_len);
		if (is_operator(get_ch)) {
			if (cal_flag) { //如果刚刚有出栈运算的过程，那么上一次取出的字符
							//已经被弹出，默认给上次的字符赋上'0',表示非运算符
				last_ch = '0';
			}
			if (num_len) {
				push_exps(num_buff);
				DBG_PRINTF("%s:%d: to_seperated_expression: top exps(%s)\n", \
						__FILE__, __LINE__, show_exps_top());
			}
			DBG_PRINTF("%s:%d: to_seperated_expression:get_ch(%c) num_len(%d)\n", \
				__FILE__, __LINE__, get_ch, num_len);
			memset(num_buff, 0, num_len);
			num_len = 0;
			
			switch (judge_op_prior(top_ops_ch, get_ch)) {
				case '<':
					cal_flag = false;
					push_ops(get_ch);
					if (*(++p_str) != '\0') {
						last_ch = get_ch;
						get_ch = *p_str;
						DBG_PRINTF("%s:%d: to_seperated_expression: get_ch(%c)\n", \
								__FILE__, __LINE__, get_ch);
					}
					break;
				case '=': //只有')'或着END时才相等，如果是')',需要将操作符栈里的
						  //'('弹出，并且连同当前的')'添加到表达式的两边
					cal_flag = false;
					if (get_ch == ')') {
						memset(expre_op, 0, MAX_EXPRE);
						memset(expre1, 0, MAX_EXPRE);
						memset(expre2, 0, MAX_EXPRE);
						expre_op[0] = pop_ops();
						expre_op[1] = '\0';
						strcpy(expre2, pop_exps());
						strcat(expre_op, expre2);
						strcpy(expre1, expre_op);
						expre_op[0] = get_ch;
						expre_op[1] = '\0';
						strcat(expre1, expre_op);
						push_exps(expre1);
					} else if (get_ch == '=') {
						get_ch = END;
						continue;
					} else if (get_ch == END) {
						pop_ops();
					}
					if (*(++p_str) != '\0') {
						last_ch = '0'; //上面的步骤将上一次的操作符丢弃了，所以
									   //这里默认给赋值一个非操作符即可
						get_ch = *p_str;
						DBG_PRINTF("%s:%d: to_seperated_expression: get_ch(%c)\n",
								__FILE__, __LINE__, get_ch);
					}
					break;
				case '>':
					cal_flag = true;
					memset(expre_op, 0, MAX_EXPRE);
					memset(expre1, 0, MAX_EXPRE);
					memset(expre2, 0, MAX_EXPRE);
					expre_op[0] = pop_ops();
					expre_op[1] = '\0';
					strcpy(expre2, pop_exps());
					strcpy(expre1, pop_exps());
					push_exps(calculate(expre1, expre_op, expre2));
					DBG_PRINTF("%s:%d: to_seperated_expression: show_exps_top(%s)\n",
							__FILE__, __LINE__, show_exps_top());
					break;
				
				default:
					break;
			}
			if (is_seperated_expression()) {
				memset(expre1, 0, MAX_EXPRE);
				strcpy(expre1,pop_exps());
				DBG_PRINTF("%s:%d: to_seperated_expression: expre1(%s) len of expre1(%d)\n",
							__FILE__, __LINE__, expre1, (int)strlen(expre1)+1);
				separated_expre[0][sep_expre_num] = malloc(MAX_EXPRE);
				memset(separated_expre[0][sep_expre_num], 0, MAX_EXPRE);
				strcpy(separated_expre[0][sep_expre_num], expre1);
				
				expre_op[0] = pop_ops();
				expre_op[1] = '\0';
				DBG_PRINTF("%s:%d: to_seperated_expression: expre_op(%s) len of expre_op(%d)\n",
							__FILE__, __LINE__, expre_op, (int)strlen(expre_op)+1);
				separated_op[sep_op_num] = malloc(strlen(expre_op)+1);
				memset(separated_op[sep_op_num], 0, strlen(expre_op)+1);
				strcpy(separated_op[sep_op_num], expre_op);
				
				sep_expre_num++;
				sep_op_num++;
			}
		} else if (is_constant(get_ch)) {
			if (cal_flag) {
				last_ch = '0';
			}
			if (last_ch == ')') { //上一次输入为')',则本次输入不能为数字
				DBG_PRINTF("%s:%d: to_seperated_expression: Wrong expression\n",
						__FILE__, __LINE__);
				exit(-1);
			}
			num_buff[num_len] = get_ch;
			num_len++;
			DBG_PRINTF("%s:%d: to_seperated_expression:num_len(%d)\n",
						__FILE__, __LINE__, num_len);
			num_buff[num_len] = '\0';
			if (*(++p_str) != '\0') {
				last_ch = get_ch;
				get_ch = *p_str;
				DBG_PRINTF("%s:%d: to_seperated_expression: get_ch(%c)\n",
						__FILE__, __LINE__, get_ch);
			}
		} else if (is_algebra(get_ch)) {
			if (cal_flag) {
				last_ch = '0';
			}
			if (last_ch == ')') { //上一次输入为')',则本次输入不能为代数
				DBG_PRINTF("%s:%d: to_seperated_expression: Wrong expression\n",
						__FILE__, __LINE__);
				exit(-1);
			}
			num_buff[num_len] = get_ch;
			num_len++;
			DBG_PRINTF("%s:%d: to_seperated_expression:num_len(%d)\n",
						__FILE__, __LINE__, num_len);
			num_buff[num_len] = '\0';
			if (*(++p_str) != '\0') {
				last_ch = get_ch;
				get_ch = *p_str;
				DBG_PRINTF("%s:%d: to_seperated_expression: get_ch(%c)\n",
						__FILE__, __LINE__, get_ch);
			}
		} else {
			DBG_PRINTF("%s:%d: to_seperated_expression: Wrong expression\n",
					__FILE__, __LINE__);
			exit(-1);
		}
		top_ops_ch = show_ops_top();
	}
	memset(expre1, 0, MAX_EXPRE);
	strcpy(expre1,pop_exps());
	DBG_PRINTF("%s:%d: to_seperated_expression: expre1(%s) len of expre1(%d)\n",
				__FILE__, __LINE__, expre1, (int)strlen(expre1)+1);
	separated_expre[0][sep_expre_num] = malloc(MAX_EXPRE);
	memset(separated_expre[0][sep_expre_num], 0, MAX_EXPRE);
	strcpy(separated_expre[0][sep_expre_num], expre1);
	sep_expre_num++;
}

void abstract_denominator() {
	int i; //循环计数用
	int i_sub; //子循环计数用
	char temp_expre[MAX_EXPRE];
	char *temp_str = NULL;
	char *temp_str_last = NULL;

	for (i=0; i < sep_expre_num; i++) {
		memset(temp_expre, 0, MAX_EXPRE);
		strcpy(temp_expre, separated_expre[0][i]);
		DBG_PRINTF("%s:%d: abstract_denominator: temp_expre(%s) len of temp_expre(%d)\n",
				__FILE__, __LINE__, temp_expre, (int)strlen(temp_expre)+1);
		if (is_have_divisor(temp_expre)) {
			/*除号之前的作为分子，除号之后的进一步判断*/
			memset(separated_expre[0][i], 0, MAX_EXPRE);
			separated_expre[1][i] = malloc(MAX_EXPRE);
			memset(separated_expre[1][i], 0, MAX_EXPRE);
			before_divisor(separated_expre[0][i], temp_expre);
			after_divisor(separated_expre[1][i], temp_expre);

			// memset(temp_expre, 0, MAX_EXPRE);
			// strcpy(temp_expre, separated_expre[1][i]);
			temp_str = separated_expre[1][i];
			DBG_PRINTF("%s:%d: abstract_denominator: temp_str(%s)\n",
					__FILE__, __LINE__, temp_str);
			while (*temp_str != '\0') {
				DBG_PRINTF("%s:%d: abstract_denominator: *temp_str(%c)\n",
						__FILE__, __LINE__, *temp_str);
				if (is_constant(*temp_str)) {
					do {
						temp_str++;
						DBG_PRINTF("%s:%d: abstract_denominator: *temp_str(%c)\n",
							__FILE__, __LINE__, *temp_str);
					} while (is_constant(*temp_str)); 	
					temp_str++;
					DBG_PRINTF("%s:%d: abstract_denominator: *temp_str(%c)\n",
							__FILE__, __LINE__, *temp_str);
				} else if (*temp_str == '(') {
					do {
						temp_str++;
						DBG_PRINTF("%s:%d: abstract_denominator: *temp_str(%c)\n",
								__FILE__, __LINE__, *temp_str);
					} while (*temp_str != ')');
					temp_str++;
					DBG_PRINTF("%s:%d: abstract_denominator: *temp_str(%c)\n",
							__FILE__, __LINE__, *temp_str);
				} else if (*temp_str == '/') {
					/*如果当前字符是除号，转换为乘号*/
					memset(temp_str, '*', 1);
					temp_str++;
					DBG_PRINTF("%s:%d: abstract_denominator: *temp_str(%c)\n",
							__FILE__, __LINE__, *temp_str);
				} else if (*temp_str == '*') {
					/*如果当前字符是乘号，把乘号以及紧跟着的表达式移到分子部分，
					分母部分删掉这一部分字符*/
					i_sub = 0;
					memset(temp_expre, 0, MAX_EXPRE);
					temp_expre[i_sub] = *temp_str;
					i_sub++;
					temp_expre[i_sub] = '\0';
					memset(temp_str, 0, 1);
					temp_str_last = temp_str; // 保存当前的位置

					temp_str++;
					DBG_PRINTF("%s:%d: abstract_denominator: *temp_str(%c)\n",
							__FILE__, __LINE__, *temp_str);
					if (is_constant(*(temp_str+1))) {
						do {
							temp_expre[i_sub] = *temp_str;
							i_sub++;
							temp_expre[i_sub] = '\0';
							memset(temp_str, 0, 1);
							temp_str++;
						} while (is_constant(*(temp_str+1))); 	
					} else if (*temp_str == '(') {
						do {
							temp_expre[i_sub] = *temp_str;
							i_sub++;
							temp_expre[i_sub] = '\0';
							memset(temp_str, 0, 1);
							temp_str++;
						} while (*temp_str != ')');
					}
					temp_expre[i_sub] = *temp_str;
					i_sub++;
					temp_expre[i_sub] = '\0';
					memset(temp_str, 0, 1);
					temp_str++;
					DBG_PRINTF("%s:%d: abstract_denominator: *temp_str(%c)\n",
							__FILE__, __LINE__, *temp_str);
					DBG_PRINTF("%s:%d: abstract_denominator: temp_str(%s)\n",
							__FILE__, __LINE__, temp_str);
					DBG_PRINTF("%s:%d: abstract_denominator: temp_expre(%s)\n",
							__FILE__, __LINE__, temp_expre);
					strcat(separated_expre[0][i], temp_expre);
					strcat(separated_expre[1][i], temp_str);  //拼接字符串以后，temp_str指向的位置会被改变
					temp_str = temp_str_last;				  //所以使用之前保存的位置充值temp_str
				} else {
					temp_str++;
					DBG_PRINTF("%s:%d: abstract_denominator: *temp_str(%c)\n",
							__FILE__, __LINE__, *temp_str);
				}
				DBG_PRINTF("%s:%d: abstract_denominator: *temp_str(%c)\n",
							__FILE__, __LINE__, *temp_str);
			}

		} else {
			/*如果没有除号，分母默认为1*/
			separated_expre[1][i] = malloc(MAX_EXPRE);
			memset(separated_expre[1][i], 0, MAX_EXPRE);
			strcpy(separated_expre[1][i], "1");
		}
	}
}

void total_denominator(char *deno, char *str) {
	char *ptr_deno = deno;
	char *ptr_str = str;
	char deno_element[MAX_EXPRE];
	char str_element[MAX_EXPRE];
	char denominator_temp[MAX_EXPRE];
	int i_deno, i_str;
	memset(deno_element, 0, MAX_EXPRE);
	memset(str_element, 0, MAX_EXPRE);
	memset(denominator_temp, 0, MAX_EXPRE);
	strcpy(denominator_temp, deno);
	while (*ptr_str != '\0') {
		i_str = 0;
		if (is_constant(*ptr_str)) {
			while (is_constant(*ptr_str)) {
				str_element[i_str] = *ptr_str;
				i_str++;
				str_element[i_str] = '\0';
				ptr_str++;
			}
		} else if(*ptr_str == '(') {
			while (*ptr_str != ')') {
				str_element[i_str] = *ptr_str;
				i_str++;
				str_element[i_str] = '\0';
				ptr_str++;
			}
			str_element[i_str] = *ptr_str;
			i_str++;
			str_element[i_str] = '\0';
			ptr_str++;
		} else {
			str_element[i_str] = *ptr_str;
			i_str++;
			str_element[i_str] = '\0';
			ptr_str++;
		}
			
		ptr_deno = deno;
		while (*ptr_deno != '\0') {
			i_deno = 0;
			if (is_constant(*ptr_deno)) {
				while (is_constant(*ptr_deno)) {
					deno_element[i_deno] = *ptr_deno;
					i_deno++;
					deno_element[i_deno] = '\0';
					ptr_deno++;
				}
			} else if(*ptr_deno == '(') {
				while (*ptr_deno != ')') {
					deno_element[i_deno] = *ptr_deno;
					i_deno++;
					deno_element[i_deno] = '\0';
					ptr_deno++;
				}
				deno_element[i_deno] = *ptr_deno;
				i_deno++;
				deno_element[i_deno] = '\0';
				ptr_deno++;
			} else {
				deno_element[i_deno] = *ptr_deno;
				i_deno++;
				deno_element[i_deno] = '\0';
				ptr_deno++;
			}
				
			DBG_PRINTF("%s:%d: total_denominator: deno_element(%s) str_element(%s)\n",
					__FILE__, __LINE__, deno_element, str_element);
			if (!strcmp(deno_element, str_element)) {
				break;
			}
		}
		if (strcmp(deno_element, str_element)) {
			DBG_PRINTF("%s:%d: total_denominator: deno_element(%s) str_element(%s)\n",
					__FILE__, __LINE__, deno_element, str_element);
			strcat(denominator_temp, "*");
			strcat(denominator_temp, str_element);
			DBG_PRINTF("%s:%d: total_denominator: *denominator_temp(%s)\n",
					__FILE__, __LINE__, denominator_temp);
		}
	}
	strcpy(deno, denominator_temp);
}

void sub_denominator(char *deno_for_element, char *self_deno) {
	char *ptr_deno_for_element = deno_for_element;
	char *ptr_self_deno = self_deno;
	char *temp_deno_for_element = NULL;
	int i_deno_for_element;
	int i_self_deno;

	char part_of_deno_for_element[MAX_EXPRE];
	char part_of_self_deno[MAX_EXPRE];
	memset(part_of_deno_for_element, 0, MAX_EXPRE);
	memset(part_of_self_deno, 0, MAX_EXPRE);

	while (*ptr_self_deno != '\0') {
		i_self_deno = 0;
		if (is_constant(*ptr_self_deno)) {
			while (is_constant(*ptr_self_deno)) {
				part_of_self_deno[i_self_deno] = *ptr_self_deno;
				i_self_deno++;
				part_of_self_deno[i_self_deno] = '\0';
				ptr_self_deno++;
			}
		} else if(*ptr_self_deno == '(') {
			while (*ptr_self_deno != ')') {
				part_of_self_deno[i_self_deno] = *ptr_self_deno;
				i_self_deno++;
				part_of_self_deno[i_self_deno] = '\0';
				ptr_self_deno++;
			}
			part_of_self_deno[i_self_deno] = *ptr_self_deno;
			i_self_deno++;
			part_of_self_deno[i_self_deno] = '\0';
			ptr_self_deno++;
		} else {
			part_of_self_deno[i_self_deno] = *ptr_self_deno;
			i_self_deno++;
			part_of_self_deno[i_self_deno] = '\0';
			ptr_self_deno++;
		}
			
		ptr_deno_for_element = deno_for_element;
		while (*ptr_deno_for_element != '\0') {
			i_deno_for_element = 0;
			if (is_constant(*ptr_deno_for_element)) {
				while (is_constant(*ptr_deno_for_element)) {
					part_of_deno_for_element[i_deno_for_element] = *ptr_deno_for_element;
					i_deno_for_element++;
					part_of_deno_for_element[i_deno_for_element] = '\0';
					ptr_deno_for_element++;
				}
			} else if(*ptr_deno_for_element == '(') {
				while (*ptr_deno_for_element != ')') {
					part_of_deno_for_element[i_deno_for_element] = *ptr_deno_for_element;
					i_deno_for_element++;
					part_of_deno_for_element[i_deno_for_element] = '\0';
					ptr_deno_for_element++;
				}
				part_of_deno_for_element[i_deno_for_element] = *ptr_deno_for_element;
				i_deno_for_element++;
				part_of_deno_for_element[i_deno_for_element] = '\0';
				ptr_deno_for_element++;
			} else {
				part_of_deno_for_element[i_deno_for_element] = *ptr_deno_for_element;
				i_deno_for_element++;
				part_of_deno_for_element[i_deno_for_element] = '\0';
				ptr_deno_for_element++;
			}
				
			DBG_PRINTF("%s:%d: deno_for_elementminator: part_of_deno_for_element(%s) part_of_self_deno(%s)\n",
					__FILE__, __LINE__, part_of_deno_for_element, part_of_self_deno);
			if (!strcmp(part_of_deno_for_element, part_of_self_deno)) {
				temp_deno_for_element = ptr_deno_for_element;
				while (i_deno_for_element > 0) {
					temp_deno_for_element--;
					i_deno_for_element--;
				}
				if (!strcmp(temp_deno_for_element, deno_for_element)) {
					ptr_deno_for_element++;
					strcpy(deno_for_element,ptr_deno_for_element);
				} else {
					if (*(temp_deno_for_element-1) == '*') {
						temp_deno_for_element--;
					} else if (*(temp_deno_for_element-1) == '*') {
						temp_deno_for_element--;
					}
					memset(temp_deno_for_element, 0, 1);
					strcat(deno_for_element, ptr_deno_for_element);
				}
				break;
			}
		}
	}
}

void parse_expression(char *p_str) {
	int i = 0;
	char denominator[MAX_EXPRE];
	char denominator_for_element[MAX_EXPRE]; // 每个独立的表达式的分子，需要乘以这个字符串来通分 
	to_seperated_expression(p_str);
	abstract_denominator();
	memset(denominator, 0, MAX_EXPRE);
	while (*separated_expre[1][i] == '1' && strlen(separated_expre[1][i]) == 1) {
		i++;
	}
	strcpy(denominator,separated_expre[1][i]);
	i++;
	for (; i<sep_expre_num; i++) {
		DBG_PRINTF("%s:%d: parse_expression: denominator(%s) separated_expre[1][%d](%s)\n",
					__FILE__, __LINE__, denominator, i, separated_expre[1][i]);
		if (*separated_expre[1][i] == '1' && strlen(separated_expre[1][i]) == 1) {
			continue;
		} else {
			total_denominator(denominator, separated_expre[1][i]);
		}
	}
	DBG_PRINTF("\ndenominator %s\n", denominator);

	for (i=0; i<sep_expre_num; i++) {
		DBG_PRINTF("%s:%d: parse_expression: separated_expre[0][%d](%s) separated_expre[1][%d](%s)\n",
					__FILE__, __LINE__, i, separated_expre[0][i], i, separated_expre[1][i]);
		if (*separated_expre[1][i] == '1' && strlen(separated_expre[1][i]) == 1) {
			DBG_PRINTF("%s:%d: parse_expression: separated_expre[0][%d](%s) separated_expre[1][%d](%s)\n",
					__FILE__, __LINE__, i, separated_expre[0][i], i, separated_expre[1][i]);
			memset(separated_expre[1][i], 0, MAX_EXPRE);
			strcpy(separated_expre[1][i], denominator);

			strcat(separated_expre[0][i], "*");
			strcat(separated_expre[0][i], denominator);
		} else {
			memset(denominator_for_element, 0, MAX_EXPRE);
			strcpy(denominator_for_element, denominator);
			sub_denominator(denominator_for_element, separated_expre[1][i]);
			printf("\n%s:%d: parse_expression: denominator_for_element(%s) denominator(%s) separated_expre[1][%d](%s)\n",
					__FILE__, __LINE__, denominator_for_element, denominator, i, separated_expre[1][i]);

			memset(separated_expre[1][i], 0, MAX_EXPRE);
			strcpy(separated_expre[1][i], denominator);

			strcat(separated_expre[0][i], "*");
			strcat(separated_expre[0][i], denominator_for_element);
		}
	}
}
