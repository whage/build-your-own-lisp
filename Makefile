BUILD_DIR = build

prompt: prompt.c
	cc -std=c99 -Wall prompt.c -ledit -o $(BUILD_DIR)/prompt

parsing: parsing.c mpc.c mpc.h
	cc -std=c99 -Wall parsing.c mpc.c -lm -ledit -o $(BUILD_DIR)/parsing

evaluation: evaluation.c mpc.c mpc.h
	cc -std=c99 -Wall evaluation.c mpc.c -lm -ledit -o $(BUILD_DIR)/evaluation

error_handling: error_handling.c mpc.c mpc.h
	cc -std=c99 -Wall error_handling.c mpc.c -lm -ledit -o $(BUILD_DIR)/error_handling

s_expressions: s_expressions.c mpc.c mpc.h
	cc -std=c99 -Wall s_expressions.c mpc.c -lm -ledit -o $(BUILD_DIR)/s_expressions
