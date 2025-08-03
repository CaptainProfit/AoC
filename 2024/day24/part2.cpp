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

string Key(char c, int i) {
	stringstream ss;
	ss << c << setw(2) << i;
	string key = ss.str();
	key[1] == ' ' ? key[1] = '0':'?';
	return key;
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
		.SetTL({0, 0})
		.SetBR({3000, 5600}));

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

void TestPrimitives();
int main(int argc, char** argv) {
	//TestPrimitives();
	//return -1;
	if (argc < 2) {
        	return -1;
	}
	ReadFile(argv[1]);
	Layout pcb(scheme);

	svg::Document doc;
	ofstream of("graph.svg");
	pcb.Draw(doc);
	doc.Render(of);
}
