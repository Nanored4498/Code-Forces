#include <iostream> 
#include <vector>

using namespace std;
typedef vector<int> vi;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int n;
	cin >> n;
	vi v(n+1);
	int start=-1, end=-1;
	cin >> v[1];
	for(int i = 2; i <= n; ++i) {
		cin >> v[i];
		if(v[i] < v[i-1]) {
			if(start < 0) start = i-1;
			end = i;
		}
	}
	if(start < 0) {
		cout << "1 1\n";
	} else {
		while(start > 1 && v[start-1] >= v[start]) --start;
		while(end < n && v[end+1] <= v[end]) ++end;
		if(start > 1 && v[start-1] > v[end]) return cout << "impossible\n", 0;
		if(end < n && v[end+1] < v[start]) return cout << "impossible\n", 0;
		for(int i = start+1; i <= end; ++i) if(v[i-1] < v[i]) return cout << "impossible\n", 0;
		cout << start << " " << end << "\n";
	}

	return 0;
}