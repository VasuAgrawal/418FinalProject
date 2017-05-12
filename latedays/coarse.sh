#!/bin/bash

cd $SCRATCH

execdir="/home/vasua/418FinalProject/bin"
exe="coarse"

cp ${execdir}/${exe} ${exe}

./${exe}
