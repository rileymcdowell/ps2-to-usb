# The name of your project (used to name the compiled .hex file)
TARGET = main

# configurable options
OPTIONS = -DF_CPU=48000000 -DUSB_HID -DLAYOUT_US_ENGLISH #-DUSB_SERIAL = debug. -DUSB_HID = keyboard


# options needed by many Arduino libraries to configure for Teensy 3.0
OPTIONS += -D__MK20DX256__ -DARDUIO=104


#************************************************************************
# Location of Teensyduino utilities, Toolchain, and Arduino Libraries.
# To use this makefile without Arduino, copy the resources from these
# locations and edit the pathnames.  The rest of Arduino is not needed.
#************************************************************************

# path location for Teensy Loader, teensy_post_compile and teensy_reboot
# Don't care, prefer to make 'deploy' using command line tool.

# path location for the arm-none-eabi compiler
COMPILERPATH = ../tools/arm-none-eabi/bin

INCLUDEPATH = ../libs

#************************************************************************
# Settings below this point usually do not need to be edited
#************************************************************************

# CPPFLAGS = compiler options for C and C++
CPPFLAGS = -Wall -Werror -g -Os -mcpu=cortex-m4 -mthumb -nostdlib -MMD $(OPTIONS) -I$(INCLUDEPATH)

# compiler options for C++ only
CXXFLAGS = -std=gnu++0x -felide-constructors -fno-exceptions -fno-rtti

# compiler options for C only
CFLAGS =

# linker options
LDFLAGS = -Os -Wl,--gc-sections -mcpu=cortex-m4 -mthumb -T$(INCLUDEPATH)/mk20dx256.ld

# additional libraries to link
LIBS = -lm

# names for the compiler programs
CC = $(abspath $(COMPILERPATH))/arm-none-eabi-gcc
CXX = $(abspath $(COMPILERPATH))/arm-none-eabi-g++
OBJCOPY = $(abspath $(COMPILERPATH))/arm-none-eabi-objcopy
SIZE = $(abspath $(COMPILERPATH))/arm-none-eabi-size

# automatically create lists of the sources and objects
# TODO: this does not handle Arduino libraries yet...
C_FILES := $(wildcard *.c)
CPP_FILES := $(wildcard *.cpp)
OBJS := $(C_FILES:.c=.o) $(CPP_FILES:.cpp=.o)


# the actual makefile rules (all .o files built by GNU make's default implicit rules)
all: $(TARGET).hex

# It doesn't understand mk20dx256, so the predecessor will do.
deploy: all
	teensy_loader_cli -mmcu=mk20dx128 -v $(TARGET).hex

$(TARGET).elf: $(OBJS) $(INCLUDEPATH)/mk20dx256.ld
	$(CC) $(LDFLAGS) -o $@ $(OBJS) $(LIBS)

%.hex: %.elf
	$(SIZE) $<
	$(OBJCOPY) -O ihex -R .eeprom $< $@

# compiler generated dependency info
-include $(OBJS:.o=.d)

clean:
	rm -f *.o *.d $(TARGET).elf $(TARGET).hex
	
cleanall: clean
	rm *~


