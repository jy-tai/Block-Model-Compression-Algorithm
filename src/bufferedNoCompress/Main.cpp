#include <iostream>
#include <cstring>

// The first line contains 6 comma separated natural numbers:
// x_count, y_count, z_count, parent_x, parent_y, parent_z

// next n lines (up to 256 is tag table) 
/*
    <tag>, <tag name>
    ...
    <blank line>
*/

// blocks are streamed

/*
start of file
    64,2,2,2,2,2
    o, sea
    t, TAZ
    
    ooooooooooooooooooooooooooooooooooooooooooooooooottooooooooooooo
    oooooooooooooooooooooooooooooooooooooooooooooooootttoooooooooooo
    
    ooooooooooooooooooooooooooooooooooooooooooooooooottttooooooooooo
    ooooooooooooooooooooooooooooooooooooooooooooooootttttooooooooooo
eof
*/

/*
The output format contains one line per block and comprises seven comma separated values:
x_position, y_position, z_position, x_size, y_size, z_size, label
*/
#define MAX_TAGS 256
#define MAX_TAG_LENGTH 256

const int MAX_BLOCK_X = 4;
const int MAX_BLOCK_Y = 4;
const int MAX_BLOCK_Z = 4;

const int MAX_BUFFER_SIZE = 128;



int parseHead(int* header) {
    char inbuf[16];
    int i = 0;
    int headIndex = 0;
    char c;
    do
    {
        c = getchar();
        if(c == ',' || c == '\n') {
            inbuf[i] = '\0';
            sscanf(inbuf, "%d", &(header[headIndex++])); 
            i = 0;
            continue;
        }
        inbuf[i] = c;
        i++;
    } while(c != '\n');
    return 0;
}

int parseTags(char* tagAliasTable, char tagTable[MAX_TAGS][MAX_TAG_LENGTH], int* tagTableLength) {
    int tagIndex = 0;


    char tagAliasIn;
    char tagIn[256];


    while(tagIndex < MAX_TAGS) {
        int i = 0;
        char c;

        c = getchar();
        if(c == '\n') {
            *tagTableLength = tagIndex; 
            return 0;
        }
        tagAliasTable[tagIndex] = c;

        do
        {
            c = getchar();         
            if(c == ',' || c == ' ') {
                continue;
            }
            if(c == '\n') {
                tagIn[i++] = '\0';
                continue;
            }
            tagIn[i] = c;
            i++;
        } while(c != '\n');

        std::strcpy(tagTable[tagAliasTable[tagIndex]], tagIn);
        tagIndex++;
    }
    return 0;
}





int main() {
    int header[6];

    // DEPENDING ON WHAT FORMAT TAGS ARE IN CAN BUILD TABLE LIKE THIS ELSE NEED TO USE HASHMAP
    char tagAliasTable[MAX_TAGS];
    char tagTable[MAX_TAGS][MAX_TAG_LENGTH];
    int tagTableSize = 0;


    if(parseHead(header) != 0) {
        std::cout << "something broke parsing the header" << std::endl;
        return 1;
    }
    if(parseTags(tagAliasTable, tagTable, &tagTableSize) != 0) {
        std::cout << "something broke parsing the header" << std::endl;
        return 1;
    }

    const int x_count = header[0];
    const int y_count = header[1];
    const int z_count = header[2];

    
    // We might only want to test MAX_BLOCK_n size blocks not 256 size blocks
    const int parent_x = std::min(header[3], MAX_BLOCK_X);
    const int parent_y = std::min(header[4], MAX_BLOCK_Y);
    const int parent_z = std::min(header[5], MAX_BLOCK_Z);

    char inBuffer[MAX_BUFFER_SIZE];




    // for(int i = 0; i < 6; i++) {
    //     std::cout << header[i] << std::endl;
    // }
    // for(int i = 0; i < tagTableSize; i++) {
    //     std::cout << tagAliasTable[i] << " " << tagTable[tagAliasTable[i]] << std::endl;
    // }


    char c;
    int bufferIncrementor = 0;

    int x_pos = 0;
    int y_pos = 0;
    int z_pos = 0;
    while (c != EOF)
    {
        // collect cells and add to buffer until EOF or buffer full
        do {
            c = getchar();
            if(c == EOF) { break; } // end of file, process last buffer then exit
            if(c == '\n') { continue; } // ignore slice and new column indicators
            inBuffer[bufferIncrementor++] = c;
        } while (bufferIncrementor < MAX_BUFFER_SIZE);


        // for each cell in buffer
        for(int i = 0; i < bufferIncrementor; i++) {
            std::cout << x_pos << "," << y_pos << "," << z_pos << ",1,1,1," << tagTable[inBuffer[i]] << std::endl;



            // increment xyz position
            x_pos++;
            if(x_pos > x_count-1) { 
                y_pos++;
                x_pos = 0;
            }
            if(y_pos > y_count-1) {
                z_pos++;
                y_pos = 0;
            }
        }
        bufferIncrementor = 0;
    }

    return 0;
}
 