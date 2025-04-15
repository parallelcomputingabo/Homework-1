.PHONY: clean

CFLAGS := $(CFLAGS) -Wall -Wextra -Wpedantic -Werror

main: main.c
	gcc $(CFLAGS) -o main main.c

clean:
	rm -f ./main
