#!/bin/bash

if [ $# -eq 0 ]; then
    echo "Please input either 'py' or 'cpp' as a command line parameter" 1>&2;
    exit -1
elif [ "$1" = "py" ]; then
    echo "python linting"
    if pip freeze | grep -q flake8; then
        python3 -m flake8 . --select=E9,E7,F63,F7,F82 --show-source && 
        python3 -m flake8 . --exit-zero --max-complexity=10 --max-line-length=127
    else
        echo "install requirements 'make install'" 1>&2;
    fi
elif [ "$1" = "cpp" ]; then
    echo "c++ Linting"
	find . -name '*.cpp' | vera++
else
    echo "Please input either 'py' or 'cpp' as a command line parameter" 1>&2;
    exit -1
fi
