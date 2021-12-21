#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;
typedef long long ll;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int n, m, q;
	cin >> n >> m >> q;
	int nm = n+m;
	vector<pair<ll, bool>> ab(nm);
	vector<ll> s(nm+1);
	vector<int> s2(nm+1);
	for(auto &x : ab) cin >> x.first;
	for(int i = 0; i < n; ++i) ab[i].second = true;
	for(int i = n; i < nm; ++i) ab[i].second = false;
	sort(ab.begin(), ab.end());
	s[0] = s2[0] = 0;
	ll ans = 0;
	for(int i = 0; i < nm; ++i) {
		s[i+1] = s[i] + ab[i].first;
		s2[i+1] = s2[i] + ab[i].second;
		if(ab[i].second) ans += ab[i].first;
	}
	vector<int> links(nm-1), opp(nm);
	for(int i = 0; i < links.size(); ++i) links[i] = i;
	for(int i = 0; i < nm; ++i) opp[i] = i;
	const auto w = [&](int i) { return ab[i+1].first - ab[i].first; };
	sort(links.rbegin(), links.rend(), [&](int i, int j) { return w(i) < w(j); });

	vector<ll> anss(q);
	vector<pair<ll, int>> ks(q);
	for(int i = 0; i < q; ++i) {
		cin >> ks[i].first;
		ks[i].second = i;
	}
	sort(ks.begin(), ks.end());
	for(const auto &[k, ind] : ks) {
		while(!links.empty() && w(links.back()) <= k) {
			int i = links.back();
			links.pop_back();
			int a = opp[i], b = opp[i+1];
			opp[a] = b;
			opp[b] = a;
			int n1 = s2[i+1] - s2[a], n2 = s2[b+1] - s2[i+1];
			int n = n1 + n2;
			ll x1 = s[i+1] - s[i+1-n1], x2 = s[b+1] - s[b+1-n2];
			ll x = s[b+1] - s[b+1-n];
			ans += x - x1 - x2;
		}
		anss[ind] = ans;
	}
	for(ll a : anss) cout << a << '\n';

	return 0;
}