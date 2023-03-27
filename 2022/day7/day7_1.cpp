//[UTF-8 en/ru]
//problem - https://adventofcode.com/2022/day/7
#include <iostream> 
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <list>
using namespace std;
#define ull unsigned long long

class cNode{
	public:
	cNode* parent;
	string name;
	list<cNode> childs;
	long count;
};

cNode root, *current;

int readFileToSMt(){	
	string line;
	root.parent = nullptr;
	root.name = "/";
	root.count = 0;
	current = &root;
	ifstream ifstr("y2022d7.input", ios::binary);
	int k = 0;
	while(1){
		k++;
		//cout<<k<<" ";
		getline(ifstr, line);
		if(ifstr.eof()){
			ifstr.close();
			break;
		}
		if(line[0] == '$'){
			//some command
			if(line.substr(2,2).compare("ls") == 0){
				// idk
			} else
			if(line.substr(2,2).compare("cd") == 0){
				if(line.substr(5,1).compare("/") == 0){
					current = &root;
				}
				else if(line.substr(5,2).compare("..") == 0){
					if(current != &root)
						current = current->parent;
				}
				else{
					string nextDir = line.substr(5);
					for(list<cNode>::iterator iter = current->childs.begin(); iter != current->childs.end();iter++)
					//for(auto[] iter:current->childs)
						if(iter->name.compare(nextDir) == 0){
							current = &(*iter);
							break;
						}
				}
			}			
		}
		else if(line.substr(0,3).compare("dir") == 0){
			//some directory
			current->childs.emplace_back();
			current->childs.back().parent = current;
			current->childs.back().count = 0;
			current->childs.back().name = line.substr(4);
		}
		else{
			//some file
			stringstream str(line);
			long s;
			str>>s;
			current->count+=s;
		}
	}
	ifstr.close();
	return 0;
}

long counter = 0;
long limit = 100000;
long round(cNode* rt){
	long cnt = 0;
	for(list<cNode>::iterator it = rt->childs.begin(); it != rt->childs.end(); it++)
		cnt+=round(&(*it));
	rt->count+=cnt;
	if(rt->count > limit)
		counter+=rt->count;
	return rt->count;
}

int main(void){
	readFileToSMt();
	//now come from tree
	round(&root);
	cout<<"there are "<<counter<<" total size of ..."<<endl;
	return 0;
	//203011970 - wrong
}
