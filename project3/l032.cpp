#include <fstream>
#include <list>
#include <iostream>
#include <math.h>
#include <vector>
#include <ctime>
#include <algorithm>
#include <cfloat>
#include <iomanip>
#include <limits>
using namespace std;

void part1(double size);
void part2();

//class for storing smallest distances, indexOne represents index of first point, indexTwo represents index of second point
class Small {
    public:
        int indexOne, indexTwo;
        double distance;
};


//class for storing points
class Point {
    public:
        double x;
        double y; 
};


int main() {
    //controls number of points 
    double size = 10000;

    part1(size);
    part2();
    return 0;
}

//part1 methods
void illuminate(bool red, int a[800][2400], int x, int y);
double randommm();
void file(int a[800][2400], list<Point> points, double finalx1, double finaly1, double finalx2, double finaly2, double dist, double time, double numberOfPoints);
void circle(bool red, int a[800][2400], double radius, double xc, double yc);
void drawcircle(bool red, int a[800][2400], int x, int y, int xc, int yc);


void part1(double size) {
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
    Point l;
    for (int i = 0; i < size; i++) {
     l.x = randommm(), l.y = randommm();
     myList.push_back(l);
    }
    // clock_t begin = clock();
    // for(iterator1 = myList.begin(); iterator1 != myList.end(); ++iterator1) {
    //     for(iterator2 = iterator1; iterator2 != myList.end(); ++iterator2) {
    //         if(iterator1 != iterator2) {
    //             temp1 = *iterator1;
    //             temp2 = *iterator2;
    //             illuminate(red, a, (int) (temp1.x * 800), (int) (temp1.y * 800));
    //             illuminate(red, a, (int) (temp2.x * 800), (int) (temp2.y * 800));
    //             circle(red, a, 3, (int) (temp1.x * 800), (int) (temp1.y * 800));
    //             circle(red, a, 3, (int) (temp2.x * 800), (int) (temp2.y * 800));
    //             distancetemp = sqrt(pow(temp2.y - temp1.y, 2) + pow(temp2.x - temp1.x, 2));
    //             if(distancetemp < mindistance) {
    //                 mindistance = distancetemp;
    //                 finalx1 = temp1.x;
    //                 finalx2 = temp2.x;
    //                 finaly1 = temp1.y;
    //                 finaly2 = temp2.y;
    //             }
    //         } 
    //     }
    // }
    // clock_t end = clock();
    // red = true;
    // illuminate(true, a, (int) (finalx1 * 800), (int) (finaly1 * 800));
    // illuminate(true, a, (int) (finalx2 * 800), (int) (finaly2 * 800));
    // circle(red, a, 3, (int) (finalx1 * 800), (int) (finaly1*800));
    // circle(red, a, 3, (int) (finalx2*800), (int) (finaly2*800));
    file(a, myList, finalx1, finaly1, finalx2, finaly2, mindistance, 0, size);
}

//generate random points
double randommm() {
    static bool one = true;
    if(one) {
        srand(time(NULL));
        one = false;
    }
    return (double) rand()/RAND_MAX;
}

//write output to file
void file(int a[800][2400], list<Point> points, double finalx1, double finaly1, double finalx2, double finaly2, double dist, double time, double numberOfPoints) {
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
        stream2 << setprecision(23) << temp.x << "  " << temp.y << endl;
    }
    stream2.close();
    ofstream stream3;
    stream3.open("results.txt");
    stream3 << "Number of Points: " << numberOfPoints << endl;
    stream3 << "Brute Force Time: " << time << " ms" << endl;
    stream3 << "Brute Force Points: (" << finalx1 << "," << finaly1 << ") , (" << finalx2 << "," << finalx2 << ")" << endl;
    stream3 << "Brute Force Distance: " << dist << endl;
    stream3.close();
    cout << "Number of Points: " << numberOfPoints << endl;
    cout << "Brute Force Time: " << time << " ms" << endl;
    cout << "Brute Force Points: (" << finalx1 << "," << finaly1 << ") , (" << finalx2 << "," << finaly2 << ")" << endl;
    cout << "Brute Force Distance: " << dist << endl;
}

//draw a circle given center with coordinates xc and yc and a radius
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

//circle helper method
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

//illuminate point with coordinates x, y
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


//*******PART 2 SECTION******

//part2 methods
double calcDistance(Point one, Point two);
Small brute_force(vector<Point> points);
Small recur(vector<Point> points, int low, int high);
vector<Point> readfile();
bool sortByX(Point & one, Point & two);
void file2(vector<Point> points, double distance);

//finds the smallest distance in an array of points
Small brute_force(vector<Point> points, int one, int two) {
    double minDistance = DBL_MAX, index1 = 0, index2 = 0;
    for(int i = one; i <= two; i++) {
        for(int j = i+1; j <= two; j++) {
            if(calcDistance(points[i], points[j]) < minDistance) {
                minDistance = calcDistance(points[i], points[j]), index1 = i, index2 = j;
            }
        }
    }
    Small ans;
    ans.indexOne = index1, ans.indexTwo = index2, ans.distance = minDistance;
    return ans;
}

void file2(vector<Point> points, double distance, double time) {
    ofstream stream("results.txt", ios::app);
    stream << "Recur Time: " << time << " ms" << endl;
    stream << "Recur Points: (" << points[0].x << "," << points[0].y << ") , (" << points[1].x << "," << points[1].y << ")" << endl;
    stream << "Recur Distance: " << distance << endl;
    stream.close();
    cout << "Recur Time: " << time << " ms" << endl;
    cout << "Recur Points: (" << points[0].x << "," << points[0].y << ") , (" << points[1].x << "," << points[1].y << ")" << endl;
    cout << "Recur Distance: " << distance << endl;
}

// calculates distance given two points
double calcDistance(Point one, Point two) {
    return sqrt(pow(two.x - one.x, 2) + pow(two.y - one.y, 2));
}

//reads points from file that part1 generated
vector<Point> readfile() {
    ifstream stream;
    stream.open("points.txt");
    double temp1, temp2;
    Point l;
    vector<Point> points;
    while(stream >> temp1 >> temp2) {
        l.x = temp1, l.y = temp2;
        points.push_back(l);
    }
    return points;
}

//uses recursive approach to find min distance
Small recur(vector<Point> points, int low, int high) {
    //if there are only two points
    if(high - low == 1) {
        Small ans; ans.indexOne = low, ans.indexTwo = high, ans.distance = calcDistance(points[low], points[high]);
        return ans;
    }

    //if there are three points
    else if(high - low == 2) {
        return brute_force(points, low, high);
    }
    //find min distance of both sides of array
    int mid = (low + high)/2;
    Small left = recur(points, low, mid);
    Small right = recur(points, mid+1, high);
    Small smallest;
    if(left.distance < right.distance) smallest = left;
    else smallest = right;
    //initialize the indices of the points that are in the strip
    bool first = false;
    int one, two = 0;
    double lowX = points[mid].x - smallest.distance, highX = points[mid].x + smallest.distance;
    vector<Point>::iterator i;
    vector<Point>::iterator endIterator = points.begin() + high + 1;
    for (i = points.begin() + low; i != endIterator; ++i) {
        Point temp = *i;
        if(temp.x >= lowX && temp.x <= highX && first == false) {
            one = i - points.begin(), first = true;
        }
        if(temp.x > highX && first == true) {
            two = i- points.begin() -1;
            break;
        }
    }
    if(two == 0) {
        two = high;
    }

    //find the minimum distance in the strip  
    Small minStrip = brute_force(points, one, two);

    //return the smaller distance
    if(minStrip.distance < smallest.distance) {
        return minStrip;
    }
    else {
        return smallest;
    }
}

//helper method for std::sort
bool sortByX(Point & one, Point & two) {
    return one.x < two.x;
}

void part2() {
    vector<Point> points;
    //initialize the vector with the points from the points.txt file
    points = readfile();

    clock_t begin = clock();
    
    //sort the array by the X coordinate
    std::sort(points.begin(), points.end(), sortByX);

    Small smallest = recur(points, 0, points.size() - 1);

    clock_t end = clock();

    vector<Point> temp; temp.push_back(points[smallest.indexOne]), temp.push_back(points[smallest.indexTwo]);

    file2(temp, smallest.distance, double(end - begin) * 1000/ CLOCKS_PER_SEC);
}