#pragma once
#include <vector>
#include <iostream>
using namespace std;
struct Point {
    int y;
    int x;
    Point& operator+=(const Point& lhs) {
        x += lhs.x;
        y += lhs.y;
        return *this;
    }
    Point& operator-=(const Point& lhs) {
        x -= lhs.x;
        y -= lhs.y;
        return *this;
    }
};

const vector<Point> dirs = {
    {0, -1},
    {1, 0},
    {0, 1},
    {-1, 0}};
map<char, Point> c2dirs = {
    {'<', {0, -1}},
    {'V', {1, 0}},
    {'>', {0, 1}},
    {'^', {-1, 0}}
};

Point operator+(Point rhs, const Point& lhs) {
    return rhs += lhs;
}

bool operator==(const Point& lhs, const Point& rhs) {
	return lhs.x == rhs.x && lhs.y == rhs.y;
}

bool operator!=(const Point& lhs, const Point& rhs) {
	return !(lhs == rhs);
}

Point operator-(Point rhs, const Point& lhs) {
    return rhs -= lhs;
}

bool operator<(const Point& lhs, const Point& rhs) {
	return lhs.y < rhs.y || (lhs.y == rhs.y && lhs.x < rhs.x);
}

bool operator>(const Point& lhs, const Point& rhs) {
	return rhs < lhs;
}

std::ostream& operator<<(std::ostream& out, const Point& pt) {
	out << "(" << pt.y << ", " << pt.x << ")";
	return out;
}

size_t Norm1(const Point& a) {
    return std::abs(a.x) + std::abs(a.y);
}

size_t dist1(const Point& lhs, const Point& rhs) {
    return std::abs(lhs.x -rhs.x) + std::abs(lhs.y -rhs.y);
}