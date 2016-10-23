CC ?= gcc
CFLAGS=-I. -fPIC
RANLIB ?= ranlib

LIB=libkiss.a
LIBDEST=./lib/$(TARGET_DIR)/

LIBSRC=kiss_fft.c tools/kiss_fftr.c

LIBOBJ=$(LIBSRC:.c=.o)

install: $(LIB)
	@echo lib Makefile - installing $(LIB)
	@mkdir -p $(LIBDEST)
	@install -m 644 $(LIB) $(LIBDEST)

$(LIB): $(LIBOBJ)
	@echo lib Makefile - archiving $(LIB)
	@$(AR) r $(LIB) $(LIBOBJ)
	@$(RANLIB) $(LIB)

%.o: %.c
	@echo lib Makefile - compiling $<
	@$(CC) $(CFLAGS) -c $< -o $@

