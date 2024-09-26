#include <iostream>
#include <iomanip>
#include <cmath>
#include <ctime>
#include <chrono>
#include <sys/time.h>
#include <string>
#include <list>
#include <cfloat>
#include <unordered_set>
#include <vector>
#include <fstream>
#include <unordered_map>
#include <algorithm>

using namespace std;

class Point {

private:
	double x;
	double y;

public:
	Point() {
		x = 0;
		y = 0;
	}

	Point(double x, double y) {
		this->x = x;
		this->y = y;
	}

	double get_x() {
		return x;
	}

	void set_x(int x) {
		this->x = x;
	}

	double get_y() {
		return y;
	}

	void set_y(int y) {
		this->y = y;
	}

	double get_distance_from(Point other) {
		return sqrt(
				pow(other.get_y() - this->get_y(), 2)
						+ pow(other.get_x() - this->get_x(), 2));
	}

	bool equals(Point other) {
		return other.get_x() == x && other.get_y() == y;
	}

	bool imprecise_equals(Point other) {
		return get_display_string() == other.get_display_string();
	}

	Point copy() {
		return Point(x, y);
	}

	string get_display_string() {
		return "(" + to_string(x) + ", " + to_string(y) + ")";
	}

	void print() {
		cout << get_display_string() << endl;
	}

};


void print_ints(vector<int>& v) {
	for (int i = 0; i < v.size(); i++) {
		cout << v[i] << " - ";
	}

	cout << endl;
}

void print_points(vector<Point>& points) {
	for (Point p : points) {
		cout << p.get_display_string();
	}

	cout << endl;
}

void print_subsquare_map(unordered_map<int, Point> subsquare_map) {
	for (auto entry : subsquare_map) {
		cout << entry.first << ", " << entry.second.get_display_string() << endl;
	}

	cout << endl;
}

int randrange(int min, int max) {
   static bool first = true;

   if (first) {
      srand(time(NULL));
      first = false;
   }

   return min + rand() % (( max + 1 ) - min);
}

double get_random_double() {
	static bool first = true;

	if (first) {
		srand(time(NULL));
		first = false;
	}

	return (double) rand() / RAND_MAX;
}

Point get_random_point() {
	return Point(get_random_double(), get_random_double());
}

vector<Point> get_n_points_vector(int n) {
	vector<Point> points;

	int i = 0;

	while (i < n) {
		points.push_back(get_random_point());

		i++;
	}

	return points;
}

bool comparatorX(Point& lhs, Point& rhs) {
   return lhs.get_x() < rhs.get_x();
}

bool comparatorY(Point& lhs, Point& rhs) {
   return lhs.get_y() < rhs.get_y();
}

vector<Point> get_min_dist_bruteforce(int start, int end, vector<Point>& points) {
	vector<Point> closest;
	Point closest1, closest2;
	double min_distance = DBL_MAX;

	unordered_set<string> visited;

	for (int i = start; i < end; i++) {
		for (int j = i + 1; j < end; j++) {
			double distance = points[i].get_distance_from(points[j]);

			if (distance < min_distance) {
				min_distance = distance;
				closest1 = points[i];
				closest2 = points[j];
			}
		}
	}

	closest.push_back(closest1);
	closest.push_back(closest2);

	return closest;
}

vector<double> get_strip_indices(double start, double end, double half, vector<Point> pseudo_closest, vector<Point>& points) {
	vector<double> indices;
	Point midpoint = points[half];

	int i = half + 1;
	int j = half - 1;

	double dist = pseudo_closest[0].get_distance_from(pseudo_closest[1]);

	while (i < end && abs(points[i].get_x() - midpoint.get_x()) < dist) {
		i++;
	}

	while (j >= start && abs(points[j].get_x() - midpoint.get_x()) < dist) {
		j--;
	}

	indices.push_back(j);
	indices.push_back(i);

	return indices;
}

vector<Point> get_min_dist_bruteforce_capped(int start, int end, vector<Point>& points) {
	vector<Point> closest;
	Point closest1, closest2;
	double min_distance = DBL_MAX;

	unordered_set<string> visited;

	for (int i = start; i < end; i++) {
		for (int j = i + 1; j < i + min(8, end - i); j++) {
			double distance = points[i].get_distance_from(points[j]);

			if (distance < min_distance) {
				min_distance = distance;
				closest1 = points[i];
				closest2 = points[j];
			}

		}
	}

	closest.push_back(closest1);
	closest.push_back(closest2);

	return closest;
}

vector<Point> get_closest_points_recur_p3(int start, int end, vector<Point>& points) {
	vector<Point> closest;

	if (end - start == 2) {
		closest.push_back(points[start]);
		closest.push_back(points[start + 1]);

		return closest;
	} else if (end - start == 3) {
		return get_min_dist_bruteforce(start, end, points);
	}

	double half = (start + end) / 2;

	vector<Point> left_min = get_closest_points_recur_p3(start, half, points); // [0, 6)
	vector<Point> right_min = get_closest_points_recur_p3(half, end, points); // [6, 12)
	vector<Point> pseudo_closest;

	if (left_min[0].get_distance_from(left_min[1]) < right_min[0].get_distance_from(right_min[1])) {
		pseudo_closest = left_min;
	} else {
		pseudo_closest = right_min;
	}

	vector<double> strip_indices = get_strip_indices(start, end, half, pseudo_closest, points);
	vector<Point> vector_strip;

	for (int i = strip_indices[0]; i <= strip_indices[1]; i++) {
		vector_strip.push_back(points[i]);
	}

	sort(vector_strip.begin(), vector_strip.end(), comparatorY);

	vector<Point> strip_closest = get_min_dist_bruteforce_capped(0, vector_strip.size(), vector_strip);

	closest = strip_closest;

	if (pseudo_closest[0].get_distance_from(pseudo_closest[1]) < strip_closest[0].get_distance_from(strip_closest[1])) {
		closest = pseudo_closest;
	}

	return closest;
}

void swap(vector<Point> points, int i, int j) {
	Point temp = points[i];

	points[i] = points[j];
	points[j] = temp;
}

void knuth_shuffle(vector<Point>& points) {
	for (int i = 0; i < points.size(); i++) {
		swap(points, i, randrange(0, points.size() - 1));
	}
}

unordered_map<int, Point> get_subsquare_map(int start, int end, vector<Point>& points, double side_length) {
	unordered_map<int, Point> subsquare_map = unordered_map<int, Point>();

	for (int i = start; i <= end; i++) {
		Point subsquare_point = Point((int) (points[i].get_x() / side_length), (int) (points[i].get_y() / side_length));

		subsquare_map[i] = subsquare_point;
	}

	return subsquare_map;
}

bool is_close(Point p1, Point p2) {
	return abs(p1.get_x() - p2.get_x()) <= 2 && abs(p1.get_y() - p2.get_y()) <= 2;
}

vector<int> get_closest_subsquares(unordered_map<int, Point> subsquare_map, Point p) {
	vector<int> closest_subsquares;

	for (auto entry : subsquare_map) {
		if (is_close(p, entry.second)) {
			closest_subsquares.push_back(entry.first);
		}
	}

//	print_ints(closest_subsquares);

	return closest_subsquares;
}

vector<Point> randomized_algorithm(vector<Point>& points) {
//	knuth_shuffle(points);
	vector<Point> closest;
	double min_dist = points[0].get_distance_from(points[1]);

	Point p1, p2;
	unordered_map<int, Point> subsquare_map = unordered_map<int, Point>();

	for (int i = 0; i < points.size(); i++) {
		Point subsquare = Point((int) (points[i].get_x() / (min_dist / 2)), (int) (points[i].get_y() / (min_dist / 2)));
		vector<int> closest_subsquares = get_closest_subsquares(subsquare_map, subsquare);

		cout << closest_subsquares.size() << endl;

		if (closest_subsquares.size() > 0) {
			for (int j : closest_subsquares) {
				cout << "bru" << endl;

				double current_dist = points[i].get_distance_from(points[j]);

				cout << current_dist << " " << min_dist << endl;

				if (current_dist < min_dist) {
					cout << "a;fdsjfs;dfjk" << endl;
					min_dist = current_dist;
					p1 = points[i];
					p2 = points[j];
					subsquare_map = get_subsquare_map(0, i, points, min_dist / 2);
				}

			}
		} else {
			subsquare_map[i] = subsquare;
		}
	}

//	print_subsquare_map(subsquare_map);

	closest.push_back(p1);
	closest.push_back(p2);

	cout << min_dist << endl;

	return closest;
}

double part3(vector<Point>& points) {
	sort(points.begin(), points.end(), comparatorX);
	print_points(points);

	vector<Point> closest = get_closest_points_recur_p3(0, points.size(), points);

	print_points(closest);

	return closest[0].get_distance_from(closest[1]);
}

double part4(vector<Point>& points) {
	vector<Point> closest = randomized_algorithm(points);

	print_points(closest);

	return closest[0].get_distance_from(closest[1]);
}

int main() {
	cout << fixed << setprecision(23);

	vector<Point> points = get_n_points_vector(4);

//	print_points(points);

	cout << part3(points) << endl;
	cout << part4(points) << endl;


	return 0;
}
