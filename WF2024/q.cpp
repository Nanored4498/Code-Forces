#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <iomanip>

using namespace std;
using ll = long long;
using vi = vector<int>;

template<typename T>
struct Fenwick {
	#define LSB(i) ((i)&(-(i)))
	vector<T> v;

	Fenwick() = default;
	Fenwick(int n): v(n+1, 0) {}

	void init() {
		for(int i = 1; i < (int) v.size(); ++i) v[i] += v[i-1];
		for(int i = v.size()-1; i > 0; --i) v[i] -= v[i-LSB(i)];
	}

	void add(int i, T x) {
		for(; i < (int) v.size(); i += LSB(i)) v[i] += x;
	}
	void set(int i, T x) { add(i, x - querry(i, i)); }

	T querry(int i) {
		T ans = 0;
		for(; i > 0; i -= LSB(i)) ans += v[i];
		return ans;
	}
	T querry(int a, int b) { return querry(b) - querry(a-1); }
};

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int n;
	cin >> n;
	vector<int> a(n);
	for(int &x : a) cin >> x;
	Fenwick<int> f(n);
	for(int i = 1; i <= n; ++i) f.v[i] = 1;
	f.init();
	double ans = n + double(n-a[0]) / 2.;
	f.add(a[0], -1);
	int last = a[0];
	for(int i = 1; i < n; ++i) {
		int x = a[i];
		f.add(x, -1);
		ans += f.querry(min(x, last), n) / 2.;
		last = x;
	}
	cout << fixed << setprecision(2) << ans << '\n';

	return 0;
}