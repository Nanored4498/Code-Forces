#include <iostream>

using namespace std;

int main() {
	cout << "?";
	for(int i = 1; i <= 100; i++) cout << " " << i;
	cout << endl;
	int ans;
	int res = 0;
	cin >> ans;
	res += ans & (1<<15 - 1) - (1<<8 - 1);
	cout << "?";
	for(int i = 1; i <= 100; i++) cout << " " << (i << 7);
	cout << endl;
	cin >> ans;
	res += ans & 0b00000001111111;
	cout << "! " << res << endl;
	return 0;
}