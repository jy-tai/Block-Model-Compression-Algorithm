#include <cassert>
#include <chrono>   //timers
#include <cstdint>  //u16
#include <iostream> //cin cout
#include <sstream>  //stringstream
#include <stdlib.h> // memory functions
#include <stdio.h>
#include <string.h> // c strings
#include <string>   // cpp strings
#include <thread>
#include <algorithm> // max
#include <unordered_map>
#include <vector>
#include <cassert> // assert
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "../../common/processInput.hpp"
#include "../../common/summedArea.hpp"
#include <omp.h>


using namespace std;

// comment these out if you do not want timer or parameter outputs
// use preprocessor macros to optimise compilation in deployment
//#define PARAMETER_OUTPUT ; // include parameter output to stdout
//#define TIMER_OUTPUT ;     // likewise for timing info

typedef uint16_t u16;

char *buffer = NULL;


void partitionBlock(int * block_map, char * char_map, Parameters * parameters, u16 z_coord,
                    u16 y_coord, u16 x_coord, u16 z_coord2, u16 y_coord2,
                    u16 x_coord2)
{
  //cout << "hoi" <<z_coord<<y_coord<<x_coord<<z_coord2<<y_coord2<<x_coord2<< endl;

  int
      depth = z_coord2 - z_coord + 1,
      length = y_coord2 - y_coord + 1,
      width = x_coord2 - x_coord + 1;

      //cout << "go again "<<z_coord<< y_coord<< x_coord<<  z_coord2<<  y_coord2<<x_coord2<<endl;

  int target_val = depth * width * length; // volume of cube formed. We want every character within the volume to be the same. Hence, the target value IS the volume.

  if (depth <= 0 || width <= 0|| length <= 0)
  {
    //  fail condition (if any value is 0)
    // note in this implementation it is guaranteed no value is less than 0
    cout << "fail cond less than 0"<<endl;
    //assert(false);
    return;
  }
  if (target_val == 1)
  {
    // success cond (smallest possible cube) Therefore we need to search for what block this belongs to;

    //threadsafe version of stdcout
    std::stringstream msg;
    msg<<x_coord<<','<<y_coord<<','<<z_coord<<",1,1,1,"<<getLabelFromTag(parameters,char_map[get3DAddress(z_coord,y_coord,x_coord,max_z,max_y,max_x)])<<"\n";
    std::cout << msg.str();
    return;
   
  }
    
  

  for (int block = 0; block < max_block; block++)
  {

    int result = sum(block_map, block, z_coord, y_coord, x_coord, z_coord2, y_coord2, x_coord2,max_block,max_z,max_y,max_x);
    assert(result <= target_val);
    if (result == target_val)
    {

      // success_cond
      std::stringstream msg;
      msg<<x_coord<<','<<y_coord<<','<<z_coord<<','<<width<<','<<length<<','<<depth<<','<<getLabelFromTag(parameters,char_map[get3DAddress(z_coord,y_coord,x_coord,max_z,max_y,max_x)])<<"\n";
      std::cout << msg.str();
      return;
    }
    if (result > 0){ // if this block is only partially filled by this block type, then we know 
      break;
    }
  }

  // good
  u16 n_z_coord = (z_coord2 + z_coord) / 2;
  u16 n_y_coord = (y_coord2 + y_coord) / 2;
  u16 n_x_coord = (x_coord2 + x_coord) / 2;

  int x1, y1, z1;
  int x2, y2, z2;

  // create 8 subsections

  // there are 3 conditions to check for each 8 cubes: is x,y,z 2 or greater
  // if no then some cubes cannot be subdivided e.g if 1x2x2 you cannot create the bottom 4 cubes
  // these tests ensure that there is enough space to make each cube
  //this is to avoid creating
  u16 check = 0;
  u16 tmp, res;
  u16 evaluate;

  // 1 = check validated, 0 = not validated.
  // in format check = 0xZYX
  // following
  if (width >= 2)
  {
    check |= 0b1;
  }
  if (length >= 2)
  {
    check |= 0b10;
  }
  if (depth >= 2)
  {
    check |= 0b100;
  }
  /**
  for (u16 i = 0; i < 8; i++)
  {
    tmp = i & check;
    res = tmp == check;

    if (!res)
    {
      cout << "fail res";
      continue;
    }

    i & 0b1 ? x1 = x_coord,x2 = n_x_coord : x1 = n_x_coord + 1,x2 = x_coord2;
    i & 0b10 ? y1 = y_coord,y2 = n_y_coord : y1 = n_y_coord + 1,y2 = y_coord2;
    i & 0b100 ? z1 = z_coord,z2 = n_z_coord : z1 = n_z_coord + 1,z2 = z_coord2;
    
    partitionBlock(block_map,char_map, z1, y1, x1, z2, y2, x2);
  }
  */

  
  for (u16 z = 0; z < 2; z++)
  {
    for (u16 y = 0; y < 2; y++)
    {
      for (u16 x = 0; x < 2; x++)
      {
        evaluate = (4 * z | 2 * y | x);
        tmp = evaluate & check;
        res = tmp == evaluate;

        // std::cout <<"c e x  y  z  eval   "<<check<<x<<' '<<y<<' '<<z<<' '<<evaluate<<endl;
        if (!res)
        {
          //cout << "skip" << evaluate << endl;
          continue;
        }

        x1 = x % 2 == 0 ? x_coord : n_x_coord + 1;
        y1 = y % 2 == 0 ? y_coord : n_y_coord + 1;
        z1 = z % 2 == 0 ? z_coord : n_z_coord + 1;
        x2 = x % 2 == 0 ? n_x_coord : x_coord2;
        y2 = y % 2 == 0 ? n_y_coord : y_coord2;
        z2 = z % 2 == 0 ? n_z_coord : z_coord2;

       // cout << x1<<y1<<z1<<x2<<y2<<z2<<' '<<z<<x<<y<<' '<<x_coord<<y_coord<<z_coord<<' '<<x_coord2<<y_coord2<<z_coord2<<endl;

        partitionBlock(block_map,char_map,parameters, z1, y1, x1, z2, y2, x2);
      }
    }
  }
  
  return;
}


int main() {
#ifdef TIMER_OUTPUT
  std::chrono::time_point<std::chrono::system_clock> startTime =
      std::chrono::high_resolution_clock::now();
  std::chrono::time_point<std::chrono::system_clock> endTime;
#endif

  Parameters * parameters = getParameters();
  setMaxDimensions(parameters->max_blocks, parameters->max_z, parameters->max_y, parameters->max_x);
  auto symbol_map = getCharMap();

  auto block_map = getBlockMap(parameters, symbol_map);

  //int pos_x,pos_y,pos_z;
  int sub_max_z = parameters->sub_max_z;
  int sub_max_y = parameters->sub_max_y;
  int sub_max_x = parameters->sub_max_x;


  for (int z = 0; z < max_z/sub_max_z; z++) {
    for (int y = 0; y < max_y/sub_max_y; y++) {
      //parallell execution
      #pragma omp parallel for
      for (int x = 0; x < max_x/sub_max_x; x++) {
        //cout << "partition block"<<z<<y<<x<<endl;
        //#pragma omp atomic
        int pos_x = x*sub_max_x,pos_y = y*sub_max_y, pos_z = z*sub_max_z;

        partitionBlock(block_map,symbol_map,parameters,pos_z,pos_y,pos_x,min(pos_z+sub_max_z-1,max_z-1),min(pos_y+sub_max_y-1,max_y-1),min(pos_x+sub_max_x-1,max_x-1));
        }
      }
    }

  std::cout.flush();

#ifdef TIMER_OUTPUT
  endTime = std::chrono::high_resolution_clock::now();

  std::chrono::duration<double> diff = endTime - startTime; // this is in ticks
  std::chrono::milliseconds d =
      std::chrono::duration_cast<std::chrono::milliseconds>(
          diff); // ticks to time

  std::cout << "Program completed in " << d.count() << " milliseconds\n";

#endif

#ifdef PARAMETER_OUTPUT
  // printing info
  // params
  cout << "parameters" << endl;
  cout << parameters.max_x << ' ' << parameters.max_y << ' ' << parameters.max_z << ' ' << parameters.sub_max_x << ' '
       << parameters.sub_max_y << ' ' << parameters.sub_max_z << endl;
  cout << endl << endl;

  // char tag
  cout << "char tags" << endl;
  for (auto const &pair : parameters.blocks) {
    cout << pair.first << ": " << pair.second << endl;
  }
  cout << endl;

  cout << "symbol map" << endl;
  for (int z = 0; z < max_z; z++) {
    for (int y = 0; y < max_y; y++) {
      for (int x = 0; x < max_x; x++) {
        cout << symbol_map[get3DAddress(z,y,x)];
      }
      cout << endl;
    }
    cout << endl;
  }

  cout << "block map" << endl;
  for (int block = 0; block < parameters.blocks.size(); block++) {
    cout << "Block " << parameters.blocks[block].second << endl;
    for (int z = 0; z < max_z+1; z++) {
      for (int y = 0; y < max_y+1; y++) {
        for (int x = 0; x < max_x+1; x++) {
          cout << block_map[get4DAddress(block,z,y,x,parameters.max_blocks,max_z+1,max_y+1,max_x+1)];
        }
        cout << endl;
      }
      cout << endl;
    }
  }
#endif
//cout<<"end";
return 0;
}