#include <fstream>
#include <stdlib.h>
#include <iostream>
#include <iomanip>
#include <math.h>
using namespace std;

bool determineCollinear(float x[], float y[]);
bool determineInTriangle(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4);
void file(double x[3], double y[3], double x4, double y4);
double distance(double x1, double y1, double x2, double y2);
double randomm();
void part1();
//void part2();
int main() {
    part1();
    //part2();
    return 0;
}
// plan part 2
// have boolean variable for when line is vertical in line class
// make point class
void part1() {
    double x[3];
    double y[3];
    double x4;
    double y4;
    bool convex = true;
    for(int i = 0; i < 3; i++) {
        x[i] = randomm();
        y[i] = randomm();
    }
    while(convex) {
        x4 = randomm();
        y4 = randomm();
        convex = determineInTriangle(x[0], y[0], x[1], y[1], x[2], y[2], x4, y4) || 
        determineInTriangle(x[1], y[1], x[2], y[2], x4, y4, x[0], y[0]) ||
        determineInTriangle(x[0], y[0], x[2], y[2], x4, y4, x[1], y[1]) ||
        determineInTriangle(x[0], y[0], x[1], y[1], x4, y4, x[2], y[2]);       
    }
    file(x, y, x4, y4);
}
void file(double x[3], double y[3], double x4, double y4) {
    ofstream stream;
    stream.open("points.txt");
    stream << "(" << std::setprecision(17) << x[0] << "," << std::setprecision(17) << y[0] << ") ," << endl;
    stream << "(" << std::setprecision(17) << x[1] << "," << std::setprecision(17) << y[1] << ") ," << endl;
    stream << "(" << std::setprecision(17) << x[2] << "," << std::setprecision(17) << y[2] << ") ," << endl;
    stream << "(" << std::setprecision(17) << x4 << "," << std::setprecision(17) << y4 << ")" << endl;
    stream.close();
}
double randomm() {
    static bool one = true;
    if(one) {
        srand(time(NULL));
        one = false;
    }
    return (double) rand()/RAND_MAX;
} 
double determinearea(double area[3][3]) {
   double t1 = (area[1][1] * area[2][2])-(area[1][2] * area[2][1]);
   double t2 = (area[1][0] * area[2][2])-(area[1][2] * area[2][0]);
   double t3 = (area[1][0] * area[2][1])-(area[1][1] * area[2][0]);
   return fabs((area[0][0]*t1) + (-area[0][1]*t2) + (area[0][2]*t3)) * 0.5;
}
double distance(double x1, double y1, double x2, double y2) {
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2) * 1.0);
}
bool determineInTriangle(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4) {
    double totalareaarray[3][3] = {{x1,y1,1},{x2,y2,1},{x3,y3,1}};
    double totalarea = determinearea(totalareaarray);

    double area1array[3][3] = {{x4,y4,1},{x2,y2,1},{x3,y3,1}};
    double area1 = determinearea(area1array);

    double area2array[3][3] = {{x1,y1,1},{x4, y4,1},{x3,y3,1}};
    double area2 = determinearea(area2array);

    double area3array[3][3] = {{x1,y1,1},{x2,y2,1},{x4,y4,1}};
    double area3 = determinearea(area3array);
    return (totalarea > area1 + area2 + area3 - 0.00000000000000001) && (totalarea < area1 + area2 + area3 + 0.00000000000000001);
}

