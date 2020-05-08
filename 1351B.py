#https://codeforces.com/contest/1351/problem/B
t = int(input())
for _ in range(t):
	a, b = map(int, input().split())
	if a < b: a, b = b, a
	a2, b2 = map(int, input().split())
	if a2 < b2: a2, b2 = b2, a2
	print("Yes" if a==a2 and a==b+b2 else "No")