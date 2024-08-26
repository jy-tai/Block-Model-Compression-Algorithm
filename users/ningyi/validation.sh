#!/bin/bash

for i in $(ls ../cases/)
	#run program and get output
	./test input.dat output.dat > result

	#compare result to the original input.dat
	diff input.dat result

	#remove result file
	rm -f result