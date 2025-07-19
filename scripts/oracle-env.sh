#!/bin/false

export TNS_ADMIN="c:\\Oracle\\network\\admin\\tnsnames.ora"

if [ 0 -eq 0 ] ; then
    export ORACLE_HOME="c:\\Oracle\\Ora12_64\\client"
    # export ORACLE_HOME="c:\\Oracle\\Ora19_64\\client"
    export ORACLE_SDK_INCLUDE="${ORACLE_HOME}\\precomp\\public"
else
    # Use Oracle Instant Client
    # ic=18_5
    ic=19_26
    # # ic=21_5
    # ic=23_8

    export ORACLE_HOME="c:\\Oracle\\instantclient_${ic}"
    export ORACLE_SDK_INCLUDE="${ORACLE_HOME}\\sdk\\include"
fi

export PATH="$(cygpath -u -a ${ORACLE_HOME})/bin:$PATH"

hash -r

#
