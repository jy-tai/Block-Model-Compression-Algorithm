
# Composition
This repository is comprised of 5 components
1. < stream > | dcmpn.cpp is the decompressor algorithm, when fed a compressed blockmap stream it will output the original map
2. DrawCaseFile.py < file > is a block map visualiser, given a blockmap text file it will output a NUMPY plot showing that map (useful for debugging test cases)
3. validateBinary.sh -f < codec path >: takes a valid codec binary as input and does the following steps
    1. get a case file *x* from /case/x and create a folder in /testOut/x/
    1. Use binary to compress *x*
        - output result *R* to /testOut/x/compressed.tmp
        - log any errors in /testOut/x/bin.err
    2. decompress the *R*
        - output result to /testOut/x/decompressed.tmp
        - log any errors in /testOut/x/validator.err
    3. compare *R* and *x*
        - output result to /testOut/x /diff.tmp
    4. if no errors /testOut/x/
    5. Repeat on every casefile in /cases
4. buildCases.sh will generate all blockmaps from the casedef files in the /caseDefinitions folder and move the output to the /cases folder for the testing harness to use
5. GenCase.cpp given a stream of input parameters as below, it will generate a block map based on perlin noise. it uses the open source PerlinNoise.hpp library from Ryo Suzuki

### Casedef file format

``` 
# CASEDEF FILE format
PLOT_X 25
PLOT_Y 25
PLOT_Z 25

SEED 502

LAYER_NUM 8
D_TAG_NUM 2

# layer parameters
Z_SHIFT .5
MODULATION_FREQ 35.0
MODULATION_WEIGHT .5
LAYER_OCTAVES 6
LAYER_PERSISTANCE .5

# debris parameters
DEBRIS_FREQ 15
DEBRIS_OCTAVES 2
DEBRIS_PERSISTANCE .5
DEBRIS_LACUNARITY 2
DEBRIS_LAYER_FALLOFF .1
DEBRIS_DENSITY .1
```


# FAQ
### IF NO GRAPHICAL OUTPUT ON MAKE
sudo apt-get install python3-tk


# HOW TO GEN TEST CASES
0. run `make install`
1. edit the `/testing/caseDefinition/in.test` file to test out parameters
2. run `make` to show a visual representation of `/testing/caseDefinition/in.test`
3. when happy with `/testing/caseDefinition/in.test` create a new case file `/testing/caseDefinition/<descriptive_name>.case`
4. run `make build` to build a .dat of each file in `/testing/caseDefinition/` except for `in.test`
