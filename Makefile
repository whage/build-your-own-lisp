BUILD_DIR = build

prompt: prompt.c
	cc -std=c99 -Wall prompt.c -ledit -o $(BUILD_DIR)/prompt
