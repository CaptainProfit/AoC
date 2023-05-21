//[UTF-8 en/ru]
//problem - https://adventofcode.com/2022/day/x
//2147483647 - LONG_MAX
//18446744073709551615 - ULLONG_MAX
#include <iostream> 
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <list>
#include <queue>
#include <tuple>
#include <cassert>
#define ull unsigned long long

using namespace std;

struct cPoint{
	int x;
	int y;
	char c;

};

bool operator==(const cPoint& lhs, const cPoint& rhs){
	return (lhs.x == rhs.x) &&
			(lhs.y == rhs.y) &&
			(lhs.c == rhs.c);
}

const map<char, string> isoCube{
	{'U', "RFLB"},
	{'F', "RDLU"},
	{'D', "RBLF"},
	{'B', "RULD"},
	{'L', "FDBU"},
	{'R', "BDFU"},
};

const cPoint directions[] = {
	{ 1, 0, '>'}, // право
	{ 0, 1, 'v'},  // вниз
	{-1, 0, '<'}, // лево
	{ 0,-1, '^'}, // вверх
};

cPoint inv(cPoint dir){
	for(int i = 0; i < 4; i++){
		if(directions[i] == dir){
			return directions[(i+2)%4];
		}
	}
	return dir;
}

struct cEdge{
	char mark = '?';
	int id = -1;
	int ofs =  0;
	vector<int> adj;
};

class cEdges{
	const int edgeN = 6;
	const int adjN = 4;
	vector<cEdge> edges;
	
	bool isCellBlank(int row, int column, vector<string>& surface, int cs, int cih, int ciw){
		if(row < 0 || column < 0){
			return true;
		}
		if(row >= cih || column >= ciw){
			return true;
		}
		return surface[1 + row*cs][1 + column*cs] == ' ';
	}

	void printLine(int w, bool flag){
		cout<<"+";
		string d("-");
		if(flag)
			d="--------";
		for(int i=0;i<w;i++)
			cout<<d;
		cout<<"+"<<endl;
	}

	void printScheme(vector<string>& surface, int cs, int h, int w){
		cout << "alph scheme: " << endl;
		printLine(w, false);
		for(int i=0; i<h; i++){
			cout << "|";
			for(int j=0; j<w; j++){
				if(!isCellBlank(i, j, surface, cs,  h, w)){
					int id = j + i*w;
					cout << (*this)[id].mark;
				}
				else{
					cout << " ";
				}
			}
			cout << "|" << endl;
		}
		printLine(w, false);
		cout << "id scheme: " << endl;
		printLine(w, true);
		for(int i=0; i<h; i++){
			cout << "|";
			for(int j=0; j<w; j++){
				if(!isCellBlank(i, j, surface, cs,  h, w)){
					int id = j + i*w;
					cout << (*this)[id].id << "\t";
				}
				else{
					cout << "\t";
				}
			}
			cout << "|" << endl;
		}
		printLine(w, true);
	}

	public:
	cEdges(vector<string>& surface, int cs, int cih, int ciw){
		edges.resize(edgeN);
		int it = 0;
		//1) строю остовное дерево по карте.
		for(int i = 0; i < cih; i++){
			for(int j = 0; j < ciw; j++){
				if(isCellBlank(i, j, surface, cs,  cih, ciw)){
					continue;
				}				
				int id = j + i*ciw;
				edges[it].mark = '?';
				edges[it].id = id;
				for(auto k: directions){
					if( !isCellBlank(i + k.y, j + k.x, surface, cs,  cih, ciw)){
						int idA = j + k.x + (i + k.y)*ciw;
						edges[it].adj.push_back(idA);
					}
					else{
						edges[it].adj.push_back(-1);
					}
				}
				it++;
			}
		}
		//2) маркирую грани
		edges[0].mark = 'U';
		queue<cEdge*> que;
		que.push(&edges[0]);
		while(!que.empty()){
			cEdge* it = &(*que.front());
			que.pop();
			//2.1) если есть соседи - синхронизирую строку соседей относительно определенных.
			for(int j = 0; j < adjN; j++){
				if(it->adj[j] == -1 ){
					continue;
				}
				char adjMark = (*this)[it->adj[j]].mark;
				if(adjMark == '?'){
					continue;
				}
				//sync
				for(int k = 0; k<adjN; k++){
					char adjIso = isoCube.at(it->mark)[(j+k)%adjN];
					if(adjMark == adjIso){
						it->ofs = k;
						break;
					}
				}
				break;
			}
			//2.2) маркирую ещё не помеченых соседей
			for(int j = 0; j < adjN; j++){
				if(it->adj[j] == -1 ){
					continue;
				}
				if((*this)[it->adj[j]].mark != '?'){
					continue;
				}
				(*this)[it->adj[j]].mark = isoCube.at(it->mark)[(j + it->ofs)%adjN];
				que.push(&(*this)[it->adj[j]]);
			}
		}
		//4) рисую схему, чтобы убедиться
		//printScheme(surface, cs, cih, ciw);
		//5) дописываю соседей.
		for(int i=0; i<edgeN; i++){
			for(int j=0; j<adjN; j++){
				if(edges[i].adj[j] != -1){
					continue;
				}
				char curMark = edges[i].mark;
				char adjMark = isoCube.at(curMark)[(j+edges[i].ofs) % adjN];
				edges[i].adj[j] = atMark(adjMark).id;
			}
		}		
	}
	
	//by id
	cEdge& operator[](int id){
		for(int i = 0; i < edgeN; i++){
			if(edges[i].id == id){
				return edges[i];
			}
		}
	}
	
	cEdge& atMark(char m){
		for(int i = 0; i < edgeN; i++){
			if(edges[i].mark == m){
				return edges[i];
			}
		}
	}

	cEdge& atVec(int i){
		return edges[i];
	}

	int size(){
		return edges.size();
	}
};

class cSolve{
	vector<pair<char, int>> moves;
	vector<string> surface;
	string border;
	string name;
	int x, y;
	
	//cell data
	int cellSize;
	int cellsInHeight;
	int cellsInWidth;
	int dirIt = 0;

	bool isCellBlank(int row, int column){
		if(row < 0 || column < 0){
			return true;
		}
		if(row >= cellsInHeight || column >= cellsInWidth){
			return true;
		}
		return surface[1 + row*cellSize][1 + column*cellSize] == ' ';
	}

	map<tuple<int, int, int>, tuple<int, int, int>> ports;
	void printMap(ostream& iout = cout){
		if(border == ""){
			border.resize(surface[0].size() + 2, '=');
		}

		iout << "map:" << endl;
		iout << border << endl;
		for(int i = 0; i < surface.size(); i++){
			iout << '|' << surface[i] << '|' << endl;
		}
		iout << border << endl;
	}

	void printBorder(int dm = 0, ostream& iout = cout){
		if(border == ""){
			border.resize(surface[0].size() + 2, '=');
		}

		iout << "map:" << endl;
		iout << border << endl;
		vector<string> newSurf(surface);
		for(int i=0;i<newSurf.size();i++){
			for(int j=0;j<newSurf[0].length();j++){
				if(newSurf[i][j] == '\r'){
					newSurf[i][j] = ' ';
				}
				if(newSurf[i][j] != ' '){
					newSurf[i][j] = '0';
				}
			}
		}
		for(auto a:ports){
			int i = get<0>(a.first);
			int j = get<1>(a.first);
			int d = get<2>(a.first);
			i+= directions[d].y;
			j+= directions[d].x;
			newSurf[i][j] = directions[d].c;

			i = get<0>(a.second);
			j = get<1>(a.second);
			d = get<2>(a.second);
			switch(dm){
				case 4:
					newSurf[i][j]++;
					break;
				default:
					if(d == dm){
						newSurf[i][j] = directions[d].c;
					}
			}
		}
		for(int i = 0; i < newSurf.size(); i++){
			iout << '|' << newSurf[i] << '|' << endl;
		}
		iout << border << endl;
	}

	void printMoves(ostream& iout = cout){
		iout << "moves:" << endl;
		for(int i = 0; i < moves.size(); i++){
			iout << moves[i].first;
			if(moves[i].first == 'M'){
				iout << moves[i].second;
			}
			iout << " ";
		}
		iout << endl;
	}

	int defineCellSize(){
		//	перебираю по горизонтали
		set<int> sizes;
		for(int i = 0; i < surface.size(); i++){
			int x1 = surface[i].find_first_of(".#");
			if(x1 == -1){
				continue;
			}
			int x2 = surface[i].find_first_of(" \r", x1);
			sizes.insert(x2 - x1);
		}
		//	перебираю по вертикали
		int N = surface[0].length();
		for(int i = 0; i < N; i++){
			int y1 = horizFind(i, ".#");
			if(y1 == -1){
				continue;
			}
			int y2 = horizFind(i, " \r", y1);
			sizes.insert(y2 - y1);
		}
		//	подвожу итог.
		return *(sizes.lower_bound(1));
	}

	int move(const pair<char, int>& move){
		static int count = 0;
		count++;
		switch(move.first){
			case 'A':
				return dirIt;				
			case 'L': 
				dirIt--;
				break;
			case 'R': 
				dirIt++;
				break;
			case 'M': 
				for(int i = 0; i < move.second; i++){
					int newx = x + directions[dirIt].x, newy = y + directions[dirIt].y;
					int newdirIt = dirIt;
					tuple<int ,int ,int> src(y, x, dirIt);
					//1) если идём за край
					if(ports.find(src) != ports.end()){
						tuple<int ,int, int> dst = ports[src];
						newy = get<0>(dst);
						newx = get<1>(dst);
						newdirIt = get<2>(dst);
						// dirIt = newdirIt;
					}
					if(surface[newy][newx] == ' ' || 
						newx > (cellSize*cellsInWidth) ||
						newy > (cellSize*cellsInHeight) ||
						dirIt >= 4 ||
						newx < 0 ||
						newy < 0 ||
						dirIt < 0){
						assert(0);
					}
					//3) если не наткнулись на камень - шаг вперёд.
					if(surface[newy][newx] != '#'){
						x = newx;
						y = newy;
						dirIt = newdirIt;
						surface[y][x] = directions[dirIt].c;
					}
					else{
						break;
					}
				}	
				break;
			default: cout << "error: incorrect move" << endl;
		}
		dirIt += 4;
		dirIt %= 4;
		surface[y][x] = directions[dirIt].c;
		return 0;
	}

	int horizFind(int xr, char c, int i0 = 0){
		int i = i0;
		for(; i < surface.size(); i++){
			if(surface[i][xr] == c){
				return i;
			}
		}
		return i;
	}

	int horizFind(int xr, const string& str, int i0 = 0){
		int i = i0;
		for(; i < surface.size(); i++){
			for(int j = 0; j < str.length(); j++){
				if(surface[i][xr] == str[j]){
					return i;
				}
			}
		}
		return i;
	}

	int getCellId(int x, int y){
		x --;
		y --;
		x /= cellSize;
		y /= cellSize;
		return x + y*cellsInWidth;
		//
	}

	void calculatePorts(){
		//1) определяю размеры граней.
		cellSize = defineCellSize();
		cellsInHeight = surface.size()/cellSize; 
		cellsInWidth = surface[0].length()/cellSize;

		cEdges edges(surface, cellSize, cellsInHeight, cellsInWidth);
		
		//формирую границу.
		set<pair<int, int>> portsPoints;
		// горизонтальные линии границы.
		int x1 = 1, x2 = 1;
		for(int yr = 0; yr < surface.size(); yr++){
			if(surface[yr][x1] == ' ' ||
				surface[yr][x1 - 1] != ' '){
				size_t  x3 = surface[yr].find_first_not_of(" \r\n");
				if(x3 == string::npos){
					continue;
				}
				x1 = x3;
			}

			if(surface[yr][x2 + 1] != ' ' ||
				surface[yr][x2] == ' '){
				x2 = surface[yr].find(' ', x1) - 1;
			}

			pair<int, int> a1(yr, x1), a2(yr, x2);
			portsPoints.emplace(a1);
			portsPoints.emplace(a2);		
		}
		// вертикальные линии границы.
		int y1 = surface.size()/2, y2 = surface.size()/2;
		for(int xr = 0; xr < surface[0].length(); xr++){
			if(surface[y1][xr] == ' ' ||
				surface[y1 -1][xr] != ' '){
				int y3 = horizFind(xr, ".#");
				if(y3 == surface.size()){
					continue;
				}
				y1 = y3;
			}

			if(surface[y2 + 1][xr] != ' ' ||
				surface[y2][xr] == ' '){
					y2 = horizFind(xr, ' ', y1)-1;
				}
			pair<int, int> a1(y1, xr), a2(y2, xr);
			portsPoints.emplace(a1);
			portsPoints.emplace(a2);
		}

		for(auto iter = portsPoints.begin(); iter!= portsPoints.end(); iter++){
			int xOut = iter->second, yOut = iter->first;
			for(int dirOut = 0;  dirOut < 4; dirOut++){
				int xIn = iter->second + directions[dirOut].x;
				int yIn = iter->first + directions[dirOut].y;
				if(surface[yIn][xIn] != ' '){
					continue;
				}
				int idOut = getCellId(iter->second, iter->first);
				int idIn = idIn = edges[idOut].adj[dirOut];
				int cInx, cOutx, cIny, cOuty;
				tie(cOuty, cOutx) = calcCellCoord(idOut);
				tie(cIny, cInx) = calcCellCoord(idIn);
				int dirIn = (getDirIn(idOut, dirOut, edges) + 2)%4;

				int dxOut = xOut - cOutx;
				int dyOut = yOut - cOuty;

				int dyIn, dxIn;
				tie(dyIn, dxIn) = magicTransform(dyOut, dxOut, dirOut, dirIn);

				xIn = cInx + dxIn;
				yIn = cIny + dyIn;
				assert(1);
				//а теперь ещё надавать.
				ports.emplace(tuple<int, int, int>(yOut, xOut, dirOut), tuple<int, int, int>(yIn, xIn, dirIn));

			}
		}
		//printPorts();
	}

	void printPorts(void){
		const map<int, char> conv{
			{0,'R'},
			{1,'D'},
			{2,'L'},
			{3,'U'},
			};
		cout<<"(x,y,dir)-(x,y,dir)"<<endl;
		for(auto a:ports){
			cout << "(" << get<0>(a.first) << 
					"," << get<1>(a.first) <<
					"," << conv.at(get<2>(a.first)) << ") - ";
			cout << "(" << get<0>(a.second) << 
					"," << get<1>(a.second) <<
					"," << conv.at(get<2>(a.second)) << ")" << endl;
		}
		cout << endl;
	}
	tuple<int, int> magicTransform(int y1, int x1, int dir1, int dir2){
		//1) (y1, x1, dir1)-> id1
		int id;
		switch(dir1){
			case 0:
				id = y1;
				break;
			case 1:
				id = cellSize - 1 - x1;
				break;
			case 2:
				id = cellSize - 1 - y1;
				break;
			case 3:
				id = x1;
				break;
		}
		//2) id1 = id2 ~id
		//3) (id,dir2) -> (x2, y2)
		int x2 = 0, y2 = 0;
		switch(dir2){
			case 0:				
				y2 = id;
				break;
			case 1:
				x2 = cellSize - 1 - id;
				break;
			case 2:
				x2 = cellSize - 1;
				y2 = cellSize - 1 - id;
				break;
			case 3:
				x2 = id;
				y2 = cellSize - 1;
				break;
		}
		return make_tuple(y2,x2);
	}

	tuple<int, int> calcCellCoord(int id){
		int i = id/cellsInWidth;
		int j = id%cellsInWidth;
		int y, x;
		y = 1 + i*cellSize;
		x = 1 + j*cellSize;
		return make_tuple(y, x);
	}
	
	int getDirIn(int idOut, int dirOut, cEdges& edges){
		
		int idIn = edges[idOut].adj[dirOut];
		for(int i=0;i<4;i++){
			if(edges[idIn].adj[i] == idOut){
				return i;
			}
		}
		assert(0);
		return -1;
	}
	public:
		
	//LRM
	cSolve(const string& name){
		string line;
		ifstream ifstr(name, ios::binary);
		this->name = name.substr(0, name.find('.'));
		int maxWidth = 0;
		surface.push_back(" ");
		for(getline(ifstr, line); !ifstr.eof(); getline(ifstr, line)){
			if(line.find_first_of("RL0123456789") == string::npos){
				// карта
				if( line != ""){
					surface.push_back(" " + line + " ");
					if(maxWidth < line.length()){
						maxWidth = line.length();
					}
				}
			}
			else{
				// последовательность ходов
				for(int it = 0; it < line.length(); it++){
					switch(line[it]){
						case '\r': break;
						case 'L':
						case 'R':
							moves.push_back(pair<char, int>(line[it], -1));
							break;
						default:
							int it1 = line.find('L', it);
							int it2 = line.find('R', it);
							if(it1 == -1 && it2 == -1){
								string subs = line.substr(it);
								int steps = stoi(subs);
								moves.push_back(pair<char, int>('M', steps));
							}
							else{
								if(it1 == -1){
									string subs = line.substr(it, it2 - it);
									int steps = stoi(subs);
									moves.push_back(pair<char, int>('M', steps));
									it = it2 - 1;
									continue;
								}
								if(it2 == -1){
									string subs = line.substr(it, it1 - it);
									int steps = stoi(subs);
									moves.push_back(pair<char, int>('M', steps));
									it = it1 - 1;
									continue;
								}
								int it3 = min(it1, it2);
								string subs = line.substr(it, it3 - it);
								int steps = stoi(subs);
								moves.push_back(pair<char, int>('M', steps));
								it = it3 - 1;
							}
					}
				}
			}
		}
		ifstr.close();
		if(surface[surface.size() - 1].find_first_not_of(" \r") != string::npos){
			surface.push_back(" ");
		}
		maxWidth++;
		maxWidth++;
		for(int i = 0; i < surface.size(); i++){
			surface[i].resize(maxWidth, ' ');
			int j = surface[i].find('\r');
			while(j != -1){
				surface[i][j] = ' ';
				j = surface[i].find('\r', j);
			}
		}
		calculatePorts();
		//printMap();
		//printMoves();
	}

	void solve(){
		//1) установить стартовую локацию.
		
		y = 1;
		x = surface[1].find('.');
		surface[y][x] = 's';
		if(1){
			for(int i = 0; i < 5; i++)
			{
				ofstream ofstr("borders/"+ name + to_string(i) + ".output", ios::binary);
				printBorder(i, ofstr);
				ofstr.close();
			}
		}
		//printMap();
		int i = 0;
		for(; i < moves.size(); i++){
			move(moves[i]);
			// if(i  >50){
			// 	break;
			// }
			if( i%500 == 0)
			{
				ofstream ofstr("maps/"+name + to_string(i) + ".output", ios::binary);
				printMap(ofstr);
				ofstr.close();
			}
		}
		ofstream ofstr("maps/"+name + to_string(i) + ".output", ios::binary);
		printMap(ofstr);
		ofstr.close();
	}

	long answer(){
		long row = y;
		long column = x;
		long facing = move(pair<char, int>('A', 0));
		return row*1000 + 4*column + facing;
	}
};

int main(void){
	long result;
	for(int i = 0; i< 10;i++){
		cSolve test1("test"+to_string(i)+".input");
	}
	cSolve test("test.input");
	test.solve();
	result = test.answer();
	if( result != 5031){
		cout << "test failed(5031): " << result << endl;
		// return -2;
	}
	cout << "test passed" << endl;
	cSolve cond("cond.input");
	cond.solve();
	result = cond.answer();
	if( result != 195032){
		cout << "cond failed(???): " << result << endl;
		return -2;
	}
	cout << "cond passed: " << result<< endl;
	// 149052 too low
	// 196036 too high
	// 196032 too high
	// 195032 correct
	return 0;
}
