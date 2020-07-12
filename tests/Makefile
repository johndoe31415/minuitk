.PHONY: test

CFLAGS := -O3 -std=c11 -D_POSIX_C_SOURCE=200112L -D_XOPEN_SOURCE=500 -D_GNU_SOURCE -I..
CFLAGS += -Wall -Wmissing-prototypes -Wstrict-prototypes -Werror=implicit-function-declaration -Werror=format -Wshadow -Wswitch -pthread

LDFLAGS := ../minuitk.a

# libcairo
CFLAGS += `pkg-config --cflags cairo`
LDFLAGS += `pkg-config --libs cairo` -lfontconfig

# SDLv2 support
CFLAGS += `pkg-config --cflags sdl2`
LDFLAGS += `pkg-config --libs sdl2`

OBJS := \
	test_simple \
	test_fps

all: $(OBJS)

test: all
	./test_simple

clean:
	rm -f $(OBJS)

.c.o:
	$(CC) $(CFLAGS) -c -o $@ $<

%: %.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)
