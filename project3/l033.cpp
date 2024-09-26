#include <fstream>
#include <iostream>
#include <math.h>
#include <vector>
#include <ctime>
#include <algorithm>
#include <cfloat>
#include <iomanip>
#include <limits>
using namespace std;

//declare part3();
void part2();
void part3();

int main() {
    part2();
    part3();
}

//class for storing smallest distances, indexOne represents index of first point, indexTwo represents index of second point
class Small {
    public:
        int indexOne, indexTwo;
        double distance;
};

class Point {
    public:
        double x;
        double y; 
};

class Line {
    public:
        Point one;
        Point two;
        double distance;
};

//part2 section
Small recurPart2(vector<Point> &points, int low, int high);
double calcDistance(Point& one, Point& two);
vector<Point> readfile();
void file(vector<Point> points, double distance, double time, int size);
Small brute_force(vector<Point>& points, int one, int two);
bool sortByX(Point& one, Point& two);

//reads file
vector<Point> readfile() {
    ifstream stream;
    stream.open("points1m.txt");
    vector<Point> points;
    double x, y;
    Point temp;
    while (stream >> x >> y) {
        temp.x = x, temp.y = y;
        points.push_back(temp);
    }
    return points;
}

//sorts By X
bool sortByX(Point& one, Point& two) {
    return one.x < two.x;
}

//calculates distance
double calcDistance(Point &one, Point &two) {
    return sqrt(pow(two.x - one.x, 2) + pow(two.y - one.y, 2));
}

void part2() {
    vector<Point> points;
    //initialize the vector with the points from the points.txt file
    points = readfile();

    clock_t begin = clock();
    
    //sort the array by the X coordinate
    std::sort(points.begin(), points.end(), sortByX);

    Small smallest = recurPart2(points, 0, points.size() - 1);

    clock_t end = clock();

    vector<Point> temp; temp.push_back(points[smallest.indexOne]), temp.push_back(points[smallest.indexTwo]);

    file(temp, smallest.distance, double(end - begin) * 1000/ CLOCKS_PER_SEC, (int) points.size());
}

Small recurPart2(vector<Point> &points, int low, int high) {
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
    Small left = recurPart2(points, low, mid);
    Small right = recurPart2(points, mid+1, high);
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

//calcs min distance in strip
Small brute_force(vector<Point>& points, int one, int two) {
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

void file(vector<Point> points, double distance, double time, int size) {
    ofstream stream("results.txt");
    stream << "Number Of Points: " << size << endl;
    stream << "Part 2 Time: " << setprecision(23) << time << " ms" << endl;
    stream << "Part 2 Points: (" << points[0].x << "," << points[0].y << ") , (" << points[1].x << "," << points[1].y << ")" << endl;
    stream << "Part 2 Distance: " << distance << endl;
    stream.close();
    cout << "Number Of Points: " << size << endl;
    cout << "Part 2 Time: " << setprecision(23) << time << " ms" << endl;
    cout << "Part 2 Points: (" << points[0].x << "," << points[0].y << ") , (" << points[1].x << "," << points[1].y << ")" << endl;
    cout << "Part 2 Distance: " << distance << endl;
}


//part3 methods declaration
Line recurPart3(vector<Point> &sortedByX, vector<Point> &sortedByY, int low, int high);
bool sortByY(Point & one, Point & two);
void output(Line l, double time, int numberOfPoints);
//sorts By Y
bool sortByY(Point& one, Point& two) {
    return one.y < two.y;
}

//creates Line object
Line createLine(Point one, Point two, double distance) {
    Line x; x.one = one; x.two = two; x.distance = distance;
    return x;
}

//fully recursive method
Line recurPart3(vector<Point> &sortedByX, vector<Point> &sortedByY, int low, int high) {
    //2 points left
    if(high - low == 1) return createLine(sortedByX[low], sortedByX[high], calcDistance(sortedByX[low], sortedByX[high]));
    
    //3 points left
    else if(high - low == 2) {
        double d1 = calcDistance(sortedByX[low], sortedByX[low+1]), d2 = calcDistance(sortedByX[low], sortedByX[high]), d3 =           calcDistance(sortedByX[low+1], sortedByX[high]);
        double minDistance = min(min(d1, d2), d3);
        if (minDistance == d1) return createLine(sortedByX[low], sortedByX[low+1], minDistance);
        else if (minDistance == d2) return createLine(sortedByX[low], sortedByX[high], minDistance);
        else return createLine(sortedByX[low+1], sortedByX[high], minDistance);
    }
    
    //split array that is sorted By Y
    int mid = (low + high) / 2;
    vector<Point> y1, y2;
    for(int i = 0; i < sortedByY.size(); i++) {
        if(sortedByY[i].x >= sortedByX[low].x && sortedByY[i].x <= sortedByX[high].x) {
            sortedByY[i].x <= sortedByX[mid].x ? y1.push_back(sortedByY[i]) : y2.push_back(sortedByY[i]);
        }
    }
    
    //find the min distances of both the left and right sides of the array
    Line left = recurPart3(sortedByX, y1, low, mid), right = recurPart3(sortedByX, y2, mid+1, high);
    Line smaller = left.distance < right.distance ? left : right;
    
    //calculate the points in sortedByY that are in the strip
    double midX = sortedByX[mid].x, lowX = sortedByX[mid].x - smaller.distance, highX = sortedByX[mid].x + smaller.distance;
    vector<Point> inStrip;
    for(int i = 0; i < sortedByY.size(); i++) {
        if(sortedByY[i].x >= lowX && sortedByY[i].x <= highX) {
            inStrip.push_back(sortedByY[i]);
        }
    }
    
    //calculate and update the smallest distance within the strip
    for(int i = 0; i < inStrip.size(); i++) {
        for(int j = i+1; j < i+8; j++) {
            if(j >= inStrip.size()) break;
            if((inStrip[i].x < midX && inStrip[j].x > midX) || (inStrip[i].x > midX && inStrip[j].x < midX)) {
                    double temp = calcDistance(inStrip[i], inStrip[j]);
                    if(temp < smaller.distance) smaller = createLine(inStrip[i], inStrip[j], temp);
            }
        }
    }
    
    return smaller;
}

void part3() {
   vector<Point> sortedX = readfile();
   vector<Point> sortedY;
   std::sort(sortedX.begin(), sortedX.end(), sortByX);
   sortedY = sortedX;
   std::sort(sortedY.begin(), sortedY.end(), sortByY);
   clock_t begin = clock();
   Line min = recurPart3(sortedX, sortedY, 0, sortedX.size() - 1);
   clock_t end = clock();
   output(min, (double(end - begin) / CLOCKS_PER_SEC) * 1000, (int) sortedX.size());
}
void output(Line l, double time, int numberOfPoints) {
    ofstream stream("results.txt", ios::app);
    stream << "Number of Points: " << numberOfPoints << endl;
    stream << "Part 3 Time: " << setprecision(23) << time << " ms" << endl;
    stream << "Part 3 Points: (" << l.one.x << "," << l.one.y << ") , (" << l.two.x << "," << l.two.y << ")" << endl;
    stream << "Part 3 Distance: " << l.distance;
    stream.close();
    cout << "Number of Points: " << numberOfPoints << endl;
    cout << "Part 3 Time: " << setprecision(23) << time << " ms" << endl;
    cout << "Part 3 Points: (" << l.one.x << "," << l.one.y << ") , (" << l.two.x << "," << l.two.y << ")" << endl;
    cout << "Part 3 Distance: " << l.distance << endl;
}