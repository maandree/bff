.POSIX:

CONFIGFILE = config.mk
include $(CONFIGFILE)


BIN =\
	bff-flip\
	bff-flop\
	bff-premultiply\
	bff-rotate-180\
	bff-rotate-270\
	bff-rotate-90\
	bff-transpose\
	bff-unpremultiply


HDR =\
	arg.h\
	common.h

all: $(BIN)

$(BIN:=.o): $(HDR)
common.o: $(HDR)
$(BIN): common.o

clean:
	-rm -- *.o $(BIN)

.PHONY: all clean
.PRECIOUS: common.o
