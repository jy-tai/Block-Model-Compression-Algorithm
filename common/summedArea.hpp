
#ifndef SUMMED_AREA_H
#define SUMMED_AREA_H

#include "processInput.hpp"
int *getBlockMap(Parameters *parameters, char *v);

int sum(int *array, int block, int z1, int y1, int x1, int z2, int y2, int x2, int block_size, int z_size, int y_size, int x_size);

#endif