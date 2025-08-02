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
map<string, set<string>> expansions;

class Structure {
	struct Entity {
		string label;
		set<string> parents;
		svg::Point center;
	};
	using Layer = map<string, Entity>;
	vector<Layer> layers;
	map<string, Entity*> entities;
	set<svg::Point> reserved_points;

	public:
	static const int dx = 70;
	static const int dy = 50;

	void PlaceEntity(int l_id, Entity &entity) {
		svg::Point pt;
		pt.x = l_id *3*dx;
		pt.y = 0;
		for (auto& label : entity.parents) {
			if (entities.count(label) == 0) {
				assert(0);
			}
			pt.y += entities[label]->center.y;
		}
		pt.y /= entity.parents.size();
		pt.y = (static_cast<int>(pt.y)/dy)*dy;
		svg::Point new_pt(pt);
		int i = 0;
		while (reserved_points.count(new_pt) != 0) {
			new_pt = pt + svg::Point{0, dy*i};
			if (reserved_points.count(new_pt) == 0)
				break;
			new_pt = pt - svg::Point{0, dy*i};
			if (reserved_points.count(new_pt) == 0)
				break;
			i++;
		}
		entity.center = new_pt;
		reserved_points.insert(new_pt);		
	}

public:
	void AddPoint(int l_id, string label, svg::Point pt) {
		AddPoint(l_id, label, {}, pt);
	}
	void AddPoint(int l_id, string label, const set<string>& parents, svg::Point pt = svg::Point{0, 0}) {
		if (layers.size() <= l_id) {
			layers.resize(l_id + 1);
		}
		layers[l_id].emplace(label,
			Entity{label, parents, pt}
		);
		entities[label] = &(layers[l_id][label]);
	}
	void SetPoint(string label, svg::Point pt) {
		assert(static_cast<int>(pt.x) % dx == 0);
		assert(static_cast<int>(pt.y) % dy == 0);
		reserved_points.erase(entities[label]->center);
		entities[label]->center = pt;
		reserved_points.insert(pt);
	}
	svg::Point GetPoint(string label) {
		return entities[label]->center;
	}
	void Sort() {
		for (int l_id = 1; l_id < layers.size(); l_id++)
			for (auto& [label, entity] : layers[l_id])
				PlaceEntity(l_id, entity);
	}
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
	for (auto& [key, expr] : exprs) {
		expansions[expr.left].insert(key);
		expansions[expr.right].insert(key);
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

template<typename T>
set<string> CalculateNextLayer(unordered_map<string, T> inputs) {
	set<string> outputs;
	for (auto& [key, expr] : exprs ) {
		if (inputs.count(expr.left) == 0)
			continue;
		if (inputs.count(expr.right) == 0)
			continue;
		if (inputs.count(key) == 0)
			outputs.emplace(key);
	}
	return outputs;
}
template <typename Container>
void Emplace(svg::Document& doc, Container& ctr) {
	for (auto& [key, value] : ctr) {
		value.Emplace(doc);
	}
}

void Draw() {
	Structure str;
	svg::Point iter{Structure::dx, Structure::dy};

	svg::Document doc;
	doc.Add(svg::Rect()
		.SetFillColor(svg::Rgb{15,15,35})
		.SetStrokeColor(svg::Rgb{15,15,35})
		.SetTL({0, 0})
		.SetBR({1000, 1600}));

	unordered_map<string, svg::WireLabel> known_values;
	unordered_map<string, svg::Element> elems;
	unordered_multimap<string, svg::Wire> wires;

	int l_id = 0;
	for (int i = 0; i < 100; i++) {
		string key1 = Key('x', i);
		string key2 = Key('y', i);
		if (values.count(key1) == 0)
			break;
		known_values.emplace(key1, svg::WireLabel{iter, key1});
		iter.y += Structure::dy;
		known_values.emplace(key2, svg::WireLabel{iter, key2});
		iter.y += 3*Structure::dy;
		str.AddPoint(l_id, key1, svg::Point{0, Structure::dy*(3*i)});
		str.AddPoint(l_id, key2, svg::Point{0, Structure::dy*(3*i + 1)});
	}
	iter.x = 0;
	set<string> new_values;
	
	do {
		l_id++;
		iter.x += 2*Structure::dx;
		iter.y = Structure::dy + 15*l_id;
		new_values = CalculateNextLayer(known_values);
		
		for (auto& key : new_values) {
			str.AddPoint(l_id, key, set{exprs[key].left, exprs[key].right});
			elems.emplace(key, svg::Element{iter, exprs[key].op});
			known_values.emplace(key, svg::WireLabel{iter + svg::Point{Structure::dx, 0}, key});
			iter.y += Structure::dy;
		}
	} while (!new_values.empty());
	
	str.Sort();
	for (auto& [key, entity] : known_values) {
		if (elems.count(key) != 0) {
			elems[key].SetCenter(str.GetPoint(key));
		}
		entity.SetCenter(str.GetPoint(key) + svg::Point{Structure::dx, 0});
	}
	for (auto& [key, entity] : known_values) {
		if (elems.count(key) != 0 ) {
			wires.emplace(key, svg::Wire{
				elems[key],
				vector{known_values[key]}
			});
		}
	}
	for (auto& [key, _] : exprs) {
		vector<svg::Element> wire_ends;
		for (auto& [key_found, expr] : exprs)
			if (expr.left == key || expr.right == key)
				wire_ends.push_back(elems[key_found]);
		wires.emplace(key, svg::Wire{
			known_values[key],
			wire_ends
		});
	}
	for (auto& [key, _] : values) {
		vector<svg::Element> wire_ends;
		for (auto& [key_found, expr] : exprs)
			if (expr.left == key || expr.right == key)
				wire_ends.push_back(elems[key_found]);
		wires.emplace(key, svg::Wire{
			known_values[key],
			wire_ends
		});
	}

	Emplace(doc, known_values);
	Emplace(doc, elems);
	Emplace(doc, wires);
	
	ofstream of("z_outs2.svg");
	doc.Render(of);
}

void TestPrimitives();
int main(int argc, char** argv) {
	//TestPrimitives();
	//return -1;
	if (argc < 2) {
        	return -1;
	}
	ReadFile(argv[1]);
	Draw();
}
