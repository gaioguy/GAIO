# GAIO 2.3.x
# Copyright (C) 2003-2009 djs² GmbH
# matlab makefile

include ../../make.inc

.SUFFIXES: .$(MEXEXT)

OBJS = subsref.$(MEXEXT) subsasgn.$(MEXEXT) delete.$(MEXEXT)

LIB = ../../src/$(ARCH)/*.o $(LIBS)
INC = -I.. -I../../src

%.$(MEXEXT): %.c
	$(MEX) CC=$(MEXCC) CFLAGS='$(MEXCFLAGS)' COPTIMFLAGS='' LD=$(MEXLD) LDFLAGS='$(MEXLDFLAGS)' $(MEXFLAGS) $(INC) $(MEXINC) $(DEFS) $(GAIOFLAGS) $< $(LIB) -output $@

lib: $(OBJS)

clean:
	rm -rf *.o

libclean: clean
	rm -rf *.$(MEXEXT)

