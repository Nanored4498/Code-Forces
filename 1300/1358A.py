t = int(input())
for _ in range(t):
	n, m = map(int, input().split())
	if n % 2 == 0 or m % 2 == 0: print((n*m) // 2)
	else: print((n//2)*m + m//2 + 1)