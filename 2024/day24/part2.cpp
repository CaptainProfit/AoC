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
#include "svg.hpp"
#include "primitives.hpp"
#include "topology.hpp"
#include "layout.hpp"
using namespace std;

Topology scheme;

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
	string_view key = ExtractField(right, ": ");
	scheme.AddValue(string(key));
}

void ParseExpr(const string& line) {
	string_view sv(line);

	string left = string(ExtractField(sv, " "));
	string op = string(ExtractField(sv, " "));
	string right = string(ExtractField(sv, " "));
	ExtractField(sv, "-> ");
	string key = string(sv);
	scheme.AddExpr(key, left, right, op);
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


template <typename Container>
void Emplace(svg::Document& doc, Container& ctr) {
	for (auto& [key, value] : ctr) {
		value.Emplace(doc);
	}
}

void Layout::Draw(svg::Document& doc) {
	
	doc.Add(svg::Rect()
		.SetFillColor(svg::Rgb{15,15,35})
		.SetStrokeColor(svg::Rgb{15,15,35})
		.SetTL({10, 10})
		.SetBR({1080, 6880}));

	unordered_map<string, svg::WireLabel> labels;
	unordered_map<string, svg::Element> elems;
	unordered_multimap<string, svg::Wire> wires;

	for (auto& label : allocated_labels) {

		labels.emplace(label, svg::WireLabel{entities[label] + svg::Point{dx, 0}, label});
		if (scheme.exprs[label].op != "=") {
			elems.emplace(label, svg::Element{entities[label], scheme.exprs[label].op});
			wires.emplace(label, svg::Wire{elems[label], vector{labels[label]}});
		}
	}
	for (auto& label : allocated_labels) {
		vector<svg::Element> nexts;
		for (auto& next : scheme.expansions[label]) {
			if (elems.count(next) > 0) {
				nexts.push_back(elems[next]);
			}
		}
		wires.emplace(label, svg::Wire{labels[label], nexts});
	}
	Emplace(doc, labels);
	Emplace(doc, elems);
	Emplace(doc, wires);
}

string Key(char c, int i) {
	stringstream ss;
	ss << c << setw(2) << i;
	string key = ss.str();
	key[1] == ' ' ? key[1] = '0':'?';
	return key;
}

const static int bit_depth = 2;
int CheckLine(int line) {
	int result = 3;
	for (int a = 0; a < (1<<bit_depth); a++) {
		for (int xi = 0; xi < bit_depth; xi++) {
			scheme.SetValue('x', line + xi, (a&(1<<xi))>0);
		}
		for (int b = 0; b < (1<<bit_depth); b++) {
			for (int yi = 0; yi < bit_depth; yi++) {
				scheme.SetValue('y', line + yi, (b&(1<<yi))>0);
			}
			int c = a + b;
			int calc = 0;
			for (int zi = 0; zi < bit_depth + 1; zi++) {
				calc += scheme.Calculate(line + zi) << zi;
			}
			if (calc != c) {
				result = 0;
				break;
			}
		}
		if (result != 3) {
			break;
		}
	}
	scheme.SetZeros();
	return result;
}

void MonkeyThinks(set<string>& susps, int line) {
	int suc = 0;
	int noncyclick = 0;
	string key1 = Key('z', line);
	multimap<string, string> swaps;
	if (scheme.exprs[key1].op != "XOR") {
		for (auto it2 = susps.begin(); it2 != susps.end(); it2++) {
			string key2 = *it2;
			if (scheme.exprs[key2].op != "XOR")
				continue;
			if (scheme.Swap(key1, key2) ) {
				int result = CheckLine(line);
				if (line > 0)
					result &= CheckLine(line - 1);
				if (result == 3) {
					cout << "  swap " << key1 << " <-> " << key2 << endl;
					swaps.insert({key1, key2});
					suc++;
				}
				noncyclick++;
			}
			scheme.Swap(key1, key2);
		}
	}
	cout << "  total " << suc << " ideas among noncyclic " << noncyclick << endl;
	if (swaps.size() == 1) {
		scheme.Swap(swaps.begin()->first, swaps.begin()->second);
	}
}

void Solve () {
	assert(scheme.Swap("z13"s, "dbv"s) == 0);
	assert(scheme.Swap("z13"s, "dbv"s) == 1);
	//scheme.Swap("z06"s, "jmq"s);
	//scheme.Swap("z13"s, "gmh"s);
	//scheme.Swap("rqf"s, "cbd"s); // lines 25, 24, 
	//scheme.Swap("z38"s, "qrh"s);
	scheme.SetZeros();

	for (int i = 43; i >= 0; i--) {
		int result = CheckLine(i);
		if ( result != 3) {
			cout << "bit " << i << " Err";
			set<string> possible_defects;
			//if ((result & 1) == 0)
				possible_defects.merge(scheme.UsedToCalculate(i));
			//if ((result & 2) == 0)
				possible_defects.merge(scheme.UsedToCalculate(i + 1));
			cout << " mask " << (3^result) << " possible defects " << possible_defects.size() << endl;
			MonkeyThinks(possible_defects, i);
			if (CheckLine(i) == 3) {
				cout << "  fixed" << endl;
			}
			else {
				cout << "  skipped" << endl;
			}
		}
		else {
			cout << "bit " << i << " OK(" << scheme.UsedToCalculate(i).size() << ")" << endl;
		}
	}
}

void TestPrimitives();
int main(int argc, char** argv) {
	//TestPrimitives();
	//return -1;
	if (argc < 2) {
        	return -1;
	}
	ReadFile(argv[1]);
	Solve();
	scheme.UpdateExpansions();
	Layout pcb(scheme);

	svg::Document doc;
	ofstream of("graph.svg");
	pcb.Draw(doc);
	doc.Render(of);
	//cbd,gmh,jmq,qrh,rqf,z06,z13,z38 correct	
}
