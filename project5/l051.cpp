#include <iostream>
#include <iomanip>
#include <vector>
#include <fstream>
#include <string>
#include <math.h>

using namespace std;

void grayScale();
void part1();
vector<vector<vector<double> > > readFile();
vector<vector<double> > grayscale();
vector<vector<double> > sobel(vector<vector<double> > &grayScale, double kernel[3][3]);
vector<vector<double> > threshold(vector<vector<double> >& x, vector<vector<double> >& y, double threshold);

int main() {
    part1();
    return 0;
}

void part1() {
    double x_kernel[3][3] = {{1, 0, -1}, {2, 0, -2}, {1, 0, -1}};
    double y_kernel[3][3] = {{1, 2, 1}, {0, 0, 0}, {-1, -2, -1}};
    vector<vector<double> > grayScale = grayscale();
    vector<vector<double> > x_direction = sobel(grayScale, x_kernel);
    vector<vector<double> > y_direction = sobel(grayScale, y_kernel);
    vector<vector<double> > output = threshold(x_direction, y_direction, 5000);

    ofstream stream; stream.open("imagem.ppm");
    stream << "P3 " << grayScale[0].size() << " " << grayScale.size() << " " << 255 << endl;
    for(int i = 0; i < output.size(); i++) {
        for(int j = 0; j < output[0].size(); j++) {
            for(int k = 0; k < 3; k++) {stream << (int) output[i][j] << " ";}
        }
        stream << endl;
    }
    stream.close();
}

vector<vector<double> > threshold(vector<vector<double> >& x, vector<vector<double> >& y, double threshold) {
    vector<vector<double> > output; vector<double> temp;
    for(int i = 0; i < x.size(); i++) {
        for(int j = 0; j < x[0].size(); j++) {
            if(i == 0 || j == 0 || i == x.size() - 1 || j == x[0].size() - 1) {
                temp.push_back(0);
                continue;
            }
            (pow(x[i][j], 2) + pow(y[i][j], 2)> threshold) ? temp.push_back(255) : temp.push_back(0);
        }
        output.push_back(temp); temp.clear();
    }
    return output;
}
vector<vector<double> > sobel(vector<vector<double> > &grayScale, double kernel[3][3]) {
    vector<vector<double> > ans; double added = 0; vector<double> temp;
    for(int i = 0; i < grayScale.size(); i++) {
        for(int j = 0; j < grayScale[0].size(); j++) {
            if(i == 0 || j == 0 || i == grayScale.size() - 1 || j == grayScale[0].size() - 1) {temp.push_back(0); continue;}
            added = 0;
            for(int k = 0; k <= 2; k++) {
                for(int l = 0; l <= 2; l++) {
                    added += grayScale[i+k-1][j+l-1] * kernel[k][l];
                }
            }
            temp.push_back(added);
        }
        ans.push_back(temp); temp.clear();
    }
    return ans;
}
vector<vector<vector<double> > > readFile() {
    ifstream in; string s; int length, width, scale; double val; in.open("image.ppm");
    in >> s >> length >> width >> scale;
    vector<vector<vector<double> > > array; vector< vector<double> > temp; vector<double> temp2;
    for(int i = 0; i < width; i++) {
        for(int j = 0; j < length; j++) {
            for(int k = 0; k < 3; k++) {
                in >> val;
                temp2.push_back(val);
            }
            temp.push_back(temp2);
            temp2.clear();
        }
        array.push_back(temp);
        temp.clear();
    }
    in.close();
    cout << length << " " << width << endl;
    return array;
}

vector<vector<double> > grayscale() {
   vector<vector<vector<double> > > file = readFile();
   vector<vector<double> > grayscale;
   vector<double> temp;
   for(int i = 0; i < file.size(); i++) {
       for(int j = 0; j < file[0].size(); j++) {
           double average = 0;
           for(int k = 0; k < 3; k++) {
               average += file[i][j][k];
           }
           average /= 3;
           temp.push_back(average);
       }
       grayscale.push_back(temp);
       temp.clear();
   }
   ofstream stream;
   stream.open("imageg.ppm");
   stream << "P3 " << file[0].size() << " " << file.size() << " " << 255 << endl;
   for(int i = 0; i < grayscale.size(); i++) {
       for(int j = 0; j < grayscale[0].size(); j++) {
           for(int k = 0; k < 3; k++) {
               stream << (int) grayscale[i][j] << " ";
           }
       }
       stream << endl;
   }
   stream.close();
   return grayscale;
}