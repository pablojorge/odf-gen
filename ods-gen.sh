#!/bin/bash

if [ $# -ne 2 ]; then
    echo "usage: $0 input.xml output.ods"
    exit
fi

python ods-gen.py $1 $2
