PROC = proc
CC = gcc
CFLAGS = -Wall -O2

PROGRAMS = dbms-output.exe dbms-output-usage.exe varchar-util-test.exe

.SUFFIXES: .pc .exe

.pc.exe:
	./scripts/debug-compile.sh $<

all: ${PROGRAMS}

# varchar-util.c: varchar-util.pc
# 	$(PROC) iname=$< cpp_suffix=c sqlcheck=semantics

# varchar-util-test.c: varchar-util-test.pc
# 	$(PROC) iname=$< cpp_suffix=c sqlcheck=semantics

# varchar-util-test: varchar-util.o varchar-util-test.o
# 	# $(CC) $(CFLAGS) -o $@ $^

# varchar-util-test: varchar_util.pc varchar-util-test.pc
# 	./scripts/debug-compile.sh varchar-util-test.pc

clean:
	rm -f *.exe *.o *.c *.lis *.log
