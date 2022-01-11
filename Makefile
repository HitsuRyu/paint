NAME		=	gui
CC			=	g++
SRCS		=				button.cc \
							example.cc \
							GUI.cc \
							window.cc \

all: $(NAME)

$(NAME): $(SRCS)
		$(CC) $(SRCS) -o $@ `pkg-config --cflags --libs gtk+-3.0`

clean: $(NAME)
	rm -f $(NAME)

re: clean
	$(MAKE) all

.PHONY: all, clean, re