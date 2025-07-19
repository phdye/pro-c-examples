# Paths
OCI_LIB := /d/app/oracle/db/23-6/dbhomeFree/oci/lib/msvc/oci.lib
OCI_DEF := build/oci.def
DLLTOOL := dlltool

# Generate oci.def from oci.lib
$(OCI_DEF): $(OCI_LIB)
	@mkdir -p $(dir $@)
	$(DLLTOOL) -z $@ -k -a $<

# Compile and link Pro*C example
dbms-output/dbms-output: dbms-output/dbms-output.pc.o
	gcc -I$(ORACLE_HOME)/rdbms/public \
	    -I$(ORACLE_HOME)/precomp/public \
	    -L$(ORACLE_HOME)/lib \
	    -o $@ $^ $(OCI_LIB)

dbms-output/dbms-output.pc.o: dbms-output/dbms-output.pc
	proc iname=$< sqlcheck=semantics userid=/@free1
	gcc -c dbms-output/dbms-output.c -o $@ \
	    -I$(ORACLE_HOME)/rdbms/public -I$(ORACLE_HOME)/precomp/public

clean:
	rm -f dbms-output/dbms-output dbms-output/*.o dbms-output/*.lis dbms-output/*.c $(OCI_DEF)
