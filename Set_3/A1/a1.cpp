#include <bits/stdc++.h>

using namespace std;

bool check(double cx, double cy, double r, double x, double y) {
    return pow((x - cx), 2) + pow((y - cy), 2) <= pow(r, 2);
}

void monteCarlo(vector<double>& a, vector<double>& b, vector<double>& c) {
    double x_min, x_max, y_min, y_max;
    x_min = std::max(std::max(a[0] - a[2], b[0] - b[2]), c[0] - c[2]);
    x_max = std::min(std::min(a[0] + a[2], b[0] + b[2]), c[0] + c[2]);
    y_min = std::max(std::max(a[1] - a[2], b[1] - b[2]), c[1] - c[2]);
    y_max = std::min(std::min(a[1] + a[2], b[1] + b[2]), c[1] + c[2]);
//    x_min = std::min(std::min(a[0] - a[2], b[0] - b[2]), c[0] - c[2]);
//    x_max = std::max(std::max(a[0] + a[2], b[0] + b[2]), c[0] + c[2]);
//    y_min = std::min(std::min(a[1] - a[2], b[1] - b[2]), c[1] - c[2]);
//    y_max = std::max(std::max(a[1] + a[2], b[1] + b[2]), c[1] + c[2]);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> distrib_x(x_min, x_max);
    std::uniform_real_distribution<> distrib_y(y_min, y_max);
    double sum = 0;
    int count = 0;
    for (int i = 100; i <= 1e5; i += 500) {
        double M = 0;
        for (int j = 0; j < i; ++j) {
            double x = distrib_x(gen);
            double y = distrib_y(gen);
            bool circle1 = check(a[0], a[1], a[2], x, y);
            bool circle2 = check(b[0], b[1], b[2], x, y);
            bool circle3 = check(c[0], c[1], c[2], x, y);
            if (circle1 && circle2 && circle3) {
                ++M;
            }
        }
        double S = (x_max - x_min) * (y_max - y_min);
        double S_rec = (M / i) * S;
        sum += S_rec;
        count++;
    }
    cout << sum / count;
}

signed main() {
    vector<double> a;
    vector<double> b;
    vector<double> c;
    a.push_back(1);
    a.push_back(1);
    a.push_back(1);
    b.push_back(1.5);
    b.push_back(2);
    b.push_back(sqrt(5) / 2);
    c.push_back(2);
    c.push_back(1.5);
    c.push_back(sqrt(5) / 2);
    monteCarlo(a, b, c);
}