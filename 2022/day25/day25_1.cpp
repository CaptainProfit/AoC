//[UTF-8 en/ru]
//problem - https://adventofcode.com/2022/day/25
//2147483647 - LONG_MAX
//18446744073709551615 - ULLONG_MAX
//used:
//[1] https://stackoverflow.com/questions/321068/returning-multiple-values-from-a-c-function
//[2] https://stackoverflow.com/questions/6963894/how-to-use-range-based-for-loop-with-stdmap
//[3] https://en.cppreference.com/w/cpp/container/vector
//[4] https://en.cppreference.com/w/cpp/algorithm/accumulate
#include <iostream> 
#include <fstream>
#include <vector>
#include <map>
#include <string>
#include <functional>
#include <numeric>
#include <tuple>

#define ull unsigned long long
using namespace std;

map<char, int> bj;
map<int, char> bji;
void initBijection(){
	bj.insert(pair<char, int>('=', -2));
	bj.insert(pair<char, int>('-', -1));
	bj.insert(pair<char, int>('0', 0));
	bj.insert(pair<char, int>('1', 1));
	bj.insert(pair<char, int>('2', 2));

	//[2]
	for(const auto& [f, s]: bj){
		bji.insert({s,f});
	}
	// bji.insert(pair<int, char>(-2, '='));
	// bji.insert(pair<int, char>(-1, '-'));
	// bji.insert(pair<int, char>(0, '0'));
	// bji.insert(pair<int, char>(1, '1'));
	// bji.insert(pair<int, char>(2, '2'));
}

class cNumber{
	public:
	vector<char> digits;
	cNumber(){
		//as zero
		digits.emplace_back('0');
	};
	
	cNumber(string const& s){
		for(auto c:s){
			//contains since 20
			if(bj.find(c) == bj.end()){
				digits.push_back(0);
				return ;
			}
		}
		digits.insert(digits.begin(), s.crbegin(), s.crend());
		//	digits.emplace_back(s.crbegin(), s.crend()); ???
	}

	cNumber(long long i){
		//to pentacimal
		do{
			int res = i%5;
			if(res >2){
				res -= 5;
				i += 5;
			}
			digits.push_back(bji[res]);
			i/=5;
		}while(i > 0);
		//+0 hack Kappa relaxing digits time
		//initial value of reference to non-const must be an lvalue
		//this->add(cPoint("0"));
		//this->add(zero);
		this->add("0");
		//digits.emplace_back(s.crbegin(), s.crend()); ???
	}

	long long toDecimal(){
		long long pow = 1;
		long long result = 0;
		for(int i = 0; i < digits.size(); i++){
			result += bj[digits[i]]*pow;
			pow*=5;
		}
		return result;
	}

	tuple<char, char> addDigits(char a, char b){
		//{carry, result} = addDigits(result, carry); not works
		//auto[carry, result] = addDigits(result, rhs.digits[i]); use of 'result' before deduction of 'auto'
		//tie(carry, result) = addDigits(result, carry);
		int carry = 0;
		int res = bj[a] + bj[b];
		//res -4..4
		if(res > 2){
			carry ++;
			res -= 5;
		}
		if(res < -2){
			carry --;
			res += 5;
		}
		return std::make_tuple(bji[res], bji[carry]);
	}

	int size(){
		return digits.size();
	}

	cNumber& add(const string& x) {
		cNumber y(x);
		this->add(y);
		return *this;
	}

	cNumber& add(cNumber& rhs) {
		char carry = '0';
		if(rhs.size() >= size()){
			digits.resize(rhs.size() + 2, '0');
		}
		else
			digits.resize(size() + 2, '0');
		for(int i = 0; i < digits.size() || i < rhs.digits.size(); i++){
			char carry1 = '0', carry2 = '0';
			char result = carry;
			carry = 0;
			if(i < digits.size() ){
				tie(result, carry1) = addDigits(result, digits[i]);
			}
			if(i < rhs.size() ){
				tie(result, carry2) = addDigits(result, rhs.digits[i]);
			}
			char trash; //devnull because no _ ?
			tie(carry, trash) = addDigits(carry1, carry2);
			digits[i] = result;
		}
		while(digits.back() == '0' && digits.size()>1)
			digits.pop_back();
		return *this;
	}

	string toString(){
		return string(digits.crbegin(), digits.crend());
	}
};

vector<cNumber> numbers;
cNumber zero("0");

cNumber& operator+=(cNumber& lhs, cNumber& rhs ){
	lhs.add(rhs);
	return lhs;
}

cNumber& operator+(cNumber lhs, cNumber &rhs){
	lhs += rhs;
	return lhs;
	//reference to local variable lhs returned warning.
	// good that i didn use this result never
}

ostream& operator<<(ostream& os, cNumber& x){
	os<<x.toString();
	return os;
}

int readFileToSMt(){	
	string line;
	ifstream ifstr("cond.input", ios::binary);
	while(1){
		getline(ifstr, line);
		numbers.emplace_back(line);
		cNumber a(line);
		if(ifstr.eof()){
			ifstr.close();
			break;
		}
	}
	return 0;
}

cNumber solve(){
	cNumber result("0");
	// test1
	// result += numbers[0];
	// result += numbers[1];
	// cNumber result = cNumber();

	// test2
	// long acc = 0;
	// for(int i = 0; i < 20; i++){
	// 	numbers.push_back(i+1);
	// 	result+=numbers[i];
	// 	acc+=i+1;
	// 	cout<<(acc)<<":\t"<<result<<endl;		
	// }
	for(int i = 0; i < numbers.size(); i++){
		result+=numbers[i];
	}

	//result = accumulate(numbers.begin(), numbers.end(), result); how to?
	return result;
}

int main(void){
	initBijection();	
	// cNumber a("1121-1110-1=0"); 
	// cout<<a.toDecimal()<<endl; ok
	// return -1;
	readFileToSMt();	
	cNumber result = solve();
	cout<<"there are "<<result<<" in result"<<endl;
	cout<<"there are "<<result.toDecimal()<<" in result"<<endl;
	//test OK
	//35023647158862 not the right, seems too big
	//2-10==12-122-=1-1-22 the same correct
	return 0;
}
2-10==12-122-=1-1-22