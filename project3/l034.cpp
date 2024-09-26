#include <fstream>
#include <iostream>
#include <math.h>
#include <vector>
#include <ctime>
#include <algorithm>
#include <cfloat>
#include <iomanip>
#include <limits>
#include <random>
#include <unordered_map>
using namespace std;

struct hash_pair {
    template <class T1, class T2>
    size_t operator()(const pair<T1, T2>& p) const
    {
        auto hash1 = hash<T1>{}(p.first);
        auto hash2 = hash<T2>{}(p.second);
        return hash1 ^ hash2;
    }
};

class Point {
    private:
        double x;
        double y; 
    public:
        double getX() {return x;}
        double getY() {return y;}
        void createPoint(double myX, double myY) {x = myX, y = myY;}
};

class Line {
    public:
        Point one;
        Point two;
        double distance;
};

double randommm(int);
double calcDistance(Point &, Point &);
vector<Point> KnuthShuffle(vector<Point>& );
vector<Point> readfile();
void part4();
vector<int> closestSubsquares(unordered_map<pair<unsigned long long, unsigned long long>, int, hash_pair>&, pair<unsigned long long, unsigned long long>&);
pair<unsigned long long, unsigned long long> calcCoordinate(double, double, double);
unordered_map<pair<unsigned long long, unsigned long long>, int, hash_pair> createMap(double, vector<Point>&, int);

void part3();

int main()  {
    part3();
    part4();
    return 0;
}
double randommm(int size) {
    static bool one = true;
    if(one) {
        srand(time(NULL));
        one = false;
    }
    return rand() % size;
}

double calcDistance(Point &one, Point &two) {
    return sqrt(pow(two.getX() - one.getX(), 2) + pow(two.getY() - one.getY(), 2));
}

vector<Point> KnuthShuffle(vector<Point>& points) {
    for(int i = 0; i < points.size(); i++) {
        iter_swap(points.begin() + i, points.begin() + (int) (randommm(points.size())));
    }
    return points;
}
vector<Point> readfile() {
    ifstream stream;
    stream.open("points.txt");
    vector<Point> points;
    double x, y;
    Point temp;
    while (stream >> x >> y) {
        temp.createPoint(x, y); points.push_back(temp);
    }
    return points;
}
void part4() {
    vector<Point> points = readfile();
    clock_t begin = clock();
    Point one = points[0], two = points[1];
    points = KnuthShuffle(points);
    double tempLambda;
    double lambda = calcDistance(points[0], points[1]), prevLambda = calcDistance(points[0], points[1]);
    unordered_map<pair<unsigned long long, unsigned long long>, int, hash_pair> map;
    pair<unsigned long long, unsigned long long> subsquare;
    for(int i = 0; i < points.size(); i++) {
        subsquare = calcCoordinate(lambda/2, points[i].getX(), points[i].getY());
        vector<int> close = closestSubsquares(map, subsquare);
        if (close.size() > 0) {
            for(int c = 0; c < close.size(); c++) {
               int j = close[c];
               tempLambda = calcDistance(points[i], points[j]);
               if (tempLambda < lambda) {
                   lambda = tempLambda, one = points[i], two = points[j];
                   map = createMap(lambda/2, points, i);
               }
               else {
                   map[subsquare] = i;
               }
            }
        }
        else {
            map[subsquare] = i;
        }
    }
    ofstream stream; 
    stream.open("results.txt", ios::app);
    stream << "Number of Points: " << points.size() << endl << setprecision(23) << "Part 4 Time: " << 1000 * (double) (clock() - begin) / CLOCKS_PER_SEC << " ms" << endl;
    stream << "Part 4 Points: " << setprecision(23) << "(" << one.getX() << "," << one.getY() << ") , (" << two.getX() << "," << two.getY() << ")" << endl;
    stream << "Part 4 Distance: " << lambda;
    cout << "Number of Points: " << points.size() << endl << "Part 4 Time: " << 1000 * (double) (clock() - begin) / CLOCKS_PER_SEC << endl;
    cout << "Part 4 Points: " << setprecision(23) << "(" << one.getX() << "," << one.getY() << ") , (" << two.getX() << "," << two.getY() << ")" << endl;
    cout << "Part 4 Distance: " << lambda << endl;
    stream.close();
}
vector<int> closestSubsquares(unordered_map<pair<unsigned long long, unsigned long long>, int, hash_pair>& map, pair<unsigned long long, unsigned long long>& temp) {
       vector<int> l;
       for(int i = -2; i <= 2; i++) for(int j = -2; j <= 2; j++) if(map.find(make_pair(temp.first + i, temp.second + j)) != map.end()) l.push_back((*map.find(make_pair(temp.first + i, temp.second + j))).second);
       return l; 
}
pair<unsigned long long, unsigned long long> calcCoordinate(double lambda, double x, double y) {
    return make_pair((unsigned long long) (x/lambda), (unsigned long long) (y/lambda));
}
unordered_map<pair<unsigned long long, unsigned long long>, int, hash_pair> createMap(double lambda, vector<Point>& points, int end) {
    unordered_map<pair<unsigned long long, unsigned long long>, int, hash_pair> mp;
    for(int i = 0; i <= end; i++) mp[calcCoordinate(lambda, points[i].getX(), points[i].getY())] = i;
    return mp;
}

//part3 methods declaration
Line recurPart3(vector<Point> &sortedByX, vector<Point> &sortedByY, int low, int high);
bool sortByY(Point & one, Point & two);
void output(Line l, double time, int numberOfPoints);
bool sortByX(Point& one, Point& two);
//sorts By Y
bool sortByY(Point& one, Point& two) {
    return one.getY() < two.getY();
}

//sorts By X
bool sortByX(Point& one, Point& two) {
    return one.getX() < two.getX();
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
        if(sortedByY[i].getX() >= sortedByX[low].getX() && sortedByY[i].getX() <= sortedByX[high].getX()) {
            sortedByY[i].getX() <= sortedByX[mid].getX() ? y1.push_back(sortedByY[i]) : y2.push_back(sortedByY[i]);
        }
    }
    
    //find the min distances of both the left and right sides of the array
    Line left = recurPart3(sortedByX, y1, low, mid), right = recurPart3(sortedByX, y2, mid+1, high);
    Line smaller = left.distance < right.distance ? left : right;
    
    //calculate the points in sortedByY that are in the strip
    double midX = sortedByX[mid].getX(), lowX = sortedByX[mid].getX() - smaller.distance, highX = sortedByX[mid].getX() + smaller.distance;
    vector<Point> inStrip;
    for(int i = 0; i < sortedByY.size(); i++) {
        if(sortedByY[i].getX() >= lowX && sortedByY[i].getX() <= highX) {
            inStrip.push_back(sortedByY[i]);
        }
    }
    
    //calculate and update the smallest distance within the strip
    for(int i = 0; i < inStrip.size(); i++) {
        for(int j = i+1; j < i+8; j++) {
            if(j >= inStrip.size()) break;
            if((inStrip[i].getX() < midX && inStrip[j].getX() > midX) || (inStrip[i].getX() > midX && inStrip[j].getX() < midX)) {
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
    ofstream stream("results.txt");
    stream << "Number of Points: " << numberOfPoints << endl;
    stream << "Part 3 Time: " << setprecision(23) << time << " ms" << endl;
    stream << "Part 3 Points: (" << l.one.getX() << "," << l.one.getY() << ") , (" << l.two.getX() << "," << l.two.getY() << ")" << endl;
    stream << "Part 3 Distance: " << l.distance << endl;
    stream.close();
    cout << "Number of Points: " << numberOfPoints << endl;
    cout << "Part 3 Time: " << setprecision(23) << time << " ms" << endl;
    cout << "Part 3 Points: (" << l.one.getX() << "," << l.one.getY() << ") , (" << l.two.getX() << "," << l.two.getY() << ")" << endl;
    cout << "Part 3 Distance: " << l.distance << endl;
}