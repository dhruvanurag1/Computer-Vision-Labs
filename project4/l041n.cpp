#include <fstream>
#include <iostream>
#include <math.h>
#include <random>
#include <limits>
#include <algorithm>
#include <vector>
#include <iomanip>

using namespace std;

//class for storing coordinates
class Point {
    private:
        double x;
        double y; 
    public:
        double getX() {return x;}
        double getY() {return y;}
        void createPoint(double myX, double myY) {x = myX, y = myY;}
        bool equals(Point next) {return this->x == next.getX() && this->y == next.getY();}
};

//declare methods and variables
vector<Point> convexHull;
void quickHull(vector<Point> & points);
void findHull(vector<Point> & points, Point & one, Point & two);
int findFarthestPoint(vector<Point>& points, Point & one, Point & two);
bool isRight(Point & one, Point & two, Point & three);
double distFromLine(Point & one, Point & two, Point & three);
bool sortByX(Point & one, Point & two);
vector<Point> createVector(int size);
double randomm();
double getSlope(Point & one, Point & two);
bool inTriangle(Point& one, Point& two, Point& three, Point& four);
double sign(Point & one, Point & two, Point & three);
vector<Point> insertFar(vector<Point> & convexhull, Point & P, Point & Q, Point & C);

int main() {
    vector<Point> points = createVector(60);
    quickHull(points);
    //cout << endl << endl << endl;
    for(int i = 0; i < points.size(); i++) {
        cout << points[i].getX() << "," << points[i].getY() << endl;
    }
    cout << endl << endl;
    for(int i = 0; i < convexHull.size(); i++) {
        cout << convexHull[i].getX() << "," << convexHull[i].getY() << endl;
    }
    return 0;
}

vector<Point> createVector(int size) {
    Point temp; vector<Point> points;
    for(int i = 0; i < size; i++) {
        temp.createPoint(randomm(), randomm()); points.push_back(temp);
    }
    return points;
}
double randomm() {
    static bool one = true;
    if(one) {
        srand(time(NULL));
        one = false;
    }
    return (double) rand()/RAND_MAX;
}

bool sortByX(Point & one, Point & two) {return one.getX() < two.getX();}

void quickHull(vector<Point> & points) {
    //sort by X coordinate
    sort(points.begin(), points.end(), sortByX);
    //find min and max points
    Point minX = points[0], maxX = points.back();
    //push back the coordinates with the biggest and smallest x coordinates
    convexHull.push_back(minX); convexHull.push_back(maxX);
    //initialize two sets with points left and right of the line from Point min to max
    vector<Point> s1, s2;
    for(int i = 1; i < points.size() - 1; i++) {
        if(isRight(minX, maxX, points[i])) s1.push_back(points[i]);
        if(isRight(maxX, minX, points[i])) s2.push_back(points[i]);
    }
    //test if sizes are right
    findHull(s1, minX, maxX);
    findHull(s2, maxX, minX);
}

double getSlope(Point & one, Point & two) {
    return (two.getY() - one.getY())/(one.getX() - two.getX());
}

bool inTriangle(Point& one, Point& two, Point& three, Point& four) {
    double done, dtwo, dthree;
    bool neg, pos;
    done = sign(one, two, three);
    dtwo = sign(one, three, four);
    dthree = sign(one, four, two);

    neg = (done < 0) || (dtwo < 0) || (dthree < 0);
    pos = (done > 0) || (dtwo > 0) || (dthree > 0);

    return !(neg && pos);
}
double sign(Point & one, Point & two, Point & three) {
    return (one.getX() - three.getX()) * (two.getY() - three.getY()) - (two.getX() - three.getX()) * (one.getY() - three.getY());
}
void findHull(vector<Point> & points, Point & one, Point & two) {
    if(points.size() == 0) return;
    //find farthest point where the point is right of the line
    int farthest = findFarthestPoint(points, one, two); Point far = points[farthest]; points.erase(points.begin() + farthest);
    convexHull = insertFar(convexHull, one, two, far);//.push_back(far);
    //calculate the points that are on the right of the lines (one, far) and (far, two)
    vector<Point> s1, s2;
    bool opposite = false, opposite2 = false;
    for(int i = 0; i < points.size(); i++) {
            //im checking if in triangle in order to account for all of the edge cases
            if(isRight(one, far, points[i]) && !opposite) {
                if(inTriangle(points[i], one, far, two)) opposite = true;
                else {s1.push_back(points[i]); continue;}
            }
            if(opposite && isRight(far, one, points[i])) {s1.push_back(points[i]); continue;}
            if(isRight(far, two, points[i]) && !opposite2) {
                if(inTriangle(points[i], one, far, two)) opposite2 = true;
                else {s2.push_back(points[i]); continue;}
            }
            if(opposite2 && isRight(far, one, points[i])) {s2.push_back(points[i]); continue;}
    }
    for(int i = 0; i < s1.size(); i++) {
        cout << s1[i].getX() << " " << s1[i].getY() << endl;
    }
    cout << "(" << one.getX() << "," << one.getY() << "),(" << far.getX() << "," << far.getY() << "),(" << two.getX() << "," << two.getY() << endl;
    for(int i = 0; i < s2.size(); i++) {
        cout << s2[i].getX() << " " << s2[i].getY() << endl;
    }
    cout << endl;
    findHull(s1, one, far);
    findHull(s2, far, two);
}

double distFromLine(Point & one, Point & two, Point & three) {
    double numerator = abs((two.getX()-one.getX())*(one.getY()-three.getY())-(one.getX()-three.getX())*(two.getY()-one.getY()));
    double denominator = sqrt(pow((two.getX()-one.getX()),2)+pow((two.getY()-one.getY()),2));
    return numerator/denominator;
}

int findFarthestPoint(vector<Point>& points, Point & one, Point & two) {
    int index = 0; double distance = 0;
    for(int i = 0; i < points.size(); i++) {
        if(distance < abs(distFromLine(one, two, points[i])) && isRight(one, two, points[i])) {
            index = i; distance = abs(distFromLine(one, two, points[i])); cout << points[i].getX() << "," << points[i].getY() << " " << distance << endl;
        }
    }
    return index;
}

bool isRight(Point & one, Point & two, Point & three) {
    double distance = (three.getX() - one.getX()) * (two.getY() - one.getY()) - (three.getY() - one.getY()) * (two.getX() - one.getX());
    double x = one.getX() + 1, y = one.getY();
    double otherdistance = (x - one.getX()) * (two.getY() - one.getY()) - (y - one.getY()) * (two.getX() - one.getX());
    return distance > 0;
}

vector<Point> insertFar(vector<Point> & convexhull, Point & P, Point & Q, Point & C) {
    bool changed = false;
    for(int i = 0; i < convexHull.size(); i++) {
        if(convexhull[i].equals(P) && convexhull[i+1].equals(Q)) {
            convexhull.insert(convexhull.begin() + i + 1, C);
            changed = true;
        }
    }
    if(!changed) convexhull.push_back(C);
    return convexhull;
}
