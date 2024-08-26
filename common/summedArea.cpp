


//4d int from 3d char

#include "processInput.hpp"
#include "summedArea.hpp"

#include <iostream>


int *
getBlockMap(Parameters * parameters, char * v)
{
  int m_b = parameters->max_blocks;
  int m_z = parameters->max_z;
  int m_y = parameters->max_y;
  int m_x = parameters->max_x;

  //std::cout << m_b << m_z << m_y << m_x<<std::endl;

  int *block_maps = new int[m_b * (m_z+1) * (m_y+1) * (m_x+1)];
  memset(block_maps, 0, m_b * (m_z+1) * (m_y+1) * (m_x+1)*sizeof(int));

  for (int block = 0; block < m_b; block++)
  {
      char char_block = parameters->block_tags[block];
      for (int z = 1; z < m_z + 1; z++)
      {
          for (int y = 1; y < m_y + 1; y++)
          {
              for (int x = 1; x < m_x + 1; x++)
              {
                  int isOnMap = (v[get3DAddress(z - 1, y - 1, x - 1, m_z, m_y, m_x)] == char_block) ? 1 : 0;

                  int result =
                      (isOnMap + block_maps[get4DAddress(block, z - 1, y, x, m_b, m_z + 1, m_y + 1, m_x + 1)] + block_maps[get4DAddress(block, z, y - 1, x, m_b, m_z + 1, m_y + 1, m_x + 1)] + block_maps[get4DAddress(block, z, y, x - 1, m_b, m_z + 1, m_y + 1, m_x + 1)]

                       - block_maps[get4DAddress(block, z - 1, y - 1, x, m_b, m_z + 1, m_y + 1, m_x + 1)] - block_maps[get4DAddress(block, z, y - 1, x - 1, m_b, m_z + 1, m_y + 1, m_x + 1)] - block_maps[get4DAddress(block, z - 1, y, x - 1, m_b, m_z + 1, m_y + 1, m_x + 1)]

                       + block_maps[get4DAddress(block, z - 1, y - 1, x - 1, m_b, m_z + 1, m_y + 1, m_x + 1)]);

                  block_maps[get4DAddress(block, z, y, x, m_b, m_z + 1, m_y + 1, m_x + 1)] = result;
              }
          }
    }
  }

  return block_maps;
}


int sum(int *array, int block, int z1, int y1, int x1, int z2, int y2, int x2, int block_size = max_block, int z_size=max_z, int y_size=max_y, int x_size=max_x){
  // we need to offset pos2 due to how a is offset by 1 (buffer)
  x2++;
  y2++;
  z2++;
  z_size++;
  y_size++;
  x_size++;
  int result =
      array[get4DAddress(block, z2, y2, x2, block_size, z_size,y_size,x_size)] 

      - array[get4DAddress(block, z1, y2, x2, block_size, z_size,y_size,x_size)] 
      - array[get4DAddress(block, z2, y1, x2, block_size, z_size,y_size,x_size)] 
      - array[get4DAddress(block, z2, y2, x1, block_size, z_size,y_size,x_size)] 
      
      + array[get4DAddress(block, z2, y1, x1, block_size, z_size,y_size,x_size)] 
      + array[get4DAddress(block, z1, y2, x1, block_size, z_size,y_size,x_size)] 
      + array[get4DAddress(block, z1, y1, x2, block_size, z_size,y_size,x_size)] 
     
      - array[get4DAddress(block, z1, y1, x1, block_size, z_size,y_size,x_size)];
  
  return result;
}

