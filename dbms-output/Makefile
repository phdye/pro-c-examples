PROGRAMS = dbms-output.exe dbms-output-usage.exe

.SUFFIXES: .pc .exe

all: ${PROGRAMS}

dbms-output.exe : dbms-output.pc
	./scripts/debug-compile.sh $<

dbms-output-usage.exe : dbms-output-usage.pc dbms_output_util.pc
	./scripts/debug-compile.sh $<

clean:
	rm -f *.exe *.o *.c *.lis *.log
