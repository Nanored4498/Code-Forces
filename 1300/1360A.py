t = int(input())
for _ in range(t):
	a, b = map(int, input().split())
	res = max(2*min(a, b), max(a, b))**2
	print(res)