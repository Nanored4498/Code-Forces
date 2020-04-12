#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>

using namespace std;

#define PII pair<long long, long long>

int main() {
	ios::sync_with_stdio(false);
	cout.tie(nullptr);

	int n;
	cin >> n;
	vector<long long> a(n), t(n);
	for(int i = 0; i < n; i++) cin >> a[i];
	for(int i = 0; i < n; i++) cin >> t[i];
	vector<PII> at;
	for(int i = 0; i < n; i++) at.push_back({a[i], t[i]});
	sort(at.begin(), at.end());

	priority_queue<PII> q;
	long long a0 = 0;
	long long res = 0;
	for(const PII &v : at) {
		while(!q.empty() && a0 < v.first) {
			PII w = q.top();
			q.pop();
			res += w.first * (a0 - w.second);
			a0 ++;
		}
		if(q.empty()) a0 = v.first;
		q.push({v.second, v.first});
	}
	while(!q.empty()) {
		PII w = q.top();
		q.pop();
		res += w.first * (a0 - w.second);
		a0 ++;
	}
	cout << res << endl;

	return 0;
}