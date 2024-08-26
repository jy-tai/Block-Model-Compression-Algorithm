#include "PerlinNoise.hpp"
#include <iostream>
#include <chrono>
#include <set>
#include <algorithm>
#include <fstream>
#include <sstream>

using namespace std::chrono;


// grid size
int PLOT_X = 25;
int PLOT_Y = 25;
int PLOT_Z = 25;


const double MIN_RECORDED = 2.54358e-06;
const double MAX_RECORDED = 0.473143;
const int ASCII_START_OFFSET = 64;


// parameters
int LAYER_NUM = 8;     // Number of geological layers
int D_TAG_NUM = 2;     // number of layers for debris


// layer parameters
double Z_SHIFT = .5;
double MODULATION_FREQ = 35.0;
double MODULATION_WEIGHT = .5;
double LAYER_OCTAVES = 6;
double LAYER_PERSISTANCE = .5;


// debris parameters
double DEBRIS_FREQ = 15;
double DEBRIS_OCTAVES = 2;
double DEBRIS_PERSISTANCE = .5;
double DEBRIS_LACUNARITY = 2;
double DEBRIS_LAYER_FALLOFF = .1;
double DEBRIS_DENSITY = .1;

siv::PerlinNoise::seed_type SEED = -1;
siv::PerlinNoise* dperlinList;


// =============================================
//
//         DEBUG STAT KEEPING
//

double minNum = INT16_MAX;
double maxNum = INT16_MIN;
std::set<int> quantisedValues;

void readParameters()
{
    std::string line;
    std::getline(std::cin, line);
    do {
        if(line.length() == 0)
        {
            continue;
        }
        if(line.at(0) == '#')
        {
            continue;
        }
        
        std::string parameter;
        std::string value;

        std::stringstream ss(line);
        std::getline(ss, parameter, ' ');
        std::getline(ss, value, ' ');

        if ("SEED" == parameter) {
            SEED = stoi(value);
        } 
        if ("PLOT_X" == parameter) {
            PLOT_X = stoi(value);
        } 
        else if ("PLOT_Y" == parameter) {
            PLOT_Y = stoi(value);
        } 
        else if ("PLOT_Z" == parameter) {
            PLOT_Z = stoi(value);
        } 
        else if ("LAYER_NUM" == parameter) {
            LAYER_NUM = stoi(value);
        }
        else if ("D_TAG_NUM" == parameter) {
            D_TAG_NUM = stoi(value);
        }
        else if ("Z_SHIFT" == parameter) {
            Z_SHIFT = stod(value);
        }
        else if ("MODULATION_FREQ" == parameter) {
            MODULATION_FREQ = stod(value);
        }
        else if ("MODULATION_WEIGHT" == parameter) {
            MODULATION_WEIGHT = stod(value);
        }
        else if ("LAYER_OCTAVES" == parameter) {
            LAYER_OCTAVES = stod(value);
        }
        else if ("LAYER_PERSISTANCE" == parameter) {
            LAYER_PERSISTANCE = stod(value);
        }
        else if ("DEBRIS_FREQ" == parameter) {
            DEBRIS_FREQ = stod(value);
        }
        else if ("DEBRIS_OCTAVES" == parameter) {
            DEBRIS_OCTAVES = stod(value);
        }
        else if ("DEBRIS_PERSISTANCE" == parameter) {
            DEBRIS_PERSISTANCE = stod(value);
        }
        else if ("DEBRIS_LACUNARITY" == parameter) {
            DEBRIS_LACUNARITY = stod(value);
        }
        else if ("DEBRIS_LAYER_FALLOFF" == parameter) {
            DEBRIS_LAYER_FALLOFF = stod(value);
        }
        else if ("DEBRIS_DENSITY" == parameter) {
            DEBRIS_DENSITY = stod(value);
        } else {
            std::cout << "WARNING UNKNOWN PARAMETER " << parameter << " " << value << std::endl;
        }
    } while (std::getline(std::cin, line));
}


double getPointIn3dArray(int x, int y, int z, double* array3d)
{
    return *(array3d + x * PLOT_Y * PLOT_Z + y * PLOT_Z + z);
}


void setPointIn3dArray(int x, int y, int z, double* array3d, double setTo)
{
    *(array3d + x * PLOT_Y * PLOT_Z + y * PLOT_Z + z) = setTo;
}

double clamp(double in, double min, double max)
{
    return (std::min)((std::max)(in, min), max);
}


void layerPass(double* geoMap)
{
    const siv::PerlinNoise perlin{SEED};

    for (int z = 0; z < PLOT_Z; z++)
    {
        for (int y = 0; y < PLOT_Y; y++)
        {
            for (int x = 0; x < PLOT_X; x++)
            {
                double nval = (double)(z) / (double)(PLOT_Z);
                
                nval += perlin.octave3D_01(
                    x/MODULATION_FREQ,
                    y/MODULATION_FREQ,
                    z/MODULATION_FREQ,
                    LAYER_OCTAVES,
                    LAYER_PERSISTANCE
                    ) * MODULATION_WEIGHT;

                // nval += Z_SHIFT;
                nval = clamp(nval, .0001, .9999);
                setPointIn3dArray(x, y, z, geoMap, nval);


                if (nval < minNum)
                {
                    minNum = nval;
                }
                if (nval > maxNum)
                {
                    maxNum = nval;
                }
            }
        }
        if (PLOT_Z > 100 && z % 10 == 0)
        {
            std::cout << z/10 << "/" << PLOT_Z/5 << std::endl;
        }
    }
}


int quantiseAndAddDebris(int x, int y, int z, double nval)
{
    nval = (nval - minNum) / (maxNum - minNum);
    nval = clamp(nval, .0001, 1.);
    nval = floor(nval * (LAYER_NUM - 1));

    for (int n = 0; n < D_TAG_NUM; n++)
    {
        double debris = dperlinList[n].octave3D_01(
            x / DEBRIS_FREQ,
            y / DEBRIS_FREQ,
            z / DEBRIS_FREQ,
            DEBRIS_OCTAVES,
            DEBRIS_PERSISTANCE
            );
        debris = (debris - minNum) / (maxNum - minNum);
        debris -= DEBRIS_DENSITY;
        if (debris < 0)
        {
            nval = LAYER_NUM + n;
        }
    }

    quantisedValues.insert(nval + ASCII_START_OFFSET);
    return nval + ASCII_START_OFFSET;
}


void writeQuantisedFileToFile(std::ofstream &outFile, double* geoMap)
{
    for (int z = 0; z < PLOT_Z; z++)
    {
        for (int y = 0; y < PLOT_Y; y++)
        {
            for (int x = 0; x < PLOT_X; x++)
            {
                outFile << (char)quantiseAndAddDebris(x, y, z, getPointIn3dArray(x, y, z, geoMap));
            }
            outFile << std::endl;
        }
        outFile << std::endl;
        if (PLOT_Z > 100 && z % 10 == 0)
        {
            std::cout << (z/10 + PLOT_Z/5) << "/" << PLOT_Z/5 << std::endl;
        }
    }
}


int main(int argc, char *argv[])
{
    auto start = high_resolution_clock::now();

    readParameters();
    dperlinList = new siv::PerlinNoise[D_TAG_NUM];
    if(SEED == -1) {
        srand (time(NULL));
        SEED = rand() % 10000;
    }

    double* geoMap = new double[PLOT_X * PLOT_Y * PLOT_Z];
    
    for (int n = 0; n < D_TAG_NUM; n++)
    {
        const siv::PerlinNoise::seed_type debrisSeed = SEED + n;
        siv::PerlinNoise dperlin{ debrisSeed };
        dperlinList[n] = dperlin;
    }

    //
    //  open file and write headers
    //
    std::string outfileName = "out.dat";
    if(argc >= 2) {
        outfileName = argv[1]; 
    }
    std::ofstream outFile(outfileName);
    
    outFile << PLOT_X << ',' << PLOT_Y << ',' << PLOT_Z << ',' << "8,8,8" << std::endl;
    for (int i = 0; i < LAYER_NUM + D_TAG_NUM; i++)
    {
        outFile << (char)(ASCII_START_OFFSET+i) << ", " << (char)(ASCII_START_OFFSET+i) << std::endl;
    }
    outFile << std::endl;


    //
    //  generate initial layers
    //
    layerPass(geoMap);


    //
    // saturate quantize add debris and write to file
    //
    writeQuantisedFileToFile(outFile, geoMap);
    outFile.close();


    //
    //  debug logging
    //
    std::cout << "Expected tags" << (LAYER_NUM + D_TAG_NUM) <<
        " recieved tags " << quantisedValues.size() << std::endl;

    for (auto &&i : quantisedValues)
    {
        std::cout << (char)i << " ";
    }
    std::cout << std::endl;

    auto duration = duration_cast<microseconds>(high_resolution_clock::now() - start);
    std::cout << "Time taken by function: " <<
        duration.count() / 1000 << " milliseconds" << std::endl;

}
