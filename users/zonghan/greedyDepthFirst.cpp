#include <iostream>
#include <cstring>
#include <vector>

using namespace std;
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

const int MAX_BLOCK_X = 64;
const int MAX_BLOCK_Y = 64;
const int MAX_BLOCK_Z = 64;

const int MAX_BUFFER_SIZE = 128;



struct ParentBlock {
    string label;
    int startX, startY, startZ;
    int sizeX, sizeY, sizeZ;
};


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

        strcpy(tagTable[tagAliasTable[tagIndex]], tagIn);
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

    // cout << "parseHead" << endl;
    parseHead(header);

    // cout << "parseTags" << endl;
    parseTags(tagAliasTable, tagTable, &tagTableSize);

    //define the buffer size
    const int x_count = header[0];
    const int y_count = header[1];
    const int z_count = header[2];

    // We might only want to test MAX_BLOCK_n size blocks not 256 size blocks
    const int parent_x = min(header[3], MAX_BLOCK_X);
    const int parent_y = min(header[4], MAX_BLOCK_Y);
    const int parent_z = min(header[5], MAX_BLOCK_Z);
    // char inBuffer[MAX_BUFFER_SIZE];
    char inBuffer[x_count][y_count][z_count];

    char c;
    // int bufferIncrementor = 0;

    int bufferI_x = 0;
    int bufferI_y = 0;
    int bufferI_z = 0;

    // cout << "write into buffer" << endl;
    while (c != EOF)
    {
        // collect cells and add to buffer until EOF or buffer full
        do {
            c = getchar();
            if(c == EOF) { break; } // end of file, process last buffer then exit
            if(c == '\n') { continue; } // ignore slice and new column indicators
            // inBuffer[bufferIncrementor++] = c;
            inBuffer[bufferI_x][bufferI_y][bufferI_z] = c;
            bufferI_x++;
            if(bufferI_x > x_count-1) { 
                bufferI_y++;
                bufferI_x = 0;
            }
            if(bufferI_y > y_count-1) {
                bufferI_z++;
                bufferI_y = 0;
            }
        } while ( ((bufferI_x+1) * (bufferI_y+1) * (bufferI_z+1)) <= MAX_BUFFER_SIZE);
    }


    //greedy
    // cout << "start greedy" << endl;
    vector<ParentBlock> result;
    ParentBlock mergedBlock;
    int startPosX = 0;
    int startPosY = 0;
    int startPosZ = 0;
    bool stopped;

    //while slice is in range (inBuffer still has unprocessed vexel)
    while(startPosZ < z_count){
        // cout << "[find parent] loop start" << endl;

        //for loop size should be min(parent_size, inbufferSize-startPos)
        int loopX = min(x_count - startPosX, parent_x);
        int loopY = min(y_count - startPosY, parent_y);
        int loopZ = min(z_count - startPosZ, parent_z);
        // cout << "loopX: " << loopX << " loopY: " << loopY << " loopZ: " << loopZ << endl;


        stopped = false;
        // Initialize label to the start vexel color
        //if this vexel is "traveled", go next vexel
        while(inBuffer[startPosX][startPosY][startPosZ] == '\n'){
            startPosX++;
            if(startPosX > x_count-1){
            startPosX = 0;
            startPosY++;
            }
            if(startPosY > y_count-1){
                startPosY = 0;
                startPosZ++;
            }
        }
        mergedBlock.label = tagTable[inBuffer[startPosX][startPosY][startPosZ]];
        mergedBlock.startX = startPosX;
        mergedBlock.startY = startPosY;
        mergedBlock.startZ = startPosZ;
        // cout << "Start from: " << startPosX << " " << startPosY << " " << startPosZ << endl;

        char currentColor = inBuffer[startPosX][startPosY][startPosZ];

        //find the largest ParentBlock
        //this algo is currently depth first, still needs to be optimised
        //currently it will first find the maximum parent block of a certain dimension,
        //and then go to the next dimension
        //The ideal greedy should be breadth first
        //e.g., firstly find out 2*2*2, then 3*3*3, ...
        for (int z = 0; z < loopZ && !stopped; z++) {
            for (int y = 0; y < loopY && !stopped; y++) {
                for (int x = 0; x < loopX && !stopped; x++) {
                    if (inBuffer[startPosX+x][startPosY+y][startPosZ+z] == currentColor) {
                        //renew current parent block size, if the current block size in 3D are all >= the previous found parent block
                        // if(x+1 >= mergedBlock.sizeX && y+1 >= mergedBlock.sizeY && z+1 >= mergedBlock.sizeZ){
                            mergedBlock.sizeX = x+1;
                            mergedBlock.sizeY = y+1;
                            mergedBlock.sizeZ = z+1;
                        // }
                        //mark this vexel as "traveled"
                        inBuffer[startPosX+x][startPosY+y][startPosZ+z] = '\n';
                        // cout << "current label: " << mergedBlock.label << ", with size: " 
                        // << mergedBlock.sizeX << " " << mergedBlock.sizeY << " " << mergedBlock.sizeZ << endl;
                    }else{
                        //locate the next start vexel
                        startPosX += mergedBlock.sizeX;
                        //end this turn of finding ParentBlock
                        stopped = true;
                        // cout << "STOP at size: " << x << " " << y << " " << z 
                        // <<", because " << inBuffer[startPosX][startPosY][startPosZ] << " != " << inBuffer[startPosX+x][startPosY+y][startPosZ+z] << endl;
                    }
                }
            }
        }
        //locate the next start vexel
        if(!stopped){
            startPosX += mergedBlock.sizeX;
            // cout << "FOUND biggest parent block. next posX: " << startPosX << endl;
        }

        //go next y/z if pos > buffer size
        if(startPosX > x_count-1){
            startPosX = 0;
            startPosY++;
            // cout << "Y++ to: " << startPosY << endl;
        }
        if(startPosY > y_count-1){
            startPosY = 0;
            startPosZ++;
            // cout << "Z++ to: " << startPosZ << endl;
        }

        //stop merging if startPosZ is out of bound(no any more slice need to be compressed)
        if (startPosZ > z_count-1) {
            break;
        }
        //put this ParentBlock into result
        result.push_back(mergedBlock);
    }

    //print out result
    // cout << "start print" << endl;
    for(int i=0; i < result.size(); i++){
        cout << result[i].startX << "," << result[i].startY << "," << result[i].startZ << ","
        << result[i].sizeX << "," << result[i].sizeY << "," << result[i].sizeZ << ","
        << result[i].label << endl;
    }

    // print out the buffer
    // for(int i = 0; i < z_count; i++) {
    //     for(int j = 0; j < y_count; j++) {
    //         for(int k = 0; k < x_count; k++) {
    //             cout << k << "," << j << "," << i << ",1,1,1," << tagTable[inBuffer[i][j][k]] << endl;
    //         }
    //     }
    // }
    
    return 0;
}