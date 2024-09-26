#include <fstream>
#include <list>
#include <iostream>
#include <math.h>
using namespace std;
void illuminate(bool red, int a[800][2400], int x, int y);
double randommm();
void file(int a[800][2400]);
void part1();
void circle(bool red, int a[800][2400], double radius, double xc, double yc);
void drawcircle(bool red, int a[800][2400], int x, int y, int xc, int yc);
class Point{
    public:
        Point(double myx, double myy);
        Point();
        void setX(double myX) {
            x = myX;
        }
        void setY(double myY) {
            y = myY;
        }
        double getX() {
            return x;
        }
        double getY() {
            return y;
        }
    private:
        double x, y;
};
Point::Point(double myX, double myY) {
    x = myX;
    y = myY;
}
Point::Point() {
    x = 0;
    y = 0;
}
int main() {
    part1();
    return 0;
}
double randommm() {
    static bool one = true;
    if(one) {
        srand(time(NULL));
        one = false;
    }
    return (double) rand()/RAND_MAX;
}
void file(int a[800][2400], list<Point> points) {
    ofstream stream1;
    stream1.open("points.ppm");
    stream1 << "P3 800 800 1" << endl;
    for (int i = 0; i < 800; i++) {
        for(int j = 0; j < 2400; j++) {
            stream1 << to_string(a[i][j]) + " ";
        }
        stream1 << endl;
    }
    stream1.close();
    ofstream stream2;
    stream2.open("points.txt");
    list <Point>::iterator iterators;
    Point temp;
    for(iterators = points.begin(); iterators != points.end(); ++iterators) {
        temp = *iterators;
        stream2 << temp.getX() << "  " << temp.getY() << endl;
    }
}
void part1() {
    double finalx1, finaly1, finalx2, finaly2;
    double mindistance = 1000000000000;
    Point temp1;
    Point temp2;
    double distancetemp = 0;
    list <Point> myList;
    list <Point>::iterator iterator1;
    list <Point>::iterator iterator2;
    bool red = false;
    int a[800][2400] = {};
	for(int i = 0; i < 800; i++) {
		for (int j = 0; j < 2400; j++) {
            a[i][j] = 1;
        }
    } 
    for (int i = 0; i < 60; i++) {
     myList.push_back(Point(randommm(), randommm()));
    }
    for(iterator1 = myList.begin(); iterator1 != myList.end(); ++iterator1) {
        for(iterator2 = iterator1; iterator2 != myList.end(); ++iterator2) {
            if(iterator1 != iterator2) {
                temp1 = *iterator1;
                temp2 = *iterator2;
                illuminate(red, a, (int) (temp1.getX() * 800), (int) (temp1.getY() * 800));
                illuminate(red, a, (int) (temp2.getX() * 800), (int) (temp2.getY() * 800));
                circle(red, a, 3, (int) (temp1.getX() * 800), (int) (temp1.getY() * 800));
                circle(red, a, 3, (int) (temp2.getX() * 800), (int) (temp2.getY() * 800));
                distancetemp = sqrt(pow(temp2.getY() - temp1.getY(), 2) + pow(temp2.getX() - temp1.getX(), 2));
                if(distancetemp < mindistance) {
                    mindistance = distancetemp;
                    finalx1 = temp1.getX();
                    finalx2 = temp2.getX();
                    finaly1 = temp1.getY();
                    finaly2 = temp2.getY();
                }
            } 
        }
    }
    red = true;
    illuminate(true, a, (int) (finalx1 * 800), (int) (finaly1 * 800));
    illuminate(true, a, (int) (finalx2 * 800), (int) (finaly2 * 800));
    circle(red, a, 3, (int) (finalx1 * 800), (int) (finaly1*800));
    circle(red, a, 3, (int) (finalx2*800), (int) (finaly2*800));
    file(a, myList);
}
void drawcircle(bool red, int a[800][2400], int x, int y, int xc, int yc) {
    if(!(int(xc+x) < 0 || int(xc+x) > 799 || int(yc+y) < 0 || int(yc+y) > 799)) 
        illuminate(red, a, int(xc+x), int(yc+y)); 

    if(!(int(xc-x) < 0 || int(xc-x) > 799 || int(yc+y) < 0 || int(yc+y) > 799)) 
        illuminate(red, a, int(xc-x), int(yc+y));

    if(!(int(xc+x) < 0 || int(xc+x) > 799 || int(yc-y) < 0 || int(yc-y) > 799)) 
        illuminate(red, a, int(xc+x), int(yc-y)); 

    if(!(int(xc-x) < 0 || int(xc-x) > 799 || int(yc-y) < 0 || int(yc-y) > 799)) 
        illuminate(red, a, int(xc-x), int(yc-y)); 
    
    if(!(int(xc+y) < 0 || int(xc+y) > 799 || int(yc+x) < 0 || int(yc+x) > 799)) 
        illuminate(red, a, int(xc+y), int(yc+x));
    
    if(!(int(xc-y) < 0 || int(xc-y) > 799 || int(yc+x) < 0 || int(yc+x) > 799)) 
        illuminate(red, a, int(xc-y), int(yc+x));

    if(!(int(xc+y) < 0 || int(xc+y) > 799 || int(yc-x) < 0 || int(yc-x) > 799)) 
        illuminate(red, a, int(xc+y), int(yc-x));

    if(!(int(xc-y) < 0 || int(xc-y) > 799 || int(yc-x) < 0 || int(yc-x) > 799)) 
        illuminate(red, a, int(xc-y), int(yc-x));
}
void circle(bool red, int a[800][2400], double radius, double xc, double yc) {
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
        drawcircle(red, a, x, y, xc, yc);
        y2_new -= (2 * x) - 3;
    }
}
void illuminate(bool red, int a[800][2400], int x, int y) {
    if(red == false) {
        for(int i = 0; i < 3; i++) {
            a[y][3*x+i] = 0;
        }
    }
    else {
        for(int i = 0; i < 3; i++) {
            if(i == 0) {
                a[y][3*x+i] = 1;
            }
            else {
                a[y][3*x+i] = 0;
            }
        }
    }
}
