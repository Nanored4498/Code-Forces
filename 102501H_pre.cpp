#include <iostream>
#include <vector>

using namespace std;

const long long M = 1LL << 40;
const long long S0 = 0x600DCAFE;

long long f(long long s) {
	return (s + (s >> 20) + 12345) % M;
}

int main() {

	long long m = 1;
	long long sm = f(S0), sm2 = f(f(S0));
	while(sm != sm2) {
		++m;
		sm = f(sm); sm2 = f(f(sm2));
	}
	long long mu = 1;
	sm = f(sm); sm2 = f(f(sm2));
	while(sm != sm2) {
		++mu;
		sm = f(sm); sm2 = f(f(sm2));
	}
	cout << "period " << mu << endl;
	long long block = (m+mu) / 1000LL;
	long long sl = S0, smu = S0, l = 0;
	long long count = 1LL;
	for(long long i = 0; i < mu; ++i) smu = f(smu);
	vector<long long> B, S;
	while(sl != smu) {
		++l;
		if(l % block == 0) {
			B.push_back(count);
			S.push_back(sl);
			count = 0;
		}
		sl = f(sl);
		smu = f(smu);
		if(sl%2==0) ++count;
	}
	if(sl%2==0) --count;
	cout << "start " << l << " / S: " << sl << endl;
	cout << "block " << block << endl;
	cout << "long long b0[" << B.size() << "] = {" << B.front() << "LL";
	for(int i = 1; i < B.size(); ++i) {
		cout << ", " << B[i] << "LL";
		count += B[i];
	}
	cout << "};\n";
	cout << "long long s0[" << S.size() << "] = {" << S.front() << "LL";
	for(int i = 1; i < S.size(); ++i) cout << ", " << S[i] << "LL";
	cout << "};\n";
	cout << "beg " << count+B[0] << endl;
	count = sl%2==0 ? 1LL : 0LL;
	B.clear(); S.clear();
	for(long long i = 1; i < mu; ++i) {
		if(i % block == 0) {
			B.push_back(count);
			S.push_back(sl);
			count = 0;
		}
		sl = f(sl);
		if(sl%2==0) ++ count;
	}
	cout << "long long b1[" << B.size() << "] = {" << B.front() << "LL";
	for(int i = 1; i < B.size(); ++i) {
		cout << ", " << B[i] << "LL";
		count += B[i];
	}
	cout << "};\n";
	cout << "long long s1[" << S.size() << "] = {" << S.front() << "LL";
	for(int i = 1; i < S.size(); ++i) cout << ", " << S[i] << "LL";
	cout << "};\n";
	cout << "loop " << count+B[0] << endl;
	return 0;
}