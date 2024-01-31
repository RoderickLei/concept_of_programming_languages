# Directory layout looks like this
# 	include/
# 		*.h
# 	src/
# 		*.cc
# 	tests/
# 		test_*.cc
# 	LICENCE
# 	Makefile
# 	README.md

# This Makefile doesn't handle the 'gtests' directory, so it won't be compiled.

INC_DIR = include
SRC_DIR = src
SOURCES = $(sort $(shell find $(SRC_DIR) -name '*.cc' )) #adding ! after '*.cc' basically disables the makefile; syntax error
OBJECTS = $(SOURCES:.cc=.o)
DEPS	= $(OBJECTS:.o=.d)
TARGET = lexer
CXX = g++
CFLAGS = -Wall -Wextra -Wpedantic -std=c++11 -g
CPPFLAGS = $(addprefix -I, $(INC_DIR))

# https://stackoverflow.com/questions/2145590/what-is-the-purpose-of-phony-in-a-makefile
# However, sometimes you want your Makefile to run commands that do not 
# represent physical files in the file system. Good examples for this are the 
# common targets "clean" and "all". Chances are this isn't the case, but you 
# may potentially have a file named clean in your main directory. In such a case 
# Make will be confused because by default the clean target would be associated 
# with this file and Make will only run it when the file doesn't appear to be 
# up-to-date with regards to its dependencies.
#
# These special targets are called phony and you can explicitly tell Make 
# they're not associated with files
.PHONY: all clean debug release

# http://haacked.com/archive/2004/02/14/difference-between-debug-vs-release-build.aspx/
# The biggest difference between these is that:
# In a debug build the complete symbolic debug information is emitted to help 
# while debugging applications and also the code optimization is not taken into 
# account.
#
# While in release build the symbolic debug info is not emitted and the code 
# execution is optimized.
#
# Also, because the symbolic info is not emitted in a release build, the size of 
# the final executable is lesser than a debug executable.
release: CFLAGS += -O0 -DNDEBUG
release: all
debug: CFAGS += -O0 -DNDEBUG -ggdb3
debug: all

all: $(TARGET)
	
clean: 
	$(RM) $(OBJECTS) $(DEPS) $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CFLAGS) $(CPPFLAGS) -o $@ $^

# If a file like test.o is required and a corresponding file test.cc exists,
# then the following command will be automatically created.
%.o: %.cc
	$(CXX) $(CFLAGS) $(CPPFLAGS) -MMD -o $@ -c $<