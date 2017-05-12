#!/bin/bash

cd $SCRATCH

execdir="/home/vasua/418FinalProject/bin"
exe="lockfree"

cp ${execdir}/${exe} ${exe}

./${exe}
