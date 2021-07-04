#include <iostream>

using namespace std;
typedef long long ll;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;
    while(t--) {
        ll n, a, b;
        cin >> n >> a >> b;
        bool yes = false;
        if(a == 1) yes = (n-1) % b == 0;
        else {
            for(ll x = 1; x <= n; x *= a) if((n-x) % b == 0) yes = true;
        }
        cout << (yes ? "Yes" : "No") << '\n';
    }

    return 0;
}