###############################################################################
# Empire ][
#
# @file    Makefile
# @version 1.0 - Initial implementation
#
# @author Mark Nelson <marknels@hawaii.edu>
# @date   29 Jan 2021
# @copyright (c) 2021 Mark Nelson
###############################################################################

TARGETS =
TESTS   = 

all: $(TARGETS)

include ./Common.mk

DOXYGEN_CONF = ./doxygen/doxygen.conf

src_docs: $(DOXYGEN_CONF)
	doxygen $(DOXYGEN_CONF)
