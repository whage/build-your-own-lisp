BUILD_DIR = build

prompt: prompt.c
	cc -std=c99 -Wall prompt.c -ledit -o $(BUILD_DIR)/prompt

parsing: parsing.c mpc.c mpc.h
	cc -std=c99 -Wall parsing.c mpc.c -lm -ledit -o $(BUILD_DIR)/parsing
