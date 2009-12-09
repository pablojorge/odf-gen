include ../Config.mk

ALL = example

all: $(ALL)

test: example
	./example > test.xml
	odsgen test.xml test.ods

example: example.cpp ../odfgen/odsgenerator.h

CFLAGS += -I ../odfgen

%: %.cpp
	$(CXX) $(CFLAGS) $< -o $@

clean:
	rm -rf $(ALL) test.*
