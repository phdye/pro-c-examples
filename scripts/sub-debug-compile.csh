#!/bin/tcsh

# CONNECT=... ./debug-compile.csh 

if ( $#argv > 1) then
    echo "Error:  more than one argument"
    echo "Usage:  ./compile.csh [ <pro-c-file> ]"
    exit 1
endif

if ( $#argv < 1) then
    echo "sub-debug-compile:  missing pro-c file argument"
    echo "Usage:  ./compile.csh [ <pro-c-file> ]"
    exit 1
endif

set pc="$1"

set n = ` echo "$pc" | sed 's/\.pc$//' `
set c = "$n.c"

# set verbose=1

#----------------------------------------------------------------------

# MaxLiteral Too large :  16, 8, 4, 2 K -- only accepts 1024 -- the default
# mode=oracle
set PROC_OPT = "SQLCHECK=SEMANTICS mode=oracle include=."
# set PROC_OPT = "SQLCHECK=none"
# set PROC_OPT = "SQLCHECK=FULL"

# echo ; echo $PATH | tr : '\n' ; echo

echo "+ proc USERID="${CONNECT}" ${PROC_OPT} '$pc' >& proc.log"
proc USERID="${CONNECT}" ${PROC_OPT} "$pc" >& proc.log
if ( $status != 0 ) then
    cat proc.log
    exit 1
endif

#----------------------------------------------------------------------

set ORA_INC = "-I${ORACLE_SDK_INCLUDE}"
if ( $?CYGWIN == 0 ) then
    set ORA_LIB = "-L$ORACLE_HOME_POSIX/lib -lclntsh"
else
    # Oracle Instant Client libs are in ORACLE_HOME
    # Oracle Client & DB libs are in ORACLE_HOME/bin
    set ORA_LIB = "-L$ORACLE_HOME_POSIX -L$ORACLE_HOME_POSIX/bin -loci"
endif

set OPT = "-O2 -std=c99 -D_XOPEN_SOURCE=700"

set FORTIFY = "-D_FORTIFY_SOURCE=2"
# set FORTIFY = ""
set STACK = "-fstack-protector-all"
# set STACK=""
# set MF = "-fmudflap -lmudflap"
set MF=""
set ARRAY = " -Warray-bounds -O2"
# set ARRAY = ""

# Ensuring GLIBC to be able to use MALLOC_CHECK_=3 -- if desired
if ( $?CYGWIN == 0 ) then
    set GLIBC = "-L/lib64 -lc"
else
    # Cygwin does not use glibc in the same way, so we leave it empty
    set GLIBC = ""
endif

set DEBUG="-g ${FORTIFY} ${STACK} ${MF} ${ARRAY} ${GLIBC}"

set WARN  = " -Wall -Wextra"
set IGN   = " -Wno-unused-variable -Wno-unused-parameter -Wno-unused-but-set-variable"
set IGN   = "${IGN} -Wno-parentheses -Wno-implicit-function-declaration -Wno-implicit-int" 

release=$(uname -r | sed -e 's/^\(.\).*/\1/')
if [[ $(uname -o) == "Cygwin" && $release -ge 2 ]] ; then
    set IGN   = "${IGN} -Wno-incompatible-pointer-types"
fi

set DEFER = " -Wno-pointer-sign -Wno-format-extra-args"

# Pro*C generates incomplete field initializers
set PROC  = " -Wno-missing-field-initializers"
# -Wno-return-type

gcc ${OPT} ${DEBUG} ${WARN} ${IGN} ${DEFER} ${PROC} ${ORA_INC} "$c" -o "$n" ${ORA_LIB} ${MF} ${GLIBC} |& tee gcc.log

echo

exit $status
