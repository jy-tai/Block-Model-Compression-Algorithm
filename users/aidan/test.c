#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int PLOT_X = 100;
int PLOT_Y = 1;
int PLOT_Z = 1;

const int MAX_TAGS = 256;
const int MAX_TAG_LENGTH = 256;
// ========================= KEY MAP =========================

int tagMapLength = 0;
char tagKeys[MAX_TAGS];
char tagMap[MAX_TAGS][MAX_TAG_LENGTH];

void mapAddTag(char key, char* value) {
    tagKeys[tagMapLength++] = key;
    strcpy(tagMap[key], value);
}


char* mapGetTag(char key) {
    return tagMap[key];
}

// ======================== END KEY MAP =======================




typedef struct {
    int x;
    int y;
    int z;
    int sizex;
    int sizey;
    int sizez;
    char* tag;
} Compressed;


void printCompressed(Compressed compressedData) {
    printf("%d,%d,%d,%d,%d,%d,%s", 
        compressedData.x,
        compressedData.y,
        compressedData.z,
        compressedData.sizex,
        compressedData.sizey,
        compressedData.sizez,
        compressedData.tag
    );
    printf("\n");
}



Compressed getPointIn3dArray(int x, int y, int z, Compressed* array3d)
{
    return *(array3d + x * PLOT_Y * PLOT_Z + y * PLOT_Z + z);
}


void setPointIn3dArray(int x, int y, int z, Compressed* array3d, Compressed setTo)
{
    *(array3d + x * PLOT_Y * PLOT_Z + y * PLOT_Z + z) = setTo;
}


int parseHead(int* header) {
    char inbuf[16];
    int i = 0;
    int headIndex = 0;
    char c;
    do
    {
        c = getchar();
        if(c == ',' || c == '\n' || c == '\r') {
            inbuf[i] = '\0';
            sscanf(inbuf, "%d", &(header[headIndex++])); 
            i = 0;
            continue;
        }
        inbuf[i] = c;
        i++;
    } while(c != '\n' || c == '\r');
    return 0;
}



int parseTags() {

    char key;
    char val[MAX_TAG_LENGTH];
    char buffer[MAX_TAG_LENGTH + 4];
    int i = 0;

    do
    {
        fgets(buffer, sizeof(buffer), stdin);
        if(buffer[0] == 10 || (buffer[0] == 13 && buffer[1] == 10)) {
            return 0;
        }
        int k = 0;

        char keybuff[5];
        sscanf(buffer, "%s%s", keybuff, val);
        key = keybuff[0];
        mapAddTag(key, val);
        i += 1;
    } while (i <= MAX_TAG_LENGTH);
    return 0;
}


const int MAX_BUFFER_SIZE = 256000;


int main() {
    
    int header[6];


    if(parseHead(header) != 0) {
        printf("something broke parsing the header\n");
        return 1;
    } 

    if(parseTags() != 0) {
        printf("something broke parsing the header\n");
        return 1;
    } 

    
    // for (int i = 0; i < tagMapLength; i++)
    // {
    //     printf("%d: %c   %s\n", i, tagKeys[i],  mapGetTag(tagKeys[i]));
    // }

    // return 0;



    const int plot_x_count = header[0];
    const int plot_y_count = header[1];
    const int plot_z_count = header[2];

    // We might only want to test MAX_BLOCK_n size blocks not 256 size blocks
    const int parent_x = header[3];
    const int parent_y = header[4];
    const int parent_z = header[5];
    



    Compressed* compressedArray = (Compressed*)malloc((plot_x_count * plot_y_count * plot_z_count)*sizeof(Compressed*));

    char inBuffer[MAX_BUFFER_SIZE];


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
            if(c == '\n' || c == '\r') { continue; } // ignore slice and new column indicators
            inBuffer[bufferIncrementor++] = c;
        } while (bufferIncrementor < MAX_BUFFER_SIZE);


        // for each cell in buffer
        #pragma acc kernels loop
        for(int i = 0; i < bufferIncrementor; i++) {
            char keyChar = inBuffer[i];
            // std::cout << x_pos << "," << y_pos << "," << z_pos << ",1,1,1," << tagTable[inBuffer[i]] << std::endl;
            Compressed* tmp = (Compressed*)malloc(sizeof(Compressed));
                tmp->x = x_pos;
                tmp->y = y_pos;
                tmp->z = z_pos;
                tmp->sizex = 1;
                tmp->sizey = 1;
                tmp->sizez = 1;
                tmp->tag = tagMap[keyChar];

            printCompressed(*tmp);


            // increment xyz position
            x_pos++;
            if(x_pos > plot_x_count-1) { 
                y_pos++;
                x_pos = 0;
            }
            if(y_pos > plot_y_count-1) {
                z_pos++;
                y_pos = 0;
            }
        }
        bufferIncrementor = 0;
    }

    return 0;
}