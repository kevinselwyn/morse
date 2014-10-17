NAME    := morse
BIN_DIR := /usr/local/bin

all: build

build: $(NAME).c
	gcc -Wall -Wextra -o $(NAME) $< -lm

test: build
	./$(NAME) -t "SOS"
	./$(NAME) -m "... --- ..."
	./$(NAME) -t "SOS" -w morse.wav

memcheck:
	gcc -o $(NAME) $(NAME).c -lm -g -O0
	valgrind --leak-check=full ./$(NAME) -t "SOS" -m "... --- ..." -w $(NAME).wav

install: build
	install -m 0755 $(NAME) $(BIN_DIR)

uninstall:
	rm -f $(BIN_DIR)/$(NAME)

clean:
	rm $(NAME) $(NAME).wav