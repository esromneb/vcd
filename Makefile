.PHONY: test all

all: test

test: test_types


test_types: csrc/test_types.cpp *.cpp *.c *.h *.hpp
	g++ csrc/test_types.cpp playback.cpp -std=gnu++1z -o $@ -I.

