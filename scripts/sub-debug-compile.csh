#!/bin/tcsh

# CONNECT=... ./debug-compile.csh 

if ( $#argv > 1) then
    echo "Error:  more than one argument"
    echo "Usage:  ./compile.csh [ <pro-c-file> ]"
    exit 1
endif

if ( $#argv < 1) then
    set pc=GL.pc
else
    set pc="$1"
endif

set n = ` echo "$pc" | sed 's/\.pc$//' `
set c = "$n.c"

set verbose=1

#----------------------------------------------------------------------

# MaxLiteral Too large :  16, 8, 4, 2 K -- only accepts 1024 -- the default
# mode=oracle
set PROC_OPT = "SQLCHECK=SEMANTICS mode=oracle"
# set PROC_OPT = "SQLCHECK=none"
# set PROC_OPT = "SQLCHECK=FULL"

echo ; echo $PATH | tr : '\n' ; echo

proc USERID="${CONNECT}" ${PROC_OPT} "$pc" |& tee proc.log
if ( $status != 0 ) exit 1

#----------------------------------------------------------------------

set ORA_INC = "-I$ORACLE_HOME/precomp/public -I${ORACLE_SDK_INCLUDE}"
if ( $?CYGWIN == 0 ) then
    set ORA_LIB = "-L$ORACLE_HOME/lib -lclntsh"
else
    # Oracle Instant Client libs are in ORACLE_HOME
    # Oracle Client & DB libs are in ORACLE_HOME/bin
    set ORA_LIB = "-L$ORACLE_HOME -L$ORACLE_HOME/bin -loci"
endif

set OPT = "-D_FORTIFY_SOURCE=2 -O2"
# set OPT="-O2"
set STACK = "-fstack-protector-all"
# set STACK=""
# set MF = "-fmudflap -lmudflap"
set MF=""
set ARRAY = " -Warray-bounds -O2"
# set ARRAY = ""

# Ensuring GLIBC to be able to use MALLOC_CHECK_=3 -- if desired
set GLIBC = "-L/lib64 -lc"
# set GLIBC=""

set DEBUG="${OPT} ${STACK} ${MF} ${ARRAY} ${GLIBC}"

set WARN  = " -Wall -Wextra"
set IGN1  = " -Wno-unused-variable -Wno-unused-parameter -Wno-unused-but-set-variable"
set IGN2  = " -Wno-parentheses -Wno-implicit-function-declaration -Wno-implicit-int" 
set DEFER = " -Wno-pointer-sign -Wno-format-extra-args"

# Pro*C generates incomplete field initializers
set PROC = " -Wno-missing-field-initializers"
# -Wno-return-type

gcc -g ${DEBUG} ${WARN} ${IGN1} ${IGN2} ${DEFER} ${PROC} ${ORA_INC} "$c" -o "$n" ${ORA_LIB} ${MF} ${GLIBC} |& tee gcc.log

exit $status
