CXX=x86_64-w64-mingw32-g++
CXXFLAGS=-g -Wall -Wextra -std=c++17 -m64 -O2 --static


runGDF: compileGDF
	./a.out < test > output

compileGDF:
	g++ greedyDepthFirst.cpp


runGBF: compileGBF
	./a.out < test > output

compileGBF:
	g++ greedyBreadthFirst.cpp


packageGDF:
	${CXX} ${CXXFLAGS} greedyDepthFirst.cpp -o ZonghanGDF.exe

packageGBF:
	$(CXX) ${CXXFLAGS} greedyBreadthFirst.cpp -o ZonghanGBF.exe


testIntroGBF: packageGBF
	./ZonghanGBF.exe < the_intro_one_32768_4x4x4.csv > output

testFastGBF: packageGBF
	./ZonghanGBF.exe < the_fast_one_376000_2x2x2.csv > output

testCombiGBF: packageGBF
	./ZonghanGBF.exe < the_combinatorial_one_42000000_14x10x12.csv > output

# packageGBF:
# 	g++ -o Zonghan.exe greedyBreadthFirst.cpp -static-libgcc -static-libstdc++ -m64 --static

#https://titan.maptek.net/Dashboard [password: country7modernyes]