## Divide and Conquer Algorithm - Detailed Documentation

The Divide and Conquer Algorithm is a recursive algorithm that uses a divide-and-conquer approach to compress 3D block models.

1. Initialisation: The algorithm starts by parsing the header and tags from the input data. This is done in the "parseHead" and "parseTags" functions. The "parseHead" function reads the header information, which includes the dimensions of the block model and the maximum block size. The "parseTags" function reads the tag table, which maps each character tag to a label.

2. Recursive Division: The algorithm recursively divides the block model into smaller sections. This is done in the "partitionBlock" function, which checks the dimensions of the block model and divides it into eight smaller volumes if possible. The function calls itself recursively for each of these smaller volumes.

3. Compression: For each section, the algorithm checks if all blocks within the section have the same label. If they do, it compresses them into one block. This is done by checking if the volume of the section is 1 (i.e., it's a single block) or if all blocks within a larger volume have the same label. If all blocks within a section have the same label, it outputs this block with its label.

4. Output: If a section can be compressed, the algorithm outputs this block with its label. If a section can't be compressed, it continues to divide it into smaller sections.

5. Recursion: The algorithm continues to divide and conquer until it has compressed all blocks.

This algorithm is efficient and effective for compressing 3D block models, especially when there are large volumes of blocks with the same label. However, its performance can vary depending on the distribution of labels in the input data.

The code implementation of this algorithm involves several key functions and data structures:

- A 3D array (block_map) for storing the block model data.
- A recursive function (partitionBlock) for dividing and conquering.
- Several helper functions (parseHead and parseTags) for parsing input data and checking conditions.
- A loop structure that iterates over each dimension (x, y, z) in turn.
- Conditional statements that check whether all blocks within a section have the same label or whether a section can be compressed.
- An output statement that outputs each compressed block with its position, size, and label.