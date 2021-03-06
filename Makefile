# Example usage:
# If you have testbench/my_test.c, you can execute the following to create a MIPS binary
# make testbench/my_test.mips.bin

# For simulator
CC = g++
CPPFLAGS = -W -Wall

# For MIPS binaries. Turn on all warnings, enable all optimisations and link everything statically
MIPS_CC = mips-linux-gnu-gcc
MIPS_OBJCOPY = mips-linux-gnu-objcopy
MIPS_OBJDUMP = mips-linux-gnu-objdump
MIPS_CPPFLAGS = -W -Wall -O3 -fno-builtin -march=mips1 -mfp32
MIPS_LDFLAGS = -nostdlib -Wl,-melf32btsmip -march=mips1 -nostartfiles -mno-check-zero-division -Wl,--gpsize=0 -static -Wl,-Bstatic -Wl,--build-id=none

# Compile C file (.c) into MIPS object file (.o)
%.mips.o: %.c
	$(MIPS_CC) $(MIPS_CPPFLAGS) -c $< -o $@

# Assemble MIPS assembly file (.s) into MIPS object file (.o)
%.mips.o: %.s
	$(MIPS_CC) $(MIPS_CPPFLAGS) -c $< -o $@

# Link MIPS object file (.o), producing .elf, using memory locations specified in spec
%.mips.elf: %.mips.o
	$(MIPS_CC) $(MIPS_CPPFLAGS) $(MIPS_LDFLAGS) -T linker.ld $< -o $@

# Extract binary instructions only from linked object file (.elf)
%.mips.bin: %.mips.elf
	$(MIPS_OBJCOPY) -O binary --only-section=.text $< $@

# Disassemble linked object file (.elf), pulling out instructions as MIPS assembly file (.s)
%.mips.s : %.mips.elf
	$(MIPS_OBJDUMP) -j .text -D $< > $@

# Build simulator
# bin/mips_simulator: src/simulator.cpp
# 	mkdir -p bin
# 	$(CC) $(CPPFLAGS) src/simulator.cpp -o bin/mips_simulator

# Dummy for build simulator to conform to spec
simulator: simulator.o instruction_api.o memory.o
	mkdir -p bin
	rm -f ./bin/mips_simulator
	$(CC) $(CPPFLAGS) ./src/simulator.o ./src/instruction_api.o ./src/memory.o -o ./bin/mips_simulator

simulator.o: ./src/simulator.cpp
	$(CC) $(CPPFLAGS) -std=c++11 -c ./src/simulator.cpp -o ./src/simulator.o

instruction_api.o: ./src/instruction_api.cpp ./src/instruction_api.hpp
	$(CC) $(CPPFLAGS) -std=c++11 -c ./src/instruction_api.cpp -o ./src/instruction_api.o

memory.o: ./src/memory.cpp ./src/memory.hpp
	$(CC) $(CPPFLAGS) -std=c++11 -c ./src/memory.cpp -o ./src/memory.o

# Dummy for build testbench to conform to spec. Could do nothing
testbench:
	chmod u+x bin/mips_testbench
	@echo "Nothing else to do"



clean:
	rm *.o ./bin/mips_simulator ./src/*.o
