#pragma once
#include <string>
#include <vector>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
using namespace std;
string Key(char c, int i);

struct Topology {
	struct Expr {
		string left;
		string right;
		string op;
	};

	unordered_map<string, int> values;
    map<string, set<string>> expansions;
    map<string, Expr> exprs;

//public:
    void UpdateExpansions() {
        expansions.clear();
        for (auto& [key, expr] : exprs) {
            string left = expr.left;
            string right = expr.right;
            if (left != "none" && left != "var")
                expansions[left].insert(key);
            if (right != "none" && right != "var")
                expansions[right].insert(key);
        }
    }
	void AddExpr(string key, string left, string right, string op) {
        exprs.emplace(key, Expr{
            .left = left,
            .right = right,
            .op = op});
        /*if (left != "none" && left != "var")
            expansions[left].insert(key);
        if (right != "none" && right != "var")
            expansions[right].insert(key);*/
    }

    void AddValue(string key) {
        AddExpr(key, "none", "var", "=");
        values.emplace(key, -1);
    }

    void SetValue(char c, int i, int val) {
        string key = Key(c, i);
        if (exprs.count(key))
            values[key] = val;
    }
    
    void SetZeros() {
        for (int i = 0; i < 100; i++) {
            string k1 = Key('x', i);
            string k2 = Key('y', i);
            if (values.count(k1) == 0)
                break;
            values[k1] = 0;
            values[k2] = 0;
        }
    }

    int Calculate(string val) {

        if(values.count(val))
            return values[val];
        int l = Calculate(exprs[val].left);
        int r = Calculate(exprs[val].right);
        if (l < 0 || r < 0)
            return -1;
        if (exprs[val].op == "AND") {
            return l & r;
        }
        if (exprs[val].op == "OR") {
            return l | r;
        }
        if (exprs[val].op == "XOR") {
            return l ^ r;
        }
        assert(0);
        return -1;
    }

    int Calculate(int i) {
        string val = Key('z', i);
        return Calculate(val);
    }
    void UsedToCalculate(string val, set<string>& result) {
        result.insert(val);
        if(values.count(val) /*|| exprs[val].op == "OR"*/)
            return;
        UsedToCalculate(exprs[val].left, result);
        UsedToCalculate(exprs[val].right, result);
    }

    set<string> UsedToCalculate(int i) {
        set<string> result = {};
        string val = Key('z', i);
        UsedToCalculate(val, result);
        return result;
    }

    bool Swap(string left, string right) {
        swap(exprs[left], exprs[right]);
        if (left[0] == 'y' || right[0] == 'y')
            return false;
        if (left[0] == 'x' || right[0] == 'x')
            return false;
        //for (auto& [start, expr] : exprs ) {
        for (auto& start : vector{left, right} ) {
            auto& expr = exprs[start];
            if (expr.op == "=")
                continue;
            
            set<string> labels;
            set<string> new_labels;
            set<string> marked;
            new_labels.insert(expr.left);
            new_labels.insert(expr.right);
            while (!new_labels.empty()) {
                labels = new_labels;
                set<string> middle_labels = move(new_labels);
                for (auto label : labels) {
                    if (marked.count(label))
                        continue;
                    if (exprs[label].op != "=") {
                        string left = exprs[label].left;
                        string right = exprs[label].right;
                        if (marked.count(left) == 0)
                            new_labels.insert(left);
                        if (marked.count(right) == 0)
                            new_labels.insert(right);
                    }
                }
                marked.merge(middle_labels);

                if (labels.count(start)) {
                    return false;
                }
            }
        }
        return true;
    }
};
