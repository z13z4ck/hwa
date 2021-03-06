
#  -*- Makefile -*-

#  This Makefile is responsible for building the example projects. It needs a
#  board name to be defined with a corresponding board definition header file
#  that defines the target device.

#$(warning INCLUDE)

#  MFD contains the directory name of this Makefile whatever has been included
#
MFD	:=	$(dir $(lastword $(MAKEFILE_LIST)))

#$(info Loaded $(MFD)Makefile.cc)

#	Choose a default board if none specified
#
ifeq (,$(BOARD))
  BOARD		:=	$(firstword $(ALLBOARDS))
endif

#	Load board's Makefile if it exists
#
#$(warning include $(MFD)../boards/Makefile.$(BOARD))
-include $(MFD)../boards/Makefile.$(BOARD)

#	Output directory and files basename
#
OUTDIR		= build/$(BOARD)
OUT		= $(OUTDIR)/out

#  Directory of boards
#
BOARDS		= $(MFD)../boards

#  Diabolo is the preferred bootloader
#
diabolo		= $(HOME)/dev/hwa/atmel/avr/examples/diabolo/software/diabolo.py

#  BOARD_H indicates the board definition header file the example project source
#  must use
#
DEFINES		+= -DBOARD_H=\<boards/$(BOARD).h\>

#  Directories searched for include files:
#   * ~hwa/include/
#   * ~hwa/atmel/avr/examples/
#
INCLUDES	= -I$(MFD)../../../../include 
INCLUDES	+= -I$(MFD)..

#  Messages in english
#
LANG		:= C

#  Toolchain commands
#
ARCH		:= arm-none-eabi-
CPP		:= $(ARCH)cpp
CC		:= $(ARCH)gcc
AS		:= $(ARCH)as
OBJCOPY		:= $(ARCH)objcopy
ODUMP		:= $(ARCH)objdump
GDB		:= $(ARCH)gdb
SIZE		:= $(ARCH)size

#  Variables used by external scripts
#
export LANG
export BOARD
export DEVICE
export OUTDIR
export OUT


################################################################################
#									       #
#			Board configuration				       #
#									       #
################################################################################

#  Create a C source file that collects HWA definitions relative to the
#  configuration of the target device
#
#	Quoting with HW_QUOTE is required for make not to show an error message
#	when it reads complex values.
#
$(BOARDS)/build/$(BOARD).c:
#	@echo "RULE: $@"
	@mkdir -p $(BOARDS)/build
	@echo "DEVICE=HW_DEVICE" >$@
	@echo "DEVICE_FLASH_SIZE=HW_DEVICE_FLASH_SIZE" >>$@
	@echo "DEVICE_SYSHZ=hw_syshz" >>$@

#  Load informations about the target device
#
#  Let CPP process board.c using the definitions found in the board's header
#  file. __ASSEMBLER__ is defined so that HWA will not load the definitions that
#  produce C code and that are useless for what we want to do.
#
$(BOARDS)/build/$(BOARD): $(BOARDS)/build/$(BOARD).c $(BOARDS)/$(BOARD).h
#	@echo "RULE: $@ : $^"
	@cpp -I$(MFD)../../../../include					\
		-D__ASSEMBLER__							\
		-imacros $(BOARDS)/$(BOARD).h $(BOARDS)/build/$(BOARD).c | grep '^DEVICE' >$@

#  Include informations about the target device, borrowed from the board header
#  file
#
#$(warning include $(BOARDS)/build/$(BOARD))
-include $(BOARDS)/build/$(BOARD)


################################################################################
#									       #
#			Dependencies					       #
#									       #
################################################################################

.PHONY: all
all:	deps bin lst size crc32

hex:	$(OUT).hex
bin:	$(OUT).bin
lst:	$(OUT).lst

.PHONY: deps
deps:	$(DEPS)

#  List of dependency files
#  One dependency file is generated for each .c or .sx source file
#
DEPS	 = $(SOURCES:.c=.d)
DEPS	:= $(DEPS:.sx=.d)
DEPS	:= $(addprefix $(OUTDIR)/,$(DEPS))

#  Include dependencies
#
-include $(DEPS)

#	Dependencies of .c files
#
$(OUTDIR)/%.d: %.c
#	@echo "RULE: $@: $^"
	@mkdir -p $(OUTDIR)
	@set -e ; rm -f $@ ; $(CPP) -MM $(INCLUDES) $(DEFINES) $< >$@.tmp	;\
	sed 's,\($*\)\.o[ :]*,$(OUTDIR)/\1.o $@ : ,g' < $@.tmp > $@

#	Dependencies of .sx files
#
$(OUTDIR)/%.d: %.sx
#	@echo "RULE: $@: $^"
	@mkdir -p $(OUTDIR)
	@set -e ; rm -f $@ ; $(CC) -MM $(INCLUDES) $(DEFINES) $< > $@.tmp	;\
	sed 's,\($*\)\.o[ :]*,$(OUTDIR)/\1.o $@ : ,g' < $@.tmp > $@


################################################################################
#									       #
#			Preprocessing					       #
#									       #
################################################################################

#AFLAGS		+= -mmcu=$(DEVICE)
AFLAGS		+= -mcpu=cortex-m3 -mthumb
AFLAGS		+= -Wall
AFLAGS		+= -fno-diagnostics-show-caret	# GCC 4.8
#AFLAGS		+= -Wextra
AFLAGS		+= $(DEFINES)
AFLAGS		+= $(INCLUDES)

#.PHONY: force
force: ;
#	@echo "RULE: $@: $^"

#  It is sometimes useful to just preprocess the source file to determine what
#  went wrong with the HWA macros
#
%.cp.c: %.c
#	@echo "RULE $@: $^"
	@echo "$(CPP) $(CFLAGS) $< >$@"
	@$(CPP) $(CFLAGS) $< >$@
#	$(CPP) $(CFLAGS) $< |sed -e 's/;/;\n/g'|bcpp -s -i 2 -bcl | sed -e 's/#/\/\//g' > $@

#  .sx files must first be preprocessed in order to catch HWA error messages and
#  format them for a correct display
#
$(OUTDIR)/%.cp.sx %.cp.sx: %.sx
#	@echo "RULE $@: $^"
	@$(CPP) $(AFLAGS) $< >$@


################################################################################
#									       #
#			Compiling					       #
#									       #
################################################################################

#  Compiling options
#
#CFLAGS		+= -mmcu=$(DEVICE)
CFLAGS		+= -mcpu=cortex-m3 -mthumb 
CFLAGS		+= -std=c1x
CFLAGS		+= -Wall
CFLAGS		+= -Wextra
#CFLAGS		+= -pedantic
CFLAGS		+= -Wpedantic	# GCC 4.8
CFLAGS		+= -fno-diagnostics-show-caret	# GCC 4.8
#CFLAGS		+= -ftrack-macro-expansion=0
#CFLAGS		+= -fomit-frame-pointer
CFLAGS		+= -ffunction-sections
#CFLAGS		+= -fno-builtins
CFLAGS		+= -fpack-struct
#CFLAGS		+= -fshort-enums
#CFLAGS		+= -mcall-prologues
CFLAGS		+= -g
#CFLAGS		+= -pipe
CFLAGS		+= -Os
#CFLAGS		+= -O0
#CFLAGS		+= -O1
#CFLAGS		+= -O2
#CFLAGS		+= -O3
CFLAGS		+= $(DEFINES)
CFLAGS		+= $(INCLUDES)
#CFLAGS		+= -save-temps

$(OUTDIR)/%.o: %.c
#	@echo "RULE: $@ : $^"
	@mkdir -p $(OUTDIR)
	$(CC) $(CFLAGS) -c $< -o $@

#  We want .sx files to be preprocessed first
#
$(OUTDIR)/%.o: $(OUTDIR)/%.cp.sx
#	@echo "RULE: $@ : $^"
	@mkdir -p $(OUTDIR)
	@$(CC) $(AFLAGS) -c $< -o $@


################################################################################
#									       #
#			Linking						       #
#									       #
################################################################################

#  Object files list (from source files list)
#
OBJECTS		 = $(SOURCES:.c=.o)
OBJECTS		:= $(OBJECTS:.sx=.o)
OBJECTS		:= $(addprefix $(OUTDIR)/,$(OBJECTS))

#LDFLAGS		+= -mmcu=$(DEVICE)
LDFLAGS		+= -mcpu=cortex-m3 -mthumb 
#LDFLAGS		+= -s
LDFLAGS		+= -Wl,-Map,$(OUT).map,--cref
#LDFLAGS		+= -Wl,-u,vfprintf -lprintf_min
#LDFLAGS		+= -nostartfiles
LDFLAGS		+= -T../make/ram.ld
LDFLAGS		+= -Wl,--gc-sections
#LDFLAGS		+= -nostdlib # Provide clear_bss aso
#LDFLAGS		+= -Wl,--section-start=.protext=0x1E00
#LDFLAGS		+= -Wl,--section-start=.bss.pro=0x800100
#LDFLAGS		+= -L/usr/lib/avr/lib
#LDFLAGS		+= -L/home/indy/dev/avr/toolchain/current/install/avr/lib
#LDFLAGS		+= -lm

$(OUT).elf: $(OBJECTS) $(LDSCRIPT)
#	@echo "RULE: $@ : $^"
	@$(CC) $(LDFLAGS) -o $@ $(OBJECTS) $(LIBS)


################################################################################
#									       #
#			Post-linking					       #
#									       #
################################################################################

#  PADTO is used to fill unused flash memory space with 0xFF bytes
#
ifneq (,$(DEVICE_FLASH_SIZE))
#  $(info PADTO=$(DEVICE_FLASH_SIZE))
  PADTO = --pad-to $(DEVICE_FLASH_SIZE)
else
  PADTO =
endif

%.bin: %.elf
	@$(OBJCOPY) --gap-fill=0xFF -O binary $^ $@

# @$(OBJCOPY) --only-section .text --only-section .data	\
# 	-O binary --gap-fill=0xFF $(PADTO) $^ $@

%.hex: %.elf
	@$(OBJCOPY) --only-section .text --only-section .data	\
		-O ihex --gap-fill=0xFF $^ $@

#  Object dump flags:
#
#  -d: disassembly
#  -S: disassembly + source code
#  -h: section headers
#  --no-show-raw-insn: no machine code
#
%.lst: %.elf
	@$(ODUMP) -d $^ >$@

#  Size of the generated binary
#
.PHONY: size
size:	$(OUT).elf
	@$(SIZE) $^

#  Diabolo's CRC of the generated binary
#
.PHONY: stat
stat:	$(OUT).bin
	@if python -V >/dev/null 2>&1 ; then					\
	  $(diabolo) --mcu=$(DEVICE) --stat $(OUT).bin -q			;\
	else									\
	  echo "Install Python if you want Diabolo to stat your application."	;\
	fi

.PHONY: crc32
crc32:	$(OUT).bin
	@echo -n "CRC32: " ; crc32 $^


################################################################################
#									       #
#			Project validation				       #
#									       #
################################################################################

#  Store application parameters that will be used later to verify that the CRC
#  remains the same for a given configuration. This helps detecting variations
#  in code production after the HWA core has been modified.

#  Create a C source file that holds the configuration of a project.
#  Prefix symbols with a '_' that will be removed after compiling.
#
$(OUT).cfg.c: $(SOURCES)
#	@echo "RULE: $@: $^  CONFIG=$(CONFIG)"
	@set -e						;\
	if [ -z "$(CONFIG)" ] ; then			\
	  echo FAIL: no config				;\
	  exit 1					;\
	fi						;\
	echo "#define HW_Q(...) HW_Q2(__VA_ARGS__)" >$@	;\
	echo "#define HW_Q2(...) #__VA_ARGS__" >>$@	;\
	for c in $(CONFIG)				;\
	  do echo "_$$c=HW_Q($$c)" >>$@			;\
	done

#  Create a file that describes the configuration of the project. Assume the
#  configuration is included in the first source file.
#
$(OUT).cfg: $(OUT).cfg.c
	@$(CPP) $(INCLUDES) $(DEFINES) -imacros $(firstword $(SOURCES)) \
		$< | grep '^_.*' | sed -e 's/_//' >$@

#	  CCVERSION=$$(cat $(OUT).ccversion)						;\

.PHONY: ccversion
ccversion:
	@CCVERSION=$$($(CC) --version	\
	 | awk 'NR==1 {x=match($$3,/([0-9]+)\.([0-9]+)\.([0-9]+)/,m);	\
	        if(x!=0){print "$(CC)-"m[1]m[2]m[3]}}')				;\
	echo CCVERSION=$$CCVERSION


#  Check that the CRC is still the same for a given configuration
#
.PHONY: check
check: $(OUT).cfg $(OUT).bin #$(OUT).ccversion
#	@echo "RULE: $@: $^"
	@OUT=$(OUT) bash $(MFD)check.sh


################################################################################
#									       #
#			Device programming				       #
#									       #
################################################################################

#  This target automatically takes the name of the target that was not found.
#  Load the Makefile for programming targets.
#
ifeq (,$(PROG_SW))
PROG_SW		= diabolo
endif

ifneq (,$(filter $(MAKECMDGOALS), install diabolo erase decode-fuses reset))
#  $(info Loading $(MFD)Makefile.$(PROG_SW))
  include $(MFD)Makefile.$(PROG_SW)
endif

ifneq (,$(filter $(MAKECMDGOALS), gdb run))
  include $(MFD)Makefile.gdb
endif
