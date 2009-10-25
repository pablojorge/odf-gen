include ../Config.mk

ALL = example

all: $(ALL)

test: example
	./example > test.xml
	odsgen test.xml test.ods

example: example.cpp ../include/odsgenerator.h

%: %.cpp
	$(CXX) $(CFLAGS) $< -o $@

clean:
	rm -rf $(ALL) test.*
