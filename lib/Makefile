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

TARGETS = EmpireExceptions.o
TESTS   = EmpireExceptionsTest

TARGET   = libempire.a

all: $(TARGET)

$(TARGET): EmpireExceptions.o
	$(AR) -rsv $(TARGET) $^

include ../Common.mk
