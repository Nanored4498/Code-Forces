#include <iostream> 
#include <vector>
#include <algorithm>

using namespace std;
typedef long long ll;
typedef pair<ll, int> pii;
typedef vector<ll> vi;
#define print_vec(v) for(int i = 0; i < (v).size(); ++i) cerr << (v)[i] << " \n"[i+1 == (v).size()]

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int n;
	ll c, r;
	cin >> n >> c >> r;
	vi s(n+2), order(n), le(n+2), ri(n+2);
	for(int i = 0; i < n; ++i) {
		cin >> s[i+1];
		order[i] = i+1;
	}
	s[0] = s[n+1] = 1e9+5;

	sort(order.begin(), order.end(), [&](ll i, ll j) {
		if(abs(s[i]) == abs(s[j])) return i < j;
		else return abs(s[i]) > abs(s[j]);
	});
	vi st = {0LL};
	for(int i = 1; i <= n; ++i) {
		while(abs(s[st.back()]) < abs(s[i])) st.pop_back();
		le[i] = st.back();
		st.push_back(i);
	}
	st = {n+1LL};
	for(int i = n; i > 0; --i) {
		while(abs(s[st.back()]) <= abs(s[i])) st.pop_back();
		ri[i] = st.back();
		st.push_back(i);
	}

	ll ans = INT64_MAX, rm = 0;
	#define BAD(i, j) ((s[(i)]*s[(j)] > 0) ^ (((j)-(i)) % 2 == 0))
	for(int i : order) {
		if(s[i] == 0) {
			if(i > 1 && s[i-1] == 0) continue;
			ans = min(ans, c+r*rm);
			rm += ri[i]-i;
			if(le[i] > 0 && ri[i] <= n && BAD(le[i], ri[i])) --rm;
			if(le[i] > 0 && ri[i] <= n && s[le[i]]*s[ri[i]] >= 0) ++rm;
			continue;
		}
		int bad = 0;
		if(le[i] > 0 && BAD(le[i], i)) ++bad;
		if(ri[i] <= n && BAD(i, ri[i])) ++bad;
		if(bad > 0) {
			ans = min(ans, (1+abs(s[i]))*c + rm*r);
			rm += bad;
			if(le[i] > 0 && ri[i] <= n && BAD(le[i], ri[i])) --rm;
		}
	}
	ans = min(ans, rm*r);
	cout << ans << endl;

	return 0;
}