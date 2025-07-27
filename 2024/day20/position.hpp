#pragma once
#include "point.hpp"

using namespace std;

struct Position {
	Point pos;
	int mana;
};

bool operator==(const Position& lhs, const Position& rhs) {
	return lhs.pos == rhs.pos && lhs.mana == rhs.mana;
}

bool operator<(const Position& lhs, const Position rhs) {
	return lhs.pos < rhs.pos || (lhs.pos == rhs.pos && lhs.mana< rhs.mana);
}

ostream& operator<<(ostream& out, const Position& pt) {
	out << "(" << pt.pos << ", " << pt.mana << ")";
	return out;
}