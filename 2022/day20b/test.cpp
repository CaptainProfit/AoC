#include <iostream>
#include <vector>
int main(void){
	std::vector<int> v = {0,1,1,2};
	for(auto it = v.begin(); it!=v.end(); ++it){
		if(*it == 1){
			v.erase(it);
		}
	}
	std::cout << v.size() << std::endl;
	return 0;
}