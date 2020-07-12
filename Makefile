.PHONY: test

CFLAGS := -O3 -std=c11 -D_POSIX_C_SOURCE=200112L -D_XOPEN_SOURCE=500 -D_GNU_SOURCE -fPIC
CFLAGS += -Wall -Wmissing-prototypes -Wstrict-prototypes -Werror=implicit-function-declaration -Werror=format -Wshadow -Wswitch -pthread

# libcairo
CFLAGS += `pkg-config --cflags cairo`
LDFLAGS := `pkg-config --libs cairo` -lfontconfig

# SDLv2 support
CFLAGS += `pkg-config --cflags sdl2`
LDFLAGS += `pkg-config --libs sdl2`

ifeq ($(DEVELOPMENT),1)
CFLAGS += -ggdb3 
#CFLAGS += -fsanitize=address -fsanitize=undefined -fsanitize=leak -fno-omit-frame-pointer -D_FORTITY_SOURCE=2
CFLAGS += -DDEVELOPMENT
TEST_FLAGS +=
endif

OBJS := \
	display.o \
	display_fb.o \
	display_sdl.o \
	cairo.o \
	cformat.o \
	cairoglue.o

LIBRARIES := minuitk.a minuitk.so

all: $(LIBRARIES)

clean:
	rm -f $(OBJS)
	rm -f $(LIBRARIES)

minuitk.a: $(OBJS)
	ar mc $@ $^

minuitk.so: $(OBJS)
	$(CC) -shared -o $@ $^ $(LDFLAGS)

test: all
	make -C tests clean test

.c.o:
	$(CC) $(CFLAGS) -c -o $@ $<
