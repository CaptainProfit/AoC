
#include <iostream> 
#include <string>
#include <vector>
#include <set>
#include <system_error>
#include <charconv>
//#include <string_view>
using namespace std;

const set<char> spaces = {
	' ',
	'\t',
	'\n',
	0,
};

// void* operator new(size_t size){
// 	cout << "allocated: " << size << endl;
// 	return malloc(size);
// }

vector<string_view> parseToWords(const string& str){
	vector<string_view> result;		
	result.clear();
	bool isWord = false;
	int wordStart = 0;
	int i = 0;
	for(; i < str.length(); i++){
		if(isWord){
			if(spaces.count(str[i]) != 0){
				isWord = false;
				string_view newShit(str.c_str() + wordStart, i - wordStart);
				wordStart = i;
				result.push_back(newShit);
			}
		}
		else{
			if(spaces.count(str[i]) == 0){
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

void* operator new(size_t size){
	cout << "allocated " << size << endl;
	return malloc(size);
}

void printData(const string& data){
	cout << '"' << data << '"';
}

int main(void){
	const string str("hello my name is 123 and 10 100 1000 10000 122 12 1 12120102010212112  89723456sdf fds43f 43f 4f34 d435 54673345634565433425342345 9874356 sdf\n\t \n\tasdfasd   asdf   asdf\tasdf\t\t\t\t\       i   1 2 3 4    live in Emporioum ");
	vector<string_view> words(parseToWords(str));
	for(auto word: words){
		int result = -1;
		auto [ptr, ec] = from_chars(word.data(), word.data() + word.size(), result, 3);
		printData(string(word)); // no allocating, nice
		//cout << '"' << word << '"';
		if (ec == std::errc()){
			cout << " = " << result <<endl;
		}
		else if (ec == std::errc::invalid_argument){ 
			cout << " not a number " << endl;
		}
		else if (ec == std::errc::result_out_of_range){ 
			cout << " too big number" << endl;
		}
	}
	string_view a(str.c_str() +  0, 5);
	string_view b(str.c_str() +  6, 2);
	string_view c(str.c_str() + 10, 4);
	string_view d(str.c_str() + 15, 2);
	string_view e(str.c_str() + 18, 3);

	return 0;
}
