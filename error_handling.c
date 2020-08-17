#include "mpc.h"

#include <editline/readline.h>
#include <editline/history.h>

enum { LERR_DIV_ZERO, LERR_BAD_OP, LERR_BAD_NUM };

enum { LVAL_NUM, LVAL_ERR };

typedef struct {
	long num;
	int type;
	int err;
} lval;

lval lval_num(long n) {
	lval v;
	v.type = LVAL_NUM;
	v.num = n;
	return v;
}

lval lval_err(int err) {
	lval v;
	v.type = LVAL_ERR;
	v.err = err;
	return v;
}

void lval_print(lval v) {
	switch (v.type) {
		case LVAL_NUM:
			printf("%li", v.num);
			break;
		case LVAL_ERR:
			if (v.err == LERR_DIV_ZERO) {
				printf("Error: division by zero");
			}
			if (v.err == LERR_BAD_OP) {
				printf("Error: invalid operator");
			}
			if (v.err == LERR_BAD_NUM) {
				printf("Error: invalid number");
			}
			break;
	}
}

void lval_println(lval v) {
	lval_print(v);
	putchar('\n');
}

lval eval_op(lval x, char *op, lval y) {
	if (x.type == LVAL_ERR) { return x; }
	if (y.type == LVAL_ERR) { return y; }

	if (strcmp(op, "+") == 0) { return lval_num(x.num + y.num); }
	if (strcmp(op, "-") == 0) { return lval_num(x.num - y.num); }
	if (strcmp(op, "*") == 0) { return lval_num(x.num * y.num); }
	if (strcmp(op, "/") == 0) {
		if (y.num == 0) {
			return lval_err(LERR_DIV_ZERO);
		} else {
			return lval_num(x.num / y.num);
		}
	}
	if (strcmp(op, "%") == 0) { return lval_num(x.num % y.num); }

	return lval_err(LERR_BAD_OP);
}

lval eval(mpc_ast_t *t) {
	if (strstr(t->tag, "number")) {
		errno = 0;
		long x = strtol(t->contents, NULL, 10);
		return errno != ERANGE ? lval_num(x) : lval_err(LERR_BAD_NUM);
	}

	// the operator is always the 2nd child
	char *op = t->children[1]->contents;

	// we store the 3rd child in x
	lval x = eval(t->children[2]);

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
			lval result = eval(r.output);
			lval_println(result);
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
