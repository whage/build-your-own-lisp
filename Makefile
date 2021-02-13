BUILD_DIR = build

prompt: 04_prompt.c
	cc -std=c99 -Wall 04_prompt.c -ledit -o $(BUILD_DIR)/prompt

parsing: 06_parsing.c mpc.c mpc.h
	cc -std=c99 -Wall 06_parsing.c mpc.c -lm -ledit -o $(BUILD_DIR)/parsing

evaluation: 07_evaluation.c mpc.c mpc.h
	cc -std=c99 -Wall 07_evaluation.c mpc.c -lm -ledit -o $(BUILD_DIR)/evaluation

error_handling: 08_error_handling.c mpc.c mpc.h
	cc -std=c99 -Wall 08_error_handling.c mpc.c -lm -ledit -o $(BUILD_DIR)/error_handling

s_expressions: 09_s_expressions.c mpc.c mpc.h
	cc -std=c99 -Wall 09_s_expressions.c mpc.c -lm -ledit -o $(BUILD_DIR)/s_expressions
