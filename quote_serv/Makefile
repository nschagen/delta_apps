ROOT=../..

include $(ROOT)/Makefile.inc
include $(ROOT)/llvm/Makefile.inc

ORIGINAL=quote_original
PATCHED=quote_patched
DELTAFIED=quote_deltafied

ifeq ($(SANITIZER),memory)
SANITIZER_FLAGS=-fsanitize=memory
endif
ifeq ($(SANITIZER),memory_origins)
SANITIZER_FLAGS=-fsanitize=memory -fsanitize-memory-track-origins
endif
ifeq ($(SANITIZER),address)
SANITIZER_FLAGS=-fsanitize=address
endif

#CFLAGS+=-lm -O0 -Wall -ggdb -I $(ROOT)/llvm/include -ffunction-sections -fdata-sections -g $(SANITIZER_FLAGS) -fno-omit-frame-pointer
CFLAGS+=-lm -O0 -Wall -g
DELTA_STUB_LIB=$(INSTALL_DIR)/deltastub.bcc

.PHONY: clean

all: $(DELTAFIED)

$(ORIGINAL).bc: $(ORIGINAL).c $(DELTA_STUB_LIB)
	$(QUIET) $(ECHO) "Compiling $@"
	$(QUIET) $(LLVMGOLD_CC) $(CFLAGS) $(LLVMGOLD_CFLAGS) $(LLVMGOLD_LDFLAGS) -o $(ORIGINAL) $?
	#$(QUIET) $(LLVMGOLD_CC) $(CFLAGS) $(LLVMGOLD_CFLAGS) $(LLVMGOLD_LDFLAGS) -o $(patsubst %.bc,%,$@) $?
	#$(QUIET) rm $(patsubst %.bc,%,$@)

$(PATCHED).bc: $(PATCHED).c $(DELTA_STUB_LIB)
	$(QUIET) $(ECHO) "Compiling $@"
	$(QUIET) $(LLVMGOLD_CC) $(CFLAGS) $(LLVMGOLD_CFLAGS) $(LLVMGOLD_LDFLAGS) -o $(PATCHED) $?
	#$(QUIET) $(LLVMGOLD_CC) $(CFLAGS) $(LLVMGOLD_CFLAGS) $(LLVMGOLD_LDFLAGS) -o $(patsubst %.bc,%,$@) $?
	#$(QUIET) rm $(patsubst %.bc,%,$@)

$(DELTAFIED): $(ORIGINAL).bc $(PATCHED).bc
	$(QUIET) $(ECHO) "Deltafying $?"
	$(QUIET) make -f $(ROOT)/llvm/shared/delta/bin/Makefile.delta \
		ORIGINAL=$(word 1,$^) PATCHED=$(word 2,$^) OUTPUT=$@.bc SANITIZER=$(SANITIZER)
	$(QUIET) $(ECHO) "Linking $@"
	$(QUIET) $(LLVMGOLD_CC) $(SANITIZER_FLAGS) $(LLVMGOLD_LDFLAGS) -Wl,-plugin-opt=O0 -o $@ $@.bc

clean:
	rm -f $(ORIGINAL) $(PATCHED) $(DELTAFIED) *.bc *.opt.bc *.o
