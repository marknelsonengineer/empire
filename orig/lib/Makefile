###############################################################################
# Empire ][
#
# @file    lib/Makefile
# @version 1.0 - Initial implementation
#
# @author Mark Nelson <marknels@hawaii.edu>
# @date   29 Jan 2021
# @copyright (c) 2021 Mark Nelson
###############################################################################

TARGETS = EmpireExceptions.o   Singleton.o   Log.o
TESTS   = EmpireExceptionsTest SingletonTest LogTest

TARGET  = libempire.a

all: $(TARGET)

$(TARGET): $(TARGETS)
	$(AR) -rsv $(TARGET) $^

include ../Common.mk
