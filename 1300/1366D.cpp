#include <iostream>
#include <vector>

using namespace std;
typedef vector<int> vi;
const int MAX = 1e7+5;

int ps[MAX];

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	for(int i = 2; i < MAX; ++i) if(!ps[i])
		for(int j = i; j < MAX; j += i)
			ps[j] = i;

	int n;
	cin >> n;
	vi d1(n, -1), d2(n, -1);
	for(int i = 0; i < n; ++i) {
		int a;
		cin >> a;
		int p = ps[a];
		while(a%p == 0) a /= p;
		if(a > 1) {
			d1[i] = p;
			d2[i] = a;
		}
	}
	for(int i = 0; i < n; ++i) cout << d1[i] << " \n"[i+1==n];
	for(int i = 0; i < n; ++i) cout << d2[i] << " \n"[i+1==n];

	return 0;
}