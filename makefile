CC=gcc
CFLAGS=-Wall
LDFLAGS=-lunwind -lunwind-x86_64 -lunwind-ptrace

INCL= -I ./include -I /usr/include


all:  test_memory test_div_0 test_segfault test_breakpoint breakPoint signaux backtrace memory main_prog 
#Library#

backtrace: ./Src/backtrace.c
	$(CC) -shared -fPIC -o libbacktrace.so $< -ldl $(LDFLAGS)
memory: ./Src/memory.c
	$(CC) -shared -fPIC -o libmemory.so $< -ldl
breakPoint: ./Src/breakPoint.c
	$(CC) -shared -fPIC -o libbreakPoint.so $< -ldl
signaux: ./Src/signaux.c
	$(CC) -shared -fPIC -o libsignaux.so $< -ldl	
#$(CC) $(CFLAGS) $(INCL) ./Src/$@.c -o $@ $(LDFLAGS)
#Tests#
test_memory: ./Tests/test_memory.c
	$(CC) $(CFLAGS) $(INCL) ./Tests/$@.c -o $@
test_div_0: ./Tests/test_div_0.c
	$(CC) $(CFLAGS) $(INCL) ./Tests/$@.c -o $@ 
test_segfault: ./Tests/test_segfault.c
	$(CC) $(CFLAGS) $(INCL) ./Tests/$@.c -o $@ 
test_breakpoint: ./Tests/test_breakpoint.c
	$(CC) $(CFLAGS) $(INCL) ./Tests/$@.c -o $@ 

main_prog: main_prog.c
	$(CC) -I./ -L./ $(CFLAGS) $@.c -o $@ -lmemory -lbacktrace -lbreakPoint -lsignaux $(LDFLAGS) -Wl,-rpath=./ 
run: test_memory 
	@LD_PRELOAD=./libmemory.so ./test_memory
run2: test_div_0 
	./main_prog ./test_div_0
run3: test_segfault 
	./main_prog ./test_segfault
run4: test_breakpoint 
	./main_prog ./test_breakPoint

.PHONY: clean test_memory test_div_0 test_segfault test_breakpoint breakPoint signaux  main_prog backtrace memory
clean:
	rm -Rf *~ *.so *.o test_memory test_div_0 test_segfault test_breakpoint breakPoint signaux  main_prog backtrace memory


