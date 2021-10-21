#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <set>
#include <map>
#include <sstream>

using namespace std;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int N;
	cin >> N;
	int count = 0, sN = sqrt(2*N);
	set<int> S0;
	map<int, vector<int>> S;
	map<int, vector<pair<int, bool>>> diff;
	S.emplace(0, vector<int>(S0.begin(), S0.end()));
	{ string s; getline(cin, s); }
	for(int d = 1; d <= N; ++d) {
		string s; getline(cin, s);
		istringstream ss(s);
		char c;
		while(ss >> c) {
			++ count;
			int i;
			ss >> i;
			auto it = (diff.empty() || (--diff.end())->first != d) ?
						diff.emplace_hint(diff.end(), d, vector<pair<int, bool>>())
						: (--diff.end());
			if(c == '+') {
				S0.insert(i);
				it->second.emplace_back(i, true);
			} else {
				S0.erase(i);
				it->second.emplace_back(i, false);
			}
		}
		if(count >= sN) {
			S.emplace_hint(S.end(), d, vector<int>(S0.begin(), S0.end()));
			count = 0;
		}
	}

	int x = 0;
	for(int i = 0; i < N; ++i) {
		int d;
		cin >> d;
		auto it = S.lower_bound(d);
		int d0 = it->first;
		if(d0 > d) d0 = (--it)->first;
		const vector<int> &s = it->second;
		int y = distance(lower_bound(s.begin(), s.end(), x), s.end());
		auto it_d = diff.upper_bound(d0);
		while(it_d != diff.end() && it_d->first <= d) {
			for(const auto &[j, a] : it_d->second) if(j >= x) {
				if(a) ++ y;
				else -- y;
			}
			++ it_d;
		}
		x = (x + y) % N;
	}
	cout << x << '\n';

	return 0;
}