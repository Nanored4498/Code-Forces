#include <iostream>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int N, K;
	char cs[52];
	cin >> N >> K;
	cin.getline(cs, 10);
	map<string, int> idx;
	vector<int> num, last, order;
	vector<string> ss;
	N *= 3;
	for(int i = 0; i < N; ++i) {
		cin.getline(cs, 51);
		string s(cs);
		if(!idx.count(s)) {
			idx[s] = num.size();
			order.push_back(num.size());
			num.push_back(1);
			last.push_back(i);
			ss.push_back(s);
		} else {
			int id = idx[s];
			++ num[id];
			last[id] = i;
		}
	}
	sort(order.begin(), order.end(), [&](int i, int j) {
		return num[i] > num[j] || (num[i] == num[j] && last[i] > last[j]);
	});
	order.resize(min(K, (int) order.size()));
	for(int i : order) cout << ss[i] << '\n';

	return 0;
}