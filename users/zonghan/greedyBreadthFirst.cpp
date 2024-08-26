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
const int MAX_BLOCK_Z = 8;

const int MAX_BUFFER_SIZE = 32768;



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


    // char tagAliasIn;
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

        strcpy(tagTable[tagIndex], tagIn);
        tagIndex++;
    }
    return 0;
}

char updateMergeDimension(char merge_dimension, bool hit_x, bool hit_y, bool hit_z){
    //travel x,y,z dimension, find the first unhit dimension and return it
    switch(merge_dimension){
        case 'x':
            merge_dimension = 'y';
            break;
        case 'y':
            merge_dimension = 'z';
            break;
        case 'z':
            merge_dimension = 'x';
            break;
    }

    for(int i=0; i<3; i++){
        switch(merge_dimension){
            case 'x':
                if(hit_x){
                    merge_dimension = 'y';
                }else{
                    return merge_dimension;
                }
                break;
            case 'y':
                if(hit_y){
                    merge_dimension = 'z';
                }else{
                    return merge_dimension;
                }
                break;
            case 'z':
                if(hit_z){
                    merge_dimension = 'x';
                }else{
                    return merge_dimension;
                }
                break;
        }
    }
    //which means all dimensions hit
    return 'n';
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
            // cout << static_cast<int>(c) << "-";
            // cout << c;
            if(c == EOF) { break; } // end of file, process last buffer then exit
            // if(c == '\r'){ continue; }
            if(c == '\n') {
                // cout << "line switch" << endl;
                continue;
            } // ignore slice and new column indicators
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

    // print out the buffer
    // // cout << "print out the buffer" << endl;
    // for(int i = 0; i < z_count; i++) {
    //     // cout << "slice: " << i << endl;
    //     for(int j = 0; j < y_count; j++) {
    //         for(int k = 0; k < x_count; k++) {
    //             string tag = "?";
    //             for(int l=0; l<tagTableSize; l++){
    //                 if(tagAliasTable[l] == inBuffer[k][j][i]){
    //                     tag = tagTable[l];
    //                 }
    //             }
    //             cout << k << "," << j << "," << i << ",1,1,1," << tag << endl;
    //             // cout << inBuffer[k][j][i];
    //         }
    //     }
    // }
    // // cout << endl;


    //greedy
    // cout << "start greedy" << endl;
    vector<ParentBlock> result;
    ParentBlock mergedBlock;
    int startPosX = 0;
    int startPosY = 0;
    int startPosZ = 0;


    //while slice is in range (inBuffer still has unprocessed vexel)
    while(startPosZ < z_count){
        // cout << "[find parent] loop start" << endl;

        // Initialize label to the start vexel color
        //if this vexel is "traveled", go next vexel
        while(inBuffer[startPosX][startPosY][startPosZ] == '?'){
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

        mergedBlock.label = "!";
        for(int i=0; i<tagTableSize; i++){
            if(tagAliasTable[i] == inBuffer[startPosX][startPosY][startPosZ]){
                mergedBlock.label = tagTable[i];
            }
        }
        mergedBlock.startX = startPosX;
        mergedBlock.startY = startPosY;
        mergedBlock.startZ = startPosZ;
        mergedBlock.sizeX = 1;
        mergedBlock.sizeY = 1;
        mergedBlock.sizeZ = 1;


        char currentTag = inBuffer[startPosX][startPosY][startPosZ];
        inBuffer[startPosX][startPosY][startPosZ] = '?';
    
        int loopStartPosX = startPosX, loopStartPosY = startPosY, loopStartPosZ = startPosZ;
        int loopEndPosX = startPosX, loopEndPosY = startPosY, loopEndPosZ = startPosZ;
        bool hit_all = false, hit_x = false, hit_y = false, hit_z = false;
        char merge_dimension = 'x';

        // cout << "Start merging tag " << currentTag << " from " << "[" << startPosX << "]" << "[" << startPosY << "]" << "[" << startPosZ << "]" << endl;

        while(!hit_all){
            // update which dimention will be +1 in this turn of loop
            // cout << "   merge_dimension: " << merge_dimension << endl;
            bool updated = false;
            switch(merge_dimension){
                case 'x':
                    if(mergedBlock.sizeX >= parent_x || mergedBlock.startX + mergedBlock.sizeX > x_count){
                        hit_x = true;
                        merge_dimension = updateMergeDimension(merge_dimension, hit_x, hit_y, hit_z);
                        updated = true;
                        // cout << "   x hit, switch to " << merge_dimension << endl;
                    }
                    break;
                case 'y':
                    if(mergedBlock.sizeY >= parent_y || mergedBlock.startY + mergedBlock.sizeY > y_count){
                        hit_y = true;
                        merge_dimension = updateMergeDimension(merge_dimension, hit_x, hit_y, hit_z);
                        updated = true;
                        // cout << "   y hit, switch to " << merge_dimension << endl;
                    }
                    break;
                case 'z':
                    if(mergedBlock.sizeZ >= parent_z || mergedBlock.startZ + mergedBlock.sizeZ > z_count){
                        hit_z = true;
                        merge_dimension = updateMergeDimension(merge_dimension, hit_x, hit_y, hit_z);
                        updated = true;
                        // cout << "   z hit, switch to " << merge_dimension << endl;
                    }
                    break;
            }
            hit_all = (hit_x && hit_y && hit_z);

            if(hit_all){ break; }
            if(updated){ continue; }

            // cout << "   dimension+1: " << merge_dimension << endl;
            switch(merge_dimension){
                case 'x':
                    loopStartPosX = startPosX + mergedBlock.sizeX;
                    loopStartPosY = startPosY;
                    loopStartPosZ = startPosZ;
                    loopEndPosX = loopStartPosX;
                    loopEndPosY = loopStartPosY + mergedBlock.sizeY-1;
                    loopEndPosZ = loopStartPosZ + mergedBlock.sizeZ-1;
                    break;
                case 'y':
                    loopStartPosX = startPosX;
                    loopStartPosY = startPosY + mergedBlock.sizeY;
                    loopStartPosZ = startPosZ;
                    loopEndPosX = loopStartPosX + mergedBlock.sizeX-1;
                    loopEndPosY = loopStartPosY;
                    loopEndPosZ = loopStartPosZ + mergedBlock.sizeZ-1;
                    break;
                case 'z':
                    loopStartPosX = startPosX;
                    loopStartPosY = startPosY;
                    loopStartPosZ = startPosZ + mergedBlock.sizeZ;
                    loopEndPosX = loopStartPosX + mergedBlock.sizeX-1;
                    loopEndPosY = loopStartPosY + mergedBlock.sizeY-1;
                    loopEndPosZ = loopStartPosZ;
                    break;
            }
            // cout << "x from " << loopStartPosX << " to " << loopEndPosX << endl;
            // cout << "y from " << loopStartPosY << " to " << loopEndPosY << endl;
            // cout << "z from " << loopStartPosZ << " to " << loopEndPosZ << endl;


            bool hit_diff_tag = false;
            for (int z = loopStartPosZ; z <= loopEndPosZ && !hit_diff_tag; z++) {
                for (int y = loopStartPosY; y <= loopEndPosY && !hit_diff_tag; y++) {
                    for (int x = loopStartPosX; x <= loopEndPosX && !hit_diff_tag; x++) {
                        if(inBuffer[x][y][z] == currentTag){
                            
                        }else{
                            hit_diff_tag = true;
                            // cout << "Hit diff tag from " << currentTag;
                            // cout << " at " << "[" << x << "]" << "[" << y << "]" << "[" << z << "] = " << inBuffer[x][y][z] << ", rather than " << currentTag << endl;
                        }
                    }
                }
            }

            if(!hit_diff_tag){
                for (int z = loopStartPosZ; z <= loopEndPosZ && !hit_diff_tag; z++) {
                    for (int y = loopStartPosY; y <= loopEndPosY && !hit_diff_tag; y++) {
                        for (int x = loopStartPosX; x <= loopEndPosX && !hit_diff_tag; x++) {
                            // cout << "mark inBuffer" << "[" << x << "]" << "[" << y << "]" << "[" << z << "] as merged" << endl;
                            inBuffer[x][y][z] = '?';
                        }
                    }
                }
            }

            if(!hit_diff_tag){
                //undate the size of parent block current dimenstion size+1
                switch(merge_dimension){
                    case 'x':
                        mergedBlock.sizeX++;
                        // cout << "size x++: " << mergedBlock.sizeX << endl;
                        break;
                    case 'y':
                        mergedBlock.sizeY++;
                        // cout << "size y++: " << mergedBlock.sizeY << endl;
                        break;
                    case 'z':
                        mergedBlock.sizeZ++;
                        // cout << "size z++: " << mergedBlock.sizeZ << endl;
                        break;
                }
            }else{
            	//mark the current +1 dimention to "hit"
                switch(merge_dimension){
                    case 'x':
                        hit_x = true;
                        break;
                    case 'y':
                        hit_y = true;
                        break;
                    case 'z':
                        hit_z = true;
                        break;
                }
            }
            // cout << "size x: " << mergedBlock.sizeX << endl;
            // cout << "size y: " << mergedBlock.sizeY << endl;
            // cout << "size z: " << mergedBlock.sizeZ << endl;
            // cout << endl;
            merge_dimension = updateMergeDimension(merge_dimension, hit_x, hit_y, hit_z);
        }
        // cout << endl;

        startPosX += mergedBlock.sizeX;
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

        // stop merging if startPosZ is out of bound(no any more slice need to be compressed)
        if (startPosZ > z_count-1) {
        	// cout << "Z is out of bound, stop merging" << endl;
            break;
        }
        //put this ParentBlock into result
        result.push_back(mergedBlock);

        // cout << "startPosX: " << mergedBlock.startX << ", x_count: " << x_count << endl;
        // cout << "startPosY: " << mergedBlock.startY << ", y_count: " << y_count << endl;
        // cout << "startPosZ: " << mergedBlock.startZ << ", z_count: " << z_count << endl;
        // cout << "sizeX: " << mergedBlock.sizeX << endl;
        // cout << "sizeY: " << mergedBlock.sizeY << endl;
        // cout << "sizeZ: " << mergedBlock.sizeZ << endl;
        // cout << endl;
        // cout << "   " << mergedBlock.startX << "," << mergedBlock.startY << "," << mergedBlock.startZ << ","
        // << mergedBlock.sizeX << "," << mergedBlock.sizeY << "," << mergedBlock.sizeZ << ","
        // << mergedBlock.label << endl << endl;

    }

    //print out result
    // cout << "start print result" << endl;
    for(long unsigned int i=0; i < result.size(); i++){
        cout << result[i].startX << "," << result[i].startY << "," << result[i].startZ << ","
        << result[i].sizeX << "," << result[i].sizeY << "," << result[i].sizeZ << ","
        << result[i].label << endl;
    }

    // cout << "inBuffer" << "[49][6][0] is: " << inBuffer[49][6][0] << endl;

    


    return 0;
}