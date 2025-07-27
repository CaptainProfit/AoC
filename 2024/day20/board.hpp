#pragma once
#include <string>
#include <iostream>
#include <vector>
using namespace std;
#include "point.hpp"

class Board {
public:

    void SetStart(const Point& pos) {
        start_ = pos;
    }

    const Point& GetFinish(void) const {
        return finish_;
    }
    
    void SetFinish(const Point& pos) {
        finish_ = pos;
    }
    
    const Point& GetStart(void) const {
        return start_;
    }

    bool OutOf(Point pt) const {
        return pt.x < 0 || pt.y < 0 || pt.x >= board_[0].length() || pt.y >= board_.size();
    }

    const char Get(Point pt) const {
        return board_[pt.y][pt.x];
    }

    void Set(Point pt, char c) {
        board_[pt.y][pt.x] = c;
    }

    char& operator[](const Point pt) {
        return board_[pt.y][pt.x];
    }

    const char operator[](const Point pt) const {
        return board_[pt.y][pt.x];
    }

    size_t Width(void) const {
        return board_[0].size();
    }

    size_t Height(void) const {
        return board_.size();
    }

    void AddLine(string&& newline) {
        board_.emplace_back(move(newline));
    }

    void Print(ostream& out) {
        for (auto& line : board_) {
            out << line << endl;
        }
    }

private:
    Point start_;
    Point finish_;
    vector<string> board_;
};