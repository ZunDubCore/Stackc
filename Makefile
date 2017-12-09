CC				=	gcc
# compiling flags here
CCFLAGS			=	-g3 -O0 -std=c11 -Wall -Wextra -I$(INCDIR)

LD				=	gcc
# linking flags here
LDFLAGS			=	-lm

PWD				=	.

# change these to proper directories where each file should be
BUILDDIR		=	$(PWD)/build
OBJDIR			=	$(BUILDDIR)/obj
BINDIR			=	$(BUILDDIR)/bin

SRCDIR			=	$(PWD)/src
INCDIR			=	$(PWD)/include

DOCDIR			=	$(PWD)/doc

TARGET			=	$(BINDIR)/stackc

SOURCES 		:=	$(shell find $(SRCDIR) -name '*.c')
INCLUDES 		:=	$(shell find $(INCDIR) -name '*.h')
OBJECTS			:=	$(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

rm				=	rm -rf
mkdir			=	mkdir -p

all: dir $(TARGET)

$(TARGET): $(OBJECTS)
	$(LD) $(OBJECTS) -o $@ $(LDFLAGS)

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.c
	$(CC) $(CCFLAGS) -c $< -o $@

dir:
	@$(mkdir) $(BUILDDIR) $(OBJDIR) $(BINDIR)

count:
	wc $(SOURCES) $(INCLUDES)

sloccount:
	sloccount $(INCDIR) $(SRCDIR)

doc: docdir
	doxygen default

docdir:
	@$(mkdir) $(DOCDIR)

.PHONY: clean
clean:
	$(rm) $(BUILDDIR)

.PHONY: remove
remove:
	$(rm) $(TARGET)

.PHONY: cleandoc
cleandoc:
	$(rm) $(DOCDIR)

.PHONY: cleanall
cleanall: remove clean cleandoc
