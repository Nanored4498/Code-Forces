t = int(input())

for _ in range(t):
	n = int(input())
	a, b = 0, 0
	for i in range(n):
		if i < n//2-1 or i == n-1: a += 2 ** (i+1)
		else: b += 2 ** (i+1)
	print(a-b)
