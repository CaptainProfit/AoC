#pragma once
#include <string>
#include <vector>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
using namespace std;

struct Topology {
	struct Expr {
		string left;
		string right;
		string op;
	};

	//unordered_set<string> values;
    map<string, set<string>> expansions;
    map<string, Expr> exprs;

//public:
	void AddExpr(string key, string left, string right, string op) {
        exprs.emplace(key, Expr{
            .left = left,
            .right = right,
            .op = op});
        if (left != "none" && left != "var")
            expansions[left].insert(key);
        if (right != "none" && right != "var")
            expansions[right].insert(key);
    }

    void AddValue(string key) {
        AddExpr(key, "none", "var", "=");
        //values.insert(key);
    }
};
