disassembler: main.o
	cc -o disassembler main.o

main.o : main.c
	cc -c main.c

clean :
	rm disassembler main.o
