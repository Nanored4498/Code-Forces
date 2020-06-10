#include <iostream>
#include <vector>
#include <set>

using namespace std;
typedef long long ll;

const ll MAXQ = 1e18;
const ll MAXX = 1e9;

struct PD {
	ll p, pp;
	int po;
	bool sure;
};

int main() {

	vector<bool> is_prime(900, true);
	for(int p = 2; p < 900; ++p) {
		if(!is_prime[p]) continue;
		for(int q = 2*p; q < 900; q += p)
			is_prime[q] = false;
	}

	int T;
	cin >> T;
	while(T --> 0) {
		ll p = 2, X = 1;
		vector<PD> pds;
		for(ll p = 2; p < 900; ++p)
			if(is_prime[p])
				pds.push_back({p, 1, 0, false});
		for(int nq = 0; nq < 22; ++nq) {
			ll Q = 1;
			vector<int> test;
			for(int i = 0; i < pds.size(); ++i) {
				PD &pd = pds[i];
				if(pd.pp*pd.p*X > MAXX) pd.sure = true;
				if(pd.sure) continue;
				ll pp = pd.pp, mpp = max(pd.p, pd.pp*pd.pp);
				while(pp < mpp && pp*pd.p*X <= MAXX && Q <= (MAXQ+pp*pd.p-1) / (pp*pd.p))
					pp *= pd.p;
				if(pp > pd.pp) {
					Q *= pp;
					test.push_back(i);
				}
			}
			cout << "? " << Q << endl;
			ll g;
			cin >> g;
			for(int i : test) {
				PD &pd = pds[i];
				pd.po = 0;
				X /= pd.pp;
				pd.pp = 1;
				while(g % pd.p == 0) {
					g /= pd.p;
					pd.pp *= pd.p;
					++ pd.po;
				}
				X *= pd.pp;
				if(Q % (pd.pp * pd.p) == 0) {
					pd.sure = true;
				}
			}
		}
		ll ans = 1;
		for(const PD &pd : pds) {
			ans *= (pd.po+1);
		}
		ans += max(7LL, ans);
		cout << "! " << ans << endl;
	}

}