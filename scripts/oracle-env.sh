#!/bin/false

# Requires that ORACLE_HOME is set to the Oracle client installation directory windows path.
# Example: export ORACLE_HOME="C:\\Oracle\\instantclient_19_8"
#
# TNS_ADMIN can also be set to the directory containing tnsnames.ora.

export ORACLE_HOME_POSIX=$(cygpath -u -a "${ORACLE_HOME}")

# Determine if ORACLE_HOME is a traditional Oracle client or Instant Client
# and set the ORACLE_SDK_INCLUDE accordingly.
if [ -d "${ORACLE_HOME_POSIX}/sdk/include" ]; then
    export ORACLE_SDK_INCLUDE="${ORACLE_HOME_POSIX}/sdk/include"
elif [ -d "${ORACLE_HOME_POSIX}/precomp/public" ]; then
    export ORACLE_SDK_INCLUDE="${ORACLE_HOME_POSIX}/precomp/public"
else
    echo "Warning: ORACLE_HOME does not contain expected SDK include directories: ${ORACLE_HOME_POSIX}"
    export ORACLE_SDK_INCLUDE=""
fi

export PATH="$(cygpath -u -a ${ORACLE_HOME_POSIX})/bin:$PATH"

hash -r

#
