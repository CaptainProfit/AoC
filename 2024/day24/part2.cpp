#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>
#include <string_view>

#include <unordered_map>
#include <unordered_set>
#include <set>
#include <optional>
#include <vector>
#include <cassert>
#include <bitset>
#include "svg.h"
#include "primitives.h"
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

unordered_set<string> holded_values;
unsigned char Calculate(string value) {
	
	if (values.count(value)) {
		return values[value];
	}
	assert(exprs.count(value));
	Expr& expr = exprs[value];
	if (expr.value) {
		return expr.value.value();
	}
	holded_values.insert(value);
	if (holded_values.count(expr.left) || holded_values.count(expr.right)) {
		throw -1;
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

string Key(char c, int i) {
	stringstream ss;
	ss << c << setw(2) << i;
	string key = ss.str();
	key[1] == ' ' ? key[1] = '0':'?';
	return key;
}

template<typename T> 
void Print(ostream& out, const vector<T>& v) {
        out << "[";
        bool first = true;
        for(auto& el : v) {
                if (!first) 
                        out << ", ";
                else first = false;
                out << el;
        }
        out << "]";
}

template<typename T> 
ostream& operator<<(ostream& out, const vector<T>& v) {
        Print(out, v);
        return out;
}

ostream& Print(ostream& out, char c, int b, int e) {
	for (int i = e; i >= b; i--) {
		string key = Key(c, i);
		if (values.count(key) == 0 &&
		    (exprs.count(key) == 0 || !exprs[key].value))
			continue;
		if (c != 'z') 
			out << (int)values[key];
		else
			out << (int)(exprs[key].value.value());
	}
	return out;
}

ostream& Print(ostream& out, int b = 0, int e = 100) {
	Print(out, 'x', b, e) << '|';
	Print(out, 'y', b, e) << '|';
	Print(out, 'z', b, e) << endl;
	return out;
}

void Setup(char c, long value) {
	for (int i = 0; i < 40; i++) {
		int bit = (value & (1 << i)) != 0;
		string key(Key(c, i));
		if (values.count(key) == 0)
			break;
		values[key] = bit;
	}
	for (auto& [key, expr] : exprs) {
		expr.value.reset();
	}
}

bool SolveIter() {
	for (int digit = 0; digit < 0x5; digit++) {
		long pad = (1 << digit)/2;
		cout << "digit " << digit << endl;
		for (int x = pad; x < pad + 4; x++) {
			Setup('x', x);
			for (int y = pad; y < pad + 4; y++) {
				Setup('y', y);
				string result;
				for (int i = digit; i < digit + 2; i++) {
					string key(Key('z', i));
					if (exprs.count(key) == 0)
						break;
					try {
						holded_values.clear();
						result = to_string((int)Calculate(key)) + result;
					}
					catch(...) {
						return false;
					}
				}
				/*string expected = bitset<2>(x + y - 2*pad).to_string();
				if (result != expected) {
					Print(cout);
					cout << "HALT" << endl;
					return false;
				}*/
				
				Print(cout);
			}
		}
	}
	return true;
}

class SwapGen{
	vector<unordered_map<string, Expr>::iterator> iters;
	bool finished;
public:
	SwapGen() {
		auto it = exprs.begin();
		for(int i = 0; i < 8; i++) {
			iters.push_back(it);
			it++;
		}
		finished = false;
		//(*this)();
	}
	~SwapGen() = default;

	void operator()(void) {
		int i = 0;
		set<unordered_map<string, Expr>::iterator> my_iters;
		bool flag = false;
		do  {
			my_iters.clear();
			for (i = 0; i < 8; i++) {
				iters[i]++;
				if(iters[i] != exprs.end())
					break;
				iters[i] = exprs.begin();
			}
			if (i == 8 && iters[7] == exprs.begin()) {
				finished = true;
			}
			flag = false;
			for (int i = 0; i < 8; i++) {
				for (int j = i + 1; j < 8; j++) {
					if (iters[i] == iters[j]) {
						flag = true;
						break;
					}
				}
				if (flag) 
					break;
			}
		} while(flag);
	}

	bool Finished() {
		return finished;
	}

	void Swap() {
		for (int i = 0; i < 8; i += 2) {
			swap(iters[i]->second, iters[i + 1]->second);
		}
	}

	void Print() {
		//cout << iters << endl;
	}
};

void Solve() {
	for (SwapGen sg; !sg.Finished(); sg()) {
		sg.Swap();
		bool result = SolveIter();
		sg.Swap();
		if (result) {
			sg.Print();
		}
	}
}


int main(int argc, char** argv) {
	svg::Document doc;
	ofstream of("out.svg");
	
	// doc.Add(svg::Rect().SetTL({0,0})
	// 	.SetBR({200, 200})
	// 	.SetRound(1)
	// 	.SetFillColor(svg::Rgb{45, 15, 35})
	// 	.SetStrokeColor(svg::Rgb{15, 55, 35}));
	doc.Add(svg::Rect()
			.SetFillColor(svg::Rgb{15,15,35})
			.SetStrokeColor(svg::Rgb{15,15,35})
			.SetTL({0, 0})
			.SetBR({500, 300}));
	svg::Element el1({160, 30}, "||"),
				el2({220, 80}, "&&"),
				el3({280, 130}, "^"),
				el4({240, 220}, "a"),
				el5({400, 230}, "b"),
				el6({460, 280}, "c");
	/*svg::Element el1({60, 30}, "||"),
				el2({120, 80}, "&&"),
				el3({180, 130}, "^"),
				el4({240, 180}, "a"),
				el5({300, 230}, "b"),
				el6({360, 280}, "c");*/
	/*svg::Element el1({160, 30}, "||"),
				el2({220, 80}, "&&"),
				el3({280, 130}, "^"),
				el4({280, 180}, "a"),
				el5({220, 230}, "b"),
				el6({160, 280}, "c");*/
				
	svg::WireLabel wl({230, 155}, "hjk");
	svg::Wire wr(wl.GetCenter(), {
			el1.GetCenter(),
			el2.GetCenter(),
			el3.GetCenter(),
			el4.GetCenter(),
			el5.GetCenter(),
			el6.GetCenter(),
	});
	wl.Emplace(doc);
	el1.Emplace(doc);
	el2.Emplace(doc);
	el3.Emplace(doc);
	el4.Emplace(doc);
	el5.Emplace(doc);
	el6.Emplace(doc);
	wr.Emplace(doc);
	
	doc.Render(of);

	return -1;
	
	if (argc < 2) {
        	return -1;
	}
	ReadFile(argv[1]);
	Solve();
}
