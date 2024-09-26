#include <fstream>
#include <stdlib.h>
#include <iostream>
#include <iomanip>
#include <math.h>
#include <vector>
#include <algorithm>
using namespace std;


//classes

class Line{
    public:
        Line(double slope, double intercept);
        Line();
        void setslope(double slope) {
            mySlope = slope;
        }
        void setintercept(double intercept) {
            myIntercept = intercept;
        }
        double getSlope() {
            return mySlope;
        }
        double getIntercept() {
            return myIntercept;
        }
    private:
        double mySlope, myIntercept;
};
Line::Line(double slope, double intercept) {
    mySlope = slope;
    myIntercept = intercept;
}
Line::Line() {
    mySlope = 0;
    myIntercept = 0;
}
class Circle{
    public:
        Circle(double x1, double y1, double radius);
        Circle();
        void setCenter(double x1, double y1) {
            myx1 = x1;
            myy1 = y1;
        }
        void setRadius(double radius) {
            myRadius = radius;
        }
        vector<double> getCenter() {
            vector<double> temp;
            temp.push_back(myx1);
            temp.push_back(myy1);
            return temp;
        }
        double getRadius() {
            return myRadius;
        }
    private:
        double myx1, myy1, myRadius;

};
Circle::Circle(double x1, double y1, double radius) {
    myx1 = x1;
    myy1 = y1;
    myRadius = radius;
}
Circle::Circle() {
    myx1 = 0;
    myy1 = 0;
    myRadius = 0;
}
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




//declarations

vector<Point> semicirclemidpoint(Point one, Point two);
bool determineCollinear(float x[], float y[]);
bool determineInTriangle(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4);
void file(double x[3], double y[3], double x4, double y4);
double distance(double x1, double y1, double x2, double y2);
double randomm();
void read_File();
void part1();
void part2();
double calcarea(vector<Line> square);
Point points[4];
Point calcIntersection(Point done, Line line, Point center, double radius);
bool contains(vector<double> v, double a) ;
double findmin(vector<Line> a);
void lines(int a[800][2400], int x1,int y1,int x2,int y2);
void illuminate(int a[800][2400], int x, int y);
void circles(int a[800][2400], double radius, double xc, double yc);
void drawcircle(int a[800][2400], int x, int y, int xc, int yc);


int main() {
    part1();
    part2();
    return 0;
}

void read_File() {
    ifstream stream;
    stream.open("points.txt");
    string a[7]; 
    string tempx[4];
    string tempy[4];
    string str, temp;
    int count = 0;
    stream >> a[0] >> a[1] >> a[2] >> a[3] >> a[4] >> a[5] >> a[6] >> a[6];
    for (int i = 0; i < 7; i++) {
        str += a[i];
    }
    for (int i = 0; i < str.length(); i++) {
        temp = str[i];
        if (str[i] == '(' || str[i] == ')' || str[i] == ',') {
            temp = "";
        } 
        else {
            if(count % 2 == 0) {
                tempx[count/2] += temp;
            }
            else {
                tempy[count/2] += temp;
            }
            if(str[i+1] == '(' || str[i+1] == ')' || str[i+1] == ',') {
                count++;
            }
        }
    }
    for (int i = 0; i < 4; i++) {
        points[i].setX(stod(tempx[i]));
        points[i].setY(stod(tempy[i]));
    }
}
vector<Point> semicirclemidpoint(Point one, Point two) {
    Point center;
    Point finalone;
    Point finaltwo;
    center.setX((one.getX() + two.getX())/2);
    center.setY((one.getY() + two.getY())/2);
    double radius = (sqrt((pow(one.getX() - two.getX(), 2) + pow(one.getY() - two.getY(), 2))))/2;
    double slope = -1/((two.getY() - one.getY())/(two.getX() - one.getX()));
    double b = center.getY() - slope * center.getX();
    double h = center.getX();
    double k = center.getY();
    double x1 = (h - b*slope + slope*k + sqrt(-slope * slope *h*h + 2*slope*k*h - 2*slope*b*h + slope*slope*radius*radius + 2*b*k + pow(radius, 2) - pow(b, 2) - pow(k, 2))) / (1 + pow(slope, 2));
    double x2 = (h - b*slope + slope*k - sqrt(-slope * slope *h*h + 2*slope*k*h - 2*slope*b*h + slope*slope*radius*radius + 2*b*k + pow(radius, 2) - pow(b, 2) - pow(k, 2))) / (1 + pow(slope, 2));
    double y1 = slope * x1 + b;
    double y2 = slope * x2 + b;
    finalone.setX(x1);
    finalone.setY(y1);
    finaltwo.setX(x2);
    finaltwo.setY(y2);
    vector<Point> temp;
    temp.push_back(finalone);
    temp.push_back(finaltwo);
    return temp;
}
double calcarea(vector<Line> square) {
    vector<Line> temp1;
    bool first = true;
    bool done = false;
    Line firstline;
    Line secondline;
    for(int i = 0; i < 4; i++) {
        if(!done) {
            if(first) {
                firstline.setslope(square[i].getSlope());
                firstline.setintercept(square[i].getIntercept());
                first = false;
            }
            else if(abs(square[i].getSlope() - firstline.getSlope()) < 0.0001 && abs(firstline.getIntercept() - square[i].getIntercept()) > 0.00001) {
                secondline.setslope(square[i].getSlope());
                secondline.setintercept(square[i].getIntercept());
                done = true;
            }
        }
    }
    double a = -1 * firstline.getSlope();
    double b = 1;
    double c1 = -1 * firstline.getIntercept();
    double c2 = -1 * secondline.getIntercept();

    return pow(abs(c1 - c2)/sqrt(pow(a, 2) + pow(b, 2)),2);
}
Line calcLine(Point one, Point two) {
    Line temp;
    double m = (two.getY() - one.getY())/(two.getX() - one.getX());
    temp.setslope(m);
    temp.setintercept(one.getY() - (m * one.getX()));
    return temp;    
}
vector<Point> calcSquareVertices(vector<Line> lines) {
    vector<pair<Line, Line> > temp;
    for(int i = 0; i < 4; i++) {
        for(int j = i+1; j < 4; j++) {
            if(abs(lines[i].getSlope() - lines[j].getSlope()) > 0.00001) {
                temp.push_back(make_pair(lines[i], lines[j]));
            }
        }
    }
    vector<Point> p;
    Point temporary;
    double x;
    for(int i = 0; i < 4; i++) {
        x = (temp[i].second.getIntercept() - temp[i].first.getIntercept())/(temp[i].first.getSlope() - temp[i].second.getSlope());
        temporary.setX(x);
        temporary.setY(temp[i].second.getSlope() * x + temp[i].second.getIntercept());
        p.push_back(temporary);
    }
    return p;

}
vector<vector<Line> > calcSquareLines(vector<Point> pointsInOrder) {
    vector< vector<Line> > ans;
    vector<Line> tempa;
    for(int i = 0; i < 4; i++) {
        ans.push_back(tempa);
    }
    ans.clear();
    vector<Point> one, two;
    Point center1;
    Point center2;
    center1.setX((pointsInOrder[0].getX() + pointsInOrder[1].getX())/2);
    center1.setY((pointsInOrder[0].getY() + pointsInOrder[1].getY())/2);
    double radius1 = (sqrt((pow(pointsInOrder[0].getX() - pointsInOrder[1].getX(), 2) + pow(pointsInOrder[0].getY() - pointsInOrder[1].getY(), 2))))/2;
    center2.setX((pointsInOrder[2].getX() + pointsInOrder[3].getX())/2);
    center2.setY((pointsInOrder[2].getY() + pointsInOrder[3].getY())/2);
    double radius2 = (sqrt((pow(pointsInOrder[2].getX() - pointsInOrder[3].getX(), 2) + pow(pointsInOrder[2].getY() - pointsInOrder[3].getY(), 2))))/2;
    one = semicirclemidpoint(pointsInOrder[0], pointsInOrder[1]);
    two = semicirclemidpoint(pointsInOrder[2], pointsInOrder[3]);
    Line temp = calcLine(one[0], two[0]);
    ans[0].push_back(calcLine(pointsInOrder[0], calcIntersection(one[0], temp, center1, radius1)));
    ans[0].push_back(calcLine(pointsInOrder[1], calcIntersection(one[0], temp, center1, radius1)));
    ans[0].push_back(calcLine(pointsInOrder[2], calcIntersection(two[0], temp, center2, radius2)));
    ans[0].push_back(calcLine(pointsInOrder[3], calcIntersection(two[0], temp, center2, radius2)));
    temp = calcLine(one[0], two[1]);
    ans[1].push_back(calcLine(pointsInOrder[0], calcIntersection(one[0], temp, center1, radius1)));
    ans[1].push_back(calcLine(pointsInOrder[1], calcIntersection(one[0], temp, center1, radius1)));
    ans[1].push_back(calcLine(pointsInOrder[2], calcIntersection(two[1], temp, center2, radius2)));
    ans[1].push_back(calcLine(pointsInOrder[3], calcIntersection(two[1], temp, center2, radius2)));
    temp = calcLine(one[1], two[0]);
    ans[2].push_back(calcLine(pointsInOrder[0], calcIntersection(one[1], temp, center1, radius1)));
    ans[2].push_back(calcLine(pointsInOrder[1], calcIntersection(one[1], temp, center1, radius1)));
    ans[2].push_back(calcLine(pointsInOrder[2], calcIntersection(two[0], temp, center2, radius2)));
    ans[2].push_back(calcLine(pointsInOrder[3], calcIntersection(two[0], temp, center2, radius2)));
    temp = calcLine(one[1], two[1]);
    ans[3].push_back(calcLine(pointsInOrder[0], calcIntersection(one[1], temp, center1, radius1)));
    ans[3].push_back(calcLine(pointsInOrder[1], calcIntersection(one[1], temp, center1, radius1)));
    ans[3].push_back(calcLine(pointsInOrder[2], calcIntersection(two[1], temp, center2, radius2)));
    ans[3].push_back(calcLine(pointsInOrder[3], calcIntersection(two[1], temp, center2, radius2)));
    return ans;
}
Circle calcCircle(Point one, Point two) {
    Circle ans;
    ans.setCenter((one.getX() + two.getX())/2, (one.getY() + two.getY())/2);
    ans.setRadius((sqrt((pow(one.getX() - two.getX(), 2) + pow(one.getY() - two.getY(), 2))))/2);
    return ans;
}
Point calcIntersection(Point done, Line line, Point center, double radius) {
    double constant = line.getIntercept() - center.getY();
    double a = pow(line.getSlope(), 2) + 1;
    double b = -2*center.getX() + 2*line.getSlope()*constant;
    double c = pow(center.getX(), 2) + pow(constant, 2) - pow(radius, 2);
    double x1 = (-b + sqrt(pow(b, 2) - 4 * a * c))/(2 * a);
    double x2 = (-b - sqrt(pow(b, 2) - 4 * a * c))/(2 * a);
    double y1 = line.getSlope() * x1 + line.getIntercept();
    double y2 = line.getSlope() * x2 + line.getIntercept();
    Point one;
    Point two;
    Point null;
    one.setX(x1);
    one.setY(y1);
    two.setX(x2);
    two.setY(y2);
    if(abs(x1 - x2) < 0.001) {
        return null;
    }
    else if(abs(done.getX() - x1) < 0.001) {
        return two;
    }
    else {
        return one;
    }

}
void returnPoint(vector<pair<double, vector<Point> > > pointsandareas, vector<Line> smallest, double minarea) {
    ofstream ppm;
    ofstream txt;
    txt.open("output.txt");
    ppm.open("output.ppm");
    for(int i = 0; i < 3; i++) {
        txt << "(" << std::setprecision(17) << points[i].getX() << "," << std::setprecision(17) << points[i].getY() << ")  ," << endl;
    }
    txt << "(" << std::setprecision(17) << points[3].getX() << "," << std::setprecision(17) << points[3].getY() << ")" << endl;
    for(int i = 0; i < 6; i++) {
        for(int j = 0; j < 4; j++) {
            if(j == 3) {
                txt << "(" << pointsandareas[i].second[j].getX() << "," << pointsandareas[i].second[j].getY() << ")  Area = " << pointsandareas[i].first << endl;
                break;
            }
            txt << "(" << pointsandareas[i].second[j].getX() << "," << pointsandareas[i].second[j].getY() << ")  ," << endl;
        }
    }
    int a[800][2400];
    for(int i = 0; i < 800; i++) {
        for(int j = 0; j < 2400; j++) {
            a[i][j] = 1;
        }
    }
    vector<Point> temp;
    for(int i = 0; i < 4; i++) {
        lines(a, 0, (int)(smallest[i].getIntercept() * 800), 800, (int) (800 * (smallest[i].getSlope() + smallest[i].getIntercept())));
    }
    for(int i = 0; i < 4; i++) {
        circles(a, 2, points[i].getX() * 800, points[i].getY() * 800);
    }
    for(int i = 0; i < 6; i++) {
        if(abs(pointsandareas[i].first - minarea) < 0.0001) {
            temp = pointsandareas[i].second;
        }
    }
    for(int i = 0; i < 4; i++) {
        circles(a, 5, temp[i].getX() * 800, temp[i].getY() * 800);
    }
    ppm << "P3 800 800 1" << endl;
    for (int i = 0; i < 800; i++) {
        for(int j = 0; j < 2400; j++) {
            ppm << std::to_string(a[i][j]) + " ";
        }
        ppm << endl;
    }
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
void circles(int a[800][2400], double radius, double xc, double yc) {
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
bool contains(vector<double> v, double a) {
    for(int i = 0; i < v.size(); i++) {
        if(abs(a - v[i]) < 0.0001) {
            return true;
        }
    }
    return false;
}
void part2() {
   vector<vector<Line> > lines;
   read_File();
   vector<Point> temp;
    //find all 3 combinations of pairs of points
   for(int i = 0; i < 4; i++) {
    temp.push_back(points[i]);
   }
   lines = calcSquareLines(temp);
   temp.clear();
   temp.push_back(points[0]);
   temp.push_back(points[2]);
   temp.push_back(points[1]);
   temp.push_back(points[3]);
   for(int i = 0; i < 4; i++) {
       lines.push_back(calcSquareLines(temp)[i]);
   }
   temp.clear();
   temp.push_back(points[0]);
   temp.push_back(points[3]);
   temp.push_back(points[1]);
   temp.push_back(points[2]);
   for(int i = 0; i < 4; i++) {
       lines.push_back(calcSquareLines(temp)[i]);
    }
   vector<vector<Line> > tempb;
    vector<double> set;
    vector<double> first;
    vector <double> linetemp;
    for(int i = 0; i < 8; i++) {
        for(int j = 0; j < 4; j++) {
            linetemp.push_back(lines[i][j].getSlope());
        }
        first.push_back(*min_element(linetemp.begin(), linetemp.end()));
        linetemp.clear();
    }
    for(int i = 0; i < 8; i++) {
       if(!contains(set, first[i])) {
           set.push_back(first[i]);
       }
    }
    for(int i = 0; i < 6; i++) {
        for(int j = 0; j < 8; j++) {
            if(abs(set[i] - findmin(lines[j])) < 0.00001) {
                tempb.push_back(lines[i]);
                break;
            }
        }
    }
    lines = tempb;
    double min = 1000000;
    vector<Line> smallest;
    for(int i = 0; i < 6; i++) {
       if(min > calcarea(lines[i])) {
           min = calcarea(lines[i]);
           smallest = lines[i];
       }
   }
   vector<pair<double, vector<Point> > > pointsandlines;
   for(int i = 0; i < 6; i++) {
    pointsandlines.push_back(make_pair(calcarea(lines[i]), calcSquareVertices(lines[i])));
   }
   returnPoint(pointsandlines, smallest, min);
    
}
double findmin(vector<Line> a) {
    vector<double> temp;
    for(int b = 0; b < a.size(); b++) {
        temp.push_back(a[b].getSlope());
    }
    return *min_element(temp.begin(), temp.end());
}
void illuminate(int a[800][2400], int x, int y) {
    for(int i = 0; i < 3; i++) {
        a[y][3*x+i] = 0;
    }
}
void lines(int a[800][2400], int x1,int y1,int x2,int y2) {
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

