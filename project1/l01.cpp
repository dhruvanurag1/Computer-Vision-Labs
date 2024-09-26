/* l01.cpp */
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <vector>
#include <string>
#include <math.h>
using namespace std;
void file(int a[800][2400]);
void line(int a[800][2400], int x1,int y1,int x2,int y2);
void random(int a[800][2400]);
void illuminate(int a[800][2400], int x, int y);
void circle(int a[800][2400], double radius, double xc, double yc);
void ninepointcircle(int a[800][2400], double circumx, double circumy, double circumradius, int columns[], int rows[]);
int main()
{
    int a[800][2400] = {};
	for(int i = 0; i < 800; i++) {
		for (int j = 0; j < 2400; j++) {
            a[i][j] = 1;
        }
    } 
    random(a);
    file(a);
    return 0;
}
void file(int a[800][2400]) {
    ofstream stream;
    stream.open("result.ppm");
    stream << "P3 800 800 1" << endl;
    for (int i = 0; i < 800; i++) {
        for(int j = 0; j < 2400; j++) {
            stream << std::to_string(a[i][j]) + " ";
        }
        stream << endl;
    }
    stream.close();

}
void circumcenter(int a[800][2400],int columns[], int rows[]){
    double distance1 = sqrt(pow(rows[2] - rows[1], 2) + pow(columns[2] - columns[1], 2));
    double distance2 = sqrt(pow(rows[0] - rows[2], 2) + pow(columns[0] - columns[2], 2));
    double distance3 = sqrt(pow(rows[1] - rows[0], 2) + pow(columns[1] - columns[0], 2));
    double semiperimeter = (distance1 + distance2 + distance3)/2;
    double radius = (distance1 * distance2 * distance3) / (4 * semiperimeter * sqrt(((semiperimeter - distance1) * (semiperimeter-distance2) * (semiperimeter-distance3))/semiperimeter));
    double x = (( (pow(columns[0], 2) + pow(rows[0], 2)) * (rows[1] - rows[2]) + (pow(columns[1], 2) + pow(rows[1], 2)) * (rows[2]-rows[0]) + (pow(columns[2], 2) + pow(rows[2], 2)) * (rows[0] - rows[1])) 
                / (2 * (columns[0] * (rows[1] - rows[2]) + columns[1] * (rows[2] - rows[0]) + columns[2] * (rows[0] - rows[1]))));
    double y = (( (pow(columns[0], 2) + pow(rows[0], 2)) * (columns[2] - columns[1]) + (pow(columns[1], 2) + pow(rows[1], 2)) * (columns[0]-columns[2]) + (pow(columns[2], 2) + pow(rows[2], 2)) * (columns[1] - columns[0])) 
                / (2 * (columns[0] * (rows[1] - rows[2]) + columns[1] * (rows[2] - rows[0]) + columns[2] * (rows[0] - rows[1]))));
    circle(a, radius, x, y);
    ninepointcircle(a, x, y, radius, columns, rows);
}
void incircle(int a[800][2400], int columns[], int rows[]) {
    double distance1 = sqrt(pow(rows[2] - rows[1], 2) + pow(columns[2] - columns[1], 2));
    double distance2 = sqrt(pow(rows[0] - rows[2], 2) + pow(columns[0] - columns[2], 2));
    double distance3 = sqrt(pow(rows[1] - rows[0], 2) + pow(columns[1] - columns[0], 2));
    double semiperimeter = (distance1 + distance2 + distance3)/2;
    double radius = sqrt(((semiperimeter - distance1) * (semiperimeter - distance2) * (semiperimeter - distance3))/semiperimeter);
    double x = (distance1 * columns[0] + distance2 * columns[1] + distance3 * columns[2])/(distance1+distance2+distance3);
    double y = (distance1 * rows[0] + distance2 * rows[1] + distance3 * rows[2])/(distance1+distance2+distance3);
    circle(a, radius, x, y);
}
void ninepointcircle(int a[800][2400], double circumx, double circumy, double circumradius, int columns[], int rows[]) {
    double radius = circumradius/2;
    double centroidx = (columns[0] + columns[1] + columns[2])/3;
    double centroidy = (rows[0] + rows[1] + rows[2])/3;
    double orthox = 3*centroidx - 2*circumx;
    double orthoy = 3*centroidy - 2*circumy;
    double x = (circumx + orthox)/2;
    double y = (circumy + orthoy)/2;
    circle(a, radius, x, y);
}
void eulerline(int a[800][2400], int columns[], int rows[]) {
    double x1 = (( (pow(columns[0], 2) + pow(rows[0], 2)) * (rows[1] - rows[2]) + (pow(columns[1], 2) + pow(rows[1], 2)) * (rows[2]-rows[0]) + (pow(columns[2], 2) + pow(rows[2], 2)) * (rows[0] - rows[1])) 
                / (2 * (columns[0] * (rows[1] - rows[2]) + columns[1] * (rows[2] - rows[0]) + columns[2] * (rows[0] - rows[1]))));
    double y1 = (( (pow(columns[0], 2) + pow(rows[0], 2)) * (columns[2] - columns[1]) + (pow(columns[1], 2) + pow(rows[1], 2)) * (columns[0]-columns[2]) + (pow(columns[2], 2) + pow(rows[2], 2)) * (columns[1] - columns[0])) 
                / (2 * (columns[0] * (rows[1] - rows[2]) + columns[1] * (rows[2] - rows[0]) + columns[2] * (rows[0] - rows[1]))));
    double x2 = (columns[0] + columns[1] + columns[2])/3;
    double y2 = (rows[0] + rows[1] + rows[2])/3;
    double slope = (y2-y1)/(x2-x1);
    double yintercept = y1 - slope*x1;
    double end = slope * 799 + yintercept;
    double finalx1 = 0;
    double finaly1 = int(yintercept);
    double finalx2 = 799;
    double finaly2 = int(end);
    line(a, finalx1, finaly1, finalx2, finaly2);
}
void illuminate(int a[800][2400], int x, int y) {
    for(int i = 0; i < 3; i++) {
        a[y][3*x+i] = 0;
    }
}
void random(int a[800][2400])
{
    int columns[3]; 
    int rows[3]; 
    int min = 0;
    int max = 800;
    srand(time(0));
    for(int i = 0; i < 3; i++) {
        columns[i] = min + rand() % ((max + 1) - min);
        rows[i] = min + rand() % ((max + 1) - min);
    }
    line(a, columns[0], rows[0], columns[1], rows[1]);
	line(a, columns[1], rows[1], columns[2], rows[2]);
	line(a, columns[2], rows[2], columns[0], rows[0]);
    eulerline(a, columns, rows);
    circumcenter(a, columns, rows);
    incircle(a, columns, rows);
} 
void drawcircle(int a[800][2400], int x, int y, int xc, int yc) {
    if(!(int(xc+x) < 0 || int(xc+x) > 799 || int(yc+y) < 0 || int(yc+y) > 799)) 
        illuminate(a, int(xc+x), int(yc+y)); 

    if(!(int(xc-x) < 0 || int(xc-x) > 799 || int(yc+y) < 0 || int(yc+y) > 799)) 
        illuminate(a, int(xc-x), int(yc+y));

    if(!(int(xc+x) < 0 || int(xc+x) > 799 || int(yc-y) < 0 || int(yc-y) > 799)) 
        illuminate(a, int(xc+x), int(yc-y)); 

    if(!(int(xc-x) < 0 || int(xc-x) > 799 || int(yc-y) < 0 || int(yc-y) > 799)) 
        illuminate(a, int(xc-x), int(yc-y)); 
    
    if(!(int(xc+y) < 0 || int(xc+y) > 799 || int(yc+x) < 0 || int(yc+x) > 799)) 
        illuminate(a, int(xc+y), int(yc+x));
    
    if(!(int(xc-y) < 0 || int(xc-y) > 799 || int(yc+x) < 0 || int(yc+x) > 799)) 
        illuminate(a, int(xc-y), int(yc+x));

    if(!(int(xc+y) < 0 || int(xc+y) > 799 || int(yc-x) < 0 || int(yc-x) > 799)) 
        illuminate(a, int(xc+y), int(yc-x));

    if(!(int(xc-y) < 0 || int(xc-y) > 799 || int(yc-x) < 0 || int(yc-x) > 799)) 
        illuminate(a, int(xc-y), int(yc-x));
}
void circle(int a[800][2400], double radius, double xc, double yc) {
    int x, y, xmax, y2, y2_new, ty;
    xmax = (int) (radius * 0.70710678);
    y = radius;
    y2 = y * y;
    ty = (2 * y) - 1; 
    y2_new = y2;
    for (x = 0; x <= xmax+1; x++) {
        if ((y2 - y2_new) >= ty) {
            y2 -= ty;     
            y -= 1;
            ty -= 2;
        }
        drawcircle(a, x, y, xc, yc);
        y2_new -= (2 * x) - 3;
    }
}
void line(int a[800][2400], int x1,int y1,int x2,int y2) {
    bool ymorethanx = abs(y2-y1) > abs(x2-x1);   
    if(ymorethanx) {
        swap(x1, y1);
        swap(x2, y2);
    }
    if(x1 > x2) {
        swap(x1, x2);
        swap(y1, y2);
    }
    int j = y1;
    int deltax = abs(x2-x1);
    int deltay = abs(y2-y1);
    int step = 1;
    if(y1 < y2) {
        step = 1;
    }
    else {
        step = -1;
    }
    int error = deltay - deltax;
    for(int i = x1; i <= x2; i++) {
        if(ymorethanx) {
            if(i <= 799 && i >= 0)
                illuminate(a, j, i);
        }
        else {
            if(j <= 799 && j >= 0)
                illuminate(a, i, j);
        }
        if(error >= 0) {
            j += step;
            error -= deltax;
        }
        error += deltay;
    }
}