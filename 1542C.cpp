#include <iostream>
#include <algorithm>

#include "../CPHeaders/mod_arith.h"

using namespace std;
typedef long long ll;
const int MOD = 1e9+7;
typedef mint<MOD> mi;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;
    while(t--) {
        ll n;
        cin >> n;
        mi ans = 0;
        ll d = 1, i = 1;
        while(d <= n) {
            ll add = n / d;
            ++ i;
            d = (d*i) / __gcd(d, i);
            add -= n / d;
            ans += i * mi(add);
        }
        cout << ans << '\n';
    }

    return 0;
}