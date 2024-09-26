#include <fstream>
#include <iostream>
#include <math.h>
#include <random>
#include <limits>
#include <algorithm>

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
int a[800][2400];
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
void illuminate(int a[800][2400], int x, int y, bool red);
vector<Point> insertFar(vector<Point> & convexhull, Point & P, Point & Q, Point & C);
void line(int a[800][2400], int x1,int y1,int x2,int y2);
void circle(bool red, int a[800][2400], double radius, double xc, double yc);
void drawcircle(bool red, int a[800][2400], int x, int y, int xc, int yc);
void display(vector<Point> & points);

int main() {
   vector<Point> points = createVector(60);
   quickHull(points);
   display(points);
    return 0;
}
void display(vector<Point> & points) {
    for(int i = 0; i < 800; i++) {
        for(int j = 0; j < 2400; j++) {
            a[i][j] = 1;
        }
    }
    for(int c = 0; c < points.size(); c++) {
        Point i = points[c];
        circle(false, a, 3, (int) (i.getX() * 800) , (int) (i.getY() * 800));
    }
    for(int i = 1; i < convexHull.size(); i++) {
        if(i == convexHull.size() - 1) {
            line(a, (int) (convexHull[i].getX() * 800), (int) (convexHull[i].getY() * 800), (int) (convexHull[0].getX() * 800), (int) (convexHull[0].getY() * 800));
        }
        line(a, (int) (convexHull[i-1].getX() * 800), (int) (convexHull[i-1].getY() * 800), (int) (convexHull[i].getX() * 800), (int) (convexHull[i].getY() * 800));
    }
    for(int c = 0; c < convexHull.size(); c++) {
        Point i = convexHull[c];
        circle(true, a, 3, (int) (i.getX() * 800) , (int) (i.getY() * 800));
    }
    ofstream stream;
    stream.open("quickhull.ppm");
    stream << "P3 800 800 1" << endl;
    for (int i = 0; i < 800; i++) {
        for(int j = 0; j < 2400; j++) {
            stream << std::to_string(a[i][j]) + " ";
        }
        stream << endl;
    }
    stream.close();
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
void illuminate(int a[800][2400], int x, int y) {
    for(int i = 0; i < 3; i++) {
        a[y][3*x+i] = 0;
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
                illuminate(a, j, i, false);
        }
        else {
            if(j <= 799 && j >= 0)
                illuminate(a, i, j, false);
        }
        if(error >= 0) {
            j += step;
            error -= deltax;
        }
        error += deltay;
    }
}
void drawcircle(bool red, int a[800][2400], int x, int y, int xc, int yc) {
    if(!(int(xc+x) < 0 || int(xc+x) > 799 || int(yc+y) < 0 || int(yc+y) > 799)) 
        illuminate(a, int(xc+x), int(yc+y), red); 

    if(!(int(xc-x) < 0 || int(xc-x) > 799 || int(yc+y) < 0 || int(yc+y) > 799)) 
        illuminate(a, int(xc-x), int(yc+y), red);

    if(!(int(xc+x) < 0 || int(xc+x) > 799 || int(yc-y) < 0 || int(yc-y) > 799)) 
        illuminate(a, int(xc+x), int(yc-y), red); 

    if(!(int(xc-x) < 0 || int(xc-x) > 799 || int(yc-y) < 0 || int(yc-y) > 799)) 
        illuminate(a, int(xc-x), int(yc-y), red); 
    
    if(!(int(xc+y) < 0 || int(xc+y) > 799 || int(yc+x) < 0 || int(yc+x) > 799)) 
        illuminate(a, int(xc+y), int(yc+x), red);
    
    if(!(int(xc-y) < 0 || int(xc-y) > 799 || int(yc+x) < 0 || int(yc+x) > 799)) 
        illuminate(a, int(xc-y), int(yc+x), red);

    if(!(int(xc+y) < 0 || int(xc+y) > 799 || int(yc-x) < 0 || int(yc-x) > 799)) 
        illuminate(a, int(xc+y), int(yc-x), red);

    if(!(int(xc-y) < 0 || int(xc-y) > 799 || int(yc-x) < 0 || int(yc-x) > 799)) 
        illuminate(a, int(xc-y), int(yc-x), red);
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
