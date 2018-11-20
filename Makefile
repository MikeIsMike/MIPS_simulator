all: main.o instruction_api.o memory.o
	g++ main.o instruction_api.o memory.o -o simulator

main.o: main.cpp
	g++ -std=c++11 -c main.cpp
	
instruction_api.o: instruction_api.cpp instruction_api.hpp
	g++ -std=c++11 -c instruction_api.cpp

memory.o: memory.cpp memory.hpp
	g++ -std=c++11 -c memory.cpp
	
run:
	./simulator ../../../../Downloads/addiu.bin
	
clean:
	rm *.o simulator
