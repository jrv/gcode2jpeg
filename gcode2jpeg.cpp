#include <iostream>
#include <fstream>
#include <Magick++.h>
#include <cstring>
using namespace Magick;
using namespace std;
// compile with:
// g++ gcode2jpeg.cpp `Magick++-config --cppflags --cxxflags --ldflags --libs` -o gcode2jpeg
//
// http://www.imagemagick.org/Magick++/tutorial/Magick++_tutorial.pdf

int getd(string line, char s);
void filename(int cnt, char * name);

int main(int argc, char *argv[]) {
    ifstream ifs; 
    ifs.open("marvin.gcode", ifstream::in);
    int cnt = 0;
    int x,y = 0;
    string line;
    Image myimage("1920x1080", "black");
    myimage.strokeWidth(1);
    myimage.fillColor("white");
    myimage.strokeAntiAlias(false);
    char name[20];
    while (ifs.good()) {
        getline(ifs, line);
        if ((line.compare(0,2,"G0")==0) || (line.compare(0,2, "G1")==0)) {
            int nx = getd(line,'X'); if (nx==0) nx=x;
            int ny = getd(line,'Y'); if (ny==0) ny=y;
            //cout << line << endl;
            // cout << x << ' ' << y << endl;
            if (line.compare(0,2,"G1")==0) {
                if (x==0) {
                    cout << line << endl;
                }
                myimage.draw(DrawableLine(x,y,nx,ny));
            }
            x = nx; y = ny;
            if (line.compare(0,14,"G1 Z15.0 F9000") !=0) {
                int nz = getd(line,'Z');
                if (nz !=0) {
                    filename(cnt, name);
                    myimage.write(name);
                    cnt++;
                    myimage.fillColor("black");
                    list<Coordinate> colist;
                    colist.push_back(Coordinate(0,0));
                    colist.push_back(Coordinate(1920,0));
                    colist.push_back(Coordinate(1920,1080));
                    colist.push_back(Coordinate(0,1080));
                    myimage.draw(DrawablePolyline(colist));
                    myimage.fillColor("white");
                }
            }
        }
    }
    filename(cnt, name);
    myimage.write(name);
    ifs.close();
    return 0;
}

int getd (string line, char s) {
    size_t fc = line.find(s);
    size_t fe = line.find(' ',fc+1);
    string str2 = line.substr (fc+1,fe-fc-1);
    const char *cstr = str2.c_str();
    float x1 = strtof(cstr,NULL) * 10;
    int x = (int) x1;
    return x;
}

void filename(int cnt, char * name) {
    sprintf(name, "image%5d.jpg",cnt);
    for (int x=0; x<strlen(name); x++) {
        if (name[x] == ' ') {
            name[x] = '0';
        }
    }
}
