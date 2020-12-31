#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;
typedef long long ll;
typedef vector<int> vi;
typedef vector<ll> vl;

vl a;
vi b, deg;
vi p0, p1;
ll ans = 0;

void f(int i) {
	deg[i] = -1;
	ans += a[i];
	int j = b[i];
	if(a[i] >= 0) {
		if(j > 0) a[j] += a[i];
		p0.push_back(i);
	} else p1.push_back(i);
	if(j > 0 && --deg[j] == 0) f(j);
}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int n;
	cin >> n;
	a.resize(n+1);
	b.resize(n+1);
	deg.resize(n+1);
	for(int i = 1; i <= n; ++i) cin >> a[i];
	for(int i = 1; i <= n; ++i) {
		cin >> b[i];
		if(b[i] > 0) ++ deg[b[i]];
	}
	for(int i = 1; i <= n; ++i) if(!deg[i]) f(i);
	cout << ans << "\n";
	for(int i : p0) cout << i << " ";
	reverse(p1.begin(), p1.end());
	for(int i : p1) cout << i << " ";
	cout << "\n";

	return 0;
}