COMPILER=clang++
CPPFLAGS= -Wall -pedantic -g 
LDLIBS= -lncurses
SRC=curses.cpp
TARGET=TestRL

all:
	$(COMPILER) $(CPPFLAGS) $(SRC) $(LDLIBS) -o $(TARGET)

clean:
	rm -rf $(TARGET) $(TARGET).dSYM
