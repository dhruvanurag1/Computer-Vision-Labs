#include <iostream>
#include <iomanip>
#include <vector>
#include <fstream>
#include <string>
#include <math.h>
#include <stack>
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



void grayScale();
void part1();
vector<vector<double> > part2();
vector<vector<vector<double> > > readFile();
vector<vector<double> > grayscale();
vector<vector<double> > sobel(vector<vector<double> > &grayScale, double kernel[3][3]);
vector<vector<double> > threshold(vector<vector<double> >& x, vector<vector<double> >& y, double threshold);
vector<vector<double> > double_threshold(vector<vector<double> >& x, vector<vector<double> >& y, double threshold1, double threshold2);
void look_through(int i, int j);
vector<vector<double> > updated;
vector<vector<double> > grayscaleWithoutOutput();
void part3(int argc, char* argv[]);
vector<vector<double> > angled(vector<vector<double> >& x, vector<vector<double> >& y);
pair<pair<int, int> , pair<int, int> > returnPair(double angle);
vector<vector<double> > suppression(vector<vector<double> > & angles, vector<vector<double> >& values);
vector<vector<double> > magnitude(vector<vector<double> >& x, vector<vector<double> >& y);
int chartoint(char s[]);
vector<int> pushed;
int lower = 0, upper = 0; string filename;
void parseArguments(int argc, char *argv[]);

int main(int argc, char* argv[]) {
    //part1();
   // part2();
    part3(argc, argv);
    return 0;
}

void part3(int argc, char* argv[]) {
    double x_kernel[3][3] = {{1, 0, -1}, {2, 0, -2}, {1, 0, -1}};
    double y_kernel[3][3] = {{1, 2, 1}, {0, 0, 0}, {-1, -2, -1}};
    for(int i = 1; i < argc; ++i) {
        if(std::string(argv[i]) == "-L") {
            i++; lower = atoi(argv[i]);
        }
        if(std::string(argv[i]) == "-H") {
            i++; upper = atoi(argv[i]);
        }
        if(std::string(argv[i]) == "-F") {
            i++; filename = argv[i];
        }
    }
    lower = pow(lower, 2), upper = pow(upper, 2);
    vector<vector<double> > grayScale = grayscale();
    vector<vector<double> > x_direction = sobel(grayScale, x_kernel);
    vector<vector<double> > y_direction = sobel(grayScale, y_kernel);
    vector<vector<double> > hysteresis = part2();
    vector<vector<double> > angles = angled(x_direction, y_direction);
    vector<vector<double> > values = magnitude(x_direction, y_direction);
    vector<vector<double> > applySuppression = suppression(angles, values);
    ofstream stream; stream.open("image2.ppm"); ofstream stream2; stream2.open("imagef.ppm");
    stream << "P3 " << values[0].size() << " " << values.size() << " " << 1 << endl;
    stream2 << "P3 " << values[0].size() << " " << values.size() << " " << 1 << endl;
    for(int i = 0; i < values.size(); i++) {
        for(int j = 0; j < values[0].size(); j++) {
            if(applySuppression[i][j] == 1) {
                if(hysteresis[i][j] == 1){stream2 << "1 1 1 ";}
                else {stream2 << "0 0 0 ";}
                stream << "1 1 1 ";
            }
            else {
                stream << "0 0 0 "; stream2 << "0 0 0 ";
            }
        }
        stream << endl;
        stream2 << endl;
    }
    stream.close(); stream2.close();
}
vector<vector<double> > suppression(vector<vector<double> > & angles, vector<vector<double> >& values) {
    vector<vector<double> > ans; vector<double> temp;
    for(int i = 0; i < angles.size(); i++) {
        for(int j = 0; j < angles[0].size(); j++) {
            if(i == 0 || j == 0 || i == angles.size() - 1 || j == angles.size() - 1) {
                temp.push_back(0); continue;
            }
            pair< pair<int, int> , pair<int, int> > pairs = returnPair(angles[i][j]);
            if(values[i][j] >= values[i + pairs.first.first][j + pairs.first.second] && values[i][j] >= values[i + pairs.second.first][j + pairs.second.second]) temp.push_back(1);
            else temp.push_back(0);
        }
        ans.push_back(temp); temp.clear();
    }
    return ans;
}

vector<vector<double> > magnitude(vector<vector<double> >& x, vector<vector<double> >& y) {
    vector<vector<double> > ans; vector<double> temp;
    for(int i = 0; i < x.size(); i++) {
        for(int j = 0; j < x[0].size(); j++) {
            temp.push_back(pow(x[i][j], 2) + pow(y[i][j], 2));
        }
        ans.push_back(temp); temp.clear();
    }
    return ans;
}
pair<pair<int, int> , pair<int, int> > returnPair(double angle) {
    if(angle == 0 || angle == 180 || angle == -180) return make_pair(make_pair(-1, 0), make_pair(1, 0));
    if(angle == 45 || angle == -135) return make_pair(make_pair(-1, -1), make_pair(1, 1));
    if(angle == 135 || angle == -45) return make_pair(make_pair(-1, 1), make_pair(1, -1));
    if(angle == 90 || angle == -90) return make_pair(make_pair(0, -1), make_pair(0, 1));
    else return make_pair(make_pair(0, 0), make_pair(0, 0));
}
vector<vector<double> > angled(vector<vector<double> >& x, vector<vector<double> >& y) {
    vector<vector<double> > angles; vector<double> temp;
    double angle = 0;
    for(int i = 0; i < x.size(); i++) {
        for(int j = 0; j < x[0].size(); j++) {
            if(i == 0 || j == 0 || i == angles.size() - 1 || j == angles.size() - 1) {
                temp.push_back(0); continue;
            }
            angle = atan(y[i][j]/x[i][j]);
            temp.push_back(((int) (round(angle * 180 / 3.1415926535)) / 45) * 45);
        }
        angles.push_back(temp); temp.clear();
    }
    return angles;
}

vector<vector<double> > part2() {
    double x_kernel[3][3] = {{1, 0, -1}, {2, 0, -2}, {1, 0, -1}};
    double y_kernel[3][3] = {{1, 2, 1}, {0, 0, 0}, {-1, -2, -1}};
    vector<vector<double> > grayScale = grayscale();
    vector<vector<double> > x_direction = sobel(grayScale, x_kernel);
    vector<vector<double> > y_direction = sobel(grayScale, y_kernel);
    vector<vector<double> > output = double_threshold(x_direction, y_direction, lower > 0 ? lower : 5000, upper > 0 ? upper : 35000);
    updated = output;
    for(int i = 1; i < output.size()-1; i++) {
        for(int j = 1; j < output[0].size()-1; j++) {
            if(output[i][j] == 2) {
                look_through(i, j);
            }
        }
    }
    ofstream stream; stream.open("image1.ppm");
    stream << "P3 " << (int) grayScale[0].size() << " " << (int) grayScale.size() << " " << 1 << endl;
    for(int i = 0; i < output.size(); i++) {
        for(int j = 0; j < output[0].size(); j++) {
            if(updated[i][j] <= 1) {
                updated[i][j] = 0; stream << "0 0 0 ";
            }
            else {
                updated[i][j] = 1; stream << "1 1 1 ";
            }
        }
        stream << endl;
    }
    stream.close();
    return updated;
}

vector<vector<double> > double_threshold(vector<vector<double> >& x, vector<vector<double> >& y, double threshold1, double threshold2) {
    vector<vector<double> > output; vector<double> temp;
    for(int i = 0; i < x.size(); i++) {
        for(int j = 0; j < x[0].size(); j++) {
            if(pow(x[i][j], 2) + pow(y[i][j], 2) < threshold1) temp.push_back(0);
            else if(threshold1 < (pow(x[i][j], 2) + pow(y[i][j], 2)) && (pow(x[i][j], 2) + pow(y[i][j], 2)) < threshold2) {temp.push_back(1);}
            else temp.push_back(2);
        }
        output.push_back(temp); temp.clear();
    }
    return output;
}

void look_through(int i, int j) {
    vector<pair<int, int> > check;
    if(updated[i-1][j-1] == 1) {check.push_back(make_pair(i-1, j-1)); updated[i-1][j-1] = 2;}
    if(updated[i-1][j] == 1) {check.push_back(make_pair(i-1, j)); updated[i-1][j] = 2;}
    if(updated[i-1][j+1] == 1) {check.push_back(make_pair(i-1, j+1)); updated[i-1][j+1] = 2;}
    if(updated[i][j-1] == 1) {check.push_back(make_pair(i, j-1)); updated[i][j-1] = 2;}
    if(updated[i][j+1] == 1) {check.push_back(make_pair(i, j+1)); updated[i][j+1] = 2;}
    if(updated[i+1][j-1] == 1) {check.push_back(make_pair(i+1, j-1)); updated[i+1][j-1] = 2;}
    if(updated[i+1][j] == 1) {check.push_back(make_pair(i+1, j)); updated[i+1][j] = 2;}
    if(updated[i+1][j+1] == 1) {check.push_back(make_pair(i+1, j+1)); updated[i+1][j+1] = 2;}
    for(int i = 0; i < check.size(); i++) look_through(check[i].first, check[i].second);
}

void part1() {
    double x_kernel[3][3] = {{1, 0, -1}, {2, 0, -2}, {1, 0, -1}};
    double y_kernel[3][3] = {{1, 2, 1}, {0, 0, 0}, {-1, -2, -1}};
    vector<vector<double> > grayScale = grayscale();
    vector<vector<double> > x_direction = sobel(grayScale, x_kernel);
    vector<vector<double> > y_direction = sobel(grayScale, y_kernel);
    vector<vector<double> > output = threshold(x_direction, y_direction, 12000);

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
    ifstream in; string s; int length, width, scale; double val; filename != "" ? in.open(filename) : in.open("image.ppm");
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

vector<vector<double> > grayscaleWithoutOutput() {
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
   return grayscale;
}