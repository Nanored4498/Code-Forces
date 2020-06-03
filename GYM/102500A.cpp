#include <iostream> 
#include <vector>
#include <iomanip>

using namespace std;
typedef long long ll;
typedef pair<int, int> pii;
typedef vector<int> vi;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int n, w;
	cin >> n >> w;
	vector<ll> score(n+1, 0);
	vector<ll> ans(n+1, w), passed(w+2, 0), sts(w+2, 0);

	for(int t = 0; t < w; ++t) {
		int k, c;
		cin >> k;
		while(k --> 0) {
			cin >> c;
			ans[c] += t*passed[score[c]] - sts[score[c]];
			++ passed[score[c]];
			sts[score[c]] += t;
			++ score[c];
			ans[c] -= t*passed[score[c]] - sts[score[c]];
		}
	}
	cout << fixed << setprecision(7);
	for(int i = 1; i <= n; ++i) {
		ans[i] += w*passed[score[i]] - sts[score[i]];
		cout << (long double)(ans[i]) / (long double)(w) << "\n";
	}

	return 0;
}