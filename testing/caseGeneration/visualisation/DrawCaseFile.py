import numpy as np
import matplotlib.pyplot as plt
from matplotlib.colors import LinearSegmentedColormap
import matplotlib as mpl
import sys

LAYER_NUM = 8
D_TAG_NUM = 2
LAYER_ALPHA = .08
DEBRIS_ALPHA = 1.
COLOUR_MAP_NAME = 'viridis' #['viridis', 'plasma', 'inferno', 'magma', 'cividis'] https://matplotlib.org/stable/tutorials/colors/colormaps.html


colourMap = mpl.colormaps[COLOUR_MAP_NAME].resampled(LAYER_NUM + D_TAG_NUM).colors

for i in range(LAYER_NUM):
    colourMap[i] = (colourMap[i][0],colourMap[i][1],colourMap[i][2],LAYER_ALPHA)

for i in range(D_TAG_NUM):
    print(LAYER_NUM+i)
    colourMap[LAYER_NUM+i] = (colourMap[LAYER_NUM+i][0],colourMap[LAYER_NUM+i][1],colourMap[LAYER_NUM+i][2],DEBRIS_ALPHA)

CUSTOM_CMAP = LinearSegmentedColormap.from_list('custom_cmap', colourMap)








inputFileName="./out.dat"

if len(sys.argv) > 1:
    inputFileName = sys.argv[1]


f = open(inputFileName, "r")

head = f.readline().rstrip().split(',')

print(head)
PLOT_X = int(head[0])
PLOT_Y = int(head[1])
PLOT_Z = int(head[2])


tags = {}

# get tag table
for line in f:
    if len(line) == 1:
        break
    tags[line.split(",")[0]] = len(tags)

for key in tags:
    tags[key] = tags[key] / len(tags)

print(tags)
print(len(tags))

data = np.zeros((PLOT_X, PLOT_Y, PLOT_Z))
x=0
y=0
z=0

for line in f:
    if line.rstrip() == 'q':
        break
    for char in line.rstrip():
        data[x][y][z] = (tags[char])
        y+=1
        if y >= PLOT_Y:
            y=0
            x+=1
        if x >= PLOT_X:
            x=0
            z+=1
        if z >= PLOT_Z:
            z=0


fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')
ax.set_xlabel('X')
ax.set_ylabel('Y')
ax.set_zlabel('Z')

# Generate meshgrid for x, y, and z coordinates
x_coords = np.linspace(0, PLOT_X - 1, PLOT_X)
y_coords = np.linspace(0, PLOT_Y - 1, PLOT_Y)
z_coords = np.linspace(0, PLOT_Z - 1, PLOT_Z)
x_mesh, y_mesh, z_mesh = np.meshgrid(
    x_coords,
    y_coords,
    z_coords
)


# Display the 3D noise as a scatter plot
ax.scatter(x_mesh, y_mesh, z_mesh, c=data, cmap=CUSTOM_CMAP, marker='s')#, alpha=GRAPH_ALPHA)
plt.show()