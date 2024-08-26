# DisSys
SEP project repository
Authors: 
- a1795819 Aidan Carling 
- a1836176 Jian Yi Tai 
- a1803759 Joshua Campbell 
- a1834181 Keshav Rana
- a1828091 Ningyi Shen
- a1809232 Zonghan Liu
- a1837100 Ruobing Wu



## File structure
### bin
Where execultable binaries are stored

### src
Source files for each program
1 subfolder for each program

### common
Source files used across projects

### users
Unstructured file system for each group member. Contains internal notes and testing.

### testcases
Collection of test inputs for programs

### testgeneration
Files for generating testcases

### obj
Intermediate folder for compiler object generation

### Makefile
run "Make all" to compile all programs

## Project Documentation

### Project Overview
The project constitutes a structured and systematic approach towards the design and development of a software solution. The primary objective is to engineer a program capable of processing uncompressed 3D image data and subsequently outputting the corresponding compressed data. The performance metrics for the program are twofold: the speed of processing and the efficiency of compression.

### Requirements
The program is expected to read uncompressed block model data from a standard input text stream. The block model data will adhere to a specific format, inclusive of a tag table that outlines all characters that will be encountered in the remainder of the stream. Subsequently, the program should compress this data, utilising a specified parent block size for compression.

### Algorithm Development
This project involved the development of two primary algorithms, and a basic no-compression algorithm, all developed using C++.

1. Divide and Conquer Algorithm: This algorithm uses a divide and conquer approach to compress 3D block models. It recursively divides the block model into smaller sections until it finds sections that can be compressed. If all blocks within a section have the same label, they are compressed into one block.

2. Greedy Compression Algorithm: This algorithm uses a greedy approach to compress 3D block models by merging adjacent blocks with identical labels. It iteratively checks each dimension (x, y, z) for possible merges and switches dimensions when no further merges are possible. The algorithm continues until all blocks are processed or until a block that can’t be merged in any dimension is encountered. After all blocks have been processed, it adds each merged block to a list of results and then outputs each block in the results list with its position, size, and label.

3. Buffered No-compression algorithm: This basic algorithm takes input in the correct format and outputs it in the same format without any compression. It serves as a baseline for comparing the performance of the other two algorithms.

Detailed documentation for each algorithm is provided separately, explaining their logic and implementation in depth.

### Testing
The project employs a comprehensive testing strategy. A shell script, validation.sh, is used to automate the testing process. It iterates over all test cases and executes each compression algorithm, comparing the output with the expected result.

The testing directory contains four subdirectories:
case definitions: Contains descriptions of test cases.
case generation: Houses a test case generator that creates realistic test scenarios based on provided case definitions.
cases: Stores the input files for test cases.
harness: Contains a test harness that simplifies running and writing tests. It takes the program’s output, decompresses it, and compares it to the input file.

This structured approach to testing ensures the validity and reliability of the developed algorithms.

### Project Management
The project was managed using Agile methodology, which allowed for iterative development and regular feedback. This approach ensured that the project could adapt to changes quickly and efficiently. Trello, a web-based Kanban-style list-making application, was used as the project management tool. Trello facilitated the organization of tasks, tracking of progress, and collaboration among team members. The combination of Agile methodology and Trello enabled effective project management, promoting productivity and ensuring timely completion of tasks.
