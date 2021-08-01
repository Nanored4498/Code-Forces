#include <iostream>

using namespace std;
typedef long long ll;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;
    while(t--) {
        int n;
        cin >> n;
        int oe[2] = { 0, 0 };
        n += n;
        while(n--) {
            int a;
            cin >> a;
            ++ oe[a&1];
        }
        if(oe[0] == oe[1]) cout << "Yes\n";
        else cout << "No\n";
    }

    return 0;
}