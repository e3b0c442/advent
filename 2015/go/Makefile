DAYS:= 1 2 3 4
TARGETS := $(addprefix day, $(DAYS))

.PHONY: all run clean

all:
	mkdir -p build
	go build -o build ./...

run: all
	for i in $(DAYS) ; do \
		build/day$$i ../input$$i.txt ; \
	done

clean:
	rm -rf build
	go clean ./...