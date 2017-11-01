# path for the Musashi source
MUSASHI_SRC := Musashi

# Host C Compiler
HOSTCC ?= $(CC)

# default compiler options
CFLAGS := -O2 -g

# default rules
all ::
clean ::

# build the host tool for generating files
m68kmake : $(MUSASHI_SRC)/m68kmake.c
	$(HOSTCC) -o m68kmake $(MUSASHI_SRC)/m68kmake.c

# generate files
m68kopac.c m68kopdm.c m68kopnz.c m68kops.c m68kops.h : $(MUSASHI_SRC)/m68k_in.c | m68kmake
	./m68kmake . $(MUSASHI_SRC)/m68k_in.c

# build a few more files in Musashi
%.o : $(MUSASHI_SRC)/%.c
	$(COMPILE.c) $(OUTPUT_OPTION) $<
m68kcpu.o : $(MUSASHI_SRC)/m68kcpu.c
m68kdasm.o : $(MUSASHI_SRC)/m68kdasm.c

# build it as a library (HACK around a bug in my copy of GNU Make)
m68k.a : m68kopac.o m68kopdm.o m68kopnz.o m68kops.o m68kcpu.o m68kdasm.o
	$(AR) rvs $@ $^
m68k.a : CFLAGS := -O3 # override compiler options
m68k.a : CPPFLAGS := -I. -I$(MUSASHI_SRC) -DM68K_COMPILE_FOR_MAME=OPT_OFF
clean :: ; $(RM) m68k.a m68kopac.o m68kopdm.o m68kopnz.o m68kops.o m68kcpu.o m68kdasm.o

# our machine
all :: fant
fant : CPPFLAGS := -I$(MUSASHI_SRC) -DM68K_COMPILE_FOR_MAME=OPT_OFF
fant : CFLAGS += -Wall -W
fant : fant.o con.o mem.o tim.o m68k.a 
clean :: ; $(RM) fant fant.o con.o mem.o tim.o

# a small test program (hello.asm)
all :: hello.bin hello.srec
clean :: ; $(RM) hello.bin hello.srec

.PRECIOUS : %.srec

%.srec : %.asm
	bin/vasmm68k_mot -m68000 -Fsrec -s19 -o $@ $^

%.bin : %.srec
	bin/srec2bin $< $@
