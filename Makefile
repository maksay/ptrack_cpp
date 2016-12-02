#
#  Makefile
#
# Determine the platform
UNAME_S := $(shell uname -s)

# CC
ifeq ($(UNAME_S),Darwin)
  #CC := clang++ -arch x86_64
  CC := g++
else
  CC := g++
endif

# Folders
LIBSRCDIR := src
RUNSRCDIR := runners
INCDIR := include
LIBBUILDDIR := build/src
RUNBUILDDIR := build/runners
TARGETDIR := bin
VERBOSE := 1
CTAGS := ctags

LGUROBI := lgurobi60
ifneq (,$(findstring Gurobi Optimizer version 7,$(shell gurobi_cl --version)))
  LGUROBI := lgurobi70
endif

TAGFLAGS := --c++-kinds=+p --fields=+ialS --extra=+q

# Code Lists
SRCEXT := cc
LIBSOURCES := $(shell find $(LIBSRCDIR) -type f -name *.$(SRCEXT))
RUNSOURCES := $(shell find $(RUNSRCDIR) -type f -name *.$(SRCEXT))
INCSOURCES := $(shell find $(INCDIR) -type f -name '*.h')

LIBOBJECTS := $(patsubst $(LIBSRCDIR)/%,$(LIBBUILDDIR)/%,$(LIBSOURCES:.$(SRCEXT)=.o))
RUNOBJECTS := $(patsubst $(RUNSRCDIR)/%,$(RUNBUILDDIR)/%,$(RUNSOURCES:.$(SRCEXT)=.o))

# Folder Lists
INCDIRS := $(shell find $(INCDIR) -name '*.h' -exec dirname {} \; | sort | uniq)
INCLIST := $(patsubst $(INCDIR)%,-I $(INCDIR)%,$(INCDIRS))

LIBSRCDIRS := $(shell find $(LIBSRCDIR) -name *.$(SRCEXT) -exec dirname {} \; | sort | uniq)
LIBBUILDLIST := $(patsubst $(LIBSRCDIR)%,$(LIBBUILDDIR)%,$(LIBSRCDIRS))

RUNSRCDIRS := $(shell find $(RUNSRCDIR) -name *.$(SRCEXT) -exec dirname {} \; | sort | uniq)
RUNBUILDLIST := $(patsubst $(RUNSRCDIR)%,$(RUNBUILDDIR)%,$(RUNSRCDIRS))
TARGETLIST := $(patsubst $(RUNSRCDIR)%,$(TARGETDIR)%,$(RUNSRCDIRS))

TARGETFILES := $(patsubst $(RUNSRCDIR)%.$(SRCEXT),$(TARGETDIR)%,$(RUNSOURCES))

# Shared Compiler Flags
# MMD - creates a .d file that contains dependencies on h files
CFLAGS := -m64 -g -MMD -D_GLIBCXX_USE_CXX11_ABI=0 -Wno-unused-result
INC := $(INCLIST) -I ${GUROBI_HOME}/include/
LIB := -L${GUROBI_HOME}/lib/ -lgurobi_c++ -$(LGUROBI)


ifeq ($(UNAME_S),Darwin)
  # GUROBI ONLY WORKS ON MAC WITH LIBSTDC++ FOR NOW
  RELEASE := $(shell uname -r | cut -f 1 -d .)
  ifneq ($(RELEASE), 11)
    CFLAGS += -stdlib=libstdc++
    LIB += -stdlib=libstdc++
  endif
endif

CFLAGS += -O2

all: $(TARGETFILES) external

external: external/hungarian_assignment/bin/hungarian_solver

external/hungarian_assignment/bin/hungarian_solver: external/hungarian_assignment/src/*
	@if [ $(VERBOSE) = 1 ]; then echo "Making external tools"; fi
	@$(MAKE) -C external/hungarian_assignment/

$(TARGETDIR)/%: $(RUNBUILDDIR)/%.o $(LIBOBJECTS)
	@mkdir -p $(TARGETLIST)
	@if [ $(VERBOSE) = 1 ]; then echo "Linking $<..."; fi
	@$(CC) $(CFLAGS) -o $@ $^ $(LIB)

$(RUNBUILDDIR)/%.o: $(RUNSRCDIR)/%.$(SRCEXT)
	@mkdir -p $(RUNBUILDLIST)
	@if [ $(VERBOSE) = 1 ]; then echo "Compiling $<..."; fi
	@$(CC) $(CFLAGS) $(INC) -c -o $@ $<

-include $(RUNBUILDDIR)/*.d

$(LIBBUILDDIR)/%.o: $(LIBSRCDIR)/%.$(SRCEXT)
	@mkdir -p $(LIBBUILDLIST)
	@if [ $(VERBOSE) = 1 ]; then echo "Compiling $<..."; fi
	@$(CC) $(CFLAGS) $(INC) -c -o $@ $<

-include $(LIBBUILDDIR)/*.d

.SECONDARY: $(RUNOBJECTS) $(LIBOBJECTS)

clean:
	@if [ $(VERBOSE) = 1 ]; then echo "Cleaning..."; fi
	@$(RM) -r $(LIBBUILDDIR) $(RUNBUILDDIR) $(TARGETDIR)

.PHONY: all clean
