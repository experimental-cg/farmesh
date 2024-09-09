##
# Hello OpenGL
#
# @file
# @version 0.1

LFLAGS ?= -fPIC -lglfw -lGL -ldl -lm
CFLAGS ?= -fPIC -O0 -std=c17 -Werror -Wall -Wunused-result \
	-Wmissing-prototypes -Wmissing-field-initializers \
	-Wundef -Wstrict-prototypes -funsigned-char

SRC = $(wildcard *.c */*.c */*/*.c */*/*/*.c */*/*/*/*.c)
objects = $(patsubst %.c, %.o, $(SRC))

all: main

main: $(objects)
	$(CC) $^ $(LFLAGS) -o $@

%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $<

clean:
	$(RM) main
	$(RM) $(objects)

# end
