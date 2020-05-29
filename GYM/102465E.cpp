#include <iostream>
#include <vector>
#include <numeric>
#include <iomanip>

using namespace std;
typedef long long ll;
typedef vector<int> vi;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int P;
	cin >> P;
	vector<string> names(P);
	vector<int> l(P), u(P);
	for(int i = 0; i < P; ++i) {
		int j;
		cin >> names[i] >> j;
		l[i] = u[i] = j*100;
		while(l[i] > 0 && (l[i]+49)/100 == j) --l[i]; 
		while(u[i]+1 <= 10000 && (u[i]+51)/100 == j) ++u[i];
	}
	int sl = accumulate(l.begin(), l.end(), 0);
	int su = accumulate(u.begin(), u.end(), 0);
	for(int i = 0; i < P; ++i) {
		int l2 = max(l[i], 10000-su+u[i]);
		sl += l2-l[i];
		l[i] = l2;
		int u2 = min(u[i], 10000-sl+l[i]);
		su += u2-u[i];
		u[i] = u2;
		if(l[i] > u[i]) return cout << "IMPOSSIBLE\n", 0;
	}
	cout << fixed << setprecision(2);
	for(int i = 0; i < P; ++i) {
		double li = double(l[i])/double(100);
		double ui = double(u[i])/double(100);
		cout << names[i] << " " << li << " " << ui << "\n";
	}

	return 0;
}