.POSIX:

CONFIGFILE = config.mk
include $(CONFIGFILE)


BIN =\
	bff-premultiply\
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
