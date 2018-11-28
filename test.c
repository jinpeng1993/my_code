#include "algebra_cal.c"

int main(int argc, char *argv[]) {
	char expression[MAX_EXPRE];
	int i;

	printf("Please input expression!\n");
	scanf("%s", expression);
	//char expression[] = "a/((b+c)/d-x)-(y+z)*l/z+(x-y)/((x+y)*z)/(a-b)+(a-(b+c)*d)=";
	char * p_str = (char *)expression;
	char * p_temp = p_str;
	printf("\n");
	do {
		if (p_temp != NULL) {
			printf("%c", *p_temp);
		}
	} while (*(++p_temp) != '\0');
	printf("\n\n");
	
	parse_expression(p_str);
	printf("\n");
	for (i=0; i<sep_expre_num; i++) {
		printf("%s", separated_expre[0][i]);
		printf("/");
		printf("%s", separated_expre[1][i]);
		if (i != sep_op_num) {
			printf("\n%s\n", separated_op[i]);
		}
	}
	printf("\n");
	return 0;
}
