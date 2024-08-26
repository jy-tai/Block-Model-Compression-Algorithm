#include <istream>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <string>
#include <cstdio>
#include <map>
using namespace std;

#define MAX_TAGS 256
#define MAX_TAG_LENGTH 256
const int MAX_BUFFER_SIZE = 128;
int PLOT_X;
int PLOT_Y;
int PLOT_Z;

int parseHead(int* header, FILE* filename) {
	//*debug: cout << "parsing header!" << endl;
    char inbuf[16];
    int i = 0;
    int headIndex = 0;
    char c;
    do
    {
        c = getc(filename);
        if(c == ','||c == '\n') {
            inbuf[i] = '\0';
            sscanf(inbuf, "%d", &(header[headIndex++])); 
            i = 0;
            continue;
        }
        inbuf[i] = c;
        i++;
    } while(c != '\n');
    return 0;
}

int parseTags(map<string,char>* tagTable, int* tagTableLength, FILE* filename) {
	//*debug: cout << "parsing tags!" << endl;
    int tagIndex = 0;
    // char tagAliasIn;
    char tagIn[256];

    while(tagIndex < MAX_TAGS) {
        int i = 0;
        char c;
        char value;
        string key;

        c = getc(filename);
        if(c == '\n') {
            *tagTableLength = tagIndex; 
            return 0;
        }
        value = c;

        do
        {
            c = getc(filename);
            if(c == ','||c == ' ') {
                continue;
            }
            if(c == '\n') {
                tagIn[i++] = '\0';
                continue;
            }
            tagIn[i] = c;
            i++;
        } while(c != '\n');

        key = tagIn;	
        (*tagTable)[key] = value;
        tagIndex++;

        //Print tags:
        cout << value << ", " << key << endl;
    }
    return 0;
}

char* getPointIn3dArray(int x, int y, int z, char* array3d) {
    return (array3d + x * PLOT_Y * PLOT_Z + y * PLOT_Z + z);
}

void setPointIn3dArray(int x, int y, int z, int x_size, int y_size, int z_size, char* array3d, char setTo) {
	for (int i = 0; i < x_size; i++) {
		for (int j = 0; j < y_size; j++) {
			for (int k = 0; k < z_size; k++) {
				*(array3d + (x+i) * PLOT_Y * PLOT_Z + (y+j) * PLOT_Z + (z+k)) = setTo;
			}
		}
	}
}

int main(int argc, char* argv[]) {

	int header[6];
	//file for read header & tag
	FILE* h_file;
	h_file = fopen(argv[1], "r");
	//file for data
	ifstream d_file(argv[2]);

    // DEPENDING ON WHAT FORMAT TAGS ARE IN CAN BUILD TABLE LIKE THIS ELSE NEED TO USE HASHMAP
    // char tagAliasTable[MAX_TAGS];
    // char tagTable[MAX_TAGS][MAX_TAG_LENGTH];
    int tagTableSize = 0;

    map <string,char>* tagTable = new map <string,char> ();

    if(parseHead(header, h_file) != 0) {
        cout << "something broke parsing the header" << endl;
        return 1;
    }

    //Print headers to output file for comparison:
    cout << header[0] << "," << header[1] << "," << header[2] << "," << header[3] << "," << header[4] << "," << header[5] <<endl;

    if(parseTags(tagTable, &tagTableSize, h_file) != 0) {
        cout << "something broke parsing the header" << endl;
        return 1;
    }

    //Print separate line:
    cout << endl;

    //*debug: cout << "done!" << endl;

    PLOT_X = header[0];
    PLOT_Y = header[1];
    PLOT_Z = header[2];

    //*debug: cout << "X: " << PLOT_X << ", Y: " << PLOT_Y << ", Z: " << PLOT_Z << endl;

 	char* geoMap = new char[PLOT_X * PLOT_Y * PLOT_Z];
 	string l;
	int x, y, z, xs, ys, zs;
	char setTo;
	int pos;

	while(d_file.peek()!= EOF) {
		getline(d_file, l);
		if(l == "\n") {
			continue;
		}
		//cout << "line: " << l << endl;

		pos = l.find_first_of(',');
		x = stoi(l.substr(0,pos));
		l = l.substr(pos+1);
		//cout << "x: " << x << endl;

		pos = l.find_first_of(',',0);
		y = stoi(l.substr(0,pos));
		l = l.substr(pos+1);
		//cout << "y: " << y << endl;

		pos = l.find_first_of(',',0);
		z = stoi(l.substr(0,pos));
		l = l.substr(pos+1);
		//cout << "z: " << z << endl;

		pos = l.find_first_of(',',0);
		xs = stoi(l.substr(0,pos));
		l = l.substr(pos+1);
		//cout << "xs: " << xs << endl;

		pos = l.find_first_of(',',0);
		ys = stoi(l.substr(0,pos));
		l = l.substr(pos+1);
		//cout << "ys: " << ys << endl;

		pos = l.find_first_of(',',0);
		zs = stoi(l.substr(0,pos));
		l = l.substr(pos+1);
		//cout << "zs: " << zs << endl;

		setTo = tagTable->find(l)->second;
		//cout << "tag: " << setTo << endl;

		setPointIn3dArray(x,y,z,xs,ys,zs,geoMap,setTo);
	}

	for (int i = 0; i < PLOT_Z; i++) {
		for (int j = 0; j < PLOT_Y; j++) {
			for (int k = 0; k < PLOT_X; k++) {
				cout << *(getPointIn3dArray(k,j,i,geoMap));
			}
			cout << endl;
		}

		cout << endl;
		
	}

	return 0;
}


