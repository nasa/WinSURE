###############################################################################
#   Makefile for creating ASSIST
###############################################################################

# ANSI "C" compiler
CC = gcc -lm

# libraries required by system
LIBS = 

.KEEP_STATE:

###############################################################################
#   Compiler flags  (Define, Optimize, Warning) -- dependent upon architecture
###############################################################################



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

IFLAGS = -Icommon 

###############################################################################
#   Other macros
###############################################################################

COMPILE.assist.cc = $(CC) $(IFLAGS) $(WFLAGS) $(OFLAGS) \
                    $(DFLAGS) -DASSIST -DBIGSPACE -c

COMPILE.extra.cc = $(CC) $(IFLAGS) $(WFLAGS) $(XFLAGS) $(OFLAGS) \
                   $(DFLAGS) -DASSIST -DBIGSPACE -c

COMPILE.misc.cc = $(CC) $(IFLAGS) $(WFLAGS) $(OFLAGS) \
                  $(DFLAGS) -c

LINK.cc = $(CC) $(WFLAGS) $(XFLAGS) $(OFLAGS) $(LFLAGS)

ASTOBJ =  astgen.o astprs.o macprs.o  objlib.o fixlib.o astlex.o iolib.o aststr.o 

COMOBJ = common/prslib.o common/prseval.o \
         common/lexlib.o common/lowiolib.o common/strlib.o \
         common/prsiolib.o common/clinelib.o common/syslib.o \
         common/factlib.o common/gamma.o common/commonio.o

COMDEPEND = comstuff

COMHDEPEND = common/cm_defs.h common/cm_prsvars.h common/cm_sys.h \
             common/cm_stdio.h \
             common/cm_types.h common/cm_vars.h common/common.h \
             common/errdefs.h common/eval_ext.h common/fact_ext.h \
             common/gamma.h common/gammalim.h common/lex_ext.h \
             common/lio_ext.h common/main_def.h common/pass.h \
             common/perrdefs.h common/pio_ext.h common/prs_ext.h \
             common/prs_math.h common/prsdefs.h common/prstypes.h \
             common/prsvars.h common/rwdefs.h common/safeeval.h \
             common/str_ext.h common/sys_ext.h common/tokdefs.h \
             common/user_ext.h common/x_stdio.h common/x_time.h \
             common/lib_def.h common/commonio.h

###############################################################################
#  Rules to build the program(s)
###############################################################################

ifeq ($(MakeWinASSIST), 1)
everything : assist winAssist

assist: $(ASTOBJ) assist.o libraries
	$(LINK.cc) -o assist  $(ASTOBJ) assist.o $(COMOBJ) $(LIBS)

winAssist: $(ASTOBJ) winAssist.o afe.o aferc.o libraries
	$(LINK.cc) -o WinASSIST.exe -mwindows winAssist.o $(ASTOBJ) afe.o aferc.o $(COMOBJ) 
else
everything : assist # misc

assist: $(ASTOBJ) assist.o libraries
	$(LINK.cc) -o assist  $(ASTOBJ) assist.o $(COMOBJ) $(LIBS)
endif

libraries: $(COMDEPEND)


###############################################################################
#  Dependencies (ASSIST)
###############################################################################

assist.o : assist.c \
           objdefs.h objtypes.h objvars.h macvars.h astdefs.h asttypes.h \
           astvars.h astr_ext.h io_ext.h \
           ast_lexio_vars.h ast_lexio_defs.h alex_ext.h \
           obj_ext.h mac_ext.h aprs_ext.h agen_ext.h \
           alextabs.h astrw.h \
           common/main_def.h \
           astoptfig.h common/clinelib_ext.h \
	$(COMHDEPEND)
	$(COMPILE.extra.cc) assist.c

ifeq ($(MakeWinASSIST), 1)
winAssist.o : winAssist.c \
           objdefs.h objtypes.h objvars.h macvars.h astdefs.h asttypes.h \
           astvars.h astr_ext.h io_ext.h \
           ast_lexio_vars.h ast_lexio_defs.h alex_ext.h \
           obj_ext.h mac_ext.h aprs_ext.h agen_ext.h \
           alextabs.h astrw.h \
           common/main_def.h \
           common/clinelib_ext.h \
           $(COMHDEPEND) 
	$(COMPILE.extra.cc) winAssist.c
endif

astgen.o : astgen.c \
           objdefs.h objtypes.h objvars.h astdefs.h asttypes.h \
           astvars.h astr_ext.h io_ext.h \
           ast_lexio_vars.h ast_lexio_defs.h alex_ext.h \
           obj_ext.h mac_ext.h macvars.h aprs_ext.h agen_ext.h \
           $(COMHDEPEND)
	$(COMPILE.assist.cc) astgen.c

astprs.o : astprs.c \
           io_ext.h ast_lexio_vars.h ast_lexio_defs.h astrw.h \
           objdefs.h objtypes.h objvars.h astdefs.h asttypes.h astvars.h \
           macvars.h astr_ext.h alex_ext.h fix_ext.h obj_ext.h mac_ext.h \
           aprs_ext.h asttabsP.h \
           $(COMHDEPEND)
	$(COMPILE.assist.cc) astprs.c

macprs.o : macprs.c \
           astrw.h objdefs.h objtypes.h objvars.h astdefs.h asttypes.h \
           macvars.h astr_ext.h aprs_ext.h io_ext.h \
           ast_lexio_vars.h ast_lexio_defs.h alex_ext.h obj_ext.h mac_ext.h \
           $(COMHDEPEND)
	$(COMPILE.extra.cc) macprs.c

objlib.o : objlib.c \
           objdefs.h objtypes.h objvarsP.h objvars.h astr_ext.h io_ext.h \
           ast_lexio_vars.h ast_lexio_defs.h alex_ext.h fix_ext.h obj_ext.h \
           $(COMHDEPEND)
	$(COMPILE.extra.cc) objlib.c

fixlib.o : fixlib.c \
           objdefs.h objtypes.h objvars.h astdefs.h asttypes.h astvars.h \
           ast_lexio_vars.h ast_lexio_defs.h io_ext.h fix_ext.h obj_ext.h \
           $(COMHDEPEND)
	$(COMPILE.assist.cc) fixlib.c

astlex.o : astlex.c \
           astrw.h objdefs.h objtypes.h astr_ext.h io_ext.h \
           ast_lexio_vars.h ast_lexio_defs.h alex_ext.h fix_ext.h obj_ext.h \
           $(COMHDEPEND)
	$(COMPILE.assist.cc) astlex.c

iolib.o : iolib.c \
          astoptfig.h \
          astr_ext.h objdefs.h objtypes.h io_ext.h \
          ast_lexio_vars.h ast_lexio_defs.h obj_ext.h errtabsP.h errvarsP.h \
          $(COMHDEPEND) common/clinelib_ext.h
	$(COMPILE.extra.cc) iolib.c

aststr.o : aststr.c astr_ext.h \
           $(COMHDEPEND)
	$(COMPILE.extra.cc) aststr.c

ind2coll.o : ind2coll.c ind_vs_coll.h
	$(COMPILE.misc.cc) -o ind2coll.o ind2coll.c
	$(COMPILE.misc.cc) -DMANY_TRANTOS -o many_ind2coll.o ind2coll.c

coll2sort.o : coll2sort.c ind_vs_coll.h
	$(COMPILE.misc.cc) -o coll2sort.o coll2sort.c

indcolcmp.o : indcolcmp.c
	$(COMPILE.misc.cc) -o indcolcmp.o indcolcmp.c

ind_vs_coll.o : ind_vs_coll.c ind_vs_coll.h
	$(COMPILE.misc.cc) -o ind_vs_coll.o ind_vs_coll.c
	$(COMPILE.misc.cc) -DMANY_TRANTOS -o many_ind_vs_coll.o ind_vs_coll.c

ifeq ($(MakeWinASSIST), 1)
afe.o: 	afe.c 
	$(COMPILE.misc.cc) afe.c

aferc.o: afe.rc
	windres afe.rc aferc.o
endif


###############################################################################
#  Dependencies (COMMON)
###############################################################################

comstuff: force
	(cd common;make)

###############################################################################
#  Force
###############################################################################

force:

all:
	echo $(OS_DETECTED)
	echo $(DFLAGS)

