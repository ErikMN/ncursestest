NAME = ncursest
PROG = $(NAME)
LDLIBS = -lm -lncurses
CC = gcc
SRC_DIR = .

CFLAGS = -g -Wall

SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(SRCS:.c=.o)

.PHONY: all debug prod dev clean

all: $(PROG)

debug:
	@echo "*** Debug info:"
	@echo "Source-files:" $(SRCS)
	@echo "Object-files:" $(OBJS)

# Production target with extra C flags:
prod: CFLAGS += -Wextra -Werror -Wmaybe-uninitialized
prod: all

# Development target:
dev: $(OBJS)
		$(CC) -DDEV $(SRCS) $(LDLIBS) -o $(PROG)

# Standard target:
$(PROG): $(OBJS)
		$(CC) $(SRCS) $(LDLIBS) -o $@

clean:
	$(RM) $(OBJS) $(PROG)
