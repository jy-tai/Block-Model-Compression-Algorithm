## Code Overview
This C++ program is designed to find and identify distinct shapes in a 3-dimensional map. 
The shapes are represented by characters and the map is implemented as a 3D vector of characters.
The program uses the flood fill algorithm to explore each shape in the map.

### Functions

- floodFill(): This function implements the flood fill algorithm. It takes as parameters a 3D map, a 3D visited array, the coordinates (x, y, z) of the current cell in the map, and the character currentChar representing the current shape being explored. 
The function marks the current cell as visited and recursively explores its neighbors.

- findShapes(): This function iterates over each cell in the 3D map and performs a flood fill for each unvisited cell. It uses OpenMP to parallelise this process.

- main(): This is the main function where the program execution begins. It tests the findShapes function with a test map.

## Makefile

- compile: This rule compiles the program. (make compile)
- runCode: This rule runs the compiled program. (make runCode)
