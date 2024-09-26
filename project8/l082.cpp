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
vector<vector<double> > rotate_x(vector<vector<double> >& edges, double theta, double d);

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
            vector<vector<double> > temporary = rotate_x(points[j], i, 100);
            if(count1 < 8 && count <= 3) {stream << "(" << temporary[0][0] << "," << temporary[1][0] << "," << temporary[2][0] << ") "; count1++;}
            cv::Point p1(temporary[0][0] * 100 + 400, temporary[1][0] * 100 + 300);
            //cout << temporary[0][0] << " " << temporary[1][0] << endl;
            temp.push_back(p1);
        }
        count += 1; count1 = 0;
        if(count <= 4) stream << endl;
        cv::Scalar color(255, 255, 255);
        for(int j = 0; j < edges.size(); j++) {
            cout << temp[edges[j].first] << " " << temp[edges[j].second] << endl;
            cv::line(image, temp[edges[j].first], temp[edges[j].second], color, 1, 8, 0);
        }
        writer << image;
  }
//   for(double i = 0; i <= 2 * M_PI; i += 0.0174533) {
//         cv::Mat image(600, 800, CV_8UC3, cv::Scalar(0, 0, 0));
//         vector<Point> temp;
//         for(int j = 0; j < points1.size(); j++) {
//             vector<vector<double> > temporary = rotate_x(points1[j], i, 1000);
//             cv::Point p1(temporary[0][0] * 100 + 400, temporary[1][0] * 100 + 300);
//             temp.push_back(p1);
//         }
//         cv::Scalar color(255, 255, 255);
//         for(int j = 0; j < edges1.size(); j++) cv::line(image, temp[edges1[j].first], temp[edges1[j].second], color, 1, 8, 0);
//         writer << image;
//   }
    writer.release();
    return 0;
}
void display(vector<vector<double> >& matrix) {
    for(int i = 0; i < matrix.size(); i++) {
        for(int j = 0; j < matrix[i].size(); j++) cout << matrix[i][j] << " ";
        cout << endl;
    }
    cout << endl;
}
vector<vector<double> > rotate_x(vector<vector<double> >& edges, double theta, double d) {
    //cout << d << endl;
    vector<double> temp = {1};
    if(edges.size() < 4) edges.push_back(temp);
    vector<vector<double> > perspective = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, -1/d, 0}};
    vector<vector<double> > z_rotation = {{cos(theta), 0, sin(theta), 0}, {0, 1, 0, 0}, {-sin(theta), 0, cos(theta), 0}, {0, 0, 0, 1}};
    vector<vector<double> > first = multiplyMatrices(perspective, z_rotation);
    vector<vector<double > > second = multiplyMatrices(first, edges);
    //display(edges); display(perspective); display(z_rotation); display(first); 
    //display(second);
    //cout << second[0][0] << " " << second[1][0] << " " << second[2][0] << " " << second[3][0] << endl;
    // for(int i = 0; i < second.size(); i++) {
    //     second[i][0] = second[i][0];
    // }
    //cout << second[0][0] << " " << second[1][0] << " " << second[2][0] << " " << second[3][0] << endl << endl;
    second.pop_back();
    return second;}
vector<vector<double> > multiplyMatrices(vector<vector<double> >& one, vector<vector<double> >& two) {
    vector<vector<double> > result; double sum = 0;
    for(int i = 0; i < one.size(); i++) {
        vector<double> row;
        for(int j = 0; j < two[0].size(); j++) {
            for(int k = 0; k < two.size(); k++) sum += (one[i][k] * two[k][j]);
            row.push_back(sum); sum = 0;}
        result.push_back(row);}
    return result;}
