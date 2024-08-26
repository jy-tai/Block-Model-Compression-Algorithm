The makefile use uname command to detect the operating system. Based on the detected OS to sets the approprate compiler ('g++` for Linux and Mac, `x86_64-w64-mingw32-g++` for Windows) and compiler flags.
Set the compiler flags 'CXXFLAGS', and '--static', -'fopenmp' are addded for Windows.

# Requirement
run 'make install' to installs required dependencies speficied in 'requirements.txt'

# How to run algorithm
run 'make all' tp compiles all source files into executables and organizes them into appropriate directories based on the source file locations.
run 'make compile' to iterates through directories in the src folder, creates corresponding bin directores and compile the c++ files. 

# Code linting
run 'make py-lint'/'make c-lint' to run lining scriptes for python and C++ respectively

# testing
run 'make test' to run testings

# validation
run 'make validate' to run algorithm validation

# Remove
run 'make clean' to removes compiled files and directories
