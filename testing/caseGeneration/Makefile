test-case: compile
	./bin/out ../cases/case.tmp < ../caseDefinitions/in.test && \
	python3 ./visualisation/DrawCaseFile.py ../cases/case.tmp && \
	rm ../cases/case.tmp


# creates a test case for every definition file in the 'caseDefinition' folder
build: compile
	./buildCases.sh


compile:
	mkdir -p ./bin
	g++ ./src/PerlinNoise.hpp ./src/GenCase.cpp -o ./bin/out


install:
	pip install -r requirements.txt


build-dependencies:
	pipreqs --force && \
	echo "PyQt5==5.15.9" >> requirements.txt && \
	echo "PyQt5-Qt5==5.15.2" >> requirements.txt && \
	echo "PyQt5-sip==12.12.2" >> requirements.txt

clean:
	rm ./out.dat
	rm ./bin/*