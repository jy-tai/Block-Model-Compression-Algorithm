#include "processInput.hpp"


char * getLabelFromTag(  Parameters * p, char c){
    return p->block_labels[c - MIN_TAG_VALUE];
}

//int getIndexFromTag(char c){
 //   return c - MIN_TAG_VALUE;
//}

Parameters::Parameters(){
    //set all character values of Parameters to space.
    memset(block_tags, 0x20, sizeof(block_tags));
    //set all character values of Parameters to space.
    memset(block_labels, 0x20, sizeof(block_labels));

}


Parameters * getParameters()
{
  Parameters * return_value = new Parameters();

  char line[64];

  // get 1st line of parameters
  fgets(line, sizeof(line), stdin);
  char *variables;

  //process each of the 6 variables

  variables = strtok(line, ",");
  return_value -> max_x = atoi(variables);
  
  variables = strtok(NULL, ",");
  return_value -> max_y = atoi(variables);

  variables = strtok(NULL, ",");
  return_value -> max_z = atoi(variables);

  variables = strtok(NULL, ",");
  return_value -> sub_max_x = atoi(variables);

  variables = strtok(NULL, ",");
  return_value -> sub_max_y = atoi(variables);
  
  variables = strtok(NULL, ",");
  return_value -> sub_max_z = atoi(variables);

  //get each char/tag in the tag table

  int block_count = 0;
  char c;
  while (true)
  {
    fgets(line, sizeof(line), stdin);

    // Remove the newline character at the end
    line[strcspn(line, "\r\n")] = '\0';

    //if the current line has a length of 2 or less, skip reading and end searching for tags
    if (strlen(line) > 2 || block_count == 0)
    {
        variables = strtok(line, ","); // get first value separated by comma
        if (variables) 
        {

            c = variables[0]; // character = first character of the string 

            variables = strtok(NULL, ","); // tokenise second value
            
            memcpy(&(return_value->block_labels[c - MIN_TAG_VALUE][0]), &variables[1], strlen(variables)); // copy second value directly into variable "variables", skipping the first char (a space)

            return_value->block_tags[block_count] = c;

            block_count ++;
        }
        else{

            break;
        }
    }
    else
    {
        break;
    }
}
return_value -> max_blocks = block_count;
return return_value;
}

inline int getIndexFromTag(char c)
{
  return c - MIN_TAG_VALUE;
}



char *getCharMap()
{
  //read in data + CRLF
  int readinsize = max_y+1+sizeof("\r\n");                                 // includes \n
  auto map = new char[max_z * max_y * max_x + sizeof("\0\r\n")]; //+3 is when the \n \0 is added at the last call. prevents overflow
  //file pointer

  for (int z = 0; z < max_z; z++)
  {
    for (int y = 0; y < max_y; y++)
    {
      fgets(&map[get3DAddress(z, y, 0,max_z,max_y,max_x)], readinsize, stdin);
    }
    fgets(&map[max_z * max_y * max_x], sizeof("\0\r\n"), stdin);
  }
  return map;
}


int max_z = 0;
int max_y = 0;
int max_x = 0;
int max_block = 0;


void setMaxDimensions(int block, int z, int y, int x)
{
  max_block = block;
  max_z = z;
  max_y = y;
  max_x = x;
}

// convert 3d or 4d addresses into 1d addresses
int get3DAddress(int z_index, int y_index, int x_index, int z_size, int y_size, int x_size ){
  return (z_index * y_size * x_size + y_index * x_size + x_index);
}

int get4DAddress(int w_index, int z_index, int y_index, int x_index, int w_size , int z_size , int y_size , int x_size )
{
  return (w_index * y_size * x_size * z_size + z_index * y_size * x_size + y_index * x_size + x_index);
}


