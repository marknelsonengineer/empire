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

TARGETS = main.o World.o
TESTS   = 

all: $(TARGETS)

include ../Common.mk

# TARGET = empire src_docs

DOXYGEN_CONF = ./doxygen/doxygen.conf

src_docs: $(DOXYGEN_CONF)
	doxygen $(DOXYGEN_CONF)
	# clear && make src_docs 2>&1 | fgrep "Commodity.cpp"

test:
	echo blob
	