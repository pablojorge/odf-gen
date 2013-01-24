# odf-gen: Simple API to generate OpenDocument documents.
#
#          Copyright Pablo Jorge 2009 - 2013.
# Distributed under the Boost Software License, Version 1.0.
#    (See accompanying file ../LICENSE_1_0.txt or copy at
#          http://www.boost.org/LICENSE_1_0.txt)
#

include ../Config.mk

ALL = example

all: $(ALL)

test: example
	./example > test.xml
	odsgen test.xml test.ods

example: example.cpp ../include/odsgenerator.h

CFLAGS += -I ../include

%: %.cpp
	$(CXX) $(CFLAGS) $< -o $@

clean:
	rm -rf $(ALL) test.*
