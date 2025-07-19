PROGRAMS = varchar-util-test.exe

.SUFFIXES: .pc .exe

all: ${PROGRAMS}

varchar-util-test.exe: varchar-util-test.pc varchar_util.pc varchar-util.h
	./scripts/debug-compile.sh $<

clean:
	rm -f *.exe *.o *.c *.lis *.log
