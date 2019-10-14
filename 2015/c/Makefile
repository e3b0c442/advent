CC      := gcc
CCFLAGS := -Wall -g -pedantic
LDFLAGS := -lcrypto -lpcreposix

DAYS := 1 2 3 4 5 6 7
TARGETS:= $(addprefix day, $(DAYS))
MAINS  := $(addsuffix .o, $(TARGETS) )
OBJ    := common.o esht.o $(MAINS)
DEPS   := common.c esht.c

.PHONY: all run clean

all: $(TARGETS)

run: all
	for i in $(DAYS) ; do \
		./day$$i ../input$$i.txt ; \
	done

clean:
	rm -f $(TARGETS) $(OBJ)

$(OBJ): %.o : %.c $(DEPS)
	$(CC) -c -o $@ $< $(CCFLAGS)

$(TARGETS): % : $(filter-out $(MAINS), $(OBJ)) %.o
	$(CC) -o $@ $(LIBS) $^ $(CCFLAGS) $(LDFLAGS)