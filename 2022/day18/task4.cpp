#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>

using namespace std;

struct point {
	short line;
	short col;
};

vector<point> getChild(point p, int n, int m, const vector < vector < int>>& matrixs) {
	vector<point> ret;
	int current = matrixs[p.line][p.col];
	--p.col;
	if (p.col >= 0 && matrixs[p.line][p.col] > current) {
		ret.push_back(p);
	}
	p.col += 2;
	if (p.col < m && matrixs[p.line][p.col] > current) {
		ret.push_back(p);
	}
	--p.col;
	--p.line;
	if (p.line >= 0 && matrixs[p.line][p.col] > current) {
		ret.push_back(p);
	}
	p.line += 2;
	if (p.line < n && matrixs[p.line][p.col] > current) {
		ret.push_back(p);
	}
	return ret;
}

void topologicalSort(int n, int m, const vector < vector < int>>& matrixs,
	stack<point>& retval, vector < vector < int>>& color, point p) {
	color[p.line][p.col] = 1;

	for (point child : getChild(p, n, m, matrixs)) {
		if (color[child.line][child.col] == 0) {
			topologicalSort(n, m, matrixs, retval, color, child);
		}
	}

	retval.push(p);
	color[p.line][p.col] = 2;
}


stack<point> topologicalSort(int n, int m, const vector < vector < int>>& matrixs) {
	stack<point> retval;
	vector < vector < int>> color(n, vector<int>(m));
	for (point p = { 0, 0 }; p.line < n; ++p.line)
		for (p.col = 0; p.col < m; ++p.col)
			if (color[p.line][p.col] == 0)
				topologicalSort(n, m, matrixs, retval, color, p);
	return retval;
}



int getLongestIncreasingPath(int n, int m, const vector < vector < int>>& matrixs) {
	stack<point> top_sorted = topologicalSort(n, m, matrixs);
	vector < vector < int>> deep(n, vector<int>(m));
	while (!top_sorted.empty()) {
		point p = top_sorted.top();
		top_sorted.pop();
		int child_deep = deep[p.line][p.col] + 1;
		for (point child : getChild(p, n, m, matrixs)) {
			deep[child.line][child.col] = max(deep[child.line][child.col], child_deep);
		}
	}
	int ret = 0;
	for (auto& line : deep)
		for (int val : line)
			ret = max(ret, val);
	return ret + 1;
}

int readInt() {
	int x;
	cin >> x;
	return x;
}

vector<int> readList(int n) {
	vector<int> res(n);
	for (int i = 0; i < n; i++) {
		cin >> res[i];
	}
	return res;
}

vector < vector < int>> readMatrix(int n, int m) {
	vector < vector < int>> matrix(n, vector<int>(m));
	for (int i = 0; i < n; i++) {
		matrix[i] = readList(m);
	}
	return matrix;
}

int main() {
	int n, m;
	cin >> n >> m;
	vector < vector < int>> matrix = readMatrix(n, m);
	cout << getLongestIncreasingPath(n, m, matrix);
}