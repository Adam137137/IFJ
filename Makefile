EXEC = Main
CC = gcc
CFLAGS = -Wall -std=c11 -MMD
LDLIBS = -lm

SRCS = $(wildcard *.c)
OBJS = $(patsubst %.c,bin/%.o,$(SRCS))
DEPS = $(OBJS:.o=.d)

.PHONY: clean all

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS)

-include $(DEPS)

bin/%.o: %.c | bin
	$(CC) $(CFLAGS) -c -o $@ $<

bin:
	mkdir -p bin

clean:
	rm -rf $(EXEC) bin $(DEPS)