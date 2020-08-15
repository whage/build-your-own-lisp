#include "mpc.h"

#include <editline/readline.h>
#include <editline/history.h>

void print_tags(mpc_ast_t *t, int indent) {
	printf("  %s\n", t->tag);

	for (int i = 0; i < t->children_num; i++) {
		for (int j = 0; j < indent; j++) {
			printf("  ");
		}
		print_tags(t->children[i], indent + 1);
	}
}

long eval_op(long x, char *op, long y) {
	if (strcmp(op, "+") == 0) { return x + y; }
	if (strcmp(op, "-") == 0) { return x - y; }
	if (strcmp(op, "*") == 0) { return x * y; }
	if (strcmp(op, "/") == 0) { return x / y; }
	if (strcmp(op, "%") == 0) { return x % y; }
	return 0;
}

long eval(mpc_ast_t *t) {
	if (strstr(t->tag, "number")) {
		return atoi(t->contents);
	}

	// the operator is always the 2nd child
	char *op = t->children[1]->contents;

	// we store the 3rd child in x
	long x = eval(t->children[2]);

	// iterate and combine the remaining children
	int i = 3;
	while (strstr(t->children[i]->tag, "expr")) {
		x = eval_op(x, op, eval(t->children[i]));
		i++;
	}

	return x;
}

int main(int argc, char** argv) {
	mpc_parser_t* Number = mpc_new("number");
	mpc_parser_t* Operator = mpc_new("operator");
	mpc_parser_t* Expr = mpc_new("expr");
	mpc_parser_t* Lispy = mpc_new("lispy");

	char *grammar = "                                                                      \
		number   : /-?[0-9]+(\\.[0-9]*)?/ ;                                                \
		operator : '+'  | /add/ | '-' | /sub/ | '*' | /mul/ | '/' | /div/ | '%' | /mod/ ;  \
		expr     : <number> | '(' <operator> <expr>+ ')' ;                                 \
		lispy    : /^/ <operator> <expr>+ /$/ ;                                            \
	";

	mpca_lang(
		MPCA_LANG_DEFAULT, 
		grammar,
		Number, Operator, Expr, Lispy
	);

	puts("Lispy Version 0.0.0.0.1");
	puts("Press Ctrl+c to Exit\n");

	while (1) {
		char *input = readline("lispy> ");
		add_history(input);

		mpc_result_t r;
		if (mpc_parse("<stdin>", input, Lispy, &r)) {
			print_tags(r.output, 0);
			long result = eval(r.output);
			printf("Reslt of evaluation: %li\n", result);
			mpc_ast_print(r.output);
			mpc_ast_delete(r.output);
		} else {
			mpc_err_print(r.error);
			mpc_err_delete(r.error);
		}
		
		free(input);
	}

	mpc_cleanup(4, Number, Expr, Operator, Lispy);

	return 0;
}
