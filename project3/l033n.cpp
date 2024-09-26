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

void part3();
void part2();
//void part1(double size);

//class for storing smallest distances, indexOne represents index of first point, indexTwo represents index of second point

//class for storing points
class Point {
    public:
        double x;
        double y; 
};

class Small {
    public:
        double distance;
        int indexOne, indexTwo;
        vector<Point> x;
};



int main() {
    //controls number of points 
    double size = 5000;

    //part1(size);
    //part2();
    part3();
    return 0;
}



//*******PART 2 SECTION******

//part2 methods
// double calcDistance2(Point one, Point two);
// Small brute_force2(vector<Point> points);
// Small recur2(vector<Point> points, int low, int high);
// vector<Point> readfile2();
// bool sortByX2(Point & one, Point & two);
// void file22(vector<Point> points, double distance);

// //finds the smallest distance in an array of points
// Small brute_force2(vector<Point> points, int one, int two) {
//     double minDistance = DBL_MAX, index1 = 0, index2 = 0;
//     for(int i = one; i <= two; i++) {
//         for(int j = i+1; j <= two; j++) {
//             if(calcDistance2(points[i], points[j]) < minDistance) {
//                 minDistance = calcDistance2(points[i], points[j]), index1 = i, index2 = j;
//             }
//         }
//     }
//     Small ans;
//     ans.indexOne = index1, ans.indexTwo = index2, ans.distance = minDistance;
//     return ans;
// }

// void file22(vector<Point> points, double distance, double time) {
//     ofstream stream("results.txt", ios::app);
//     stream << "Recur Time: " << time << " ms" << endl;
//     stream << "Recur Points: (" << points[0].x << "," << points[0].y << ") , (" << points[1].x << "," << points[1].y << ")" << endl;
//     stream << "Recur Distance: " << distance << endl;
//     stream.close();
//     cout << "Recur Time: " << time << " ms" << endl;
//     cout << "Recur Points: (" << points[0].x << "," << points[0].y << ") , (" << points[1].x << "," << points[1].y << ")" << endl;
//     cout << "Recur Distance: " << distance << endl;
// }

// // calculates distance given two points
// double calcDistance2(Point one, Point two) {
//     return sqrt(pow(two.x - one.x, 2) + pow(two.y - one.y, 2));
// }

// //reads points from file that part1 generated
// vector<Point> readfile2() {
//     ifstream stream;
//     stream.open("points1k.txt");
//     double temp1, temp2;
//     Point l;
//     vector<Point> points;
//     while(stream >> temp1 >> temp2) {
//         l.x = temp1, l.y = temp2;
//         points.push_back(l);
//     }
//     return points;
// }

// //uses recursive approach to find min distance
// Small recur2(vector<Point> points, int low, int high) {
//     //if there are only two points
//     if(high - low == 1) {
//         Small ans; ans.indexOne = low, ans.indexTwo = high, ans.distance = calcDistance2(points[low], points[high]);
//         return ans;
//     }

//     //if there are three points
//     else if(high - low == 2) {
//         return brute_force2(points, low, high);
//     }
//     //find min distance of both sides of array
//     int mid = (low + high)/2;
//     Small left = recur2(points, low, mid);
//     Small right = recur2(points, mid+1, high);
//     Small smallest;
//     if(left.distance < right.distance) smallest = left;
//     else smallest = right;
//     //initialize the indices of the points that are in the strip
//     bool first = false;
//     int one, two = 0;
//     double lowX = points[mid].x - smallest.distance, highX = points[mid].x + smallest.distance;
//     vector<Point>::iterator i;
//     vector<Point>::iterator endIterator = points.begin() + high + 1;
//     for (i = points.begin() + low; i != endIterator; ++i) {
//         Point temp = *i;
//         if(temp.x >= lowX && temp.x <= highX && first == false) {
//             one = i - points.begin(), first = true;
//         }
//         if(temp.x > highX && first == true) {
//             two = i- points.begin() -1;
//             break;
//         }
//     }
//     if(two == 0) {
//         two = high;
//     }

//     //find the minimum distance in the strip  
//     Small minStrip = brute_force2(points, one, two);

//     //return the smaller distance
//     if(minStrip.distance < smallest.distance) {
//         return minStrip;
//     }
//     else {
//         return smallest;
//     }
// }

// //helper method for std::sort
// bool sortByX2(Point & one, Point & two) {
//     return one.x < two.x;
// }

// void part2() {
//     vector<Point> points;
//     //initialize the vector with the points from the points.txt file
//     points = readfile2();

//     clock_t begin = clock();
    
//     //sort the array by the X coordinate
//     std::sort(points.begin(), points.end(), sortByX2);

//     Small smallest = recur2(points, 0, points.size() - 1);

//     clock_t end = clock();

//     vector<Point> temp; temp.push_back(points[smallest.indexOne]), temp.push_back(points[smallest.indexTwo]);

//     file22(temp, smallest.distance, double(end - begin) * 1000/ CLOCKS_PER_SEC);
// }






double calcDistance(Point one, Point two);
Small brute_force(vector<Point> points);
Small recur(vector<Point> points, int low, int high);
bool sortByX(Point & one, Point & two);
void file2(vector<Point> points, vector<Point> sortedByY, double distance, double time);
bool sortByY(Point & one, Point & two);

//finds the smallest distance in an array of points
Small brute_force(vector<Point> points, int one, int two) {
    vector<Point> sortedByY(two - one + 1);
    std::copy(points.begin() + one, points.begin() + two + 1, sortedByY.begin());
    std::sort(sortedByY.begin(), sortedByY.end(), sortByY);
    double minDistance = DBL_MAX, index1 = 0, index2 = 0;
    for(int i = 0; i < sortedByY.size(); i++) {
        for(int j = i+1; j < i+min(16, two - i); j++) {
            if(j == sortedByY.size()) {
                break;
            }
            if (minDistance > calcDistance(sortedByY[i], sortedByY[j])) {
                    minDistance = calcDistance(sortedByY[i], sortedByY[j]), index1 = i, index2 = j;
            }
        }
    }
    Small ans; ans.indexOne = index1, ans.indexTwo = index2, ans.distance = minDistance, ans.x = sortedByY;
    return ans;
}

void file2(vector<Point> points, double distance, double time) {
    ofstream stream("results.txt", ios::app);
    stream << "Recur Time: " << time << " ms" << endl;
    stream << "Recur Points: (" << points[0].x << "," << points[0].y << ") , (" << points[1].x << "," << points[1].y << ")" << endl;
    stream << "Recur Distance: " << distance << endl;
    stream.close();
    cout << "Full Recur Time: " << time << " ms" << endl;
    cout << "Full Recur Points: (" << points[0].x << "," << points[0].y << ") , (" << points[1].x << "," << points[1].y << ")" << endl;
    cout << "Full Recur Distance: " << distance << endl;
}

// calculates distance given two points
double calcDistance(Point one, Point two) {
    return sqrt(pow(two.x - one.x, 2) + pow(two.y - one.y, 2));
}

//reads points from file that part1 generated
vector<Point> readfile() {
    ifstream stream;
    stream.open("points10k.txt");
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
        Small ans; ans.indexOne = low, ans.indexTwo = high, ans.distance = calcDistance(points[low], points[high]); ans.x = points;
        return ans;
    }

    //if there are three points
    else if(high - low == 2) {
        double minDistance = min(min(calcDistance(points[low], points[low+1]), calcDistance(points[low], points[high])), calcDistance(points[low+1], points[high]));
        if (minDistance == calcDistance(points[low], points[low+1])) {
            Small ans = {calcDistance(points[low], points[low+1]), low, low+1, points};
            return ans;
        }
        else if (minDistance == calcDistance(points[low], points[high])) {
            Small ans = {calcDistance(points[low], points[high]), low, high, points};
            return ans;
        }
        else {
            Small ans = {calcDistance(points[low+1], points[high]), low+1, high, points};
            return ans;
        }
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
    // vector<Point> temp;
    // for(int i = low; i <= high; i++) {
    //     if(points[i].x >= lowX && points[i].x <= highX) {
    //         temp.push_back(points[i]);
    //     }
    // }
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

bool sortByY(Point & one, Point & two) {
    return one.y < two.y;
}

//helper method for std::sort
bool sortByX(Point & one, Point & two) {
    return one.x < two.x;
}

void part3() {
    vector<Point> points;
    vector<Point> sortedByY;
    //initialize the vector with the points from the points.txt file
    points = readfile();

    clock_t begin = clock();
    //sort the array by the X coordinate
    std::sort(points.begin(), points.end(), sortByX);
    sortedByY = points;
    std::sort(sortedByY.begin(), sortedByY.end(), sortByY);
    Small smallest = recur(points, 0, points.size() - 1);

    clock_t end = clock();
    vector<Point> temp;
    // if(abs(smallest.distance - calcDistance(sortedByY[smallest.indexOne], sortedByY[smallest.indexTwo])) < 0.00000000000000000001) {
    //     temp.push_back(sortedByY[smallest.indexOne]), temp.push_back(sortedByY[smallest.indexTwo]);
    // }
    // else {
    //     temp.push_back(points[smallest.indexOne]), temp.push_back(points[smallest.indexTwo]);
    // }
    temp.push_back(smallest.x[smallest.indexOne]), temp.push_back(smallest.x[smallest.indexTwo]);

    file2(temp, smallest.distance, double(end - begin) * 1000/ CLOCKS_PER_SEC);
}