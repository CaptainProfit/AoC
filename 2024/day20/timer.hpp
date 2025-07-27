#pragma once
#include <chrono>
#include <cmath>
using namespace std;
using namespace chrono;
using namespace literals;
using dur = duration<float, micro>;
template<>
struct std::hash<dur> {
	hash<float> proxy;
	size_t operator()(const dur& s) const noexcept {
		return proxy(s.count());
	}
};

static dur accum, il, ih;

static float accum2;
size_t cnt = 0;
map <dur, size_t> meases;

class TimeCounter{
	time_point<steady_clock> start;
	public:
	TimeCounter() :
		start(steady_clock::now()) {}
	~TimeCounter() {
		dur meas = duration_cast<dur>(steady_clock::now() - start);
		cnt++;
		accum += meas;
		accum2 += meas.count()*meas.count();
		meases[meas]++;
		if (il > meas || il == 0ms) {
			il = meas;
		}
		if (ih < meas) {
			ih = meas;
		}
	}
	static void PrintStatistics(ostream& out) {
		if (cnt == 0) {
			out << "no measurements";
			return;
		}
		float d = accum2/cnt - accum.count()/cnt*accum.count()/cnt;
		out << "dd: " << d << endl;
		d = sqrt(d);
		out << "total: " << accum.count()<< "us, average: " << accum.count()/cnt << "us (disp:" << d << "us , min: " << il.count() << "us, max:" << ih.count() << "us)";
		return;
	}
	static void PrintHist(ostream& out, int bars) {
		if (bars == 0) {
			return;
		}
		dur t0 = 0us;
		dur dif = (ih - il)/bars;
		for (int i = 1; i < bars; i++) {
			dur t1 = t0 + dif;
			size_t a = 0;
			for (auto it = meases.lower_bound(t0); it != meases.upper_bound(t1); it++) {
				a += it->second;
			}
			out << i << ": " << a << ": " << t0.count() << "us to " << t1.count() << "us" << endl;
			t0 = t1;
		}
	}
};