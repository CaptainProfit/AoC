//[UTF-8 en/ru]
//problem - https://adventofcode.com/2022/day/x
//2147483647 - LONG_MAX
//18446744073709551615 - ULLONG_MAX
#include <cmath>
#include <chrono> 
#include <iostream> 
#include <fstream>
#include <algorithm>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <list>
#include <unordered_set>
#include "log_duration.h"
#include <cassert>
#define ull unsigned long long

using namespace std;
using namespace chrono;

typedef enum{caseBlueprint, caseEach, caseRobot, caseOre, caseClay, caseObsidian, caseGeode} eCases;
typedef enum{stateWaitBlueprint, stateWaitRobot, stateWaitMaterial} eParseState;
static long long stateCounter = 0;
static int timeLimit = 24;
static uint8_t bestResult = 0;
static list<int> bestBuildOrder;
static list<int> currentBuildOrder;
static int secondPruneRuleHits = 0;
static int enoughRuleHits = 0;
static int unreachableRuleHits = 0;
const set<char> spaces = {
	' ',
	'\t',
	'\n',
	'\r',
	':',
	'.',
	',',
};

const map<string, eCases> strToCase = {
	{"Blueprint", 	caseBlueprint},
	{"Each", 		caseEach},
	{"robot", 		caseRobot},
	{"ore", 		caseOre},
	{"clay", 		caseClay},
	{"obsidian", 	caseObsidian},
	{"geode", 		caseGeode},
};

const map<string, int> strToMaterial = {
	{"ore", 		0},
	{"clay", 		1},
	{"obsidian", 	2},
	{"geode", 		3},
};

const map<int, string> materialToStr = {
	{0, "ore"},
	{1, "clay"},
	{2, "obsidian"},
	{3, "geode"},
};

struct cRobot{
	string name;
	int id;
	vector<int> recipe;
	int produces;
	cRobot(){
		recipe = {0,0,0,0};
		id = -1;
		name = "undefined";
		produces = -1;
	}

	void print(void){
		cout << "\tname: " << name << endl;
		cout << "\tid: " << id << ":" << endl;
		cout << "\t\tproduces: " << materialToStr.at(produces) <<"("<< produces<<")" << endl;
		cout << "\t\tcosts:" << endl;
		for(int i=0; i<4; i++){
			cout << "\t\t\t" << recipe[i] << " " << materialToStr.at(i) << "s" << endl;
		}
	}
};

class cBlueprint{
	vector<cRobot> robots;
	vector<int> maximalDemands;
	int id;
	int quality;
	//int timeLimit;

	void print(void){
		cout << "Blueprint id: " << id << endl;
		for(auto robot:robots){
			robot.print();
		}
		cout << endl;
	}

	void print(list<int>& buildOrder){
		if(buildOrder.empty()){
			cout << "{}";
			return;
		}
		int prevRobotType = *buildOrder.begin();
		int count = 0;
		cout << "{";
		for(auto robotType:buildOrder){
			if(prevRobotType != robotType){
				cout << count << " " << materialToStr.at(prevRobotType) << " robots, ";
				prevRobotType = robotType;
				count = 1;
			}
			else{
				count++;
			}
		}
		cout << count << materialToStr.at(prevRobotType) << " robots";
		cout << "}";
	}

	vector<string_view> parseToWords(const string& str){
		vector<string_view> result;		
		result.clear();
		bool isWord = false;
		int wordStart = 0;
		int i = 0;
		for(; i < str.length(); i++){
			if(isWord){
				if(spaces.find(str[i]) != spaces.end()){
					isWord = false;
					string_view newShit(str.c_str() + wordStart, i - wordStart);
					wordStart = i;
					result.push_back(newShit);
				}
			}
			else{
				if(spaces.find(str[i]) == spaces.end()){
					wordStart = i;
					isWord = true;
				}
			}
		}
		if(isWord){
			string_view newShit(str.c_str() + wordStart, i - wordStart);
			result.push_back(newShit);
		}
		return result;
	}

	struct cState{
		// static long long counter;
		// static int bestResult;
		vector<uint8_t> resources;
		vector<uint8_t> workers;
		uint8_t time;
		
		uint64_t serialize(void){
			uint64_t ser= 0;
			for(int i = 0; i < 4; i++){
				ser += resources[i] + (workers[i] << 8);
				ser <<= 16;
			}
			return ser;
		}

		void deserialize(uint64_t ser){
			for(int i = 3; i >= 0; i--){
				resources[i] = ser&0xFF;
				ser >>= 8;
				workers[i] =  ser&0xFF;
				ser >>= 8;
			}
		}
		void step(int steps){
			for(int i =0; i <4; i++){
				resources[i] += workers[i]*steps;
			}
			time+= steps;
		}

		int stepsToBuild(cBlueprint* enclose, int newRobotType){
			// if(newRobotType == -1){
			// 	return true;
			// }
			int steps = 0;
			for(int i=0; i<4; i++){
				if(enclose->robots[newRobotType].recipe[i] == 0){
					continue;
				}
				if(workers[i] == 0){
					return -1;
				}
				float val = enclose->robots[newRobotType].recipe[i] - resources[i];
				if(val < 0){
					continue;
				}
				steps = max(steps, (int)(ceil(val/workers[i])));
			}
			if(time + steps >= timeLimit ){
				return -1;
			}
			return steps;
		}

		bool isEnough(cBlueprint* enclose, int newRobotType){
			return enclose->maximalDemands[newRobotType] <= workers[newRobotType];
		}

		bool secondPruneRule(void){
			int timeLeft = timeLimit - time;
			int limit = resources[3] + workers[3]*(timeLeft) + (timeLeft*(timeLeft -1))/2;
			return limit < bestResult;
		}

		bool strongSecondPruneRule(cBlueprint* enclose){
			cState state(*this);
			for(int t = time; t < timeLimit; t++){
				state.step(1);
			 	if(enclose->robots[3].recipe[2] <= state.resources[2]){
					state.workers[3]++;
					state.resources[2] -= enclose->robots[3].recipe[2];
				}
				if(enclose->robots[2].recipe[1] <= state.resources[1]){
					state.workers[2]++;
					state.resources[1] -= enclose->robots[2].recipe[1];
				}
				if(enclose->robots[1].recipe[0] <= state.resources[0]){
					state.workers[1]++;
					state.resources[0] -= enclose->robots[1].recipe[0];
				}
			}
			return state.resources[3] < bestResult;
		}

		void step(cBlueprint* enclose, int newRobotType){
			for(int i=0; i<4; i++){
				resources[i] += workers[i];
			}
			if(newRobotType != -1){
				for(int i=0; i<4; i++){
					resources[i] -= enclose->robots[newRobotType].recipe[i];
				}
				workers[newRobotType]++;
			}
			time++;
		}

		cState(void){
			resources.resize(4, 0);
			workers.resize(4, 0);
			workers[0] = 1;
			time = 0;
			//counter = 0;
			stateCounter = 0;
		}

		void expand(cBlueprint *enclose){
			stateCounter++;
			for(int newRobotType = 3; newRobotType >=0; newRobotType--){
				if(isEnough(enclose, newRobotType)){
					enoughRuleHits++;
					continue;
				}
				int steps = stepsToBuild(enclose, newRobotType);
				if(steps == -1){
					unreachableRuleHits++;
					continue;
				}
				cState newState(*this);
				newState.step(steps);
				if(newState.strongSecondPruneRule(enclose)){
					secondPruneRuleHits++;
					continue;
				}
				currentBuildOrder.push_back(newRobotType);
				newState.step(enclose, newRobotType);
				newState.expand(enclose);
				currentBuildOrder.pop_back();
			}
			step(timeLimit - time);
			if(bestResult < resources[3]){
				bestResult = resources[3];
				bestBuildOrder = currentBuildOrder;
			}
		}
	};	

	public:
	cBlueprint(const string& str){
		// 1) parse to words
		vector<string_view> words(parseToWords(str));
		eParseState state = stateWaitBlueprint;
		int robotIt = -1;
		timeLimit = 24;
		quality = 0;	
		robots.resize(4);
		for(int i = 0; i < words.size(); i++){
			if(strToCase.count(string(words[i])) == 0){
				continue;
			}
			switch(strToCase.at(string(words[i]))){
				case caseBlueprint:
					id = stol(string(words[i + 1]));
					state = stateWaitBlueprint;
					robotIt = -1;
					break;
				case caseEach:
					state = stateWaitRobot;
					break;
				case caseRobot:
					robotIt++;
					robots[robotIt].id = robotIt;
					robots[robotIt].name = string(words[i - 1]) + " " + string(words[i]);
					robots[robotIt].produces = strToMaterial.at(string(words[i - 1]));
					state = stateWaitMaterial;
					break;
				case caseOre:
				case caseClay:
				case caseObsidian:
				case caseGeode:
					if(state == stateWaitMaterial){
						{// if(isnumber(words[i + 1]))
							robots[robotIt].recipe[strToMaterial.at(string(words[i]))] = stol(string(words[i - 1]));
						}
					}
					else /*if stateWaitRobot*/{
						
					}
					break;
				default:
					break;
			}
		}
		maximalDemands.resize(4,0);
		for(int resourceType = 0; resourceType < 4; resourceType++){
			for(int robotType = 0; robotType < 4; robotType++){
				if( maximalDemands[resourceType] < robots[robotType].recipe[resourceType]){
					maximalDemands[resourceType] = robots[robotType].recipe[resourceType];
				}
			}
		}
		maximalDemands[3] = 999;
		//print();
	}

	void calcQuality(void){
		int result = -1;
		timeLimit = 24;
		bestResult = 0;
		secondPruneRuleHits = 0;
		enoughRuleHits = 0;
		unreachableRuleHits = 0;
		cState state;
		{
			LOG_DURATION("calculating blueprint");
			state.expand(this);
			quality = bestResult;
		}
		cout << "states visited: " << stateCounter << endl; //state.counter
		cout << "quality is: " << quality << endl;
		cout << "build order is: "; print(bestBuildOrder); cout << endl;
		cout << "enough rule hits: " << enoughRuleHits << endl;
		cout << "second rule hits: " << secondPruneRuleHits << endl;
		cout << "unreach rule hits: " << unreachableRuleHits << endl;
		cout << "--------------------------------------------------" << endl;
	}

	long long getID(void){
		return id;
	}

	long long getQuality(void){
		return quality;
	}

};

class cSolve{
	string name;
	vector<cBlueprint> blueprints;
	time_point<system_clock> tStart;
	time_point<system_clock> tEnd;

	public:
	cSolve(string& filename){
		LOG_LIFE_TIME_DURATION("0) reading file");
		name = filename;
		tStart = chrono::system_clock::now();
		ifstream ifstr(name + ".input", ios::binary);			
		string line;
		string acc;
		for(getline(ifstr, line); !ifstr.eof(); getline(ifstr, line)){
			acc += line;
			if(line.compare("\r") == 0 || line.compare("") == 0){
				blueprints.push_back(acc);
				acc = "";	
			}
			// do something
		}
		if(acc.length() != 0){
			blueprints.push_back(acc);
		}
		ifstr.close();
	}

	void solve(void){
		for(int i = 0; i < blueprints.size(); i++){
			blueprints[i].calcQuality();
		}
		tEnd = chrono::system_clock::now();
	}

	void printUsedTime(void){
		system_clock::duration timeInterval = tEnd - tStart;
		auto dta= microseconds(duration_cast<microseconds>(timeInterval));		
		long long dt = dta.count();
		cout << "time used: ";
		if(dt < 1000){
			cout << dt << "us" << endl;
			return;
		}

		dt /= 1000;
		if(dt < 1000){
			cout << dt << "ms" << endl;
			return;
		}
		
		dt /= 1000;
		if(dt < 60){
			cout << dt << "s" << endl;
			return;
		}

		dt /= 60;
			cout << dt << "min" << endl;
			return;
	}	
	
	void printUsedMemory(void){
		cout << "i dont know how much memory was used" << endl;
	}

	int getResult(void){
		long long result = 0;
		for(auto bp:blueprints){
			result += bp.getQuality() * bp.getID();
		}
		//elves.updateSizes();
		return result;
	}
};

int main(void){
	long long result;
	string names[] = {"test", "cond"};
	int answers[] = {33, 1127}; 
	
	for(int i = 0; i < 2; i++){
		cSolve test1(names[i]);
		test1.solve();
		result = test1.getResult();
		test1.printUsedTime();
		test1.printUsedMemory();
		if(result != answers[i]){
			cout << names[i] + " failed: " << result << endl;
			return -2;
		}
		cout << names[i] + " passed" << endl;
	}
	cout<<"there are "<<result<<" in result"<<endl;
	return 0;
}
