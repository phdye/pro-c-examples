#!/bin/bash

# Symlink 'scripts' in each build directory

# oracle-env.sh permits selection of various Oracle Client installations but
# one must compile and execute a given program with the same Oracle Client
# details in one's environment.

# Use the same oracle env setup for my interactive bash
source ./scripts/oracle-env.sh

# Then call the c-shell compile script needed for this shop
./scripts/sub-debug-compile.csh "$@"
