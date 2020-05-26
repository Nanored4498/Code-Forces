#include <iostream>
#include <vector>

using namespace std;

int n, pass, mi = 0;
vector<int> a, b, seen, pred;

bool augment(int i) {
	if(seen[i] == pass || i < mi) return false;
	seen[i] = pass;
	for(int j = std::max(mi, a[i]); j <= b[i]; ++j) if(pred[j] == -1 || augment(pred[j])) {
		pred[j] = i;
		return true;
	}
	return false;
}

int main() {
	ios::sync_with_stdio(false);
	cout.tie(nullptr);

	cin >> n;
	a.resize(n+1);
	b.resize(n+1);
	pred.assign(n+1, -1);
	seen.assign(n+1, -1);
	vector<int> ass(n+1);
	for(int i = 1; i <= n; ++i) cin >> a[i] >> b[i];
	for(pass = 1; pass <= n; ++pass) augment(pass);
	for(int j = 1; j <= n; ++j) ass[pred[j]] = j;
	seen.assign(n+1, -1);
	for(int j = 1; j <= n; ++j) {
		int i = pred[j];
		pred[j] = -1;
		a[i] = j+1;
		++ pass;
		mi = j;
		if(augment(i)) {
			cout << "NO\n" << ass[1];
			for(int i = 2; i <= n; ++i) cout << " " << ass[i];
			for(int j = 1; j <= n; ++j) ass[pred[j]] = j;
			cout << "\n" << ass[1];
			for(int i = 2; i <= n; ++i) cout << " " << ass[i];
			cout << "\n";
			return 0;
		}
		a[i] = j;
		pred[j] = i;
	}
	cout << "YES\n" << ass[1];
	for(int i = 2; i <= n; ++i) cout << " " << ass[i];
	cout << "\n";

	return 0;
}