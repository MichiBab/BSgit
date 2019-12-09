TARGET   = Philosophenproblem

CC       = gcc
# compiling flags here
CFLAGS   = -lrt -pthread -MD -MMD

LINKER   = gcc -o 
# linking flags here
LFLAGS   = -lrt -g -Wall -I. -lm -pthread #-m fuer autodependencys

# change these to set the proper directories where each files shoould be
SRCDIR   = src
OBJDIR   = obj
TESTDIR   = test
INSTALLDIR = bin

SOURCES  := $(wildcard $(SRCDIR)/*.c)
INCLUDES := $(wildcard $(SRCDIR)/*.h)
OBJECTS  := $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
rm       = rm -f


INCLUDES := $(OBJDIR)/:.o=.d

all: dirs $(TESTDIR)/$(TARGET)


-include $(OBJECTS:.o=.d)

.PHONEY: dirs
dirs: 
	mkdir -p $(TESTDIR)
	mkdir -p $(OBJDIR)
	mkdir -p $(INSTALLDIR)

$(TESTDIR)/$(TARGET): $(OBJECTS)
	@$(LINKER) $@ $(LFLAGS) $(OBJECTS) -lrt
	@echo "Linking complete!"

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@ -lrt
	echo "Compiled "$<" successfully!"

.PHONEY: install
install: 
	cp $(TESTDIR)/* $(INSTALLDIR)/
	chmod 744 $(INSTALLDIR)/$(TARGET)

.PHONEY: help
help:
	echo "make to create, make install to create application"

.PHONEY: clean
clean:
	@$(rm) $(OBJECTS)
	@echo "Cleanup complete!"

.PHONEY: remove
remove: clean
	rm -r $(TESTDIR)
	rm -r $(OBJDIR)
	rm -r $(INSTALLDIR)
	@echo "Executable removed!"
