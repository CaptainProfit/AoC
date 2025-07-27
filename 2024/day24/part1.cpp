#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>
#include <string_view>

#include <unordered_map>
#include <unordered_set>
#include <optional>
#include <cassert>
using namespace std;

unordered_map<string, unsigned char> values;
struct Expr {
	string left;
	string right;
	string op;
	optional<unsigned char> value;
};

unordered_map<string, Expr> exprs;

string_view ExtractField(string_view& right, string_view delim) {
	string_view left;
	left = right;
	size_t pos = right.find(delim);
	left.remove_suffix(left.size() - pos);
	right.remove_prefix(pos + delim.size());
	return left;
}

void ParseVal(const string& line) {
	string_view right(line);
	string_view left = ExtractField(right, ": ");
	//size_t delim = line.find(":");
	//left.remove_suffix(delim + 1);
	
	//right.remove_prefix(delim + 2);
	int value = -1;
	stringstream ss;
	ss << right;
	ss >> value;
	values[string(left)] = value;
}

void ParseExpr(const string& line) {
	Expr new_expr;
	string_view right(line);

	new_expr.left = string(ExtractField(right, " "));
	new_expr.op = string(ExtractField(right, " "));
	new_expr.right = string(ExtractField(right, " "));
	ExtractField(right, "-> ");
	string result = string(right);
	exprs.emplace(result, new_expr);
}

void ReadFile(string filename) {
	ifstream f(filename);
	string line;
	while(getline(f, line) && !f.eof() && !line.empty()) {
		ParseVal(line);
	}

	while(getline(f, line) && !f.eof() && !line.empty()) {
		ParseExpr(line);
	}
}

unsigned char Calculate(string value) {
	if (values.count(value)) {
		return values[value];
	}
	assert(exprs.count(value));
	Expr& expr = exprs[value];
	if (expr.value) {
		return expr.value.value();
	}
	unsigned char left = Calculate(expr.left);
	unsigned char right = Calculate(expr.right);
	if (expr.op == "XOR"s) {
		expr.value = left ^ right;
	}
	else if (expr.op == "OR"s) {
		expr.value = left | right;
	}
	else if (expr.op == "AND"s) {
		expr.value = left & right;
	}
	else {
		assert(0);
	}
	return expr.value.value();
}

string Solve() {
	string result;
	for (int i = 0; i < 1000; i++) {
		stringstream ss;
		ss << 'z' << setw(2) << i;
		string key = ss.str();
		key[1] == ' ' ? key[1] = '0':'?';
		if (exprs.count(key) == 0) {
			break;
		}
		result = to_string((int)Calculate(key)) + result;
	}
	return result;
}

int main(int argc, char** argv) {
	if (argc < 2) {
        	return -1;
	}
	ReadFile(argv[1]);
	string line = Solve();
	unsigned long result = stol(line, nullptr, 2);
	cout << "answer: " << result << endl;
	// 0b1010100010101010001101100100011110011100100110 really too high
	// 46362252142374 good
}
