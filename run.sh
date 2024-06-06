#!/bin/bash
make
if (( $? != 0 )); then
    exit $?
fi
desmume $(basename $(pwd)).nds
