#include <iostream>
#include <vector>
#include <cstdint>
#include <algorithm>
#include <fstream>
#include <limits>
#include <iomanip>
#include <array>
#include <cmath>
#include <bit>

using namespace std;
using ll = long long;
using vi = vector<int>;

int N, D=0, S=0, H=0;
vector<double> a;
vector<bool> used;
vector<int> orderDP;
const double MAX_FP16 = 1.3107199999999999e+05;
const double MIN_FP16 = 3.0517578125000000e-05;

double toFP16(double x) {
	if(abs(x) > MAX_FP16)
		return signbit(x) ? -numeric_limits<double>::infinity() : numeric_limits<double>::infinity();
	if(abs(x) < MIN_FP16)
		return 0.;
	*reinterpret_cast<uint64_t*>(&x) &= ~((1ull<<42)-1ull);
	return x;
}

ifstream out;
double f() {
	if(isdigit(out.peek())) {
		int i;
		out >> i;
		if(i <= 0 || i > N) {
			cerr << "Bad index: " << i << endl;
			exit(1);
		}
		--i;
		if(used[i]) {
			cerr << "Already used index: " << i << endl;
			exit(1);
		}
		orderDP.push_back(i);
		used[i] = true;		
		return a[i];
	}
	if(out.peek() != '{') {
		cerr << "Bad output format" << endl;
		exit(1);
	}
	out.get();
	char op; out >> op;
	vector<double> values;
	if(out.peek() != ':') {
		cerr << "Bad output format" << endl;
		exit(1);
	}
	do {
		out.get();
		values.push_back(f());
	} while(out.peek() == ',');
	if(out.peek() != '}') {
		cerr << "Bad output format" << endl;
		exit(1);
	}
	out.get();
	double ans = 0.;
	for(int i = 0; i < (int) values.size(); ++i) {
		if(op == 'd') {
			ans += values[i];
		} else if(op == 's') {
			ans += float(values[i]);
		} else if(op == 'h') {
			ans = toFP16(ans + toFP16(values[i]));
		} else {
			cerr << "Bad operation type: " << op << endl;
			exit(1);
		}
	}
	if(op == 'd') D += values.size()-1;
	else if(op == 's') S += values.size()-1;
	else if(op == 'h') H += values.size()-1;
	return ans;
}

array<uint64_t, 33> SUM_P, SUM_N;

double kahan_sum(const vector<double>& vec) {
    long double trueSum=0, corr=0;
    vector<double> dvtmp=vec;
    sort(dvtmp.begin(),dvtmp.end(), [](const double x, const double y) {
        return fabs(x) < fabs(y);
    });
    for(auto i : dvtmp) {
        long double y = static_cast<long double>(i) - corr;
        long double t = trueSum + y;
        corr = (t - trueSum) - y;
        trueSum = t;
    }
    return (double)trueSum;
}

int main(int argc, char* argv[]) {
	if(argc <= 2) {
		cerr << "Usage: " << argv[0] << " in.txt out.txt\n";
		return 1;
	}

	ifstream in(argv[1]);
	if(!in) {
		cerr << "Bad input file\n";
		return 1;
	}

	in >> N;
	a.resize(N);
	used.assign(N, false);
	for(double &x : a) if(!(in >> x)) {
		cerr << "Bad imput\n";
		return 1;
	}

	const double GOAL = kahan_sum(a);

	out.open(argv[2]);
	if(!out) {
		cerr << "Bad output file\n";
		return 1;
	}

	const double x = f();

	if(orderDP.size() != N) {
		cerr << "output does not use all input values" << endl;
		return 1;
	}

	ll P0 = 0;
	const int BLOCK = 16;
	for(int i = 0; i < N; i += BLOCK) {
		for(int j = 1; j < BLOCK; ++j) if(i+j < N && abs(orderDP[i] - orderDP[i+j]) > BLOCK-1) {
			// cerr << i << ' ' << i+j << ' ' << orderDP[i] << ' ' << orderDP[i+j] << endl;
			++P0;
		}
	}
	P0 = P0*(P0+1)/2;
	const double P = (double) P0 / 20'000.;
	const double W = 4*D + 2*S + H;
	const double C = (W + P) / double(N-1);
	const double A = pow(max(abs(x-GOAL)/max(abs(GOAL), 1e-200), 1e-20), 0.05);
	const double Score = (10. / sqrt(C + 0.5)) / A;

	cout << scientific << setprecision(16);
	cout << "Computed sum: " << x << endl;
	cout << "Exact sum: " << GOAL << endl;
	cout << fixed << setprecision(4);
	cout << "D: " << D << "   S: " << S << "   H: " << H << endl;
	cout << "W: " << W << endl;
	cout << "P: " << P << endl;
	cout << "A: " << A << endl;
	cout << "Score: " << Score << endl;

	return 0;
}