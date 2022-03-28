CC=gcc
CFLAGS=-Wall
LDFLAGS=-lunwind -lunwind-x86_64 -lunwind-ptrace
CFLAGSs?=-g -O0
INCL= -I ./include -I /usr/include


all:  test_memory test_div_0 test_segfault test_breakpoint test_stack test_mem test_dlk test_rogue test_rogue_sp test_voidp backtrace breakPoint signaux memory main_prog main

#Library#

backtrace.o: ./Src/backtrace.c
	$(CC) $(CFLAGS) -c ./Src/backtrace.c $(LDFLAGS)
memory: ./Src/memory.c
	$(CC) -shared -fPIC -o libmemory.so $< -ldl
breakPoint.o: ./Src/breakPoint.c
	$(CC) $(CFLAGS) -c ./Src/breakPoint.c
signaux.o: ./Src/signaux.c
	$(CC) $(CFLAGS) -c ./Src/signaux.c	

#Tests#
test_memory: ./Tests/test_memory.c
	$(CC) $(CFLAGS) ./Tests/$@.c -o $@
test_div_0: ./Tests/test_div_0.c
	$(CC) $(CFLAGS) $(INCL) ./Tests/$@.c -o $@ 
test_segfault: ./Tests/test_segfault.c
	$(CC) $(CFLAGS) $(INCL) ./Tests/$@.c -o $@ 
test_breakpoint: ./Tests/test_breakpoint.c
	$(CC) $(CFLAGS) $(INCL) ./Tests/$@.c -o $@ 
test_stack: ./Tests/test_stack.c
	$(CC) $(CFLAGS) $(INCL) ./Tests/$@.c -o $@
test_mem: ./Tests/test_mem.c
	$(CC) $(CFLAGS) $(INCL) ./Tests/$@.c -o $@ 
test_dlk: ./Tests/test_dlk.c
	$(CC) $(CFLAGS) $(INCL) ./Tests/$@.c -o $@  
test_rogue: ./Tests/test_rogue.c
	$(CC) $(CFLAGS) $(INCL) ./Tests/$@.c -o $@ 
test_rogue_sp: ./Tests/test_rogue_sp.c
	$(CC) $(CFLAGS) $(INCL) ./Tests/$@.c -o $@
test_voidp: ./Tests/test_voidp.c
	$(CC) $(CFLAGS) $(INCL) ./Tests/$@.c -o $@
main_prog.o: main_prog.c 
	$(CC) -I./ -L./ $(CFLAGS)  -c main_prog.c -lmemory -Wl,-rpath=./ $(LDFLAGS) 


#execution
main : main_prog.o backtrace.o breakPoint.o signaux.o
	$(CC) -I./ -L./ $(CFLAGS) -o $@ main_prog.o backtrace.o breakPoint.o signaux.o -lmemory -Wl,-rpath=./ $(LDFLAGS) 
run1: test_memory 
	@LD_PRELOAD=./libmemory.so ./main ./test_memory
run: test_memory 
	@LD_PRELOAD=./libmemory.so ./test_memory
run2: test_div_0 
	./main ./test_div_0
run_test: test_div_0
	./test_div_0
run3: test_segfault 
	./main ./test_segfault
run4: test_breakpoint 
	./main ./test_breakPoint
run5: test_mem 
	@LD_PRELOAD=./libmemory.so ./test_mem
run6: test_stack 
	./main ./test_stack

run7: test_dlk 
	./main ./test_dlk
run8: test_rogue 
	./main ./test_rogue
run9: test_rogue_sp 
	./main ./test_rogue_sp
run10: test_voidp 
	./main ./test_voidp
.PHONY: clean test_memory test_div_0 test_segfault test_breakpoint test_stack test_mem test_dlk test_rogue test_rogue_sp test_voidp breakPoint signaux  main_prog backtrace memory

clean:
	rm -Rf *~ *.so *.o test_memory test_div_0 test_segfault test_breakpoint breakPoint signaux main_prog backtrace memory main test_stack test_mem test_dlk test_rogue test_rogue_sp test_voidp


