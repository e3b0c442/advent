CC      := gcc
CCFLAGS := -Wall -g -pedantic
LDFLAGS := -lcrypto

TARGETS:= day1 day2 day3 day4
MAINS  := $(addsuffix .o, $(TARGETS) )
OBJ    := common.o $(MAINS)
DEPS   := common.c

.PHONY: all clean

all: $(TARGETS)

clean:
	rm -f $(TARGETS) $(OBJ)

$(OBJ): %.o : %.c $(DEPS)
	$(CC) -c -o $@ $< $(CCFLAGS)

$(TARGETS): % : $(filter-out $(MAINS), $(OBJ)) %.o
	$(CC) -o $@ $(LIBS) $^ $(CCFLAGS) $(LDFLAGS)