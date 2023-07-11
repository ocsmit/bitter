CC = gcc
CFLAGS = -g -Wall -O0 -std=c11 -Wextra -Wwrite-strings \
-Wno-parentheses -Wpedantic -Warray-bounds -Wconversion  -Wstrict-prototypes -Wnewline-eof

# Test for leaks with llvm
# Apple clang does not support sanitize
ifeq ($(CHECKLEAK),1)
	CC=/opt/homebrew/opt/llvm/bin/clang
	CFLAGS += -fsanitize=address -fsanitize=leak
endif
#LDFLAGS = -lm

SOURCES=$(wildcard src/**/*.c src/*.c)
OBJECTS=$(patsubst %.c, %.o, $(SOURCES)) # list *.c -> *.o
TARGET = tests


# The first target defined in the makefile is the one
# used when make is invoked with no argument. Given the definitions
# above, this Makefile file will build the one named TARGET and
# assume that it depends on all the named OBJECTS files.
$(TARGET) : $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Phony means not a "real" target, it doesn't build anything
# The phony target "clean" is used to remove all compiled object files.

test: $(TARGET)
	@echo ------------------ TEST ------------------
	@./tests

.PHONY: clean
# clean
clean:
	rm -rf build $(OBJECTS) $(TESTS) $(TARGET)
	find . -name "*.gc*" -exec rm {} \;
	rm -rf `find . -name "*.dSYM" -print` # Remove XCode junk


check:
	@echo Files with pontentially dangerous functions.
	@egrep '[^_.>a-zA-Z0-9] (str(n?cpy|n?cat|xfrm|n?dup|str|pbrk|tok|_)\
		|stpn?cpy|a?sn?printf|byte_)' $(SOURCES) || true
