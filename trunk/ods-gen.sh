#!/bin/bash

if [ $# -ne 2 ]; then
    echo "usage: $0 input.xml output.ods"
    exit
fi

python `dirname $0`/ods-gen.py $1 $2
