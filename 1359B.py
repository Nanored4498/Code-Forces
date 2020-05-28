t = int(input())
for _ in range(t):
	n, m, x, y = map(int, input().split())
	y = min(y, 2*x)
	res = 0
	for _ in range(n):
		a = input()
		i = 0
		while i < m:
			if a[i] == '.':
				if i+1 < m and a[i+1] == '.':
					res += y
					i += 1
				else: res += x
			i += 1
	print(res)