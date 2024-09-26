#include <fstream>
#include <list>
#include <iostream>
#include <math.h>
#include <vector>
#include <ctime>
#include <algorithm>
using namespace std;
void illuminate(bool red, int a[800][2400], int x, int y);
double randommm();
void file(int a[800][2400]);
void part1();
void circle(bool red, int a[800][2400], double radius, double xc, double yc);
void drawcircle(bool red, int a[800][2400], int x, int y, int xc, int yc);
void file2(int a[800][2400]);
// Class to store the min points
class SmallestDist {
    public:
        SmallestDist(int index1, int index2, double myDistance);
        SmallestDist();
        void setIndex1(int myIndex) {
            one = myIndex;
        }
        void setIndex2(int myIndex) {
            two = myIndex;
        }
        void setDistance(double myDistance) {
            dist = myDistance;
        }
        int getOne() {
            return one;
        }
        int getTwo() {
            return two;
        }
        double getDistance() {
            return dist;
        }
    private:
        int one, two;
        double dist;
};
SmallestDist::SmallestDist(int index1, int index2, double myDistance) {
    one = index1;
    two = index2;
    dist = myDistance;
}
SmallestDist::SmallestDist() {
    one = 0, two = 0, dist = 0;
}
//class to store other points
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

double calcDistance(Point one, Point two);
SmallestDist recur(vector<Point> points, int low, int high);
bool sortByX(Point & one, Point & two);
vector<Point> read_file();
void part2();

int main() {
    part1();
    part2();
    return 0;
}

double calcDistance(Point one, Point two) {
   // cout << sqrt(pow(two.getX() - one.getX(), 2) + pow(two.getY() - one.getY(), 2)) << " ";
    return sqrt(pow(two.getX() - one.getX(), 2) + pow(two.getY() - one.getY(), 2));
}
SmallestDist recur(vector<Point> points, int low, int high) {
    //if there are two points
    if(high - low == 1) {
        return SmallestDist(low, high, calcDistance(points[low], points[high]));
    }

    //if there are 3 points 
    else if(high - low == 2) {
        double d1 = calcDistance(points[low], points[low+1]);
        double d2 = calcDistance(points[low], points[high]);
        double d3 = calcDistance(points[low+1], points[high]);
        double minDistance = min(min(d1, d2), d3);
        if(d1 == minDistance) {
            return SmallestDist(low, low+1, d1);
        }
        else if(d2 == minDistance) {
            return SmallestDist(low, high, d2);
        }
        else {
            return SmallestDist(low+1, high, d3);
        }
    }

    //calculate minimum distance of both sides of the array  
    int mid = (low+high)/2;
    SmallestDist leftSmall = recur(points, low, mid);
    SmallestDist rightSmall = recur(points, mid+1, high);
    SmallestDist globalSmall;
    if(leftSmall.getDistance() < rightSmall.getDistance()) {globalSmall = leftSmall;}
    else {globalSmall = rightSmall;}
    double minDistance = globalSmall.getDistance();

    //make vector with all points in the middle line
    double lineXCoordinate;
    if((low+high) % 2 == 0) {lineXCoordinate = (2 * points[mid].getX() + 1) / 2;}
    else {lineXCoordinate = points[mid].getX();}

    double lowX = lineXCoordinate - minDistance;
    double highX = lineXCoordinate + minDistance;
    vector <Point>::iterator pointsiterator;
    vector <Point> inLine;
    Point temp;
    for(pointsiterator = points.begin(); pointsiterator != points.end(); ++pointsiterator) {
        temp = *pointsiterator;
        if (temp.getX() > lowX && temp.getX() < highX) {
            inLine.push_back(*pointsiterator);
        }
    }
    vector <Point>::iterator lineIterator1;
    vector <Point>::iterator lineIterator2;
    //find min distance within the strip
    double minDistanceInStrip = 1000000000;
    double finalIndexOne = 0;
    double finalIndexTwo = 0;
    double tempdistance = 0;
    for(int i = 0; i < inLine.size(); i++) {
        for(int j = i+1; j < inLine.size(); j++) {
            if(i != j) {
                tempdistance = calcDistance(inLine[i], inLine[j]);
                if(tempdistance < minDistanceInStrip) {
                    minDistance = tempdistance;
                    finalIndexOne = i;
                    finalIndexTwo = j;
                }
            }
        }
    } 
    //return the smallest of the two distances with indexes
    if(minDistanceInStrip < minDistance) {
        return SmallestDist(finalIndexOne, finalIndexTwo, minDistanceInStrip);
    }
    else {
        return globalSmall;
    }

}

bool sortByX(Point & one, Point & two) {
    return one.getX() < two.getX();
}
vector<Point> read_file() {
    ifstream stream;
    stream.open("points.txt");
    double temp1, temp2;
    vector<Point> points;
    while(stream >> temp1 >> temp2) {
        points.push_back(Point(temp1, temp2));
    }
    return points;
}
void part2() {
    int size = 60;
    vector<Point> points;
    points = read_file();
    // for(int i = 0; i < 60; i++) {
    //     cout << points[i].getX() << " " << points[i].getY() << endl;
    // }
    std::sort(points.begin(), points.end(), sortByX);
    SmallestDist smallest = recur(points, 0, size - 1);
    Point pointone = points[smallest.getOne()];
    Point pointtwo = points[smallest.getTwo()];
    double minDistance = smallest.getDistance();
    cout << minDistance;
    int a[800][2400];
    for(int i = 0; i < 800; i++) {
		for (int j = 0; j < 2400; j++) {
            a[i][j] = 1;
        }
    } 
    illuminate(true, a, (int)(pointone.getX() * 800), (int) (pointone.getY() * 800));
    illuminate(true, a, (int)(pointtwo.getX() * 800), (int) (pointtwo.getY() * 800));
    circle(true, a, 3, (int) (pointone.getX() * 800), (int) (pointone.getY() * 800));
    circle(true, a, 3, (int) (pointtwo.getX() * 800), (int) (pointtwo.getY() * 800));
    file2(a);
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
void file2(int a[800][2400]) {
    ofstream stream1;
    stream1.open("points2.ppm");
    stream1 << "P3 800 800 1" << endl;
    for (int i = 0; i < 800; i++) {
        for(int j = 0; j < 2400; j++) {
            stream1 << to_string(a[i][j]) + " ";
        }
        stream1 << endl;
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
    double size = 60;
    for (int i = 0; i < size; i++) {
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
    cout << mindistance << endl;
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
