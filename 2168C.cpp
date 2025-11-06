#include <iostream>
#include <vector>
#include <bit>
#include <cstdint>

using namespace std;
using vi = vector<int>;

int main() {
	vi mask(20, 0);
	for(int i = 0; i < 15; ++i) {
		mask[i] = 1<<i;
		for(int j = 0; j < 4; ++j)
			if(((i>>j)&1) == 0)
				mask[15+j] |= 1<<i;
	}
	for(int j = 0; j < 4; ++j)
		mask[19] ^= mask[15+j];

	string run;
	int t;
	cin >> run >> t;
	while(t--) {
		if(run == "first") {
			int x;
			cin >> x;
			-- x;
			vi ans;
			for(int i = 0; i < 20; ++i)
				if(popcount((uint32_t)x&mask[i])&1)
					ans.push_back(i);
			cout << ans.size() << '\n';
			for(int i : ans) cout << i+1 << ' ';
			cout << '\n';
		} else {
			int n, x = 0;
			cin >> n;
			while(n--) {
				int s;
				cin >> s;
				x |= (1<<(s-1));
			}
			int y = x >> 15;
			x &= (1<<15)-1;
			if(!(popcount((uint32_t)y)&1)) {
				// error on x
				int i = 0xf;
				for(int j = 0; j < 4; ++j)
					if((popcount((uint32_t)x&mask[15+j])&1) ^ ((y>>j)&1))
						i ^= (1<<j);
				if(i < 15) x ^= (1<<i);
			}
			cout << x+1 << '\n';
		}
	}
	return 0;
}