CXX = g++-4.9
CC = gcc-4.9
DOXYGEN = doxygen
CFLAGS = -fdiagnostics-color=always -std=gnu11 -s -c -g3 -O3
WARNINGS = -Werror -Wall -Wextra -pedantic-errors -Wformat=2 -Wno-import -Wimplicit -Wmain -Wchar-subscripts -Wsequence-point -Wmissing-braces -Wparentheses -Winit-self -Wswitch-enum -Wstrict-aliasing=2 -Wundef -Wshadow -Wpointer-arith -Wbad-function-cast -Wcast-qual -Wcast-align -Wwrite-strings -Wold-style-definition -Wredundant-decls -Wnested-externs -Winline -Wdisabled-optimization -Wunused-macros -Wno-unused
SOURCES = main.c
OBJECTS = $(SOURCES:.c=.o)
EXECUTABLE = UTTT

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@

debug: CFLAGS += -DDEBUG -g
debug: $(SOURCES) $(EXECUTABLE)

.c.o:
	$(CC) $< -o $@ $(CFLAGS) $(WARNINGS)

.PHONY: doc clean
doc:
	$(DOXYGEN) doxygen.config
clean:
	rm -rf $(EXECUTABLE) $(OBJECTS)
