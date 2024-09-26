#include <fstream>
#include <iostream>
#include <math.h>
#include <random>
#include <limits>
#include <algorithm>
#include <stack>
#include <iomanip>

using namespace std;

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

vector<Point> convexHull;
vector<Point> grahamScan;
int a[400][1200];
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
void illuminate(int a[400][1200], int x, int y, bool red);
vector<Point> insertFar(vector<Point> & convexhull, Point & P, Point & Q, Point & C);
void line(int a[400][1200], int x1,int y1,int x2,int y2);
void circle(bool red, int a[400][1200], double radius, double xc, double yc);
void drawcircle(bool red, int a[400][1200], int x, int y, int xc, int yc);
void display(vector<Point> & points);
void part2();
void display2(vector<Point> & points);
bool sortByAngle(Point& p1, Point& p2);
int orientation(Point p1, Point & p2, Point & p3);
double calcDistance(Point & p1, Point & p2);
Point second(stack<Point> & pointstack);
vector<Point> readFile();
Point p0;

int main() {
   vector<Point> points = createVector(60);
   quickHull(points);
   display(points);
   part2();
    return 0;
}
void display(vector<Point> & points) {
    int a[400][1200];
    for(int i = 0; i < 400; i++) {
        for(int j = 0; j < 1200; j++) {
            a[i][j] = 1;
        }
    }
    for(int c = 0; c < points.size(); c++) {
        Point i = points[c];
        circle(false, a, 3, (int) (i.getX() * 400) , (int) (i.getY() * 400));
    }
    for(int i = 1; i < convexHull.size(); i++) {
        if(i == convexHull.size() - 1) {
            line(a, (int) (convexHull[i].getX() * 400), (int) (convexHull[i].getY() * 400), (int) (convexHull[0].getX() * 400), (int) (convexHull[0].getY() * 400));
        }
        line(a, (int) (convexHull[i-1].getX() * 400), (int) (convexHull[i-1].getY() * 400), (int) (convexHull[i].getX() * 400), (int) (convexHull[i].getY() * 400));
    }
    for(int c = 0; c < convexHull.size(); c++) {
        Point i = convexHull[c];
        circle(true, a, 3, (int) (i.getX() * 400) , (int) (i.getY() * 400));
    }
    ofstream stream;
    ofstream stream2;
    stream.open("quickhull.ppm");
    stream2.open("points.txt");
    stream << "P3 400 400 1" << endl;
    for (int i = 0; i < 400; i++) {
        for(int j = 0; j < 1200; j++) {
            stream << std::to_string(a[i][j]) + " ";
        }
        stream << endl;
    }
    stream.close();
    for(int i = 0; i < points.size(); i++) {
        stream2 << setprecision(23) << points[i].getX() << "  " << points[i].getY() << endl;
    }
    stream2.close();
}
void line(int a[400][1200], int x1,int y1,int x2,int y2) {
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
            if(i <= 399 && i >= 0)
                illuminate(a, j, i, false);
        }
        else {
            if(j <= 399 && j >= 0)
                illuminate(a, i, j, false);
        }
        if(error >= 0) {
            j += step;
            error -= deltax;
        }
        error += deltay;
    }
}
void drawcircle(bool red, int a[400][1200], int x, int y, int xc, int yc) {
    if(!(int(xc+x) < 0 || int(xc+x) > 399 || int(yc+y) < 0 || int(yc+y) > 399)) 
        illuminate(a, int(xc+x), int(yc+y), red); 

    if(!(int(xc-x) < 0 || int(xc-x) > 399 || int(yc+y) < 0 || int(yc+y) > 399)) 
        illuminate(a, int(xc-x), int(yc+y), red);

    if(!(int(xc+x) < 0 || int(xc+x) > 399 || int(yc-y) < 0 || int(yc-y) > 399)) 
        illuminate(a, int(xc+x), int(yc-y), red); 

    if(!(int(xc-x) < 0 || int(xc-x) > 399 || int(yc-y) < 0 || int(yc-y) > 399)) 
        illuminate(a, int(xc-x), int(yc-y), red); 
    
    if(!(int(xc+y) < 0 || int(xc+y) > 399 || int(yc+x) < 0 || int(yc+x) > 399)) 
        illuminate(a, int(xc+y), int(yc+x), red);
    
    if(!(int(xc-y) < 0 || int(xc-y) > 399 || int(yc+x) < 0 || int(yc+x) > 399)) 
        illuminate(a, int(xc-y), int(yc+x), red);

    if(!(int(xc+y) < 0 || int(xc+y) > 399 || int(yc-x) < 0 || int(yc-x) > 399)) 
        illuminate(a, int(xc+y), int(yc-x), red);

    if(!(int(xc-y) < 0 || int(xc-y) > 399 || int(yc-x) < 0 || int(yc-x) > 399)) 
        illuminate(a, int(xc-y), int(yc-x), red);
}
void circle(bool red, int a[400][1200], double radius, double xc, double yc) {
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
void illuminate(int a[400][1200], int x, int y, bool red) {
    if(red == true) {
        for(int i = 0; i < 3; i++) {
            if(i == 0) a[y][3*x+i] = 1;
            else a[y][3*x+i] = 0;
        }
        return;
    }
    for(int i = 0; i < 3; i++) {
        a[y][3*x+i] = 0;
    }
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
void part2() {
    vector<Point> points; points = readFile();
    vector<int> lowestY; double lowY = 1, lowX = 1; int index = -1;
    for(int i = 0; i < points.size(); i++) {
        if(points[i].getY() < lowY) {
            lowY = points[i].getY(); index = i; lowestY.erase(lowestY.begin(), lowestY.end()); lowestY.push_back(index);
        }
        if(points[i].getY() == lowY) lowestY.push_back(i);
    }
    for(int i = 0; i < lowestY.size(); i++) {
        if(points[lowestY[i]].getX() < lowX) {
            lowX = points[lowestY[i]].getX(); index = lowestY[i];
        }
    }
    p0 = points[index]; points.erase(points.begin() + index);
    sort(points.begin(), points.end(), sortByAngle);
    int m = 1; // Initialize size of modified array
    for (int i=1; i<points.size(); i++)
    {
       while (i < points.size()-1 && orientation(p0, points[i], points[i+1]) == 0) i++;
       points[m] = points[i];
       m++;
   }
   if (m < 3) return;
   stack<Point> pointstack;
   pointstack.push(p0);
   pointstack.push(points[0]);
   pointstack.push(points[1]);
   for(int i = 2; i < m; i++) {
       while(pointstack.size() > 1 && orientation(second(pointstack), pointstack.top(), points[i]) != 2) {
           
           pointstack.pop(); 
       }
       pointstack.push(points[i]);
   }
   int n = pointstack.size(); Point temp;
   for(int i = 0; i < n; i++) {
       temp.createPoint(pointstack.top().getX(),pointstack.top().getY()); grahamScan.push_back(temp);
       pointstack.pop();
   }
   display2(points);
}
vector<Point> readFile() {
    ifstream stream;
    stream.open("points.txt");
    double temp1, temp2;
    Point l;
    vector<Point> points;
    while(stream >> temp1 >> temp2) {
        l.createPoint(temp1, temp2);
        points.push_back(l);
    }
    return points;
}
void display2(vector<Point> & points) {
    int a[400][1200];
    for(int i = 0; i < 400; i++) {
        for(int j = 0; j < 1200; j++) {
            a[i][j] = 1;
        }
    }
    for(int c = 0; c < points.size(); c++) {
        Point i = points[c];
        circle(false, a, 3, (int) (i.getX() * 400) , (int) (i.getY() * 400));
    }
    for(int i = 1; i < grahamScan.size(); i++) {
        if(i == grahamScan.size() - 1) {
            line(a, (int) (grahamScan[i].getX() * 400), (int) (grahamScan[i].getY() * 400), (int) (grahamScan[0].getX() * 400), (int) (grahamScan[0].getY() * 400));
        }
        line(a, (int) (grahamScan[i-1].getX() * 400), (int) (grahamScan[i-1].getY() * 400), (int) (grahamScan[i].getX() * 400), (int) (grahamScan[i].getY() * 400));
    }
    for(int c = 0; c < grahamScan.size(); c++) {
        Point i = grahamScan[c];
        circle(true, a, 3, (int) (i.getX() * 400) , (int) (i.getY() * 400));
    }
    ofstream stream;
    stream.open("grahamscan.ppm");
    stream << "P3 400 400 1" << endl;
    for (int i = 0; i < 400; i++) {
        for(int j = 0; j < 1200; j++) {
            stream << std::to_string(a[i][j]) + " ";
        }
        stream << endl;
    }
    stream.close();
}
Point second(stack<Point> & pointstack) {
    Point p = pointstack.top();
    pointstack.pop();
    Point two = pointstack.top();
    pointstack.push(p);
    return two;
}
bool sortByAngle(Point& p1, Point& p2) {
    int direction = orientation(p0, p1, p2);
    if(direction == 0) {return (calcDistance(p0, p2) >= calcDistance(p0, p1)) ? false : true;}
    return direction == 2 ? true : false;
}
double calcDistance(Point & p1, Point & p2) {
    return sqrt(pow(p2.getX() - p1.getX(), 2) + pow(p2.getY() - p1.getY(), 2));
}
int orientation(Point p1, Point & p2, Point & p3) {
    double val = (p2.getY() - p1.getY()) * (p3.getX() - p2.getX()) - (p2.getX() - p1.getX()) * (p3.getY() - p2.getY());
    if (val == 0) return 0;
    return (val > 0)? 1: 2; //0: collinear 1: clockwise 2: counterclockwise
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
    convexHull = insertFar(convexHull, one, two, far);
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
            index = i; distance = abs(distFromLine(one, two, points[i]));
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
