#include <iostream>
#include <vector>

using namespace std;

// Directions for exploring neighbors
int dx[6] = {-1, 0, 1, 0, 0, 0};
int dy[6] = {0, 1, 0, -1, 0, 0};
int dz[6] = {0, 0, 0, 0, -1, 1};

void floodFill(vector<vector<vector<char> > > &map, vector<vector<vector<bool> > > &visited, int x, int y, int z, char currentChar)
{
    int n = map.size();
    int m = map[0].size();
    int l = map[0][0].size();

    // Mark the current cell as visited
    visited[x][y][z] = true;

    // Explore the neighbors (computing coordinates of neighboring cells for each direction)
    for (int i = 0; i < 6; i++)
    {
        int newX = x + dx[i];
        int newY = y + dy[i];
        int newZ = z + dz[i];

        // checking if the neighboring cell is within bounds of map and has not been visited yet and has same character as currentChar then call
        // floodFill recursively on neighboring cell
        if (newX >= 0 && newX < n && newY >= 0 && newY < m && newZ >= 0 && newZ < l && !visited[newX][newY][newZ] && map[newX][newY][newZ] == currentChar)
        {
            cout << "merged " << currentChar << " "
                 << "coordinates are:"
                 << "[" << newZ << "]"
                 << "[" << newY << "]"
                 << "[" << newX << "]" << endl;
            floodFill(map, visited, newX, newY, newZ, currentChar);
        }
    }
}

void findShapes(vector<vector<vector<char> > > &map)
{
    int n = map.size();
    int m = map[0].size();
    int l = map[0][0].size();

    // Initialize the visited array
    vector<vector<vector<bool> > > visited(n, vector<vector<bool> >(m, vector<bool>(l, false)));

// Iterate over the map and perform flood fill for each unvisited cell
#pragma omp parallel for collapse(3)
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            for (int k = 0; k < l; k++)
            {
                if (!visited[i][j][k])
                {
#pragma omp critical
                    {
                        if (!visited[i][j][k])
                        {
                            floodFill(map, visited, i, j, k, map[i][j][k]);
                            // output a message indicating a shape was found
                            cout << "Found a shape of type " << map[i][j][k] << endl;
                            cout << "The coordinates are "
                                 << "[" << k << "]"
                                 << "[" << j << "]"
                                 << "[" << i << "]" << endl;
                        }
                    }
                }
            }
        }
    }
}

int main()
{
    // Testing the findShapes function with a test map
    vector<vector<vector<char> > > map = {{{'a', 'a', 'a'},
                                         {'a', 'b', 'a'},
                                         {'f', 'a', 'a'}},
                                        {{'l', 'a', 'k'},
                                         {'m', 'a', 'k'},
                                         {'j', 'a', 'j'}}};
    findShapes(map);

    return 0;
}