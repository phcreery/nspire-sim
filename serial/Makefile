# ifeq ($(strip $(ARCH)),)
# ARCH = nspire
# endif
# include Makefile.$(ARCH)



# DEBUG = FALSE

# GCC = nspire-gcc
# AS  = nspire-as
# GXX = nspire-g++
# LD  = nspire-ld
# GENZEHN = genzehn

# GCCFLAGS = -Wall -W -marm
# LDFLAGS = -lnspireio
# ZEHNFLAGS = --name "serial"

# ifeq ($(DEBUG),FALSE)
# 	GCCFLAGS += -Os
# else
# 	GCCFLAGS += -O0 -g
# endif

# OBJS = $(patsubst %.c, %.o, $(shell find . -name \*.c))
# OBJS += $(patsubst %.cpp, %.o, $(shell find . -name \*.cpp))
# OBJS += $(patsubst %.S, %.o, $(shell find . -name \*.S))
# EXE = serial
# DISTDIR = .
# vpath %.tns $(DISTDIR)
# vpath %.elf $(DISTDIR)

# all: $(EXE).prg.tns

# %.o: %.c
# 	$(GCC) $(GCCFLAGS) -c $<

# %.o: %.cpp
# 	$(GXX) $(GCCFLAGS) -c $<
	
# %.o: %.S
# 	$(AS) -c $<

# $(EXE).elf: $(OBJS)
# 	mkdir -p $(DISTDIR)
# 	$(LD) $^ -o $(DISTDIR)/$@ $(LDFLAGS)

# $(EXE).tns: $(EXE).elf
# 	$(GENZEHN) --input $(DISTDIR)/$^ --output $(DISTDIR)/$@ $(ZEHNFLAGS)

# $(EXE).prg.tns: $(EXE).tns
# 	make-prg $(DISTDIR)/$^ $(DISTDIR)/$@

# clean:
# 	rm -f *.o $(DISTDIR)/$(EXE).tns $(DISTDIR)/$(EXE).elf $(DISTDIR)/$(EXE).prg.tns



MAIN = serial
SRCS = serial.c

DEBUG = FALSE

GCC = nspire-gcc
AS  = nspire-as
GXX = nspire-g++
LD  = nspire-ld
GENZEHN = genzehn

# GCCFLAGS = -Wall -W -marm -I../../include
GCCFLAGS = -Wall -W -marm
# LDFLAGS = ../nspire-io/lib/libnspireio.a
LDFLAGS = -lnspireio
ZEHNFLAGS = --name "tests"

ifeq ($(DEBUG),FALSE)
	GCCFLAGS += -Os
else
	GCCFLAGS += -O0 -g
endif

OBJS = $(patsubst %.c, %.o, $(shell find . -name \*.c))
OBJS += $(patsubst %.cpp, %.o, $(shell find . -name \*.cpp))
OBJS += $(patsubst %.S, %.o, $(shell find . -name \*.S))
EXE = serial
# DISTDIR = ../../bin
DISTDIR = .
vpath %.tns $(DISTDIR)
vpath %.elf $(DISTDIR)

all: $(EXE).prg.tns

%.o: %.c
	$(GCC) $(GCCFLAGS) -c $<

%.o: %.cpp
	$(GXX) $(GCCFLAGS) -c $<
	
%.o: %.S
	$(AS) -c $<

$(EXE).elf: $(OBJS)
	mkdir -p $(DISTDIR)
	$(LD) $^ -o $@ $(LDFLAGS)

$(EXE).tns: $(EXE).elf
	$(GENZEHN) --input $^ --output $@ $(ZEHNFLAGS)

$(EXE).prg.tns: $(EXE).tns
	make-prg $^ $@

clean:
	rm -f *.o $(DISTDIR)/$(EXE).tns $(DISTDIR)/$(EXE).elf $(DISTDIR)/$(EXE).prg.tns
