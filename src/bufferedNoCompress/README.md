## Buffered No-Compression Algorithm - Detailed Documentation

The Buffered No-Compression Algorithm is a basic algorithm that takes input in the correct format and outputs it in the same format without any compression.

1. Initialisation: The algorithm starts by parsing the header and tags from the input data. This is done in the "parseHead" and "parseTags" functions. The "parseHead" function reads the header information, which includes the dimensions of the block model and the maximum block size. The "parseTags" function reads the tag table, which maps each character tag to a label.

2. Reading Block Model Data: The algorithm then reads the block model data from the input stream into a buffer. The size of the buffer is defined by "MAX_BUFFER_SIZE". This is done in a loop that continues until it reaches the end of the input stream.

3. Output: For each block in the buffer, it outputs the block's position, size (which is always 1x1x1 since there's no compression), and label. This is done in a loop that iterates over each block in the buffer.

4. Continuation: The algorithm continues to read blocks into the buffer and output them until it reaches the end of the input stream.

This algorithm is straightforward and efficient, but it doesn't provide any compression. It's useful for testing and debugging, as well as for providing a performance baseline.

The code implementation of this algorithm involves several key functions and data structures:

- A 3D array (inBuffer) for storing the block model data.
- Several helper functions (parseHead and parseTags) for parsing input data.
- A loop structure that iterates over each block in the buffer.
- An output statement that outputs each block with its position, size, and label.