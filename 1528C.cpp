#include <iostream>
#include <vector>
#include <set>

using namespace std;
typedef pair<int, int> pii;
const int MAX_n = 3e5+5;

vector<int> a[MAX_n], b[MAX_n];
vector<pii> range(MAX_n);
int ind;
set<pii> S;

void dfsB(int i) {
	range[i].first = ind++;
	for(int j : b[i]) dfsB(j);
	range[i].second = ind++;
}

int dfsA(int i) {
	optional<pii> rmvd;
	auto it = S.insert(range[i]).first;
	if(it != S.begin()) {
		-- it;
		if(range[i].first < it->second) {
			rmvd.emplace(*it);
			S.erase(it);
		}
	}
	int ans = S.size();
	for(int j : a[i]) ans = max(ans, dfsA(j));
	S.erase(range[i]);
	if(rmvd.has_value()) S.insert(rmvd.value());
	return ans;
}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int t;
	cin >> t;
	while(t--) {
		int n;
		cin >> n;
		a[1].clear();
		b[1].clear();
		for(int i = 2; i <= n; ++i) {
			int p;
			cin >> p;
			a[p].push_back(i);
			a[i].clear();
		}
		for(int i = 2; i <= n; ++i) {
			int p;
			cin >> p;
			b[p].push_back(i);
			b[i].clear();
		}
		ind = 1;
		dfsB(1);
		cout << dfsA(1) << '\n';
	}

	return 0;
}