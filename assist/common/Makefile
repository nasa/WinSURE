###############################################################################
#   Makefile for creating common stuff
###############################################################################

# ANSI "C" compiler
GCC = gcc 
CC = cc

# libraries required by system
LIBS = -lm

.KEEP_STATE:

###############################################################################
#   Compiler flags  (Define, Optimize, Warning) -- dependent upon architecture
###############################################################################

#IFLAGS = -I../lib



ifeq ($(OS),Windows_NT)
  OS_DETECTED := Windows
else
  UNAME_S := $(shell uname -s)
  ifeq ($(UNAME_S),Linux)
    OS_DETECTED := Linux
  endif
  ifeq ($(UNAME_S),Darwin)
    OS_DETECTED := MacOS
  endif
endif
ifeq ($(OS_DETECTED),Linux)
  # --------------------------------------------  LINUX
  DFLAGS = -DLINUX -DMUST_ALIGN
  OFLAGS =  #-fno-rerun-cse-after-loop -g
  WFLAGS = -Wall -Wno-parentheses -Wpointer-arith -Wcast-qual -Wshadow -Wmissing-prototypes
  XFLAGS = -Wcast-align
  MakeWinASSIST = 0
else
  ifeq ($(OS_DETECTED),MacOS)
    # --------------------------------------------  MAC
    DFLAGS = -DUNIX -DLINUX -DMUST_ALIGN
    OFLAGS =  #-fno-rerun-cse-after-loop -g
    WFLAGS = -Wall -Wno-parentheses -Wpointer-arith -Wcast-qual -Wshadow -Wmissing-prototypes
    XFLAGS = -Wcast-align
    MakeWinASSIST = 0
  else
    # --------------------------------------------  Windows 7
    DFLAGS = -DMSDOS
    WFLAGS = -Wall
    MakeWinASSIST = 1
  endif
endif

###############################################################################
#   Other macros
###############################################################################

COMPILE.common.gcc = $(GCC) $(IFLAGS) $(WFLAGS) $(OFLAGS) $(DFLAGS) -c
COMPILE.extra.gcc = $(GCC) $(IFLAGS) $(WFLAGS) $(XFLAGS) $(OFLAGS) $(DFLAGS) -c

COMHFILES = cm_sys.h cm_stdio.h cm_defs.h cm_types.h cm_vars.h cm_prsvars.h \
            lib_def.h user_ext.h sys_ext.h commonio.h

HFILES = $(COMHFILES) \
         pass.h tokdefs.h errdefs.h rwdefs.h str_ext.h \
         lio_ext.h pio_ext.h lex_ext.h prs_ext.h eval_ext.h

ALLHFILES = $(HFILES) prsdefs.h prstypes.h prsvars.h safeeval.h


###############################################################################
#  Rules to build the files
###############################################################################

everything: prslib.o prseval.o lexlib.o lowiolib.o strlib.o \
            prsiolib.o clinelib.o syslib.o factlib.o gamma.o commonio.o


###############################################################################
#  Dependencies (COMMON)
###############################################################################

prslib.o : prslib.c $(ALLHFILES) prstabsP.h $(LIBHDEPEND) 
	$(COMPILE.common.gcc) prslib.c

prseval.o : prseval.c $(ALLHFILES) \
            fact_ext.h gamma.h prs_math.h doopval.c evalexpr.c \
            $(LIBHDEPEND)
	$(COMPILE.extra.gcc) prseval.c

lexlib.o : lexlib.c $(COMHFILES) lextabsP.h $(LIBHDEPEND)
	$(COMPILE.common.gcc) lexlib.c

lowiolib.o : lowiolib.c $(COMHFILES) errdefs.h  lio_ext.h $(LIBHDEPEND)
	$(COMPILE.extra.gcc) lowiolib.c

strlib.o : strlib.c $(COMHFILES) \
           pass.h errdefs.h str_ext.h strtabsP.h \
           $(LIBHDEPEND)
	$(COMPILE.extra.gcc) strlib.c

prsiolib.o : prsiolib.c $(HFILES) $(LIBHDEPEND)
	$(COMPILE.extra.gcc) prsiolib.c

clinelib.o : clinelib.c $(COMHFILES) pass.h $(LIBHDEPEND)
	$(COMPILE.extra.gcc) clinelib.c

syslib.o : syslib.c $(COMHFILES) pass.h $(LIBHDEPEND)
	$(COMPILE.extra.gcc) syslib.c

factlib.o : factlib.c $(COMHFILES) \
            errdefs.h safeeval.h fact_ext.h prs_math.h \
            $(LIBHDEPEND)
	$(COMPILE.extra.gcc) factlib.c

gamma.o : gamma.c $(COMHFILES) gammalim.h gamma.h $(LIBHDEPEND)
	$(COMPILE.extra.gcc) gamma.c

commonio.o : commonio.c commonio.h
	$(COMPILE.extra.gcc) commonio.c

###############################################################################
#  Dependencies (LIB)
###############################################################################
#
#libstuff: force
#	(cd ../lib;make)

###############################################################################
#  Force
###############################################################################

force:
