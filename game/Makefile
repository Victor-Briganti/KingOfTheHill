.PHONY: all start build clean

GDK := "/home/nessuno/Documents/UTFPR/Jogos/sgdk/sgdk"
GENS := "/home/nessuno/Documents/UTFPR/Jogos/sgdk/gens/gens.exe"
CWD := $(shell pwd)
JOBS ?= 4

all: build

bear: clean
	bear -- make -j$(JOBS) GDK=$(GDK) -f $(GDK)/makefile_wine.gen 2> >(grep -v "wrong ELF class: ELFCLASS64" >&2)
	@sed -i '/m68000/d' compile_commands.json

build: clean
	make -j$(JOBS) GDK=$(GDK) -f $(GDK)/makefile_wine.gen

run:
	@wine $(GENS) $(CWD)/out/rom.bin

start: build run

start_full: clean start

clean:
	make GDK=$(GDK) -f $(GDK)/makefile_wine.gen clean
