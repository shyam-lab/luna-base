
include Makefile.inc
ifeq ($(ARCH),WINDOWS)
  TARGETS = luna destrat behead
else
  TARGETS = luna libluna destrat behead merge
endif

SRCS = globals.cpp eval.cpp cmddefs.cpp \
        $(wildcard edf/*.cpp) \
        $(wildcard edfz/*.cpp) \
        $(wildcard defs/*.cpp) \
	$(wildcard tinyxml/*.cpp) \
	$(wildcard helper/*.cpp) \
	$(wildcard timeline/*.cpp) \
	$(wildcard annot/*.cpp) \
	$(wildcard dsp/*.cpp) \
	$(wildcard miscmath/*.cpp) \
	$(wildcard artifacts/*.cpp) \
	$(wildcard spindles/*.cpp) \
	$(wildcard intervals/*.cpp) \
	$(wildcard fftw/*.cpp) \
	$(wildcard cwt/*.cpp) \
        $(wildcard stats/*.cpp) \
	$(wildcard staging/*.cpp) \
	$(wildcard suds/*.cpp) \
	$(wildcard db/*.cpp) \
	$(wildcard ica/*.cpp) \
	$(wildcard clocs/*.cpp) \
        $(wildcard pdc/*.cpp) \
	$(wildcard sstore/*.cpp) \
	$(wildcard dsp/mtm/*.cpp) \
	$(wildcard dsp/libsamplerate/*.cpp)

CSRCS = $(wildcard db/*.c) \
        $(wildcard dsp/libsamplerate/*.c)

OBJS = $(SRCS:.cpp=.o) $(CSRCS:.c=.o) 

DEPS := $(OBJS:.o=.d)

#
# targets
#

all : $(TARGETS)

luna: main.o $(OBJS)
	$(CXX) -o $@ $^ $(LDFLAGS)

libluna: libluna.a $(SHARED_LIB)

# header dependencies

-include $(DEPS)

# shared library (libluna)

ifeq ($(ARCH),MAC)
$(SHARED_LIB) : $(OBJS)
	$(LD) -dynamiclib $(LDFLAGS) -o $(SHARED_LIB)  $(OBJS)
else ifeq ($(ARCH),LINUX)
$(SHARED_LIB) : $(OBJS)
	$(LD) -shared $(LDFLAGS) -o $(SHARED_LIB) $(OBJS)
endif

# onjects
libluna.a : $(OBJS)
	$(AR) $(ARFLAGS) $@ $?
	$(RANLIB) $@

static: main.o $(OBJS) $(FFTW)/lib/libfftw3.a
	$(CXX) -static -static-libgcc -static-libstdc++ -o luna-static $^ 

destrat: utils/reader.o libluna.a
	$(CXX) -o $@ $^ -L. -lz 

behead: utils/behead.o
	$(CXX) -o $@ $^ 

merge: utils/merge.o utils/merge-helpers.o
	$(CXX) -o $@ $^ 

.PHONY: clean

clean:
	-$(RM) $(TARGETS) libluna.dylib libluna.so main.o $(OBJS) $(DEPS) $(addsuffix ~,$(SRCS) $(CSRCS)) 
