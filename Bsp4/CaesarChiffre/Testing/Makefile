TARGET   = caesarTest

CC       = gcc
# compiling flags here
CFLAGS   = -g -pthread -MD -MMD

LINKER   = gcc -o 
# linking flags here
LFLAGS   = -g -pthread #-m fuer autodependencys

# change these to set the proper directories where each files shoould be
SRCDIR   = src
OBJDIR   = obj
TESTDIR   = .
INSTALLDIR = bin

SOURCES  := $(wildcard $(SRCDIR)/*.c)
INCLUDES := $(wildcard $(SRCDIR)/*.h)
OBJECTS  := $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
rm       = rm -f


INCLUDES := $(OBJDIR)/:.o=.d

all: $(TESTDIR)/$(TARGET) install

-include $(OBJECTS:.o=.d)

.PHONEY: dirs
dirs: 
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
	mv $(TARGET) $(INSTALLDIR)/
	chmod 744 $(INSTALLDIR)/$(TARGET)

.PHONEY: help
help:
	echo "make to create, make install to create application"

.PHONEY: clean
clean:
	@$(rm) $(OBJDIR)/*
	@echo "Cleanup complete!"

.PHONEY: run
run:
	clear
	$(INSTALLDIR)/$(TARGET)
	#gnome-terminal -x sh -c "$(INSTALLDIR)/$(TARGET)"

.PHONEY: remove
remove: clean
	rm -r $(OBJDIR)
	rm -r $(INSTALLDIR)
	@echo "Executable removed!"
