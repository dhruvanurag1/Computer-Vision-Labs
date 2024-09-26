#include <iostream>
#include <iomanip>
#include <vector>
#include <fstream>
#include <string>
#include <math.h>
#include <stack>
#include <unordered_map>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

vector<vector<double> > multiplyMatrices(vector<vector<double> >& one, vector<vector<double> >& two);
vector<vector<double> > rotate_x(vector<vector<double> >& edges, double theta);

int main() {
    // rotate a cube around the x-axis
    VideoWriter writer;
    writer.open("rotation.avi", VideoWriter::fourcc('M', 'J', 'P', 'G'), 30, Size(800, 600));
    ofstream stream; stream.open("coordinates.txt"); int count = 0; int count1 = 0;
    vector<vector<vector<double> > > points = {{{-1}, {-1}, {-1}}, {{-1}, {-1}, {1}}, {{-1}, {1}, {-1}}, {{-1}, {1}, {1}}, {{1}, {1}, {1}}, {{1}, {1}, {-1}}, {{1}, {-1}, {1}}, {{1}, {-1}, {-1}}};
    vector<vector<vector<double> > > points1 = {{{pow(8/9, 1/2)}, {0}, {-1/3}}, {{-pow(2/9, 1/2)}, {pow(2/3, 1/2)}, {-1/3}}, {{-pow(2/9, 1/2)}, {-pow(2/3, 1/2)}, {-1/3}}, {{0}, {0}, {1}}};
    vector<pair<int, int> > edges = {make_pair(0, 1), make_pair(0, 2), make_pair(0, 7), make_pair(1, 3), make_pair(1, 6), make_pair(2, 5), make_pair(2, 3), make_pair(3, 4), make_pair(4, 5), make_pair(4, 6), make_pair(5, 7), make_pair(6, 7)};
    vector<pair<int, int> > edges1 = {make_pair(0, 1), make_pair(0, 2), make_pair(0, 3), make_pair(1, 2), make_pair(1, 3), make_pair(2, 3)};
    for(double i = 0; i <= 2 * M_PI; i += 0.0174533) {
        cv::Mat image(600, 800, CV_8UC3, cv::Scalar(0, 0, 0));
        vector<Point> temp;
        for(int j = 0; j < points.size(); j++) {
            vector<vector<double> > temporary = rotate_x(points[j], i);
            if(count1 < 8 && count <= 3) {stream << "(" << temporary[0][0] << "," << temporary[1][0] << "," << temporary[2][0] << ") "; count1++;}
            cv::Point p1(temporary[0][0] * 100 + 400, temporary[1][0] * 100 + 300);
            temp.push_back(p1);
        }
        count += 1; count1 = 0;
        if(count <= 4) stream << endl;
        cv::Scalar color(255, 255, 255);
        for(int j = 0; j < edgokes.size(); j++) cv::line(image, temp[edges[j].first], temp[edges[j].second], color, 1, 8, 0);
        writer << image;
  }
  for(double i = 0; i <= 2 * M_PI; i += 0.0174533) {
        cv::Mat image(600, 800, CV_8UC3, cv::Scalar(0, 0, 0));
        vector<Point> temp;
        for(int j = 0; j < points1.size(); j++) {
            vector<vector<double> > temporary = rotate_x(points1[j], i);
            cv::Point p1(temporary[0][0] * 100 + 400, temporary[1][0] * 100 + 300);
            temp.push_back(p1);
        }
        cv::Scalar color(255, 255, 255);
        for(int j = 0; j < edges1.size(); j++) cv::line(image, temp[edges1[j].first], temp[edges1[j].second], color, 1, 8, 0);
        writer << image;
  }
    writer.release();
    return 0;
}
vector<vector<double> > rotate_x(vector<vector<double> >& edges, double theta) {
    for(int i = 0; i < edges.size(); i++) {
        for(int j = 0; j < edges[0].size(); j++) 
            cout << edges[i][j] << " ";
        cout << endl;
    }
    vector<vector<double> > x_rotation = {{1, 0, 0}, {0, cos(theta), -sin(theta)}, {0, sin(theta), cos(theta)}}; vector<vector<double> > new_coords = multiplyMatrices(x_rotation, edges);
    return new_coords;}
vector<vector<double> > multiplyMatrices(vector<vector<double> >& one, vector<vector<double> >& two) {
    vector<vector<double> > result; double sum = 0;
    for(int i = 0; i < one.size(); i++) {
        vector<double> row;
        for(int j = 0; j < two[0].size(); j++) {
            for(int k = 0; k < two.size(); k++) sum += (one[i][k] * two[k][j]);
            row.push_back(sum); sum = 0;}
        result.push_back(row);}
    return result;}
