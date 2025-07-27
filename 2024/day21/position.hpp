#pragma once
#include "point.hpp"

using namespace std;

struct Position {
	vector<Point> pos;
};

bool operator==(const Position& lhs, const Position& rhs) {
	bool flag = true;
	for(int i = 0; i < lhs.pos.size(); i++) {
		flag &= lhs.pos[i] == rhs.pos[i];
	}
	return flag;
}

bool operator<(const Position& lhs, const Position rhs) {
	for(int i = 0; i < lhs.pos.size(); i++) {
		if ( lhs.pos[i] < rhs.pos[i]) {
			return true;
		}
		if ( lhs.pos[i] > rhs.pos[i]) {
			return false;
		}
	}
	return false;
}

ostream& operator<<(ostream& out, const Position& pt) {
	bool first = true;
	out << "(";
	for(int i = 0; i < pt.pos.size(); i++) {
		if (!first) {
			cout << ", ";
		}
		first = false;
		out <<  pt.pos[i];
	}
	out << ")";
	return out;
}