ifndef PREFIX
PREFIX = /usr/local/bin
endif

ifndef CFLAGS
CFLAGS = -O3 -pipe -g
endif

LIBS = -lm -lxml2 -lm -lz
INCLUDES = -I/usr/include/libxml2

SRC = utils.c monitor.c main.c
PROGNAME = switchmon

GENARGS = -Wall -D_FILE_OFFSET_BITS=64 -D_FILE_OFFSET_BITS=64 -D_LARGEFILE_SOURCE -D_GNU_SOURCE
STATARGS = -Wl,-z,noexecstack -ffast-math -rdynamic -static -pthread
WINSTATARGS = -Wl,--large-address-aware -lpthread -ffast-math -static

LDFLAGS := $(LDFLAGS) $(LIBS)

MACHINE = $(shell uname -m)
SYSTEM = $(shell uname -s)

ifeq ($(MACHINE),x86_64)
else
endif

ifeq ($(SYSTEM),Linux)
OS = -DSYS_LINUX -D_VERBOSE
else
OS = -D__WIN32__
endif
ifeq ($(SYSTEM),Darwin)
OS = -DSYS_MACOSX
endif
ifeq ($(SYSTEM),CYGWIN_NT-5.1)
OS := -D__CYGWIN__
ifndef CC
CC = gcc-4
endif
STATARGS = $(WINSTATARGS) -static-libgcc
LDFLAGS := $(LDFLAGS) -liconv -luser32 -lgdi32
STATIC := 1
endif

ifndef CC
CC = gcc
endif

ifeq ($(STATIC), 1)
ARGS := $(GENARGS) $(STATARGS)
else
ARGS := $(GENARGS)
endif

all: switchmon

switchmon: $(SRC)
	$(CC) -o $(PROGNAME) $(CFLAGS) $(INCLUDES) $(OS) \
		$(ARGS) \
		$(SRC) \
		$(LDFLAGS)

clean:
	rm -f $(PROGNAME)

install: $(PROGNAME)
	strip $(PROGNAME)
	cp -f $(PROGNAME) $(PREFIX)/$(PROGNAME)
