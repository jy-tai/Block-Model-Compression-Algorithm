#ifndef PROCESS_INPUT_H
#define PROCESS_INPUT_H

#include <vector>
#include <string>
#include <cstring>

const int MAX_LABEL_LENGTH = 200; // char length of label
const int MAX_TAG_VALUE = 0x7E;
const int MIN_TAG_VALUE = 0x21;
const int MAX_TAG_SIZE = MAX_TAG_VALUE - MIN_TAG_VALUE + 1; //tags can have values between 33 to 126 inclusive

struct Parameters {
    Parameters();
    int max_x;
    int max_y;
    int max_z;
    int sub_max_x;
    int sub_max_y;
    int sub_max_z;
    int max_blocks;
    char block_tags[MAX_TAG_SIZE];
    char block_labels[MAX_TAG_SIZE][MAX_LABEL_LENGTH + 1]; // index by block_labels[getIndexFromTag(c)]
};


//process stdin, get parameters
Parameters * getParameters();

//inline int getIndexFromTag(char c);

char *getLabelFromTag(Parameters * p, char c);

// sets extern values for use in functions and default values;
extern int max_z, max_y, max_x, max_block;

// MUST call setMaxDimensions function to use getCharMap, get 3D address;
void setMaxDimensions(int block, int z, int y, int x);

//shortcut to address 3d and 4d arrays.
int get3DAddress(int z_index, int y_index, int x_index, int z_size, int y_size, int x_size );
int get4DAddress(int w_index, int z_index, int y_index, int x_index, int w_size , int z_size , int y_size , int x_size );

//process character map from stdin, get 3d array representing character map
char * getCharMap();

#endif